/************************************************************************
    > File Name: singleton_2.cpp
    > Author: Lao Zhenyu
    > Mail: LaoZhenyu_961112@163.com 
    > Created Time: 二  1/26 13:54:18 2021
************************************************************************/

#include <iostream>

using std::cout, std::endl;

class Singleton {
    public:
        static Singleton * getInstance(){
            if(nullptr == _pInstance){
                _pInstance = new Singleton();
            }
            return _pInstance;
        }

        void print() { cout << "Singleton::print()" << endl; }
    private:
        static Singleton *_pInstance;
        Singleton() { cout << "Constructor" << endl; }
        ~Singleton() { cout << "Destructor" << endl; }
        class Deleter{
            public:
                ~Deleter(){
                    if(_pInstance != nullptr){
                        delete _pInstance;
                    }
                }
        };
        static Deleter _deleter;
};  

Singleton * Singleton::_pInstance = nullptr;
Singleton::Deleter Singleton::_deleter;

int main(int argc, char *argv[])
{
    Singleton *p = Singleton::getInstance();
    p->print();
    return 0;
}

