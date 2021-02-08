/************************************************************************
    > File Name: singleton_1.cpp
    > Author: Lao Zhenyu
    > Mail: LaoZhenyu_961112@163.com 
    > Created Time: 二  1/26 13:29:35 2021
************************************************************************/

#include <iostream>
using std::cout, std::endl;

class Singleton{
    public:
        static Singleton& getInstance(); 
    private:
        Singleton(){
            cout << "Constructor" << endl;
        }
        ~Singleton(){
            cout << "Destructor" << endl;
        }
        Singleton(const Singleton& rhs);
};

Singleton& Singleton::getInstance(){
    static Singleton s;
    return s;
}


int main(int argc, char *argv[])
{
    Singleton::getInstance();
    exit(1);
    return 0;
}
