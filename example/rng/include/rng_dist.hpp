//============================================================================
// vSMC/example/rng/include/rng_dist.hpp
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

#ifndef VSMC_EXAMPLE_RNG_DIST_HPP
#define VSMC_EXAMPLE_RNG_DIST_HPP

#include <vsmc/rng/engine.hpp>
#include <vsmc/utility/stop_watch.hpp>
#include <boost/math/distributions.hpp>
#include <boost/random.hpp>

#if VSMC_HAS_RUNTIME_LIBRARY
#include <vsmc/rng/mkl_brng.hpp>
#endif

#define VSMC_RNG_DIST_TEST(K, Name, STD)                                      \
    rng_dist_test<float, K, vsmc::Name##Distribution<float>, STD<float>>(     \
        argc, argv, #Name, params);                                           \
    rng_dist_test<double, K, vsmc::Name##Distribution<double>, STD<double>>(  \
        argc, argv, #Name, params);

template <typename RealType, std::size_t K>
inline std::string rng_dist_name(
    const std::string &name, const std::array<RealType, K> &param)
{
    std::stringstream ss;
    ss << name << '<';
    if (sizeof(RealType) == sizeof(float))
        ss << "float";
    if (sizeof(RealType) == sizeof(double))
        ss << "double";
    if (sizeof(RealType) == sizeof(long double))
        ss << "long double";
    ss << '>';
    if (K > 0)
        ss << "(" << param[0];
    for (std::size_t k = 1; k < K; ++k)
        ss << "," << param[k];
    if (K > 0)
        ss << ")";

    return ss.str();
}

template <typename RealType, typename DistType>
inline DistType rng_dist_init(const std::array<RealType, 0> &)
{
    return DistType();
}

template <typename RealType, typename DistType>
inline DistType rng_dist_init(const std::array<RealType, 1> &param)
{
    return DistType(param[0]);
}

template <typename RealType, typename DistType>
inline DistType rng_dist_init(const std::array<RealType, 2> &param)
{
    return DistType(param[0], param[1]);
}

template <typename RealType, typename QuantileType>
inline vsmc::Vector<RealType> rng_dist_partition_quantile(
    std::size_t n, const QuantileType &quantile)
{
    std::size_t k = n / 100;
    RealType h = static_cast<RealType>(1.0 / k);
    vsmc::Vector<RealType> partition;
    for (std::size_t i = 0; i != k - 1; ++i) {
        RealType p = h * (i + 1);
        p = std::max(p, static_cast<RealType>(0));
        p = std::min(p, static_cast<RealType>(1));
        partition.push_back(quantile(p));
    }

    return partition;
}

template <typename RealType, typename BoostDistType>
inline vsmc::Vector<RealType> rng_dist_partition_boost(
    std::size_t n, const BoostDistType &dist)
{
    auto quantile = [&](RealType p) { return boost::math::quantile(dist, p); };

    return rng_dist_partition_quantile<RealType>(n, quantile);
}

template <typename RealType, typename DistType>
inline vsmc::Vector<RealType> rng_dist_partition(std::size_t, DistType &);

template <typename RealType>
inline vsmc::Vector<RealType> rng_dist_partition(
    std::size_t n, vsmc::BetaDistribution<RealType> &dist)
{
    return rng_dist_partition_boost<RealType>(n,
        boost::math::beta_distribution<RealType>(dist.alpha(), dist.beta()));
}

template <typename RealType>
inline vsmc::Vector<RealType> rng_dist_partition(
    std::size_t n, vsmc::CauchyDistribution<RealType> &dist)
{
    return rng_dist_partition_quantile<RealType>(n, [&](RealType p) {
        return dist.a() +
            dist.b() * std::tan(vsmc::const_pi<RealType>() *
                           (p - static_cast<RealType>(0.5)));
    });
}

template <typename RealType>
inline vsmc::Vector<RealType> rng_dist_partition(
    std::size_t n, vsmc::ChiSquaredDistribution<RealType> &dist)
{
    return rng_dist_partition_boost<RealType>(
        n, boost::math::chi_squared_distribution<RealType>(dist.n()));
}

template <typename RealType>
inline vsmc::Vector<RealType> rng_dist_partition(
    std::size_t n, vsmc::ExponentialDistribution<RealType> &dist)
{
    return rng_dist_partition_quantile<RealType>(
        n, [&](RealType p) { return -std::log(1 - p) / dist.lambda(); });
}

template <typename RealType>
inline vsmc::Vector<RealType> rng_dist_partition(
    std::size_t n, vsmc::ExtremeValueDistribution<RealType> &dist)
{
    return rng_dist_partition_quantile<RealType>(n, [&](RealType p) {
        return dist.a() - dist.b() * std::log(-std::log(p));
    });
}

template <typename RealType>
inline vsmc::Vector<RealType> rng_dist_partition(
    std::size_t n, vsmc::FisherFDistribution<RealType> &dist)
{
    return rng_dist_partition_boost<RealType>(
        n, boost::math::fisher_f_distribution<RealType>(dist.m(), dist.n()));
}

template <typename RealType>
inline vsmc::Vector<RealType> rng_dist_partition(
    std::size_t n, vsmc::GammaDistribution<RealType> &dist)
{
    return rng_dist_partition_boost<RealType>(n,
        boost::math::gamma_distribution<RealType>(dist.alpha(), dist.beta()));
}

template <typename RealType>
inline vsmc::Vector<RealType> rng_dist_partition(
    std::size_t n, vsmc::LaplaceDistribution<RealType> &dist)
{
    return rng_dist_partition_quantile<RealType>(n, [&](RealType p) {
        RealType q = p - static_cast<RealType>(0.5);
        return q > 0 ? dist.a() - dist.b() * std::log(1 - 2 * q) :
                       dist.a() + dist.b() * std::log(1 + 2 * q);
    });
}

template <typename RealType>
inline vsmc::Vector<RealType> rng_dist_partition(
    std::size_t n, vsmc::LevyDistribution<RealType> &dist)
{
    boost::math::normal_distribution<RealType> normal(0, 1);
    return rng_dist_partition_quantile<RealType>(n, [&](RealType p) {
        RealType q = boost::math::quantile(normal, 1 - p / 2);
        return dist.a() + dist.b() / (q * q);
    });
}

template <typename RealType>
inline vsmc::Vector<RealType> rng_dist_partition(
    std::size_t n, vsmc::LogisticDistribution<RealType> &dist)
{
    return rng_dist_partition_quantile<RealType>(n, [&](RealType p) {
        return dist.a() + dist.b() * std::log(p / (1 - p));
    });
}

template <typename RealType>
inline vsmc::Vector<RealType> rng_dist_partition(
    std::size_t n, vsmc::LognormalDistribution<RealType> &dist)
{
    return rng_dist_partition_boost<RealType>(
        n, boost::math::lognormal_distribution<RealType>(dist.m(), dist.s()));
}

template <typename RealType>
inline vsmc::Vector<RealType> rng_dist_partition(
    std::size_t n, vsmc::NormalDistribution<RealType> &dist)
{
    return rng_dist_partition_boost<RealType>(
        n, boost::math::normal_distribution<RealType>(
               dist.mean(), dist.stddev()));
}

template <typename RealType>
inline vsmc::Vector<RealType> rng_dist_partition(
    std::size_t n, vsmc::ParetoDistribution<RealType> &dist)
{
    return rng_dist_partition_quantile<RealType>(n, [&](RealType p) {
        return dist.b() / std::exp(std::log(1 - p) / dist.a());
    });
}

template <typename RealType>
inline vsmc::Vector<RealType> rng_dist_partition(
    std::size_t n, vsmc::RayleighDistribution<RealType> &dist)
{
    return rng_dist_partition_quantile<RealType>(n, [&](RealType p) {
        return std::sqrt(-2 * std::log(1 - p) * dist.sigma() * dist.sigma());
    });
}

template <typename RealType>
inline vsmc::Vector<RealType> rng_dist_partition(
    std::size_t n, vsmc::StudentTDistribution<RealType> &dist)
{
    return rng_dist_partition_boost<RealType>(
        n, boost::math::students_t_distribution<RealType>(dist.n()));
}

template <typename RealType>
inline vsmc::Vector<RealType> rng_dist_partition(
    std::size_t n, vsmc::U01Distribution<RealType> &)
{
    return rng_dist_partition_quantile<RealType>(
        n, [&](RealType p) { return p; });
}

template <typename RealType>
inline vsmc::Vector<RealType> rng_dist_partition(
    std::size_t n, vsmc::U01CCDistribution<RealType> &)
{
    return rng_dist_partition_quantile<RealType>(
        n, [&](RealType p) { return p; });
}

template <typename RealType>
inline vsmc::Vector<RealType> rng_dist_partition(
    std::size_t n, vsmc::U01CODistribution<RealType> &)
{
    return rng_dist_partition_quantile<RealType>(
        n, [&](RealType p) { return p; });
}

template <typename RealType>
inline vsmc::Vector<RealType> rng_dist_partition(
    std::size_t n, vsmc::U01OCDistribution<RealType> &)
{
    return rng_dist_partition_quantile<RealType>(
        n, [&](RealType p) { return p; });
}

template <typename RealType>
inline vsmc::Vector<RealType> rng_dist_partition(
    std::size_t n, vsmc::U01OODistribution<RealType> &)
{
    return rng_dist_partition_quantile<RealType>(
        n, [&](RealType p) { return p; });
}

template <typename RealType>
inline vsmc::Vector<RealType> rng_dist_partition(
    std::size_t n, vsmc::UniformRealDistribution<RealType> &dist)
{
    return rng_dist_partition_quantile<RealType>(
        n, [&](RealType p) { return dist.a() + p * (dist.b() - dist.a()); });
}

template <typename RealType>
inline vsmc::Vector<RealType> rng_dist_partition(
    std::size_t n, vsmc::WeibullDistribution<RealType> &dist)
{
    return rng_dist_partition_quantile<RealType>(n, [&](RealType p) {
        return dist.b() * std::pow(-std::log(1 - p), 1 / dist.a());
    });
}

template <typename RealType>
inline RealType rng_dist_chi2(
    const vsmc::Vector<RealType> &r, const vsmc::Vector<RealType> &partition)
{
    vsmc::Vector<std::size_t> count(partition.size() + 1);
    vsmc::Vector<RealType> rval(r);
    std::sort(rval.begin(), rval.end());
    std::size_t j = 0;
    for (std::size_t i = 0; i != partition.size(); ++i) {
        std::size_t n = 0;
        while (j != rval.size() && rval[j] <= partition[i]) {
            ++n;
            ++j;
        }
        count[i] = n;
    }
    count.back() = rval.size() - j;
    RealType e = static_cast<RealType>(1.0 / partition.size() * rval.size());
    RealType p = 0;
    for (std::size_t i = 0; i != partition.size(); ++i)
        p += (count[i] - e) * (count[i] - e) / e;
    boost::math::chi_squared_distribution<RealType> chi2(
        static_cast<RealType>(partition.size() - 1));

    return boost::math::cdf(chi2, p);
}

template <typename RealType>
inline RealType rng_dist_ksad(
    const vsmc::Vector<RealType> &r, const vsmc::Vector<RealType> &partition)
{
    const std::size_t n = 100;
    const std::size_t m = r.size() / n;
    vsmc::Vector<RealType> rval(m);
    vsmc::Vector<RealType> pval(n);
    vsmc::Vector<RealType> head(n);
    vsmc::Vector<RealType> tail(n);
    for (std::size_t i = 0; i != n; ++i) {
        std::copy(r.data() + i * m, r.data() + i * m + m, rval.data());
        pval[i] = rng_dist_chi2<RealType>(rval, partition);
    }
    std::sort(pval.begin(), pval.end());
    vsmc::log(n, pval.data(), head.data());
    std::reverse(pval.begin(), pval.end());
    vsmc::sub(n, static_cast<RealType>(1), pval.data(), pval.data());
    vsmc::log(n, pval.data(), tail.data());
    vsmc::add(n, head.data(), tail.data(), pval.data());
    for (std::size_t i = 0; i != n; ++i)
        pval[i] *= 2 * (i + 1) - static_cast<RealType>(1);

    return -(n +
        std::accumulate(pval.begin(), pval.end(), static_cast<RealType>(0)) /
            n);
}

template <typename RealType>
inline void rng_dist_moments(const vsmc::Vector<RealType> &r,
    vsmc::Vector<RealType> &mean, vsmc::Vector<RealType> &variance)
{
    mean.push_back(
        std::accumulate(r.begin(), r.end(), static_cast<RealType>(0)) /
        r.size());
    RealType var = 0;
    for (auto v : r)
        var += v * v;
    variance.push_back(var / r.size() - mean.back() * mean.back());
}

template <typename RealType>
inline void rng_dist_pval(const vsmc::Vector<RealType> &chi2,
    const vsmc::Vector<RealType> &ksad,
    std::array<vsmc::Vector<RealType>, 6> &pval)
{
    std::size_t alpha1;
    std::size_t alpha5;
    std::size_t alpha10;

    alpha1 = alpha5 = alpha10 = 0;
    for (std::size_t i = 0; i != chi2.size(); ++i) {
        if (chi2[i] > static_cast<RealType>(0.01) &&
            chi2[i] < static_cast<RealType>(1 - 0.01))
            ++alpha1;
        if (chi2[i] > static_cast<RealType>(0.05) &&
            chi2[i] < static_cast<RealType>(1 - 0.05))
            ++alpha5;
        if (chi2[i] > static_cast<RealType>(0.1) &&
            chi2[i] < static_cast<RealType>(1 - 0.1))
            ++alpha10;
    }
    pval[0].push_back(static_cast<RealType>(100.0 * alpha1 / chi2.size()));
    pval[1].push_back(static_cast<RealType>(100.0 * alpha5 / chi2.size()));
    pval[2].push_back(static_cast<RealType>(100.0 * alpha10 / chi2.size()));

    alpha1 = alpha5 = alpha10 = 0;
    for (std::size_t i = 0; i != ksad.size(); ++i) {
        if (ksad[i] < static_cast<RealType>(3.857))
            ++alpha1;
        if (ksad[i] < static_cast<RealType>(2.492))
            ++alpha5;
        if (ksad[i] < static_cast<RealType>(1.933))
            ++alpha10;
    }
    pval[3].push_back(static_cast<RealType>(100.0 * alpha1 / ksad.size()));
    pval[4].push_back(static_cast<RealType>(100.0 * alpha5 / ksad.size()));
    pval[5].push_back(static_cast<RealType>(100.0 * alpha10 / ksad.size()));
}

template <typename RealType, typename vSMCDistType, typename STDDistType,
    std::size_t K>
inline void rng_dist_test(std::size_t n, std::size_t m,
    const std::array<RealType, K> &param, const std::string &name,
    vsmc::Vector<std::string> &names, vsmc::Vector<RealType> &mean,
    vsmc::Vector<RealType> &variance,
    std::array<vsmc::Vector<RealType>, 6> &pval,
    vsmc::Vector<vsmc::StopWatch> &sw)
{
    names.push_back(rng_dist_name(name, param));

    vsmc::RNG rng_vsmc;
    vSMCDistType dist_vsmc(rng_dist_init<RealType, vSMCDistType>(param));

    std::mt19937 rng_std;
    STDDistType dist_std(rng_dist_init<RealType, STDDistType>(param));

    vsmc::Vector<RealType> r(n);
    vsmc::Vector<RealType> chi2;
    vsmc::Vector<RealType> ksad;
    vsmc::Vector<RealType> partition(
        rng_dist_partition<RealType>(n, dist_vsmc));
    vsmc::StopWatch watch;

    watch.reset();
    chi2.clear();
    ksad.clear();
    for (std::size_t i = 0; i != m; ++i) {
        watch.start();
        for (std::size_t j = 0; j != n; ++j)
            r[j] = dist_std(rng_std);
        watch.stop();
        chi2.push_back(rng_dist_chi2<RealType>(r, partition));
        ksad.push_back(rng_dist_ksad<RealType>(r, partition));
    }
    sw.push_back(watch);
    rng_dist_moments<RealType>(r, mean, variance);
    rng_dist_pval<RealType>(chi2, ksad, pval);

    watch.reset();
    chi2.clear();
    ksad.clear();
    for (std::size_t i = 0; i != m; ++i) {
        watch.start();
        for (std::size_t j = 0; j != n; ++j)
            r[j] = dist_vsmc(rng_vsmc);
        watch.stop();
        chi2.push_back(rng_dist_chi2<RealType>(r, partition));
        ksad.push_back(rng_dist_ksad<RealType>(r, partition));
    }
    sw.push_back(watch);
    rng_dist_moments<RealType>(r, mean, variance);
    rng_dist_pval<RealType>(chi2, ksad, pval);

    watch.reset();
    chi2.clear();
    ksad.clear();
    for (std::size_t i = 0; i != m; ++i) {
        watch.start();
        vsmc::rng_rand(rng_vsmc, dist_vsmc, n, r.data());
        watch.stop();
        chi2.push_back(rng_dist_chi2<RealType>(r, partition));
        ksad.push_back(rng_dist_ksad<RealType>(r, partition));
    }
    sw.push_back(watch);
    rng_dist_moments<RealType>(r, mean, variance);
    rng_dist_pval<RealType>(chi2, ksad, pval);

#if VSMC_HAS_MKL
    vsmc::MKL_SFMT19937 rng_mkl;
    watch.reset();
    chi2.clear();
    ksad.clear();
    for (std::size_t i = 0; i != m; ++i) {
        watch.start();
        vsmc::rng_rand(rng_mkl, dist_vsmc, n, r.data());
        watch.stop();
        chi2.push_back(rng_dist_chi2<RealType>(r, partition));
        ksad.push_back(rng_dist_ksad<RealType>(r, partition));
    }
    sw.push_back(watch);
    rng_dist_moments<RealType>(r, mean, variance);
    rng_dist_pval<RealType>(chi2, ksad, pval);

#if VSMC_HAS_RUNTIME_LIBRARY && defined(RNG_DIST_STREAM)
    int brng = vsmc::mkl_brng<vsmc::RNG>();
    vsmc::MKLStream stream(brng, 101);
    watch.reset();
    chi2.clear();
    ksad.clear();
    for (std::size_t i = 0; i != m; ++i) {
        watch.start();
        RNG_DIST_STREAM(stream, n, r, param);
        watch.stop();
        chi2.push_back(rng_dist_chi2<RealType>(r, partition));
        ksad.push_back(rng_dist_ksad<RealType>(r, partition));
    }
    sw.push_back(watch);
    rng_dist_moments<RealType>(r, mean, variance);
    rng_dist_pval<RealType>(chi2, ksad, pval);
#endif // VSMC_HAS_RUNTIME_LIBRARY && defined(RNG_DIST_STREAM)
#endif // VSMC_HAS_MKL
}

template <typename RealType>
inline void rng_dist_output_pval(RealType pval)
{
    std::stringstream ss;
    if (pval < 50)
        ss << '*';
    ss << pval << '%';
    std::cout << std::right << std::setw(15) << ss.str();
}

template <typename RealType>
inline void rng_dist_output(const vsmc::Vector<std::string> &names,
    const vsmc::Vector<RealType> &mean, const vsmc::Vector<RealType> &variance,
    const std::array<vsmc::Vector<RealType>, 6> &pval,
    const vsmc::Vector<vsmc::StopWatch> &sw)
{
    std::size_t N = names.size();
    std::size_t R = sw.size() / N;
    int nwid = 40;
    int twid = 15;
    int Twid = static_cast<int>(R) * twid;
    std::size_t lwid = static_cast<std::size_t>(nwid + Twid);

    const vsmc::StopWatch *w = sw.data();
    const RealType *m = mean.data();
    const RealType *v = variance.data();
    const RealType *p0 = pval[0].data();
    const RealType *p1 = pval[1].data();
    const RealType *p2 = pval[2].data();
    const RealType *p3 = pval[3].data();
    const RealType *p4 = pval[4].data();
    const RealType *p5 = pval[5].data();
    for (std::size_t i = 0; i != N; ++i) {
        std::cout << std::string(lwid, '=') << std::endl;
        std::cout << std::left << std::setw(nwid) << names[i];
        std::cout << std::right << std::setw(twid) << "STD";
        std::cout << std::right << std::setw(twid) << "vSMC";
        std::cout << std::right << std::setw(twid) << "Batch";
#if VSMC_HAS_MKL
        std::cout << std::right << std::setw(twid) << "MKL";
#if VSMC_HAS_RUNTIME_LIBRARY && defined(RNG_DIST_STREAM)
        std::cout << std::right << std::setw(twid) << "MKL (vSMC)";
#endif
#endif
        std::cout << std::endl;
        std::cout << std::string(lwid, '-') << std::endl;

        std::cout << std::left << std::setw(nwid) << "Time";
        for (std::size_t r = 0; r != R; ++r, ++w)
            std::cout << std::right << std::setw(twid) << w->milliseconds();
        std::cout << std::endl;

        std::cout << std::left << std::setw(nwid) << "Mean";
        for (std::size_t r = 0; r != R; ++r)
            std::cout << std::right << std::setw(twid) << *m++;
        std::cout << std::endl;

        std::cout << std::left << std::setw(nwid) << "Variance";
        for (std::size_t r = 0; r != R; ++r)
            std::cout << std::right << std::setw(twid) << *v++;
        std::cout << std::endl;

        std::cout << std::left << std::setw(nwid) << "One level test (1%)";
        for (std::size_t r = 0; r != R; ++r)
            rng_dist_output_pval(*p0++);
        std::cout << std::endl;

        std::cout << std::left << std::setw(nwid) << "One level test (5%)";
        for (std::size_t r = 0; r != R; ++r)
            rng_dist_output_pval(*p1++);
        std::cout << std::endl;

        std::cout << std::left << std::setw(nwid) << "One level test (10%)";
        for (std::size_t r = 0; r != R; ++r)
            rng_dist_output_pval(*p2++);
        std::cout << std::endl;

        std::cout << std::left << std::setw(nwid) << "Two level test (1%)";
        for (std::size_t r = 0; r != R; ++r)
            rng_dist_output_pval(*p3++);
        std::cout << std::endl;

        std::cout << std::left << std::setw(nwid) << "Two level test (5%)";
        for (std::size_t r = 0; r != R; ++r)
            rng_dist_output_pval(*p4++);
        std::cout << std::endl;

        std::cout << std::left << std::setw(nwid) << "Two level test (10%)";
        for (std::size_t r = 0; r != R; ++r)
            rng_dist_output_pval(*p5++);
        std::cout << std::endl;
    }
    std::cout << std::string(lwid, '-') << std::endl;
}

template <typename RealType, std::size_t K, typename vSMCDistType,
    typename STDDistType, typename ParamType>
inline void rng_dist_test(
    int argc, char **argv, const std::string &name, const ParamType &params)
{
    std::size_t N = 10000;
    if (argc > 1)
        N = static_cast<std::size_t>(std::atoi(argv[1]));
    std::size_t M = 100;
    if (argc > 2)
        M = static_cast<std::size_t>(std::atoi(argv[2]));

    vsmc::Vector<std::string> names;
    vsmc::Vector<RealType> mean;
    vsmc::Vector<RealType> variance;
    std::array<vsmc::Vector<RealType>, 6> pval;
    vsmc::Vector<vsmc::StopWatch> sw;
    for (const auto &p : params) {
        std::array<RealType, K> param;
        for (std::size_t i = 0; i != p.size(); ++i)
            param[i] = static_cast<RealType>(p[i]);
        rng_dist_test<RealType, vSMCDistType, STDDistType>(
            N, M, param, name, names, mean, variance, pval, sw);
    }
    rng_dist_output<RealType>(names, mean, variance, pval, sw);
}

#endif // VSMC_EXAMPLE_RNG_DIST_HPP
