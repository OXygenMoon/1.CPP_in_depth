/************************************************************************
    > File Name: 01_3.cpp
    > Author: Lao Zhenyu
    > Mail: LaoZhenyu_961112@163.com 
    > Created Time: 五  3/ 5 13:48:07 2021
************************************************************************/

#include <iostream>
using namespace std;

enum class shape_type {
    circle,
    triangle,
    rectangle,
};

class shape {
    public:
        virtual void print() {
            cout << "shape" << endl;
        }
        ~shape() { cout << "shape destructor." << endl; }
};

class circle : public shape {
    public:
        void print() override {
            cout << "circle" << endl;
        }
        ~circle() { cout << "circle destructor." << endl; }
};

class triangle : public shape {
    public:
        void print() override {
            cout << "triangle" << endl;
        }
};

shape* create_shape(shape_type type){
    switch(type){
        case shape_type::circle : return new circle;
        case shape_type::triangle : return new triangle;
    }
}

class shape_wrapper {
    public:
        explicit shape_wrapper(shape* ptr = nullptr) : ptr_(ptr) {}
        ~shape_wrapper() { delete ptr_; }
        shape* get() const {
            return ptr_;
        }
    private:
        shape* ptr_;
};

int main(int argc, char *argv[])
{
    try{
        shape_wrapper sw(create_shape(shape_type::circle));
        sw.get()->print();
        throw "test";
    } catch(const char* err_info){                            //<--- 异常, 仍然会栈展开释放资源
        cerr << err_info << endl;
    }
    //exit(1);                                                //<--- 终止仍然不能释放
    //delete sw.get(); 不需要写 因为有栈展开, 会自动调用栈上的对象的析构函数
    return 0;
}

