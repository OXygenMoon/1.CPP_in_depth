自动类型推断是 C++11 之后在易用性上的改进.

## auto关键词

使用 auto 关键词进行类型推导, 并不是改变了C++是静态类型语言这一事实. 使用 auto 返回类型仍然是在编译期确定的.

```cpp
//原来
for(vector<int>::iterator it = vec.begin(); it != vec.end(); ++ it);

//现在
for(auto it = vec.begin(); it != vec.end(); ++ it);
```

虽然说, 在这段演示代码中原来的写法也不复杂. 但是像下面这一段就相当复杂了.

```cpp
template <typename T>
void foo(const T& container){
	for(typename T::const_iterator it = v.begin(),
	...
}
```

在这里我们只有模板类型T, 因此要表示该类型的迭代器, 我们还需要使用 typename, 否则无法表达是内部声明的类型还是成员变量.

### 兼容C数组指针的遍历函数

```cpp
//不使用auto的方案
//重载函数

template <typename T, std::size_t N>
void foo(const T (&a)[N]){
		typedef const T* ptr_t;
		for(ptr_t it = a, end = a + N; it != end; ++it){
				//循环体
		}
}

template <typename T>
void foo(const T& c){
		for(typename T::const_iterator it = c.begin(), end = c.end(); it != end; ++it){
				//循环体
		}
}
```

如果不使用类型自动推导的话, 必须写成以上两个重载函数, 重载函数一是用来兼容C数组的.

```cpp
//使用auto的方案
template <typename T>
void foo(const T& c){
		using std::begin;
		using std::end;
		for(auto it = begin(c), ite = end(c); it != ite; ++it){
				//循环体
		}
}
```

## decltype关键词

`decltype` 的用法是推导一个表达式的类型.

- `decltype(变量名)` 可以获得精确的类型.
- `decltype(表达式)` 可以获得表达式的引用类型, 除非表达式的结果是纯右值, 此时结果仍然是值类型.

如果我们有 `int a;` , 那么 : 

- `decltype(a)` 获得 int 类型
- `decltype((a))` 获得 int & 类型 (a) 是一个表达式
- `decltype(a+a)` 获得 int 类型 ( 因为 a + a 是纯右值)

### decltype(auto)

写下 `decltype(auto)` 时, 必须确定 auto 是值类型还是引用类型.

其中 : 

- auto 是值类型
- auto & 是左值引用类型
- auto && 是转发引用类型(可以是左值引用, 也可以是右值引用)

C++14之前 : 

```cpp
decltype(expr)
a = expr;
// 把 a 作为返回值, 此时如果推导出 expr 是值类型还是引用类型, 返回 a 都没有问题
```

C++14之后 : 

```cpp
decltype(auto)
a = expr;
// 如果 expr 表达式过长, 在前面一种写法中过于冗长
```

这段代码主要在通用的转发函数模板中出现, 因为你不知道你调用的函数最终是否返回的是引用.

### 函数返回值类型推断

C++14之后, 函数返回值也可以用 `auto` 和 `decltype(auto)`, 同样的使用 auto 捕获值, auto & 捕获左值引用, auto && 捕获转发引用. 而且 decltype(auto) 可以自动推导返回的是值类型还是引用类型.

后置返回类型语法如下 : 

```cpp
//在返回类型需要传入参数进行推导时, 写成后置的
auto foo(参数) -> 返回值类型声明(decltype()) {
	// 函数体
}
```

### 类模板的模板参数推导

```cpp
//不推导
pair<int,int> pr{1,42};

//推导(C++17)
auto pr{1,42};
```

```cpp
//不推导
array<int, 3> a{1,2,3};

//推导(C++17)
array a{1,2,3};
```

```cpp
//编译器根据传入参数自动推导模板参数
//自动推导
template <typename T>
struct Obj{
		Obj(T value);
		...
};

Obj o1{string("hello")); //得到 Obj<string>
Obj o2{"hello");         //得到 Obj<const char*>

//手动推导
template <typename T>
struct Obj{
		Obj(T value);
		...
};

Obj(const char *) -> Obj(string);
Obj o1("hello");         //得到Obj(string)
```

### 结构化绑定

之前有一个例子用于绑定 pair 返回的两个值.

```cpp
multimap<string, int>::iterator lower, upper;
std::tie(lower, upper) = mmp.equal_range("four");
```

C++17之后, 引入了一个新的语法, 用于解决这个问题.

```cpp
auto [lower, upper] = mmp.equal_range("four");
```
