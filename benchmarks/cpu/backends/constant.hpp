/*
 * This file is part of covfie, a part of the ACTS project
 *
 * Copyright (c) 2022 CERN
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <covfie/core/backend/primitive/constant.hpp>
#include <covfie/core/field.hpp>
#include <covfie/core/parameter_pack.hpp>
#include <covfie/core/vector.hpp>

template <typename S, std::size_t N, std::size_t M>
struct Constant {
    using backend_t = covfie::backend::constant<
        covfie::vector::vector_d<S, N>,
        covfie::vector::vector_d<S, M>>;

    static covfie::field<backend_t> get_field()
    {
        return covfie::field<backend_t>(covfie::make_parameter_pack(
            typename backend_t::configuration_t{0.f}
        ));
    }
};
