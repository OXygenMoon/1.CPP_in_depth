/************************************************************************
    > File Name: 01.cpp
    > Author: Lao Zhenyu
    > Mail: LaoZhenyu_961112@163.com 
    > Created Time: 五  3/ 5 13:17:03 2021
************************************************************************/
#include <iostream>

using namespace std;

class bar{
    public:
        bar() { cout << "called construct function." << endl; }
        ~bar(){ cout << "called destruct function." << endl; }
};

void foo() {
    bar *ptr = new bar();
    exit(1);                    //<--- 异常或者终止会导致内存泄漏,无法释放掉ptr所指向的内存空间
    delete ptr;
}

int main(int argc, char *argv[])
{
    foo();
    return 0;
}
