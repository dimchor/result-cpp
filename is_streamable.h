#pragma once

namespace dlight {

template <typename T>
concept is_streamable = requires (std::ostream os, T t) {
    os << t;
};

}
