#pragma once

#include <iostream>
#include <cstdlib>
#include <functional>
#include <type_traits>

#include "is_streamable.h"

namespace dlight {

template<typename T, typename E>
class [[nodiscard]] Result {
public:
    Result(T t) noexcept : t(std::move(t)), type(Type::TypeT) {}
    Result(E e) noexcept : e(std::move(e)), type(Type::TypeE) {}

    Result(Result const& other) : type(other.type) {
        if (&other == this) {
            return;
        }

        if (other.type == Type::TypeT) {
            new (&t) T(other.t);
        } else {
            new (&e) E(other.e);
        }
    }

    Result& operator=(Result const& other) {
        if (&other == this) {
            return *this;
        }
        delete_data();
        type = other.type;
        if (other.type == Type::TypeT) {
            new (&t) T(other.t);
        } else {
            new (&e) E(other.e);
        }
        return *this;
    }

    Result(Result&& other) : type(std::move(other.type)) {
        if (&other == this) {
            return;
        }
        if (other.type == Type::TypeT) {
            new (&t) T(std::move(other.t));
        } else {
            new (&e) E(std::move(other.e));
        }
    }

    Result& operator=(Result&& other) {
        if (&other == this) {
            return *this;
        }
        delete_data();
        type = std::move(other.type);
        if (other.type == Type::TypeT) {
            new (&t) T(std::move(other.t));
        } else {
            new (&e) E(std::move(other.e));
        }
        return *this;
    }

    ~Result() { delete_data(); };

    bool ok() const noexcept { return type == Type::TypeT; }

    T unpack() const noexcept {
        if (ok()) {
            return t;
        }

        std::cerr << "Failed to unpack Result: ";
        if constexpr (is_streamable<E>) {
            std::cerr << e << '\n';
        } else {
            std::cerr << "No further information available.\n";
        }
        std::exit(EXIT_FAILURE);
    }

    Result then(std::function<Result(T)> const& f) const noexcept {
        if(ok()) {
            return f(std::move(t));
        }
        return e;
    }

    T const& get_value() const noexcept { return t; }
    E const& get_error() const noexcept { return e; }

private:
    union {
        T t;
        E e;
    };
    enum class Type { TypeT, TypeE } type;

    void delete_data() noexcept {
        type == Type::TypeT ? t.~T() : e.~E();
    }
};

}
