/************************************************************************
    > File Name: singleton_4.cpp
    > Author: Lao Zhenyu
    > Mail: LaoZhenyu_961112@163.com 
    > Created Time: 一  2/22 13:41:37 2021
************************************************************************/

// lazy
// 1. thread is safe now
// 2. memory doesn't leak


#include <iostream>
#include <memory>
#include <mutex>

class Singleton {
public:
    typedef std::shared_ptr<Singleton> Ptr;
    ~Singleton(){
        std::cout << "destruct call!" << std::endl;
    }
    Singleton(Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    static Ptr get_instance(){
        // double check lock
        if(m_instance_ptr == nullptr){
            std::lock_guard<std::mutex> lk(m_mutex);
            if(m_instance_ptr == nullptr){
                m_instance_ptr = std::shared_ptr<Singleton>(new Singleton);
            }
        }
        return m_instance_ptr;
    }
private:
    Singleton(){
        std::cout << "construct called!" << std::endl;
    }

    static Ptr m_instance_ptr;
    static std::mutex m_mutex;
};

Singleton::Ptr Singleton::m_instance_ptr = nullptr;
std::mutex Singleton::m_mutex;

int main(int argc, char *argv[])
{
    Singleton::Ptr instance = Singleton::get_instance();
    Singleton::Ptr instance_2 = Singleton::get_instance();
    return 0;
}
