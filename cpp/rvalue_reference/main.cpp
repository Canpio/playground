// g++ main.cpp -std=c++11 -fno-elide-constructors

#include <iostream>
#include <string>

int copy_count = 0;
int make_count = 0;
int move_count = 0;

struct Int {
    // constructor
    Int(int v) : _p_value(new int(v)) {
        ++make_count;
        std::cout << "new: " << *_p_value << std::endl;
    }
    // copy constructor, deep copy
    Int(const Int &b) : _p_value(new int(*b._p_value)) {
        ++copy_count;
        std::cout << "copy" << std::endl;
    }
    // move constructor
    // rvalue copy will use this constructor first
    // after this, rvalue can no more be used
    Int(Int &&b) {
        ++move_count;
        _p_value = b._p_value;
        b._p_value = nullptr;
        std::cout << "move" << std::endl;
    }

    ~Int() { delete _p_value; }

    Int operator+(const Int &b) {
        Int res(*_p_value + *b._p_value);
        return res;
    }

    int *_p_value;
};

int main() {
    Int a(1);
    Int b(2);
    Int c(4);
    Int &&res = a + b + c;
    Int res2 = std::move(res);
    std::cout << *res2._p_value << std::endl;
    std::cout << "make count: " << make_count << std::endl;
    std::cout << "copy count: " << copy_count << std::endl;
    std::cout << "move count: " << move_count << std::endl;
    return 0;
}
