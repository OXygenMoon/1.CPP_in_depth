/************************************************************************
    > File Name: shared_ptr.cpp
    > Author: Lao Zhenyu
    > Mail: LaoZhenyu_961112@163.com 
    > Created Time: 二  1/26 12:42:39 2021
************************************************************************/

#include <iostream>
using namespace std;

namespace smart_pointer{

    template <typename T>
        struct defaultDeleter{
            public:
                void operator()(T * ptr){
                    if(ptr){
                        delete ptr;
                        ptr = nullptr;
                    }
                }
        };

    template <typename T, typename Deleter = defaultDeleter<T>> 
        class shared_ptr{
            private:
                int *count{nullptr};
                T   *ptr{nullptr};

            public:
                shared_ptr() {}
                shared_ptr(T * _ptr) : ptr(_ptr), count(new int(1)) {}
                shared_ptr(const shared_ptr<T>& lhs) : ptr(lhs.ptr), count(lhs.count){
                    ++(*count);
                }
                shared_ptr(shared_ptr<T> && rhs) noexcept {
                    std::swap(ptr, rhs.ptr);
                    std::swap(count, rhs.count);
                }
                ~shared_ptr() {
                    if(ptr){
                        --(*count);
                    }
                    if(*count == 0){
                        //delete ptr;
                        Deleter d;
                        d(ptr);
                        d(count);
                        //delete count;
                    }
                }
                
                shared_ptr & operator=(T * ptr){
                    if(this->ptr && --(*(this->count)) == 0){
                        delete ptr;
                        delete count;
                    }
                    this->ptr = ptr;
                    this->count = new int(1);
                    return *this;
                }

                shared_ptr & operator=(shared_ptr<T> & lhs){
                    if(&lhs != this){
                        return *this;
                    }
                    ++(*lhs.count);
                    if(this->ptr && --(*(this->count)) == 0){
                        delete ptr;
                        delete count;
                    }
                    this->ptr = lhs.ptr;
                    this->count = lhs.count;
                    return *this;
                }

                T* operator->(){
                    return ptr;
                }
                T& operator*(){
                    return *ptr;
                }
                int getcount(){
                    return *count;
                }
        };
}

int main(){
    
    smart_pointer::shared_ptr<int> sp(new int(2));
    smart_pointer::shared_ptr<int> sp2(sp);
    cout << sp2.getcount() << endl;
    sp = new int(10);
    cout << sp2.getcount() << endl;
    cout << *sp << endl;
    return 0;
}
