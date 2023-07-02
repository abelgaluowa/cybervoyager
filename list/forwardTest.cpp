/// test std::forward<T>(t);
#include <iostream>

using namespace std;

struct A {
    A() = default;
    ~A() = default;
    A(A&& ){ cout << "pr" << endl; }
    A(const A& ){ cout << "cr" << endl; }
};

template<typename T>
struct B {
    template<typename D>
    void insert(D&& t){
        T* pn = new T(std::forward<D>(t));
    }
};

int main()
{
    B<A> ba;
    A a;
    ba.insert(a);
    ba.insert(A());

    return 0;
}
