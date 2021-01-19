## 堆

这里说的堆是动态分配内存的区域, 与数据结构里的堆不同.

这里的内存必须手动释放, 否则会造成内存泄漏.

内存泄漏两大原因 : 异常或者分支导致 delete 未执行; 分配和释放在两个函数里, 忘记 delete.

`new` 和 `delete` 通常底层使用 `malloc` 和 `free` 来实现.

```cpp
auto ptr = new std::vector<int>();
```

堆的内存管理器的操作通常有 : 

1. **让内存管理器分配一个某个大小的内存块**
2. **让内存管理器释放一个之前分配的内存块**
3. 让内存管理器进行垃圾收集操作, 寻找不再使用的内存块并予以释放

C++会做1和2. Java会做1和3. Python会做1,2和3.

注意 : 内存回收会把连续未使用的内存块合并成一块, 以便满足后续的较大内存分配要求.

```cpp
void foo(){
	bar *ptr = new bar();
	...
	delete ptr;
}
```

以上代码如果在...处发生了异常, 程序终止的话, 会导致内存泄漏.

---

## 栈

和数据结构里的栈十分相似.

满足后进先出.

```cpp
void foo(){
	...
}

void bar(int n){
	int a = n + 1;
	foo(a);
}

int main(){
	...
	bar(42);
	...
}
```

### 函数调用

在 x86 在内的大部分计算机体系架构中, 栈的增长方向是低地址, 也就是意味着我们平时画的栈上分是低地址.

任何一个函数只能使用进入函数时, 栈指针上分部分的栈空间.

当调用另一个函数时, 会把参数压入栈中, 然后把下一行汇编指令的地址也压入栈, 并且跳转到新的函数.

随后, 新的函数会继续分配一部分栈空间保存本地变量. 结束后, 会根据之前压入栈的地址回到未执行的代码中.

### 使用栈空间的优势

1. 栈上的分配极为简单, 移动一下栈指针.
2. 栈上的释放也极为简单, 函数执行完毕后移动一下栈指针.
3. 由于后进先出的执行过程, 不可能出现内部碎片.

### 栈帧

每一个函数所占用的栈内存空间称为栈帧, GCC 和 Clang 的命令行参数中提到的 frame 的, 如 `-fomit-frame-pointer` 一般就是指栈帧.

### 栈展开

编译器会自动调用析构函数, 包括在函数执行发生异常的情况.

---

## RAII(读 a - re - i - i)

RAII : Resource Acquisition Is Initialization

是C++所特有的资源管理方式. 其他语言如 D / Ada / Rust 也采用 RAII. 在主流编程语言中只有 C++ 依赖 RAII 来做资源管理.

RAII依托于栈和析构函数, 对所有的资源包括堆内存在内进行管理. 这也是在C++中没有使用垃圾回收机制的原因

很多时候, 对象不能或者不应该存储在栈上, 比如 : 

1. 对象很大;
2. 对象的大小在编译时不能确定;
3. 对象是函数的返回值, 但由于特殊的原因, 不应使用对象的值返回.

比如 : 在工厂方法或其他面向对象编程的时候, 返回值类型是基类的指针或者引用.

```cpp
// 工厂方法
enum class shape_type {
	circle,
	triangle,
	rectangle,
	...
};

class shape {...};
class circle : public shape { ... };
class triangle : public shape { ... };
class rectangle : public shape { ... };

shape *create_shape(shape_type type){
	...
	switch(type){
		case shape_type::circle:
			return new circle(...);
		case shape_type::triangle:
			return new triangle(...);
		case shape_type::rectangle:
			return new rectangle  (...);
	}
}
```

这里应该返回的是一个对象的指针, 而不是一个对象. 因为对象是继承关系的, 无法确定返回类型的大小. 会引发对象切片

如何确保不会发生内存泄漏?

答案 : 在析构函数和它的栈展开行为上. 需要把返回值放在一个本地变量中, 并确保其析构函数会删除该对象即可. 实现如下 : 

```cpp
class shape_wrapper{
	public:
		explicit shape_wrapper(shape *ptr = nullptr) : ptr_(ptr) {}
		~shape_wrapper(){
			delete ptr_;
		}
		shape *get() const {
			return ptr_;
		}
	private:
		shape *ptr_;
};

void foo(){
	...
	shape_wrapper ptr_wrapper(create_shape(...));
	...
}
```

delete 一个空指针是一种合法的空操作.

```cpp
// new circle(...)
{
	void *temp = operator new(sizeof(circle));
	try{
		circle *ptr = static_cast<circle *>(temp);
		ptr->circle(...);
		return ptr;
	}
	catch (...) {
		operator delete(ptr);
		throw;
	}
}
```

```cpp
if(ptr != nullptr){
	ptr->shape();
	operator delete(ptr);
}
```

在析构函数里做必要的清理工作, 这就是 RAII 的基本用法. 这种清理并不限于释放内存, 也可以是 : 

- 关闭文件 (fstream 的析构就会这么做)
- 释放同步锁
- 释放其他重要的系统资源

例如, 我们应该使用 :

```cpp
std::mutex mtx;

void some_func(){
	std::lock_guard<std::mutex> guard(mtx);
	//做需要同步的工作
}
```

而不是 : 

```cpp
std::mutex mtx;

void some_func(){
	//做需要同步的工作
	//如果发生异常或提前返回
	//下面这一句不会自动执行
	mtx.unlock();
}
```

上面的 `shape_wrapper` 差不多是一个最简单的智能指针.

## 总结

使用栈是最自然的内存使用方式.

使用基于栈和析构函数的RAII, 可以有效地对包括堆内存在内的系统资源进行统一管理.
