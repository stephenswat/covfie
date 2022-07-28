/*
 * This file is part of covfie, a part of the ACTS project
 *
 * Copyright (c) 2022 CERN
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <random>

#include <covfie/benchmark/pattern.hpp>
#include <covfie/core/field_view.hpp>

struct LorentzEulerWide : covfie::benchmark::AccessPattern<LorentzEulerWide> {
    struct parameters {
        std::size_t particles, steps;
    };

    static constexpr std::array<std::string_view, 2> parameter_names = {
        "N", "S"};

    template <typename backend_t>
    static void iteration(
        const parameters & p,
        const covfie::field_view<backend_t> & f,
        ::benchmark::State & state
    )
    {
        state.PauseTiming();

        std::random_device rd;
        std::mt19937 e(rd());

        std::uniform_real_distribution<> phi_dist(0.f, 2.f * 3.1415927f);
        std::uniform_real_distribution<> costheta_dist(-1.f, 1.f);

        float ss = 0.001f;

        std::vector<covfie::benchmark::lorentz_agent<3>> objs(p.particles);

        for (std::size_t i = 0; i < p.particles; ++i) {
            float theta = std::acos(costheta_dist(e));
            float phi = phi_dist(e);

            objs[i].pos[0] = 0.f;
            objs[i].pos[1] = 0.f;
            objs[i].pos[2] = 0.f;

            objs[i].mom[0] = 100.f * std::sin(theta) * std::cos(phi);
            objs[i].mom[1] = 100.f * std::sin(theta) * std::sin(phi);
            objs[i].mom[2] = 100.f * std::cos(theta);
        }

        state.ResumeTiming();

        for (std::size_t s = 0; s < p.steps; ++s) {
            for (std::size_t i = 0; i < p.particles; ++i) {
                covfie::benchmark::lorentz_agent<3> & o = objs[i];
                typename std::decay_t<decltype(f)>::output_t b =
                    f.at(o.pos[0], o.pos[1], o.pos[2]);
                float f[3] = {
                    o.mom[1] * b[2] - o.mom[2] * b[1],
                    o.mom[2] * b[0] - o.mom[0] * b[2],
                    o.mom[0] * b[1] - o.mom[1] * b[0]};

                o.pos[0] += o.mom[0] * ss;
                o.pos[1] += o.mom[1] * ss;
                o.pos[2] += o.mom[2] * ss;

                o.mom[0] += f[0] * ss;
                o.mom[1] += f[1] * ss;
                o.mom[2] += f[2] * ss;
            }
        }
    }

    static std::vector<std::vector<int64_t>> get_parameter_ranges()
    {
        return {{1024, 2048, 4096, 8192}, {1024, 2048, 4096}};
    }

    static parameters get_parameters(benchmark::State & state)
    {
        return {
            static_cast<std::size_t>(state.range(0)),
            static_cast<std::size_t>(state.range(1))};
    }

    template <typename S, std::size_t N>
    static covfie::benchmark::counters get_counters(const parameters & p)
    {
        return {
            p.particles * p.steps,
            p.particles * p.steps * N * sizeof(S),
            p.particles * p.steps * 21};
    }
};

struct LorentzEulerDeep : covfie::benchmark::AccessPattern<LorentzEulerDeep> {
    struct parameters {
        std::size_t particles, steps, imom;
    };

    static constexpr std::array<std::string_view, 3> parameter_names = {
        "N", "S", "I"};

    template <typename backend_t>
    static void iteration(
        const parameters & p,
        const covfie::field_view<backend_t> & f,
        ::benchmark::State & state
    )
    {
        state.PauseTiming();

        std::random_device rd;
        std::mt19937 e(rd());

        std::uniform_real_distribution<> phi_dist(0.f, 2.f * 3.1415927f);
        std::uniform_real_distribution<> costheta_dist(-1.f, 1.f);

        float ss = 0.001f;

        std::vector<covfie::benchmark::lorentz_agent<3>> objs(p.particles);

        for (std::size_t i = 0; i < p.particles; ++i) {
            float theta = std::acos(costheta_dist(e));
            float phi = phi_dist(e);

            objs[i].pos[0] = 0.f;
            objs[i].pos[1] = 0.f;
            objs[i].pos[2] = 0.f;

            objs[i].mom[0] =
                static_cast<float>(p.imom) * std::sin(theta) * std::cos(phi);
            objs[i].mom[1] =
                static_cast<float>(p.imom) * std::sin(theta) * std::sin(phi);
            objs[i].mom[2] = static_cast<float>(p.imom) * std::cos(theta);
        }

        state.ResumeTiming();

        for (std::size_t i = 0; i < p.particles; ++i) {
            for (std::size_t s = 0; s < p.steps; ++s) {
                covfie::benchmark::lorentz_agent<3> & o = objs[i];
                typename std::decay_t<decltype(f)>::output_t b =
                    f.at(o.pos[0], o.pos[1], o.pos[2]);
                float f[3] = {
                    o.mom[1] * b[2] - o.mom[2] * b[1],
                    o.mom[2] * b[0] - o.mom[0] * b[2],
                    o.mom[0] * b[1] - o.mom[1] * b[0]};

                o.pos[0] += o.mom[0] * ss;
                o.pos[1] += o.mom[1] * ss;
                o.pos[2] += o.mom[2] * ss;

                if (__builtin_expect(
                        o.pos[0] < -10000.f || o.pos[0] > 10000.f ||
                            o.pos[1] < -10000.f || o.pos[1] > 10000.f ||
                            o.pos[2] < -15000.f || o.pos[2] > 15000.f,
                        0
                    ))
                {
                    if (o.pos[0] < -10000.f) {
                        o.pos[0] += 20000.f;
                    } else if (o.pos[0] > 10000.f) {
                        o.pos[0] -= 20000.f;
                    }

                    if (o.pos[1] < -10000.f) {
                        o.pos[1] += 20000.f;
                    } else if (o.pos[1] > 10000.f) {
                        o.pos[1] -= 20000.f;
                    }

                    if (o.pos[2] < -10000.f) {
                        o.pos[2] += 20000.f;
                    } else if (o.pos[2] > 10000.f) {
                        o.pos[2] -= 20000.f;
                    }
                }

                o.mom[0] += f[0] * ss;
                o.mom[1] += f[1] * ss;
                o.mom[2] += f[2] * ss;
            }
        }
    }

    static std::vector<std::vector<int64_t>> get_parameter_ranges()
    {
        return {
            {1, 4096},
            {1024, 2048, 4096, 8192, 16384, 32768, 65536},
            {128,   192,   256,   384,   512,    768,    1024,  1536,
             2048,  3072,  4096,  6144,  8192,   12288,  16384, 24576,
             32768, 49152, 65536, 98304, 131072, 196608, 262144}};
    }

    static parameters get_parameters(benchmark::State & state)
    {
        return {
            static_cast<std::size_t>(state.range(0)),
            static_cast<std::size_t>(state.range(1)),
            static_cast<std::size_t>(state.range(2))};
    }

    template <typename S, std::size_t N>
    static covfie::benchmark::counters get_counters(const parameters & p)
    {
        return {
            p.particles * p.steps,
            p.particles * p.steps * N * sizeof(S),
            p.particles * p.steps * 21};
    }
};
