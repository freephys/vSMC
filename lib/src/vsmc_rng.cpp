//============================================================================
// vSMC/lib/src/vsmc_rng.cpp
//----------------------------------------------------------------------------
//                         vSMC: Scalable Monte Carlo
//----------------------------------------------------------------------------
// Copyright (c) 2013-2016, Yan Zhou
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
//   Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//============================================================================

#include <vsmc/rng/rng.hpp>
#include <vsmc/vsmc.h>
#include "vsmc_rng_cast.hpp"

#define VSMC_DEFINE_RNG_DIST                                                  \
    ::vsmc::RNG &rng = ::vsmc::internal::rng_cast(rng_ptr);                   \
    for (int i = 0; i != n; ++i)                                              \
        r[i] = dist(rng);

#define VSMC_DEFINE_RNG_DIST_0(name)                                          \
    void vsmc_rng_##name(vsmc_rng *rng_ptr, int n, double *r)                 \
    {                                                                         \
        ::vsmc::RNG &rng = ::vsmc::internal::rng_cast(rng_ptr);               \
        ::vsmc::name##_distribution<double>(                                  \
            rng, static_cast<std::size_t>(n), r);                             \
    }

#define VSMC_DEFINE_RNG_DIST_1(name)                                          \
    void vsmc_rng_##name(vsmc_rng *rng_ptr, int n, double *r, double p1)      \
    {                                                                         \
        ::vsmc::RNG &rng = ::vsmc::internal::rng_cast(rng_ptr);               \
        ::vsmc::name##_distribution<double>(                                  \
            rng, static_cast<std::size_t>(n), r, p1);                         \
    }

#define VSMC_DEFINE_RNG_DIST_2(name)                                          \
    void vsmc_rng_##name(                                                     \
        vsmc_rng *rng_ptr, int n, double *r, double p1, double p2)            \
    {                                                                         \
        ::vsmc::RNG &rng = ::vsmc::internal::rng_cast(rng_ptr);               \
        ::vsmc::name##_distribution<double>(                                  \
            rng, static_cast<std::size_t>(n), r, p1, p2);                     \
    }

extern "C" {

int vsmc_rng_size() { return sizeof(::vsmc::RNG) + 4; }

void vsmc_rng_init(vsmc_rng *rng_ptr, int seed)
{
    ::vsmc::RNG &rng = ::vsmc::internal::rng_cast(rng_ptr);
    rng = ::vsmc::RNG(static_cast<::vsmc::RNG::result_type>(seed));
}

void vsmc_rng_seed(vsmc_rng *rng_ptr, int seed)
{
    ::vsmc::RNG &rng = ::vsmc::internal::rng_cast(rng_ptr);
    rng.seed(static_cast<::vsmc::RNG::result_type>(seed));
}

void vsmc_rng_get_key(const vsmc_rng *rng_ptr, int n, int *key)
{
    const ::vsmc::RNG &rng = ::vsmc::internal::rng_cast(rng_ptr);
    typename ::vsmc::RNG::key_type k(rng.key());
    std::size_t dst_size = static_cast<std::size_t>(n) * sizeof(int);
    std::size_t src_size = sizeof(typename ::vsmc::RNG::key_type);
    std::size_t size = src_size < dst_size ? src_size : dst_size;
    std::memcpy(key, k.data(), size);
}

void vsmc_rng_set_key(vsmc_rng *rng_ptr, int n, const int *key)
{
    ::vsmc::RNG &rng = ::vsmc::internal::rng_cast(rng_ptr);
    typename ::vsmc::RNG::key_type k;
    std::size_t src_size = static_cast<std::size_t>(n) * sizeof(int);
    std::size_t dst_size = sizeof(typename ::vsmc::RNG::key_type);
    std::size_t size = src_size < dst_size ? src_size : dst_size;
    std::memcpy(k.data(), key, size);
    rng.key(k);
}

void vsmc_rng_get_ctr(const vsmc_rng *rng_ptr, int n, int *ctr)
{
    const ::vsmc::RNG &rng = ::vsmc::internal::rng_cast(rng_ptr);
    typename ::vsmc::RNG::ctr_type c(rng.ctr());
    std::size_t dst_size = static_cast<std::size_t>(n) * sizeof(int);
    std::size_t src_size = sizeof(typename ::vsmc::RNG::ctr_type);
    std::size_t size = src_size < dst_size ? src_size : dst_size;
    std::memcpy(ctr, c.data(), size);
}

void vsmc_rng_set_ctr(vsmc_rng *rng_ptr, int n, const int *ctr)
{
    ::vsmc::RNG &rng = ::vsmc::internal::rng_cast(rng_ptr);
    typename ::vsmc::RNG::ctr_type c;
    std::size_t src_size = static_cast<std::size_t>(n) * sizeof(int);
    std::size_t dst_size = sizeof(typename ::vsmc::RNG::ctr_type);
    std::size_t size = src_size < dst_size ? src_size : dst_size;
    std::memcpy(c.data(), ctr, size);
    rng.ctr(c);
}

void vsmc_rng_rand(vsmc_rng *rng_ptr, int n, int *r)
{
    ::vsmc::RNG &rng = ::vsmc::internal::rng_cast(rng_ptr);
    ::vsmc::rng_rand(rng, static_cast<std::size_t>(n),
        reinterpret_cast<::vsmc::RNG::result_type *>(r));
}

void vsmc_rng_u01_sorted(int n, const double *u01, double *u01seq)
{
    ::vsmc::u01_sorted(static_cast<std::size_t>(n), u01, u01seq);
}

void vsmc_rng_u01_stratified(int n, const double *u01, double *u01seq)
{
    ::vsmc::u01_stratified(static_cast<std::size_t>(n), u01, u01seq);
}

void vsmc_rng_u01_systematic(int n, double u01, double *u01seq)
{
    ::vsmc::u01_systematic(static_cast<std::size_t>(n), u01, u01seq);
}

void vsmc_rng_discrete(vsmc_rng *rng_ptr, int n, int *r, int m,
    const double *weight, int normalized)
{
    ::vsmc::DiscreteDistribution<int> dist;
    ::vsmc::RNG &rng = ::vsmc::internal::rng_cast(rng_ptr);
    bool norm = normalized != 0;
    const double *first = weight;
    const double *last = weight + m;
    for (int i = 0; i != n; ++i)
        r[i] = dist(rng, first, last, norm);
}

void vsmc_rng_uniform_int(vsmc_rng *rng_ptr, int n, int *r, int a, int b)
{
    std::uniform_int_distribution<int> dist(a, b);
    VSMC_DEFINE_RNG_DIST;
}

void vsmc_rng_bernoulli(vsmc_rng *rng_ptr, int n, int *r, double p)
{
    std::bernoulli_distribution dist(p);
    VSMC_DEFINE_RNG_DIST;
}

void vsmc_rng_binomial(vsmc_rng *rng_ptr, int n, int *r, int t, double p)
{
    std::binomial_distribution<int> dist(t, p);
    VSMC_DEFINE_RNG_DIST;
}

void vsmc_rng_negative_binomial(
    vsmc_rng *rng_ptr, int n, int *r, int k, double p)
{
    std::negative_binomial_distribution<int> dist(k, p);
    VSMC_DEFINE_RNG_DIST;
}

void vsmc_rng_geometric(vsmc_rng *rng_ptr, int n, int *r, double p)
{
    std::geometric_distribution<int> dist(p);
    VSMC_DEFINE_RNG_DIST;
}

void vsmc_rng_poisson(vsmc_rng *rng_ptr, int n, int *r, double mean)
{
    std::poisson_distribution<int> dist(mean);
    VSMC_DEFINE_RNG_DIST;
}

void vsmc_rng_normal_mv(vsmc_rng *rng_ptr, int n, double *r, int dim,
    const double *mean, const double *chol)
{
    ::vsmc::RNG &rng = ::vsmc::internal::rng_cast(rng_ptr);
    ::vsmc::normal_mv_distribution<double>(rng, static_cast<std::size_t>(n), r,
        static_cast<std::size_t>(dim), mean, chol);
}

VSMC_DEFINE_RNG_DIST_0(u01)
VSMC_DEFINE_RNG_DIST_1(chi_squared)
VSMC_DEFINE_RNG_DIST_1(exponential)
VSMC_DEFINE_RNG_DIST_1(rayleigh)
VSMC_DEFINE_RNG_DIST_1(student_t)
VSMC_DEFINE_RNG_DIST_2(beta)
VSMC_DEFINE_RNG_DIST_2(cauchy)
VSMC_DEFINE_RNG_DIST_2(extreme_value)
VSMC_DEFINE_RNG_DIST_2(fisher_f)
VSMC_DEFINE_RNG_DIST_2(gamma)
VSMC_DEFINE_RNG_DIST_2(laplace)
VSMC_DEFINE_RNG_DIST_2(levy)
VSMC_DEFINE_RNG_DIST_2(logistic)
VSMC_DEFINE_RNG_DIST_2(lognormal)
VSMC_DEFINE_RNG_DIST_2(normal)
VSMC_DEFINE_RNG_DIST_2(pareto)
VSMC_DEFINE_RNG_DIST_2(uniform_real)
VSMC_DEFINE_RNG_DIST_2(weibull)

int vsmc_random_walk(vsmc_rng *rng_ptr, int dim, double *x, double *ltx,
    double (*log_target)(int, const double *),
    double (*proposal)(vsmc_rng *, int, const double *, double *))
{
    ::vsmc::RNG &rng = ::vsmc::internal::rng_cast(rng_ptr);
    ::vsmc::RandomWalk<double, ::vsmc::Dynamic> rw(
        static_cast<std::size_t>(dim));

    auto lt = [log_target, dim](
        std::size_t, const double *lx) { return log_target(dim, lx); };

    auto prop = [proposal, rng_ptr, dim](::vsmc::RNG &, std::size_t,
        const double *px,
        double *py) { return proposal(rng_ptr, dim, px, py); };

    return static_cast<int>(rw(rng, x, ltx, lt, prop));
}

int vsmc_random_walk_g(vsmc_rng *rng_ptr, int dim_x, int dim_g, double *x,
    double *ltx, double *g,
    double (*log_target)(int, int, const double *, double *),
    double (*proposal)(vsmc_rng *, int, const double *, double *))
{
    ::vsmc::RNG &rng = ::vsmc::internal::rng_cast(rng_ptr);
    ::vsmc::RandomWalkG<double, vsmc::Dynamic, ::vsmc::Dynamic> rw(
        static_cast<std::size_t>(dim_x), static_cast<std::size_t>(dim_g));

    auto lt = [log_target, dim_x, dim_g](std::size_t, std::size_t,
        const double *lx,
        double *lg) { return log_target(dim_x, dim_g, lx, lg); };

    auto prop = [proposal, rng_ptr, dim_x](::vsmc::RNG &, std::size_t,
        const double *px,
        double *py) { return proposal(rng_ptr, dim_x, px, py); };

    return static_cast<int>(rw(rng, x, ltx, g, lt, prop));
}

double vsmc_normal_proposal(vsmc_rng *rng_ptr, int, const double *x,
    double *y, double stddev, double a, double b)
{
    ::vsmc::RNG &rng = ::vsmc::internal::rng_cast(rng_ptr);
    ::vsmc::NormalProposal<double> prop(stddev, a, b);

    return prop(rng, 1, x, y);
}

double vsmc_normal_mv_proposal(vsmc_rng *rng_ptr, int dim, const double *x,
    double *y, const double *chol, const double *a, const double *b)
{
    ::vsmc::RNG &rng = ::vsmc::internal::rng_cast(rng_ptr);
    ::vsmc::NormalMVProposal<double, vsmc::Dynamic> prop(
        static_cast<std::size_t>(dim), chol, a, b);

    return prop(rng, static_cast<std::size_t>(dim), x, y);
}

} // extern "C"
