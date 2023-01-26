// -std=c++20 -fno-exceptions -fno-unwind-tables -fno-rtti

#include <iostream>
#include <string_view>

#include "result.h"

using Error = std::string_view;

struct NonStreamableStruct {};

struct StreamableStruct {
    friend std::ostream& operator<<(std::ostream& os, StreamableStruct const& obj) {
        os << "StreamableStruct";
        return os;
    }
};

Result<int, Error> get_int() {
    int a;
    std::cin >> a;
    return a;
}

int main() {

    //Result<int, StreamableStruct> test = StreamableStruct();

    //test.unpack();

    int b = 0;
    auto result = get_int()
        .then([] (int a) -> Result<int, Error> {
            return a * 2;
        })
        .then([b] (int a) -> Result<int, Error> {
            if (b == 0) {
                return { "Division by zero" };
            }
            return a / b;
        });

    if (result.ok()) {
        std::cout << result.get_value() << '\n';
    } else {
        std::cout << result.get_error() << '\n';
    }
 
    return 0;
}
