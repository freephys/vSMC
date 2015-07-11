//============================================================================
// vSMC/include/vsmc/rng/gamma_distribution.hpp
//----------------------------------------------------------------------------
//                         vSMC: Scalable Monte Carlo
//----------------------------------------------------------------------------
// Copyright (c) 2013-2015, Yan Zhou
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

#ifndef VSMC_RNG_GAMMA_DISTRIBUTION_HPP
#define VSMC_RNG_GAMMA_DISTRIBUTION_HPP

#include <vsmc/rng/internal/common.hpp>
#include <vsmc/rng/u01_distribution.hpp>
#include <vsmc/rng/normal_distribution.hpp>

#define VSMC_RUNTIME_ASSERT_RNG_GAMMA_DISTRIBUTION_PARAM_ALPHA_CHECK(alpha)   \
    VSMC_RUNTIME_ASSERT((alpha > 0), "**GammaDistribution** CONSTRUCTED "     \
                                     "WITH INVALID SHAPE PARAMETER VALUE")

#define VSMC_RUNTIME_ASSERT_RNG_GAMMA_DISTRIBUTION_PARAM_BETA_CHECK(beta)     \
    VSMC_RUNTIME_ASSERT((beta > 0), "**GammaDistribution** CONSTRUCTED "      \
                                    "WITH INVALID SCALE PARAMETER VALUE")

#define VSMC_RUNTIME_ASSERT_RNG_GAMMA_DISTRIBUTION_PARAM_CHECK(alpha, beta)   \
    VSMC_RUNTIME_ASSERT_RNG_GAMMA_DISTRIBUTION_PARAM_ALPHA_CHECK(alpha);      \
    VSMC_RUNTIME_ASSERT_RNG_GAMMA_DISTRIBUTION_PARAM_BETA_CHECK(beta);

namespace vsmc
{

namespace internal
{

enum GammaDistributionAlgorithm {
    GammaDistributionAlgorithmT,
    GammaDistributionAlgorithmW,
    GammaDistributionAlgorithmN,
    GammaDistributionAlgorithmE
}; // enum GammaDistributionAlgorithm

template <typename RealType>
GammaDistributionAlgorithm gamma_distribution_algorithm(RealType alpha)
{
    if (alpha < 0.6)
        return GammaDistributionAlgorithmT;
    else if (alpha < 1)
        return GammaDistributionAlgorithmW;
    else if (alpha > 1)
        return GammaDistributionAlgorithmN;
    else
        return GammaDistributionAlgorithmE;
}

template <typename RealType>
class GammaDistributionConstant
{
    public:
    GammaDistributionConstant(RealType alpha)
        : algorithm(gamma_distribution_algorithm(alpha)), d(0), c(0)
    {
        switch (algorithm) {
            case GammaDistributionAlgorithmT:
                d = 1 - alpha;
                c = 1 / alpha;
                break;
            case GammaDistributionAlgorithmW:
                d = std::pow(alpha, alpha / (1 - alpha)) * (1 - alpha);
                c = 1 / alpha;
                break;
            case GammaDistributionAlgorithmN:
                d = alpha - static_cast<RealType>(1) / 3;
                c = 1 / (3 * std::sqrt(d));
                break;
            case GammaDistributionAlgorithmE: break;
        }
    }

    GammaDistributionAlgorithm algorithm;
    RealType d;
    RealType c;
}; // class GammaDistributionConstant

} // namespace internal

template <typename RealType, typename RNGType>
inline void gamma_distribution(
    RNGType &, std::size_t, RealType *, RealType, RealType);

/// \brief Gamma distribution
/// \ingroup Distribution
template <typename RealType>
class GammaDistribution
{
    public:
    using result_type = RealType;
    using distribution_type = GammaDistribution<RealType>;

    class param_type
    {
        public:
        using result_type = RealType;
        using distribution_type = GammaDistribution<RealType>;

        explicit param_type(result_type alpha = 1, result_type beta = 1)
            : alpha_(alpha), beta_(beta), d_(0), c_(0), constant_(alpha_)
        {
            invariant();
        }

        result_type alpha() const { return alpha_; }
        result_type beta() const { return beta_; }

        friend bool operator==(
            const param_type &param1, const param_type &param2)
        {
            if (!internal::is_equal(param1.alpha_, param2.alpha_))
                return false;
            if (!internal::is_equal(param1.beta_, param2.beta_))
                return false;
            return true;
        }

        friend bool operator!=(
            const param_type &param1, const param_type &param2)
        {
            return !(param1 == param2);
        }

        template <typename CharT, typename Traits>
        friend std::basic_ostream<CharT, Traits> &operator<<(
            std::basic_ostream<CharT, Traits> &os, const param_type &param)
        {
            if (!os.good())
                return os;

            os << param.alpha_ << ' ';
            os << param.beta_;

            return os;
        }

        template <typename CharT, typename Traits>
        friend std::basic_istream<CharT, Traits> &operator>>(
            std::basic_istream<CharT, Traits> &is, param_type &param)
        {
            if (!is.good())
                return is;

            result_type alpha = 0;
            result_type beta = 0;
            is >> std::ws >> alpha;
            is >> std::ws >> beta;

            if (is.good()) {
                if (alpha > 0 && beta > 0)
                    param = param_type(alpha, beta);
                else
                    is.setstate(std::ios_base::failbit);
            }

            return is;
        }

        private:
        result_type alpha_;
        result_type beta_;
        result_type d_;
        result_type c_;
        const internal::GammaDistributionConstant<RealType> constant_;

        friend distribution_type;

        void invariant()
        {
            VSMC_RUNTIME_ASSERT_RNG_GAMMA_DISTRIBUTION_PARAM_CHECK(
                alpha_, beta_);
        }

        void reset() {}
    }; // class param_type

    explicit GammaDistribution(result_type alpha = 1, result_type beta = 1)
        : param_(alpha, beta)
    {
    }

    explicit GammaDistribution(const param_type &param) : param_(param) {}

    result_type alpha() const { return param_.alpha(); }
    result_type beta() const { return param_.beta(); }

    result_type min() const { return 0; }

    result_type max() const
    {
        return std::numeric_limits<result_type>::infinity();
    }

    template <typename RNGType>
    result_type operator()(RNGType &rng)
    {
        result_type r = 0;
        switch (param_.constant_.algorithm) {
            case internal::GammaDistributionAlgorithmT:
                r = generate_t(rng);
                break;
            case internal::GammaDistributionAlgorithmW:
                r = generate_w(rng);
                break;
            case internal::GammaDistributionAlgorithmN:
                r = generate_n(rng);
                break;
            case internal::GammaDistributionAlgorithmE:
                r = generate_e(rng);
                break;
        }

        return param_.beta_ * r;
    }

    template <typename RNGType>
    void operator()(RNGType &rng, std::size_t n, result_type *r)
    {
        gamma_distribution(rng, n, r, alpha(), beta());
    }

    VSMC_DEFINE_RNG_DISTRIBUTION_OPERATORS

    private:
    param_type param_;

    template <typename RNGType>
    result_type generate_t(RNGType &rng)
    {
        U01CODistribution<RealType> runif;
        while (true) {
            result_type u = runif(rng);
            result_type e = -std::log(runif(rng));
            if (u > param_.constant_.d) {
                u = -std::log(param_.constant_.c * (1 - u));
                e += u;
                u = param_.constant_.d + param_.alpha_ * u;
            }
            result_type r = std::exp(param_.constant_.c * std::log(u));
            if (std::abs(r) < e)
                return r;
        }
    }

    template <typename RNGType>
    result_type generate_w(RNGType &rng)
    {
        U01CODistribution<RealType> runif;
        result_type u = 0;
        result_type e = 0;
        result_type r = 0;
        do {
            u = -std::log(runif(rng));
            e = -std::log(runif(rng));
            r = std::exp(param_.constant_.c * std::log(u));
        } while (u + e < param_.constant_.d + r);

        return r;
    }

    template <typename RNGType>
    result_type generate_n(RNGType &rng)
    {
        U01CODistribution<RealType> runif;
        NormalDistribution<RealType> rnorm(0, 1);
        while (true) {
            result_type u = runif(rng);
            result_type e = 0;
            result_type r = 0;
            result_type v = 0;
            do {
                r = rnorm(rng);
                v = 1 + param_.constant_.c * r;
            } while (v <= 0);
            v = v * v * v;

            e = 1 - static_cast<result_type>(0.0331) * (r * r) * (r * r);
            if (u < e)
                return param_.constant_.d * v;

            e = r * r / 2 + param_.constant_.d * (1 - v + std::log(v));
            if (std::log(u) < e)
                return param_.constant_.d * v;
        }
    }

    template <typename RNGType>
    result_type generate_e(RNGType &rng)
    {
        U01CODistribution<RealType> runif;

        return -std::log(runif(rng));
    }
}; // class GammaDistribution

namespace internal
{

template <std::size_t K, typename RealType, typename RNGType>
inline std::size_t gamma_distribution_impl_t(RNGType &rng, std::size_t n,
    RealType *r, RealType alpha, RealType beta,
    const GammaDistributionConstant<RealType> &constant)
{
    const RealType d = constant.d;
    const RealType c = constant.c;
    RealType s[K * 3];
    RealType *const u = s;
    RealType *const e = s + n;
    RealType *const x = s + n * 2;

    u01_co_distribution(rng, n * 2, s);
    log(n, e, e);
    mul(n, static_cast<RealType>(-1), e, e);
    for (std::size_t i = 0; i != n; ++i) {
        if (u[i] > d) {
            u[i] = -std::log(c * (1 - u[i]));
            e[i] += u[i];
            u[i] = d + alpha * u[i];
        }
    }
    log(n, u, x);
    mul(n, c, x, x);
    exp(n, x, x);
    abs(n, x, u);
    mul(n, beta, x, x);

    std::size_t m = 0;
    for (std::size_t i = 0; i != n; ++i)
        if (u[i] < e[i])
            r[m++] = x[i];

    return m;
}

template <std::size_t K, typename RealType, typename RNGType>
inline std::size_t gamma_distribution_impl_w(RNGType &rng, std::size_t n,
    RealType *r, RealType, RealType beta,
    const GammaDistributionConstant<RealType> &constant)
{
    const RealType d = constant.d;
    const RealType c = constant.c;
    RealType s[K * 3];
    RealType *const u = s;
    RealType *const e = s + n;
    RealType *const x = s + n * 2;

    u01_co_distribution(rng, n * 2, s);
    log(n * 2, s, s);
    mul(n * 2, static_cast<RealType>(-1), s, s);
    log(n, s, x);
    mul(n, c, x, x);
    exp(n, x, x);
    add(n, u, e, u);
    add(n, d, x, e);
    mul(n, beta, x, x);

    std::size_t m = 0;
    for (std::size_t i = 0; i != n; ++i)
        if (u[i] > e[i])
            r[m++] = x[i];

    return m;
}

template <std::size_t K, typename RealType, typename RNGType>
inline std::size_t gamma_distribution_impl_n(RNGType &rng, std::size_t n,
    RealType *r, RealType, RealType beta,
    const GammaDistributionConstant<RealType> &constant)
{
    const RealType d = constant.d;
    const RealType c = constant.c;
    RealType s[K * 4];
    RealType *const u = s;
    RealType *const e = s + n;
    RealType *const v = s + n * 2;
    RealType *const x = s + n * 3;

    u01_co_distribution(rng, n, u);
    normal_distribution(
        rng, n, r, static_cast<RealType>(0), static_cast<RealType>(1));
    fma(n, static_cast<RealType>(1), c, r, v);
    sqr(n, v, e);
    mul(n, v, e, v);
    sqr(n, r, e);
    sqr(n, e, e);
    fma(n, static_cast<RealType>(1), -static_cast<RealType>(0.0331), e, e);
    mul(n, d * beta, v, x);

    std::size_t m = 0;
    for (std::size_t i = 0; i != n; ++i) {
        if (v[i] > 0) {
            if (u[i] < e[i]) {
                r[m++] = x[i];
            } else {
                RealType w = r[i] * r[i] / 2 + d * (1 - v[i] * std::log(v[i]));
                if (std::log(u[i]) < w)
                    r[m++] = x[i];
            }
        }
    }

    return m;
}

template <std::size_t, typename RealType, typename RNGType>
inline std::size_t gamma_distribution_impl_e(RNGType &rng, std::size_t n,
    RealType *r, RealType, RealType beta,
    const GammaDistributionConstant<RealType> &)
{
    u01_co_distribution(rng, n, r);
    log(n, r, r);
    mul(n, -beta, r, r);

    return n;
}

template <std::size_t K, typename RealType, typename RNGType>
inline std::size_t gamma_distribution_impl(RNGType &rng, std::size_t n,
    RealType *r, RealType alpha, RealType beta,
    const GammaDistributionConstant<RealType> &constant)
{
    switch (gamma_distribution_algorithm(alpha)) {
        case GammaDistributionAlgorithmT:
            return gamma_distribution_impl_t<K>(
                rng, n, r, alpha, beta, constant);
        case GammaDistributionAlgorithmW:
            return gamma_distribution_impl_w<K>(
                rng, n, r, alpha, beta, constant);
        case GammaDistributionAlgorithmN:
            return gamma_distribution_impl_n<K>(
                rng, n, r, alpha, beta, constant);
        case GammaDistributionAlgorithmE:
            return gamma_distribution_impl_e<K>(
                rng, n, r, alpha, beta, constant);
    }

    return 0;
}

} // namespace vsmc::internal

/// \brief Generating gamma random variates
/// \ingroup Distribution
template <typename RealType, typename RNGType>
inline void gamma_distribution(
    RNGType &rng, std::size_t n, RealType *r, RealType alpha, RealType beta)
{
    const std::size_t k = 1000;
    const internal::GammaDistributionConstant<RealType> constant(alpha);
    while (n > k) {
        std::size_t m = internal::gamma_distribution_impl<k>(
            rng, k, r, alpha, beta, constant);
        n -= m;
        r += m;
    }
    std::size_t m =
        internal::gamma_distribution_impl<k>(rng, n, r, alpha, beta, constant);
    n -= m;
    r += m;
    if (n > 0) {
        GammaDistribution<RealType> dist(alpha, beta);
        for (std::size_t i = 0; i != n; ++i)
            r[i] = dist(rng);
    }
}

} // namespace vsmc

#endif // VSMC_RNG_GAMMA_DISTRIBUTION_HPP
