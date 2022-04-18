/*
 * @Author: dylan
 * @Date: 2022-04-05 15:02:48
 * @LastEditTime: 2022-04-05 15:06:16
 * @LastEditors: dylan
 * @Description: 
 * @FilePath: \undefinede:\Personal\Workspaces\PyEditor\PyEditor\test\test.cc
 */
#include <iostream>

using namespace std;

class Test {
public:
    int a = 1;
    int b = 2;
}

class TestB : public Test {
public:
    cout << a << "\n";
    a = b;
    cout << a << "\n";
}

int main()
{
    TestB obj;

    return 0;
}



