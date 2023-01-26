#pragma once

template <typename T>
concept is_streamable = requires (std::ostream os, T t) {
    os << t;
};
