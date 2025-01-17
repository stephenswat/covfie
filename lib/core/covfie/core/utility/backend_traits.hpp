/*
 * This file is part of covfie, a part of the ACTS project
 *
 * Copyright (c) 2023 CERN
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <variant>
#include <cstddef>

namespace covfie::utility {
template <typename B, std::size_t N, bool = B::is_initial>
struct nth_backend {
    using type = typename nth_backend<typename B::backend_t, N - 1>::type;
};

template <typename B>
struct nth_backend<B, 0, false> {
    using type = B;
};

template <typename B>
struct nth_backend<B, 0, true> {
    using type = B;
};

template <typename B, std::size_t N>
struct nth_backend<B, N, true> {
    using type = std::monostate;
};

template <typename B, bool = B::is_initial>
struct backend_depth {
};

template <typename B>
struct backend_depth<B, true> {
    static constexpr std::size_t value = 1;
};

template <typename B>
struct backend_depth<B, false> {
    static constexpr std::size_t value =
        backend_depth<typename B::backend_t>::value + 1;
};
}
