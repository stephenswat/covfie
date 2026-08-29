/*
 * SPDX-PackageName: "covfie, a part of the ACTS project"
 * SPDX-FileCopyrightText: 2022 CERN
 * SPDX-License-Identifier: MPL-2.0
 */

#include <cstddef>
#include <type_traits>

#include <gtest/gtest.h>

#include <covfie/core/backend/primitive/array.hpp>
#include <covfie/core/backend/transformer/morton.hpp>
#include <covfie/core/backend/transformer/strided.hpp>
#include <covfie/core/field.hpp>
#include <covfie/core/parameter_pack.hpp>

namespace {
using array_t = covfie::backend::array<covfie::vector::float2>;
using strided_t = covfie::backend::strided<covfie::vector::size2, array_t>;
using morton_bmi2_t =
    covfie::backend::morton<covfie::vector::size2, array_t, true>;
using morton_naive_t =
    covfie::backend::morton<covfie::vector::size2, array_t, false>;

/*
 * Both Morton backends must name themselves, including the flag which
 * selects the index calculation. If they do not, a field built on the naive
 * backend claims to be a field built on the BMI2 backend, and any layout
 * conversion out of it picks the wrong index calculation.
 */
static_assert(std::is_same_v<
              morton_bmi2_t::owning_data_t::parent_t,
              morton_bmi2_t>);
static_assert(std::is_same_v<
              morton_bmi2_t::non_owning_data_t::parent_t,
              morton_bmi2_t>);
static_assert(std::is_same_v<
              morton_naive_t::owning_data_t::parent_t,
              morton_naive_t>);
static_assert(std::is_same_v<
              morton_naive_t::non_owning_data_t::parent_t,
              morton_naive_t>);

covfie::field<strided_t> make_strided_field()
{
    covfie::field<strided_t> f(
        covfie::make_parameter_pack(strided_t::configuration_t{4ul, 4ul})
    );
    covfie::field<strided_t>::view_t fv(f);

    for (std::size_t x = 0ul; x < 4ul; ++x) {
        for (std::size_t y = 0ul; y < 4ul; ++y) {
            fv.at(x, y)[0] = static_cast<float>(x);
            fv.at(x, y)[1] = static_cast<float>(y);
        }
    }

    return f;
}

/*
 * Convert a strided field to a Morton field, then convert it back, and check
 * that every element survives the round trip.
 */
template <typename morton_t>
void check_round_trip()
{
    covfie::field<strided_t> s = make_strided_field();
    covfie::field<morton_t> m(typename morton_t::owning_data_t(s.backend()));
    covfie::field<strided_t> r(strided_t::owning_data_t(m.backend()));

    typename covfie::field<morton_t>::view_t mv(m);
    covfie::field<strided_t>::view_t rv(r);

    for (std::size_t x = 0ul; x < 4ul; ++x) {
        for (std::size_t y = 0ul; y < 4ul; ++y) {
            EXPECT_EQ(mv.at(x, y)[0], static_cast<float>(x));
            EXPECT_EQ(mv.at(x, y)[1], static_cast<float>(y));
            EXPECT_EQ(rv.at(x, y)[0], static_cast<float>(x));
            EXPECT_EQ(rv.at(x, y)[1], static_cast<float>(y));
        }
    }
}
}

TEST(TestMortonLayout, RoundTrip2DBmi2)
{
    check_round_trip<morton_bmi2_t>();
}

TEST(TestMortonLayout, RoundTrip2DNaive)
{
    check_round_trip<morton_naive_t>();
}
