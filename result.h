#pragma once

#include <iostream>
#include <cstdlib>
#include <functional>
#include <type_traits>

#include "is_streamable.h"

template<typename T, typename E>
class [[nodiscard]] Result {
public:
    Result(T t) noexcept : data(std::move(t)), type(Type::TypeT) {}
    Result(E e) noexcept : data(std::move(e)), type(Type::TypeE) {}

    Result(Result const&) = delete;
    Result& operator=(Result const&) = delete;
    Result(Result&&) = default;
    Result& operator=(Result&&) = delete;

    ~Result() { delete_data(); };

    bool ok() const noexcept { return type == Type::TypeT; }

    T unpack() const noexcept {
        if (ok()) {
            return data.t;
        }

        std::cerr << "Failed to unpack Result: ";
        if constexpr (is_streamable<E>) {
            std::cerr << data.e << '\n';
        } else {
            std::cerr << "No further information available.\n";
        }
        std::exit(EXIT_FAILURE);
    }

    Result then(std::function<Result(T)> const& f) const noexcept {
        if(ok()) {
            return f(std::move(data.t));
        }
        return data.e;
    }

    T const& get_value() const noexcept { return data.t; }
    E const& get_error() const noexcept { return data.e; }

private:
    union Data {
        T t;
        E e;

        Data(T t) noexcept : t(std::move(t)) {}
        Data(E e) noexcept : e(std::move(e)) {}
        
        ~Data() {}
    } const data {};
    enum class Type { None, TypeT, TypeE } const type { Type::None };

    void delete_data() noexcept {
        switch(type) {
            case Type::TypeT:
                data.t.~T();
                break;
            case Type::TypeE:
                data.e.~E();
                break;
            default:
                break;
        }
    }
};
