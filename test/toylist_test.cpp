/*
    Project:        toylist_test
    Update date:    2020/1/14
    Author:         Zhuofan Zhang
*/
#include"toylist.hpp"
#include<vector>
using toy_std::tlist;
using std::cout;
using std::endl;
using std::vector;

void ConstructorTest()
{   
    tlist<int> Default;
    tlist<float> InitList({ 1,2,3,4,5 });
    tlist<int> SameValue(5, 1);
    tlist<float> Copy(InitList);
    tlist<float> FromOther(InitList.begin(), InitList.end());

    cout << "**** Constructors Check ****" << endl;

    cout << "Default Size: " << Default.size() << endl;

    cout << "InitList: ";
    for (auto idx = InitList.begin(); idx != InitList.end(); ++idx)
        cout << *idx << ' ';
    cout << endl;
    
    cout << "SameValue: ";
    for (auto idx = SameValue.begin(); idx != SameValue.end(); ++idx)
        cout << *idx << ' ';
    cout << endl;

    cout << "Copy: ";
    for (auto idx = Copy.begin(); idx != Copy.end(); ++idx)
        cout << *idx << ' ';
    cout << endl;

    cout << "FromOther: ";
    for (auto idx = FromOther.begin(); idx != FromOther.end(); ++idx)
        cout << *idx << ' ';
    cout << endl;

    cout << "****************************" << endl;
}

int main()
{
    ConstructorTest();
}