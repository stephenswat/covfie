/*
 * SPDX-PackageName: "covfie, a part of the ACTS project"
 * SPDX-FileCopyrightText: 2022 CERN
 * SPDX-License-Identifier: MPL-2.0
 */

#include <cstddef>
#include <sstream>
#include <utility>

#include <gtest/gtest.h>

#include <covfie/core/backend/primitive/identity.hpp>
#include <covfie/core/backend/transformer/shuffle.hpp>
#include <covfie/core/field.hpp>
#include <covfie/core/parameter_pack.hpp>

namespace {
/*
 * The shuffle backend permutes the coordinate before it hands the coordinate
 * to the backend that it wraps, so element n of the permuted coordinate is
 * element Is[n] of the original one. Over an identity field this means that
 * a lookup at (x, y, z) through <1, 2, 0> gives (y, z, x).
 */
using field_t = covfie::field<covfie::backend::shuffle<
    covfie::backend::identity<covfie::vector::int3>,
    std::index_sequence<1, 2, 0>>>;

field_t make_field()
{
    return field_t(covfie::make_parameter_pack(
        field_t::backend_t::configuration_t{},
        field_t::backend_t::backend_t::configuration_t{}
    ));
}
}

TEST(TestTransformerShuffle, PermutesTheCoordinate)
{
    field_t f = make_field();
    field_t::view_t fv(f);

    for (int x = -4; x < 4; ++x) {
        for (int y = -4; y < 4; ++y) {
            for (int z = -4; z < 4; ++z) {
                EXPECT_EQ(fv.at(x, y, z)[0], y);
                EXPECT_EQ(fv.at(x, y, z)[1], z);
                EXPECT_EQ(fv.at(x, y, z)[2], x);
            }
        }
    }
}

TEST(TestTransformerShuffle, WriteRead)
{
    field_t f = make_field();

    std::stringstream ss;

    f.dump(ss);

    field_t nf(ss);
    field_t::view_t nfv(nf);

    for (int x = -4; x < 4; ++x) {
        for (int y = -4; y < 4; ++y) {
            for (int z = -4; z < 4; ++z) {
                EXPECT_EQ(nfv.at(x, y, z)[0], y);
                EXPECT_EQ(nfv.at(x, y, z)[1], z);
                EXPECT_EQ(nfv.at(x, y, z)[2], x);
            }
        }
    }
}
