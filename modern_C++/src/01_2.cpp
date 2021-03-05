/************************************************************************
    > File Name: 01_2.cpp
    > Author: Lao Zhenyu
    > Mail: LaoZhenyu_961112@163.com 
    > Created Time: 五  3/ 5 13:34:55 2021
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
        ~shape() {}
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

int main(int argc, char *argv[])
{
    shape * sptr = create_shape(shape_type::circle);         //<--- 通过 create_shape 来创建对象
    sptr->print();
    delete sptr;
    return 0;
}
