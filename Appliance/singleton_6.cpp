/************************************************************************
    > File Name: singleton_6.cpp
    > Author: Lao Zhenyu
    > Mail: LaoZhenyu_961112@163.com 
    > Created Time: 一  2/22 14:01:44 2021
************************************************************************/

// lazy
// template
// magic static

#include <iostream>

template <typename T>
class Singleton {
    public:
        static T& get_instance(){
            static T instance;
            return instance;
        }
        virtual ~Singleton(){
            std::cout << "destructor called!" << std::endl;
        }
        Singleton(Singleton &) = delete;
        Singleton & operator=(const Singleton &) = delete;
    protected:
        Singleton(){
            std::cout << "constructor called!" << std::endl;
        }
};

class DerivedSingle : public Singleton<DerivedSingle> {
    friend class Singleton<DerivedSingle>;
public:
    DerivedSingle(const DerivedSingle&) = delete;
    DerivedSingle & operator = (const DerivedSingle &) = delete;
private:
    DerivedSingle() = default;
};


int main(int argc, char *argv[])
{
    DerivedSingle & instance_1 = DerivedSingle::get_instance();
    DerivedSingle & instance_2 = DerivedSingle::get_instance();
    return 0;
}
