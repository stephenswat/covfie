/*
 * SPDX-PackageName: "covfie, a part of the ACTS project"
 * SPDX-FileCopyrightText: 2022 CERN
 * SPDX-License-Identifier: MPL-2.0
 */

#include <cstddef>
#include <sstream>

#include <gtest/gtest.h>

#include <covfie/core/backend/primitive/identity.hpp>
#include <covfie/core/backend/transformer/clamp.hpp>
#include <covfie/core/field.hpp>
#include <covfie/core/parameter_pack.hpp>

TEST(TestTransformerClamp, ClampIdentityInt1D)
{
    using field_t = covfie::field<covfie::backend::clamp<
        covfie::backend::identity<covfie::vector::int1>>>;

    field_t f(covfie::make_parameter_pack(
        field_t::backend_t::configuration_t{{0}, {5}},
        field_t::backend_t::backend_t::configuration_t{}
    ));

    field_t::view_t fv(f);

    EXPECT_EQ(fv.at(-10)[0], 0);
    EXPECT_EQ(fv.at(-1)[0], 0);
    EXPECT_EQ(fv.at(0)[0], 0);
    EXPECT_EQ(fv.at(1)[0], 1);
    EXPECT_EQ(fv.at(3)[0], 3);
    EXPECT_EQ(fv.at(5)[0], 5);
    EXPECT_EQ(fv.at(6)[0], 5);
    EXPECT_EQ(fv.at(10)[0], 5);
}

TEST(TestTransformerClamp, ClampIdentityInt2D)
{
    using field_t = covfie::field<covfie::backend::clamp<
        covfie::backend::identity<covfie::vector::int2>>>;

    field_t f(covfie::make_parameter_pack(
        field_t::backend_t::configuration_t{{0, 4}, {5, 8}},
        field_t::backend_t::backend_t::configuration_t{}
    ));

    field_t::view_t fv(f);

    EXPECT_EQ(fv.at(-10, -10)[0], 0);
    EXPECT_EQ(fv.at(-10, -10)[1], 4);
    EXPECT_EQ(fv.at(-1, -10)[0], 0);
    EXPECT_EQ(fv.at(-1, -10)[1], 4);
    EXPECT_EQ(fv.at(0, -10)[0], 0);
    EXPECT_EQ(fv.at(0, -10)[1], 4);
    EXPECT_EQ(fv.at(1, -10)[0], 1);
    EXPECT_EQ(fv.at(1, -10)[1], 4);
    EXPECT_EQ(fv.at(2, 5)[0], 2);
    EXPECT_EQ(fv.at(2, 5)[1], 5);
}

TEST(TestTransformerClamp, WriteReadClampIdentityInt2D)
{
    using field_t = covfie::field<covfie::backend::clamp<
        covfie::backend::identity<covfie::vector::int2>>>;

    field_t f(covfie::make_parameter_pack(
        field_t::backend_t::configuration_t{{0, 4}, {5, 8}},
        field_t::backend_t::backend_t::configuration_t{}
    ));

    std::stringstream ss;

    f.dump(ss);

    field_t nf(ss);

    /*
     * The bounds are stored in the file, so they must survive the round trip
     * and the deserialized field must clamp in the same way.
     */
    field_t::backend_t::configuration_t conf = nf.backend().get_configuration();

    EXPECT_EQ(conf.min[0], 0);
    EXPECT_EQ(conf.min[1], 4);
    EXPECT_EQ(conf.max[0], 5);
    EXPECT_EQ(conf.max[1], 8);

    field_t::view_t nfv(nf);

    EXPECT_EQ(nfv.at(-10, -10)[0], 0);
    EXPECT_EQ(nfv.at(-10, -10)[1], 4);
    EXPECT_EQ(nfv.at(2, 5)[0], 2);
    EXPECT_EQ(nfv.at(2, 5)[1], 5);
    EXPECT_EQ(nfv.at(10, 10)[0], 5);
    EXPECT_EQ(nfv.at(10, 10)[1], 8);
}
