#include <iostream>
#include <utility>
using namespace std;
class object{
private:
    string st;
public:
    explicit object(string str):st(std::move(str)){}
    friend ostream&operator<<(ostream& os,const  object& obj){
        os << obj.st << " ";
        return os;
    }
};
class class1{
public:
    object* obj;
public:
    explicit class1(string str){
        obj = new object(std::move(str));
    }
    friend ostream&operator<<(ostream& os,const  class1& class11){
        os << *class11.obj << " ";
        return os;
    }
};
class class2{
private:
    object* obj;
public:
    explicit class2(class1* obj):obj(obj->obj){}
    friend ostream&operator<<(ostream& os,const  class2& class12){
        os << *class12.obj << " ";
        return os;
    }
};
int main(int n, char** c) {
    class1 cl("hello");
    class2* cls2 = new class2(&cl);
    cout << *cls2;
    return 0;
}