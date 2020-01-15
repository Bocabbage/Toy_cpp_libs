/*
    Project:        toylist_test
    Update date:    2020/1/15
    Author:         Zhuofan Zhang
*/
#include"toylist.hpp"
// #include<vector>
using toy_std::tlist;
using std::cout;
using std::endl;
// using std::vector;

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

void ElementAccess()
{
    cout << "**** ElementAccess Check ****" << endl;
    tlist<int> TestList = { 1,2,3,4,5,6 };
    cout << "front: " << TestList.front() << endl;
    cout << "back: " << TestList.back() << endl;
    cout << "*****************************" << endl;
}

void Iterators()
{
    cout << "**** Iterators Check ****" << endl;

    tlist<char> TestList = { 'H','e','l','l','o' };
    cout << "TestList: ";
    for(auto it = TestList.begin(); it != TestList.end(); ++it)
        cout << *it << ' ';
    cout << endl;
    cout << "Reverse TestList: ";
    for (auto rit = TestList.rbegin(); rit != TestList.rend(); ++rit)
        cout << *rit << ' ';
    cout << endl;

    cout << "*************************" << endl;
}

void Modifiers()
{
    cout << "**** Modifers Check ****" << endl;
    tlist<char> TestList = { 'H','e','l','l','o' };

    /* erase */
    cout << "Erase the first: ";
    TestList.erase(TestList.begin());
    for (auto it = TestList.begin(); it != TestList.end(); ++it)
        cout << *it << ' ';
    cout << endl;
    cout << "Erase the all(Size shoukd be 0): ";
    TestList.clear();
    cout << TestList.size() << endl;

    /* insert */
    cout << "Insert: ";
    auto _oit = TestList.insert(TestList.begin(), { 'o', ',', 'w', 'o', 'r', 'l', 'd' });
    TestList.insert(TestList.begin(), 2, 'l');
    TestList.insert(TestList.begin(), 'e');
    TestList.insert(TestList.begin(), 'H');
    for (auto it = TestList.begin(); it != TestList.end(); ++it)
        cout << *it << ' ';
    cout << endl << "pos check: " << *_oit;
    cout << endl;

    /* push_back/front */
    cout << "Push Back: ";
    TestList.push_back('!');
    for (auto it = TestList.begin(); it != TestList.end(); ++it)
        cout << *it;
    cout << endl << "Push front: ";
    TestList.push_front('H');
    for (auto it = TestList.begin(); it != TestList.end(); ++it)
        cout << *it;
    cout << endl;


    cout << "************************" << endl;
}

void Operations()
{
    cout << "**** Operations Check ****" << endl;
    tlist<int> A = { 3,7,5,9,1 };
    tlist<int> B = { 8,2,6,4,10 };
    
    /* sort */
    A.sort();
    B.sort();
    for (auto it = A.begin(); it != A.end(); ++it)
        cout << *it << ' ';
    cout << endl;
    for (auto it = B.begin(); it != B.end(); ++it)
        cout << *it << ' ';
    cout << endl;

    /* merge */
    A.merge(std::move(B));
    cout << "Merge (B size: " << B.size() << "): ";
    for (auto it = A.begin(); it != A.end(); ++it)
        cout << *it << ' ';
    cout << endl;

    /* remove */
    A.remove(5);
    cout << "Remove 5: ";
    for (auto it = A.begin(); it != A.end(); ++it)
        cout << *it << ' ';
    cout << endl;

    /* reverse */
    A.reverse();
    cout << "Reverse: ";
    for (auto it = A.begin(); it != A.end(); ++it)
        cout << *it << ' ';
    cout << endl;

    /* unique */
    tlist<int> C = { 1,1,2,2,3,3,4,4 };
    C.unique();
    cout << "Unique: ";
    for (auto it = C.begin(); it != C.end(); ++it)
        cout << *it << ' ';
    cout << endl;

    cout << "**************************" << endl;

}

int main()
{
    ConstructorTest();
    ElementAccess();
    Iterators();
    Modifiers();
    Operations();
}