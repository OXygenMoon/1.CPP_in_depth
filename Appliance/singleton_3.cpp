/************************************************************************
    > File Name: singleton_3.cpp
    > Author: Lao Zhenyu
    > Mail: LaoZhenyu_961112@163.com 
    > Created Time: 一  2/22 13:34:30 2021
************************************************************************/

// lazy-initlalization 
// 使用时才会实例化对象


#include <iostream>

class Singleton {
    private:
        Singleton() {
            std::cout << "construct called!" << std::endl;
        }
        Singleton(Singleton &) = delete;
        Singleton & operator=(const Singleton &) = delete;
        static Singleton * m_instance_ptr;

    public:
        ~Singleton() {
            std::cout << "destruct called!" << std::endl;
        }
        static Singleton* get_instance() {
            if(m_instance_ptr == nullptr){
                m_instance_ptr = new Singleton;
            }
            return m_instance_ptr;
        }
};

Singleton * Singleton::m_instance_ptr = nullptr;

int main(int argc, char *argv[])
{
    Singleton * instance = Singleton::get_instance();
    Singleton * instance_2 = Singleton::get_instance();
    return 0;
}
