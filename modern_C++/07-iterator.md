## 迭代器

迭代器是一个通用的概念, 而不是一个特定的类型.

在STL中, 迭代器是一种用起来像指针的对象.

一个容器拥有 `begin` 和 `end` 成员函数, 返回的是迭代器. 因此, 迭代器有以下要求 : 

1. 迭代器必须支持 * 操作, 解引用得到容器内的某个对象
2. 迭代器必须支持 ++, 指向下一个对象
3. 迭代器必须能够进行比较, 判断是否遍历到了特别的位置

在 * C++17 * 之前, begin 和 end 返回的类型必须相同; 

在 * C++17 * 之后, begin 和 end 返回的类型可以不同.

### 迭代器的要求

1. 迭代器, 最基础的要求是可以使用 `++` 运算符, 既可以前+也可以后+. (input iterator)
2. 对同一个迭代器使用多次 * 运算, 以及保存迭代器来重新遍历对象. (forward iterator)
3. 同时满足 `--` 运算符. (bidirectional iterator)
4. 额外支持, `+` `-` `+=` `-=` 跳跃式移动迭代器, 支持 [] 运算符, 支持迭代器的大小比较. (random access iterator)
5. 如果 * i 只能作为左值来写而不能读. (output iterator)

### 迭代器的最基本要求

- 可以被拷贝构造, 拷贝赋值和析构.
- 对象支持 * 运算符.
- 对象支持前置 ++ 运算符.

![https://static001.geekbang.org/resource/image/dd/5a/dd25c3f074fe0b792dddecfd15f74e5a.png](https://static001.geekbang.org/resource/image/dd/5a/dd25c3f074fe0b792dddecfd15f74e5a.png)

虽然说, 迭代器通常是对象, 但是指针可以满足以上所有迭代器的要求, 因此指针也是迭代器.

vector 的迭代器, 很多实现里直接使用指针.

## 常用迭代器

顺序容器中, 都定义了嵌套的 iterator 和 const_iterator. 其中, iterator 可写入, const_iterator 不可写入.

RandomAccessIterator : vector::iterator 和 array::iterator 和 deque::iterator

BidirectionalIterator : list::iterator

ForwardIteraot : forward_list::iterator

OutputIterator : back_inserter 返回的类型 back_inserter_iterator 和 ostream_iterator

```cpp
vector<int> v1{1, 2, 3, 4, 5};
vector<int> v2;
copy(v1.begin(), v1.end(), back_inserter(v2)); //总是保持在尾迭代器上
copy(v2.begin(), v2.end(), ostream_iterator<int>(cout, " "));
```

### 输入行迭代器

```cpp
//输入行迭代器

for (const string& line : istream_line_reader(is)) {
  // 示例循环体中仅进行简单输出
  cout << line << endl;
}

```

```cpp
//输入行迭代器的实现
class istream_line_reader {
public:
  class iterator {  // 实现 InputIterator
  public:
    typedef ptrdiff_t difference_type;
    typedef string value_type;
    typedef const value_type* pointer;
    typedef const value_type& reference;
    typedef input_iterator_tag iterator_category;
    …
  };
  …

```

```cpp
class istream_line_reader {
public:
  class iterator {
    …
    iterator() noexcept : stream_(nullptr) {}

    explicit iterator(istream& is) : stream_(&is) {
      ++*this;
    }

    reference operator*() const noexcept {
      return line_;
    }
    
		pointer operator->() const noexcept {
      return &line_;
    }
    
		iterator& operator++() {
      getline(*stream_, line_);
      if (!*stream_) {
        stream_ = nullptr;
      }
      return *this;
    }
    
		iterator operator++(int) {
			//惯用法:前置++和拷贝构造
      iterator temp(*this);
      ++*this;
      return temp;
    }

  private:
    istream* stream_;
    string line_;
  };
  …
};
```

```cpp
//比较运算符
bool operator==(const iterator& rhs)
      const noexcept
    {
      return stream_ == rhs.stream_;
    }
    bool operator!=(const iterator& rhs)
      const noexcept
    {
      return !operator==(rhs);
    }
```

```cpp
//其他成员函数
class istream_line_reader {
public:
  class iterator {…};
  istream_line_reader() noexcept
    : stream_(nullptr) {}
  explicit istream_line_reader(
    istream& is) noexcept
    : stream_(&is) {}
  iterator begin()
  {
    return iterator(*stream_);
  }
  iterator end() const noexcept
  {
    return iterator();
  }

private:
  istream* stream_;
};
```
