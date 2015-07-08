//============================================================================
// vSMC/include/vsmc/rng/beta_distribution.hpp
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

#ifndef VSMC_RNG_BETA_DISTRIBUTION_HPP
#define VSMC_RNG_BETA_DISTRIBUTION_HPP

#include <vsmc/rng/internal/common.hpp>
#include <vsmc/rng/u01_distribution.hpp>
#include <vsmc/rng/normal_distribution.hpp>

#define VSMC_RUNTIME_ASSERT_RNG_BETA_DISTRIBUTION_PARAM_ALPHA_CHECK(alpha)    \
    VSMC_RUNTIME_ASSERT((alpha > 0), "**BetaDistribution** CONSTRUCTED "      \
                                     "WITH INVALID SHAPE PARAMETER VALUE")

#define VSMC_RUNTIME_ASSERT_RNG_BETA_DISTRIBUTION_PARAM_BETA_CHECK(beta)      \
    VSMC_RUNTIME_ASSERT((beta > 0), "**BetaDistribution** CONSTRUCTED "       \
                                    "WITH INVALID SHAPE PARAMETER VALUE")

#define VSMC_RUNTIME_ASSERT_RNG_BETA_DISTRIBUTION_PARAM_CHECK(alpha, beta)    \
    VSMC_RUNTIME_ASSERT_RNG_BETA_DISTRIBUTION_PARAM_ALPHA_CHECK(alpha);       \
    VSMC_RUNTIME_ASSERT_RNG_BETA_DISTRIBUTION_PARAM_BETA_CHECK(beta);

namespace vsmc
{

namespace internal
{

enum BetaDistributionAlgorithm {
    BetaDistributionAlgorithm11,
    BetaDistributionAlgorithm1X,
    BetaDistributionAlgorithmX1,
    BetaDistributionAlgorithmC,
    BetaDistributionAlgorithmJ,
    BetaDistributionAlgorithmA1,
    BetaDistributionAlgorithmA2,
    BetaDistributionAlgorithmA3
}; // enum BetaDistributionAlgorithm

template <typename RealType>
inline BetaDistributionAlgorithm beta_distribution_algorithm(
    RealType alpha, RealType beta)
{
    const RealType K = static_cast<RealType>(0.852);
    const RealType C = static_cast<RealType>(-0.956);
    const RealType D = beta + K * alpha * alpha + C;
    if (is_equal<RealType>(alpha, 1) && is_equal<RealType>(beta, 1))
        return BetaDistributionAlgorithm11;
    else if (is_equal<RealType>(alpha, 1))
        return BetaDistributionAlgorithm1X;
    else if (is_equal<RealType>(beta, 1))
        return BetaDistributionAlgorithmX1;
    else if (alpha > 1 && beta > 1)
        return BetaDistributionAlgorithmC;
    else if (alpha < 1 && beta < 1 && D <= 0)
        return BetaDistributionAlgorithmJ;
    else if (alpha < 1 && beta < 1)
        return BetaDistributionAlgorithmA1;
    else if (alpha < 1 && beta > 1)
        return BetaDistributionAlgorithmA2;
    else if (alpha > 1 && beta < 1)
        return BetaDistributionAlgorithmA3;
    return BetaDistributionAlgorithmC;
}

template <typename RealType>
class BetaDistributionConstant
{
    public:
    BetaDistributionConstant(RealType alpha, RealType beta)
        : algorithm(beta_distribution_algorithm(alpha, beta))
    {
        switch (algorithm) {
            case BetaDistributionAlgorithm11: break;
            case BetaDistributionAlgorithm1X: b = 1 / beta; break;
            case BetaDistributionAlgorithmX1: a = 1 / alpha; break;
            case BetaDistributionAlgorithmC:
                a = alpha + beta;
                b = std::min(alpha, beta);
                if (b > 1)
                    b = std::sqrt((2 * alpha * beta - a) / (a - 2));
                b = 1 / b;
                t = alpha + 1 / b;
                p = a * std::log(a);
                break;
            case BetaDistributionAlgorithmJ:
                a = 1 / alpha;
                b = 1 / beta;
                break;
            case BetaDistributionAlgorithmA1:
                a = 1 / alpha;
                b = 1 / beta;
                t = std::sqrt(alpha * (1 - alpha));
                t /= t + std::sqrt(beta * (1 - beta));
                p = beta * t;
                p /= p + alpha * (1 - t);
                break;
            case BetaDistributionAlgorithmA2:
                a = 1 / alpha;
                b = 1 / beta;
                t = 1 - alpha;
                t /= t + beta;
                p = beta * t;
                p /= p + alpha * std::pow(1 - t, beta);
                break;
            case BetaDistributionAlgorithmA3:
                a = 1 / beta;
                b = 1 / alpha;
                t = 1 - beta;
                t /= t + alpha;
                p = alpha * t;
                p /= p + beta * std::pow(1 - t, alpha);
                break;
        }
    }

    BetaDistributionAlgorithm algorithm;
    RealType a;
    RealType b;
    RealType t;
    RealType p;
}; // class BetaDistributionConstant

} // namespace internal

/// \brief Generating beta random variates
/// \ingroup Distribution
template <typename RealType, typename RNGType>
inline void beta_distribution(RNGType &rng, std::size_t n, RealType *r,
    RealType alpha = 1, RealType beta = 1);

/// \brief beta distribution
/// \ingroup Distribution
template <typename RealType>
class BetaDistribution
{
    public:
    using result_type = RealType;
    using distribution_type = BetaDistribution<RealType>;

    class param_type
    {
        public:
        using result_type = RealType;
        using distribution_type = BetaDistribution<RealType>;

        explicit param_type(result_type alpha = 1, result_type beta = 1)
            : alpha_(alpha), beta_(beta), constant_(alpha_, beta_)
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
        const internal::BetaDistributionConstant<RealType> constant_;

        friend distribution_type;

        void invariant()
        {
            VSMC_RUNTIME_ASSERT_RNG_BETA_DISTRIBUTION_PARAM_CHECK(
                alpha_, beta_);
        }

        void reset() {}
    }; // class param_type

    explicit BetaDistribution(result_type alpha = 1, result_type beta = 1)
        : param_(alpha, beta)
    {
    }

    explicit BetaDistribution(const param_type &param) : param_(param) {}

    result_type alpha() const { return param_.alpha(); }
    result_type beta() const { return param_.beta(); }

    result_type min() const { return 0; }
    result_type max() const { return 1; }

    template <typename RNGType>
    result_type operator()(RNGType &rng)
    {
        result_type r = 0;
        switch (param_.constant_.algorithm) {
            case internal::BetaDistributionAlgorithm11:
                r = generate_11(rng);
                break;
            case internal::BetaDistributionAlgorithm1X:
                r = generate_1x(rng);
                break;
            case internal::BetaDistributionAlgorithmX1:
                r = generate_x1(rng);
                break;
            case internal::BetaDistributionAlgorithmC:
                r = generate_c(rng);
                break;
            case internal::BetaDistributionAlgorithmJ:
                r = generate_j(rng);
                break;
            case internal::BetaDistributionAlgorithmA1:
                r = generate_a1(rng);
                break;
            case internal::BetaDistributionAlgorithmA2:
                r = generate_a2(rng);
                break;
            case internal::BetaDistributionAlgorithmA3:
                r = generate_a3(rng);
                break;
        }

        return r;
    }

    template <typename RNGType>
    void operator()(RNGType &rng, std::size_t n, result_type *r)
    {
        beta_distribution(rng, n, r, alpha(), beta());
    }

    VSMC_DEFINE_RNG_DISTRIBUTION_OPERATORS

    private:
    param_type param_;

    template <typename RNGType>
    result_type generate_11(RNGType &rng)
    {
        U01DistributionType<RNGType, RealType> runif;

        return runif(rng);
    }

    template <typename RNGType>
    result_type generate_1x(RNGType &rng)
    {
        U01DistributionType<RNGType, RealType> runif;

        return 1 - std::pow(runif(rng), param_.constant_.b);
    }

    template <typename RNGType>
    result_type generate_x1(RNGType &rng)
    {
        U01DistributionType<RNGType, RealType> runif;

        return std::pow(runif(rng), param_.constant_.a);
    }

    template <typename RNGType>
    result_type generate_c(RNGType &rng)
    {
        U01DistributionType<RNGType, RealType> runif;
        const result_type ln_4 = 2 * const_ln_2<result_type>();
        result_type y = 0;
        result_type z = 0;
        result_type left = 0;
        result_type right = 0;
        do {
            result_type u1 = runif(rng);
            result_type u2 = runif(rng);
            result_type v = param_.constant_.b * std::log(u1 / (1 - u1));
            y = param_.alpha_ * std::exp(v);
            z = param_.beta_ + y;
            left = (param_.constant_.p - param_.constant_.a * std::log(z)) +
                (param_.constant_.t * v - ln_4);
            right = std::log(u1 * u1 * u2);
        } while (left < right);

        return y / z;
    }

    template <typename RNGType>
    result_type generate_j(RNGType &rng)
    {
        U01DistributionType<RNGType, RealType> runif;
        result_type x = 0;
        result_type y = 0;
        do {
            x = std::pow(runif(rng), param_.constant_.a);
            y = std::pow(runif(rng), param_.constant_.b);
        } while (x + y > 1);

        return x / (x + y);
    }

    template <typename RNGType>
    result_type generate_a1(RNGType &rng)
    {
        U01DistributionType<RNGType, RealType> runif;
        while (true) {
            result_type u = runif(rng);
            result_type e = -std::log(runif(rng));
            result_type x = 0;
            result_type v = 0;
            if (u < param_.constant_.p) {
                x = param_.constant_.t *
                    std::pow(u / param_.constant_.p, param_.constant_.a);
                v = (1 - param_.beta_) *
                    std::log((1 - x) / (1 - param_.constant_.t));
            } else {
                x = 1 -
                    (1 - param_.constant_.t) *
                        std::pow((1 - u) / (1 - param_.constant_.p),
                            param_.constant_.b);
                v = (1 - param_.alpha_) * std::log(x / param_.constant_.t);
            }
            if (v < e)
                return x;
        }
    }

    template <typename RNGType>
    result_type generate_a2(RNGType &rng)
    {
        U01DistributionType<RNGType, RealType> runif;
        while (true) {
            result_type u = runif(rng);
            result_type e = -std::log(runif(rng));
            result_type x = 0;
            result_type v = 0;
            if (u < param_.constant_.p) {
                x = param_.constant_.t *
                    std::pow(u / param_.constant_.p, param_.constant_.a);
                v = (1 - param_.beta_) * std::log(1 - x);
            } else {
                x = 1 -
                    (1 - param_.constant_.t) *
                        std::pow((1 - u) / (1 - param_.constant_.p),
                            param_.constant_.b);
                v = (1 - param_.alpha_) * std::log(x / param_.constant_.t);
            }
            if (v < e)
                return x;
        }
    }

    template <typename RNGType>
    result_type generate_a3(RNGType &rng)
    {
        U01DistributionType<RNGType, RealType> runif;
        while (true) {
            result_type u = runif(rng);
            result_type e = -std::log(runif(rng));
            result_type x = 0;
            result_type v = 0;
            if (u < param_.constant_.p) {
                x = param_.constant_.t *
                    std::pow(u / param_.constant_.p, param_.constant_.a);
                v = (1 - param_.alpha_) * std::log(1 - x);
            } else {
                x = 1 -
                    (1 - param_.constant_.t) *
                        std::pow((1 - u) / (1 - param_.constant_.p),
                            param_.constant_.b);
                v = (1 - param_.beta_) * std::log(x / param_.constant_.t);
            }
            if (v < e)
                return 1 - x;
        }
    }
}; // class BetaDistribution

namespace internal
{

template <std::size_t, typename RealType, typename RNGType>
inline void beta_distribution_impl_11(RNGType &rng, std::size_t n, RealType *r,
    RealType, RealType, BetaDistribution<RealType> &,
    const BetaDistributionConstant<RealType> &)
{
    u01_distribution(rng, n, r);
}

template <std::size_t, typename RealType, typename RNGType>
inline void beta_distribution_impl_1x(RNGType &rng, std::size_t n, RealType *r,
    RealType, RealType beta, BetaDistribution<RealType> &,
    const BetaDistributionConstant<RealType> &)
{
    u01_distribution(rng, n, r);
    pow(n, r, 1 / beta, r);
    sub(n, static_cast<RealType>(1), r, r);
}

template <std::size_t, typename RealType, typename RNGType>
inline void beta_distribution_impl_x1(RNGType &rng, std::size_t n, RealType *r,
    RealType alpha, RealType, BetaDistribution<RealType> &,
    const BetaDistributionConstant<RealType> &)
{
    u01_distribution(rng, n, r);
    pow(n, r, 1 / alpha, r);
}

template <std::size_t K, typename RealType, typename RNGType>
inline void beta_distribution_impl_c(RNGType &rng, std::size_t n, RealType *r,
    RealType alpha, RealType beta, BetaDistribution<RealType> &dist,
    const BetaDistributionConstant<RealType> &constant)
{
    const RealType a = constant.a;
    const RealType b = constant.b;
    const RealType t = constant.t;
    const RealType p = constant.p;
    const RealType ln_4 = 2 * const_ln_2<RealType>();
    RealType s[K * 5];
    RealType *const u1 = s;
    RealType *const u2 = s + n;
    RealType *const v = s + n * 2;
    RealType *const y = s + n * 3;
    RealType *const z = s + n * 4;
    u01_distribution(rng, n * 2, s);
    sub(n, static_cast<RealType>(1), u1, v);
    div(n, u1, v, v);
    log(n, v, v);
    mul(n, b, v, v);
    exp(n, v, y);
    mul(n, alpha, y, y);
    add(n, beta, y, z);
    div(n, y, z, r);

    sqr(n, u1, u1);
    mul(n, u1, u2, u2);
    log(n, u2, u2);
    log(n, z, u1);
    fma(n, p, -a, u1, u1);
    fma(n, -ln_4, t, v, v);
    add(n, v, u1, u1);
    for (std::size_t i = 0; i != n; ++i)
        if (u1[i] < u2[i])
            r[i] = dist(rng);
}

template <std::size_t K, typename RealType, typename RNGType>
inline void beta_distribution_impl_j(RNGType &rng, std::size_t n, RealType *r,
    RealType, RealType, BetaDistribution<RealType> &dist,
    const BetaDistributionConstant<RealType> &constant)
{
    const RealType a = constant.a;
    const RealType b = constant.b;
    RealType s[K * 3];
    RealType *const x = s;
    RealType *const y = s + n;
    RealType *const u = s + n * 2;
    u01_distribution(rng, n * 2, s);
    pow(n, x, a, x);
    pow(n, y, b, y);
    add(n, x, y, u);
    div(n, x, u, r);

    for (std::size_t i = 0; i != n; ++i)
        if (u[i] > 1)
            r[i] = dist(rng);
}

template <std::size_t, typename RealType, typename RNGType>
inline void beta_distribution_impl_a1(RNGType &rng, std::size_t n, RealType *r,
    RealType, RealType, BetaDistribution<RealType> &dist,
    const BetaDistributionConstant<RealType> &)
{
    for (std::size_t i = 0; i != n; ++i)
        r[i] = dist(rng);
}

template <std::size_t, typename RealType, typename RNGType>
inline void beta_distribution_impl_a2(RNGType &rng, std::size_t n, RealType *r,
    RealType, RealType, BetaDistribution<RealType> &dist,
    const BetaDistributionConstant<RealType> &)
{
    for (std::size_t i = 0; i != n; ++i)
        r[i] = dist(rng);
}

template <std::size_t, typename RealType, typename RNGType>
inline void beta_distribution_impl_a3(RNGType &rng, std::size_t n, RealType *r,
    RealType, RealType, BetaDistribution<RealType> &dist,
    const BetaDistributionConstant<RealType> &)
{
    for (std::size_t i = 0; i != n; ++i)
        r[i] = dist(rng);
}

template <std::size_t K, typename RealType, typename RNGType>
inline void beta_distribution_impl(RNGType &rng, std::size_t n, RealType *r,
    RealType alpha, RealType beta, BetaDistribution<RealType> &dist,
    const BetaDistributionConstant<RealType> &constant)
{
    switch (beta_distribution_algorithm(alpha, beta)) {
        case BetaDistributionAlgorithm11:
            beta_distribution_impl_11<K>(
                rng, n, r, alpha, beta, dist, constant);
            break;
        case BetaDistributionAlgorithm1X:
            beta_distribution_impl_1x<K>(
                rng, n, r, alpha, beta, dist, constant);
            break;
        case BetaDistributionAlgorithmX1:
            beta_distribution_impl_a1<K>(
                rng, n, r, alpha, beta, dist, constant);
            break;
        case BetaDistributionAlgorithmC:
            beta_distribution_impl_c<K>(
                rng, n, r, alpha, beta, dist, constant);
            break;
        case BetaDistributionAlgorithmJ:
            beta_distribution_impl_j<K>(
                rng, n, r, alpha, beta, dist, constant);
            break;
        case BetaDistributionAlgorithmA1:
            beta_distribution_impl_a1<K>(
                rng, n, r, alpha, beta, dist, constant);
            break;
        case BetaDistributionAlgorithmA2:
            beta_distribution_impl_a2<K>(
                rng, n, r, alpha, beta, dist, constant);
            break;
        case BetaDistributionAlgorithmA3:
            beta_distribution_impl_a3<K>(
                rng, n, r, alpha, beta, dist, constant);
            break;
    }
}

} // namespace vsmc::internal

template <typename RealType, typename RNGType>
inline void beta_distribution(
    RNGType &rng, std::size_t n, RealType *r, RealType alpha, RealType beta)
{
    const std::size_t k = 1000;
    const std::size_t m = n / k;
    const std::size_t l = n % k;
    BetaDistribution<RealType> dist(alpha, beta);
    const internal::BetaDistributionConstant<RealType> constant(alpha, beta);
    for (std::size_t i = 0; i != m; ++i) {
        internal::beta_distribution_impl<k>(
            rng, k, r + i * k, alpha, beta, dist, constant);
    }
    internal::beta_distribution_impl<k>(
        rng, l, r + m * k, alpha, beta, dist, constant);
}

} // namespace vsmc

#endif // VSMC_RNG_BETA_DISTRIBUTION_HPP