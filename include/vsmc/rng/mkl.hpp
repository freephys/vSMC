//============================================================================
// vSMC/include/vsmc/rng/mkl.hpp
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

#ifndef VSMC_RNG_MKL_HPP
#define VSMC_RNG_MKL_HPP

#include <vsmc/rng/internal/common.hpp>
#include <vsmc/utility/aligned_memory.hpp>
#include <vsmc/utility/mkl.hpp>

#define VSMC_STATIC_ASSERT_RNG_MKL_VSL_DISTRIBUTION_FP_TYPE(RealType, Dist)   \
    VSMC_STATIC_ASSERT((std::is_same<RealType, float>::value ||               \
                           std::is_same<RealType, double>::value),            \
        "**MKL" #Dist                                                         \
        "Distribution** USED WITH ResultType OTHER THAN float OR double")

#ifndef VSMC_RNG_MKL_VSL_BUFFER_SIZE
#define VSMC_RNG_MKL_VSL_BUFFER_SIZE 1024
#endif

namespace vsmc
{

template <typename, typename>
class MKLDistribution;
template <MKL_INT, typename>
class MKLEngine;

class MKLUniformBits32Distribution;
class MKLUniformBits64Distribution;

template <typename, MKL_INT = VSL_RNG_METHOD_UNIFORM_STD>
class MKLUniformDistribution;

template <MKL_INT = VSL_RNG_METHOD_BERNOULLI_ICDF>
class MKLBernoulliDistribution;
template <MKL_INT = VSL_RNG_METHOD_GEOMETRIC_ICDF>
class MKLGeometricDistribution;
template <MKL_INT = VSL_RNG_METHOD_BINOMIAL_BTPE>
class MKLBinomialDistribution;
template <MKL_INT = VSL_RNG_METHOD_HYPERGEOMETRIC_H2PE>
class MKLHypergeometricDistribution;
template <MKL_INT = VSL_RNG_METHOD_POISSON_PTPE>
class MKLPoissonDistribution;
template <MKL_INT = VSL_RNG_METHOD_NEGBINOMIAL_NBAR>
class MKLNegBinomialDistribution;
template <typename = double, MKL_INT = VSL_RNG_METHOD_GAUSSIAN_BOXMULLER2>
class MKLGaussianDistribution;
template <typename = double, MKL_INT = VSL_RNG_METHOD_EXPONENTIAL_ICDF>
class MKLExponentialDistribution;
template <typename = double, MKL_INT = VSL_RNG_METHOD_LAPLACE_ICDF>
class MKLLaplaceDistribution;
template <typename = double, MKL_INT = VSL_RNG_METHOD_WEIBULL_ICDF>
class MKLWeibullDistribution;
template <typename = double, MKL_INT = VSL_RNG_METHOD_CAUCHY_ICDF>
class MKLCauchyDistribution;
template <typename = double, MKL_INT = VSL_RNG_METHOD_RAYLEIGH_ICDF>
class MKLRayleighDistribution;
template <typename = double, MKL_INT = VSL_RNG_METHOD_LOGNORMAL_BOXMULLER2>
class MKLLognormalDistribution;
template <typename = double, MKL_INT = VSL_RNG_METHOD_GUMBEL_ICDF>
class MKLGumbelDistribution;
template <typename = double, MKL_INT = VSL_RNG_METHOD_GAMMA_GNORM>
class MKLGammaDistribution;
template <typename = double, MKL_INT = VSL_RNG_METHOD_BETA_CJA>
class MKLBetaDistribution;

/// \brief MKLEngine uniform bits trait
/// \ingroup Traits
///
/// \details
/// To use MKLEngine with those MKL BRNG that has not been aliased by vSMC,
/// one need to specialize this trait, which has member type `type`, and this
/// type has member `operator() (MKLStream<BRNG> &, MKL_INT, ResultType *)`
/// such that given the stream object, it is able to generate uniform integers.
///
/// This traits also need to have two static constant member data, `min` and
/// `max`
template <MKL_INT, typename>
class MKLUniformBitsTrait;

/// \brief Default uniform bits generator for MKLEngine with `unsigned` output
/// \ingroup Traits
template <MKL_INT BRNG>
class MKLUniformBitsTrait<BRNG, unsigned>
{
    public:
    using type = MKLUniformBits32Distribution;
    static constexpr unsigned min VSMC_MNE = 0;
    static constexpr unsigned max VSMC_MNE = VSMC_MAX_UINT(unsigned);
}; // class MKLUniformBitsTrait

/// \brief Default uniform bits generator for MKLEngine with
/// `unsigned MKL_INT64` output
/// \ingroup Traits
template <MKL_INT BRNG>
class MKLUniformBitsTrait<BRNG, unsigned MKL_INT64>
{
    public:
    using type = MKLUniformBits64Distribution;
    static constexpr unsigned MKL_INT64 min VSMC_MNE = 0;
    static constexpr unsigned MKL_INT64 max VSMC_MNE =
        VSMC_MAX_UINT(unsigned MKL_INT64);
}; // class MKLUniformBitsTrait

namespace internal
{

class MKLSkipAheadVSL
{
    public:
    using size_type = long long;

    template <MKL_INT BRNG>
    void operator()(MKLStream<BRNG> &stream, size_type nskip)
    {
        if (nskip == 0)
            return;

        int status = ::vslSkipAheadStream(stream.ptr(), nskip);
        mkl_error_check(
            status, "MKLSkipAheadVSL::skip", "::vslSkipAheadStream");
    }

    static void buffer_size(MKL_INT) {}
    static MKL_INT buffer_size() { return 0; }
}; // class SkipAheadVSL

template <MKL_INT BRNG, typename ResultType>
class MKLSkipAheadForce
{
    public:
    using size_type = MKL_INT;

    MKLSkipAheadForce() : buffer_size_(VSMC_RNG_MKL_VSL_BUFFER_SIZE) {}

    void operator()(MKLStream<BRNG> &stream, size_type nskip)
    {
        if (nskip == 0)
            return;

        if (nskip < buffer_size_) {
            if (buffer_.size() < nskip)
                buffer_.resize(nskip);
            uniform_bits_(stream, nskip, buffer_.data());
        } else {
            buffer_.resize(buffer_size_);
            size_type repeat = nskip / buffer_size_;
            size_type remain = nskip - repeat * buffer_size_;
            for (size_type r = 1; r != repeat + 1; ++r) {
                size_type n = r * buffer_size_;
                uniform_bits_(stream, n, buffer_.data());
            }
            uniform_bits_(stream, remain, buffer_.data());
        }
    }

    void buffer_size(MKL_INT size)
    {
        buffer_size_ = size > 0 ? size : VSMC_RNG_MKL_VSL_BUFFER_SIZE;
    }

    MKL_INT buffer_size() { return buffer_size_; }

    private:
    Vector<ResultType> buffer_;
    typename MKLUniformBitsTrait<BRNG, ResultType>::type uniform_bits_;
    MKL_INT buffer_size_;
}; // class SkipAheadForce

template <MKL_INT BRNG, typename ResultType>
class MKLSkipAhead
{
    public:
    using type = MKLSkipAheadForce<BRNG, ResultType>;
}; // clas MKLSkipAhead

template <typename ResultType>
class MKLSkipAhead<VSL_BRNG_MCG31, ResultType>
{
    public:
    using type = MKLSkipAheadVSL;
}; // clas MKLSkipAhead

template <typename ResultType>
class MKLSkipAhead<VSL_BRNG_MCG59, ResultType>
{
    public:
    using type = MKLSkipAheadVSL;
}; // clas MKLSkipAhead

template <typename ResultType>
class MKLSkipAhead<VSL_BRNG_MRG32K3A, ResultType>
{
    public:
    using type = MKLSkipAheadVSL;
}; // clas MKLSkipAhead

template <typename ResultType>
class MKLSkipAhead<VSL_BRNG_SOBOL, ResultType>
{
    public:
    using type = MKLSkipAheadVSL;
}; // clas MKLSkipAhead

template <typename ResultType>
class MKLSkipAhead<VSL_BRNG_NIEDERR, ResultType>
{
    public:
    using type = MKLSkipAheadVSL;
}; // clas MKLSkipAhead

} // namespace vsmc::internal

/// \brief MKL RNG C++11 engine
/// \ingroup MKLRNG
template <MKL_INT BRNG, typename ResultType>
class MKLEngine
{
    public:
    using result_type = ResultType;
    using stream_type = MKLStream<BRNG>;

    explicit MKLEngine(MKL_UINT s = MKLSeed<BRNG>::value, MKL_INT offset = 0)
        : stream_(s, offset)
        , buffer_size_(VSMC_RNG_MKL_VSL_BUFFER_SIZE)
        , index_(buffer_size_)
    {
    }

    template <typename SeedSeq>
    explicit MKLEngine(SeedSeq &seq,
        typename std::enable_if<internal::is_seed_seq<SeedSeq, MKL_UINT,
            MKLEngine<BRNG, ResultType>>::value>::type * = nullptr)
        : stream_(seq)
        , buffer_size_(VSMC_RNG_MKL_VSL_BUFFER_SIZE)
        , index_(buffer_size_)
    {
    }

    void seed(MKL_UINT s) { stream_.seed(s); }

    template <typename SeedSeq>
    void seed(SeedSeq &seq,
        typename std::enable_if<internal::is_seed_seq<SeedSeq, MKL_UINT,
            MKLEngine<BRNG, ResultType>>::value>::type * = nullptr)
    {
        stream_.seed(seq);
    }

    result_type operator()()
    {
        if (index_ == buffer_size_) {
            buffer_.resize(static_cast<std::size_t>(buffer_size_));
            uniform_bits_(stream_, buffer_size_, buffer_.data());
            index_ = 0;
        }

        return buffer_[static_cast<std::size_t>(index_++)];
    }

    /// \brief Discard results
    ///
    /// \details
    /// The the behavior is slightly different from that in C++11 standard.
    /// Calling `discard(nskip)` is not equivalent to call `operator()`
    /// `nskip`
    /// times. Instead, it ensures that at least `nskip` results are
    /// discarded.
    /// There may be a few more than `nskip` also discarded.
    void discard(std::size_t nskip)
    {
        skip_ahead_(stream_, static_cast<typename internal::MKLSkipAhead<BRNG,
                                 ResultType>::type::size_type>(nskip));
        index_ = buffer_size_;
    }

    static constexpr result_type _Min =
        MKLUniformBitsTrait<BRNG, ResultType>::min VSMC_MNE;
    static constexpr result_type _Max =
        MKLUniformBitsTrait<BRNG, ResultType>::max VSMC_MNE;

    static constexpr result_type min VSMC_MNE() { return _Min; }
    static constexpr result_type max VSMC_MNE() { return _Max; }

    stream_type &stream() { return stream_; }
    const stream_type &stream() const { return stream_; }

    /// \brief Set the buffer size, zero or negative value restore the default
    void buffer_size(MKL_INT size)
    {
        buffer_size_ = size > 0 ? size : VSMC_RNG_MKL_VSL_BUFFER_SIZE;
    }

    MKL_INT buffer_size() { return buffer_size_; }

    private:
    stream_type stream_;
    typename internal::MKLSkipAhead<BRNG, ResultType>::type skip_ahead_;
    typename MKLUniformBitsTrait<BRNG, ResultType>::type uniform_bits_;
    Vector<result_type> buffer_;
    MKL_INT buffer_size_;
    MKL_INT index_;
}; // class MKLEngine

/// \brief A 59-bits multiplicative congruential generator
/// \ingroup MKLRNG
using MKL_MCG59 = MKLEngine<VSL_BRNG_MCG59, unsigned>;

/// \brief A Mersenne-Twister pseudoranom number genertor
/// \ingroup MKLRNG
using MKL_MT19937 = MKLEngine<VSL_BRNG_MT19937, unsigned>;

/// \brief A Mersenne-Twister pseudoranom number genertor (64-bits)
/// \ingroup MKLRNG
using MKL_MT19937_64 = MKLEngine<VSL_BRNG_MT19937, unsigned MKL_INT64>;

/// \brief A set of 6024 Mersenne-Twister pseudoranom number genertor
/// \ingroup MKLRNG
using MKL_MT2203 = MKLEngine<VSL_BRNG_MT2203, unsigned>;

/// \brief A set of 6024 Mersenne-Twister pseudoranom number genertor
/// (64-bits)
/// \ingroup MKLRNG
using MKL_MT2203_64 = MKLEngine<VSL_BRNG_MT2203, unsigned MKL_INT64>;

/// \brief A SIMD-oriented fast Mersenne-Twister pseudoranom number genertor
/// \ingroup MKLRNG
using MKL_SFMT19937 = MKLEngine<VSL_BRNG_SFMT19937, unsigned>;

/// \brief A SIMD-oriented fast Mersenne-Twister pseudoranom number genertor
/// (64-bits)
/// \ingroup MKLRNG
using MKL_SFMT19937_64 = MKLEngine<VSL_BRNG_SFMT19937, unsigned MKL_INT64>;

/// \brief A non-determinstic random number generator
/// \ingroup MKLRNG
using MKL_NONDETERM = MKLEngine<VSL_BRNG_NONDETERM, unsigned>;

/// \brief A non-determinstic random number generator (64-bits)
/// \ingroup MKLRNG
using MKL_NONDETERM_64 = MKLEngine<VSL_BRNG_NONDETERM, unsigned MKL_INT64>;

#if INTEL_MKL_VERSION >= 110300

/// \brief A counter-based random number generator
/// \ingroup MKLRNG
using MKL_ARS5 = MKLEngine<VSL_BRNG_ARS5, unsigned>;

/// \brief A counter-based random number generator (64-bits)
/// \ingroup MKLRNG
using MKL_ARS5_64 = MKLEngine<VSL_BRNG_ARS5, unsigned MKL_INT64>;

/// \brief A counter-based random number generator
/// \ingroup MKLRNG
using MKL_PHILOX4X32X10 = MKLEngine<VSL_BRNG_PHILOX4X32X10, unsigned>;

/// \brief A counter-based random number generator (64-bits)
/// \ingroup MKLRNG
using MKL_PHILOX4X32X10_64 =
    MKLEngine<VSL_BRNG_PHILOX4X32X10, unsigned MKL_INT64>;

#endif // INTEL_MKL_VERSION >= 110300

/// \brief Base class of MKL distribution
/// \ingroup MKLRNG
template <typename ResultType, typename Derived>
class MKLDistribution
{
    public:
    using result_type = ResultType;

    MKLDistribution()
        : buffer_size_(VSMC_RNG_MKL_VSL_BUFFER_SIZE), index_(buffer_size_)
    {
    }

    template <MKL_INT BRNG>
    result_type operator()(MKLStream<BRNG> &stream)
    {
        if (index_ == buffer_size_) {
            buffer_.resize(static_cast<std::size_t>(buffer_size_));
            static_cast<Derived *>(this)->generate(
                stream, buffer_size_, buffer_.data());
            index_ = 0;
        }

        return buffer_[index_++];
    }

    template <MKL_INT BRNG>
    void operator()(MKLStream<BRNG> &stream, MKL_INT n, result_type *r)
    {
        static_cast<Derived *>(this)->generate(stream, n, r);
    }

    void reset() { index_ = buffer_size_; }

    /// \brief Set the buffer size, zero or negative value restore the default
    void buffer_size(MKL_INT size)
    {
        buffer_size_ = size > 0 ? size : VSMC_RNG_MKL_VSL_BUFFER_SIZE;
    }

    MKL_INT buffer_size() const { return buffer_size_; }

    protected:
    template <MKL_INT BRNG>
    void generate_error_check(int status, const char *name)
    {
        if (status == VSL_ERROR_OK)
            return;

        std::string dist_name(name);
        dist_name = "MKL" + dist_name + "Distribution::generate";
        std::string mkl_vsl_name(
            mkl_vsl_name_prefix(static_cast<result_type>(0)));
        mkl_vsl_name += "Rng";
        mkl_vsl_name += name;
        internal::mkl_error_check(
            status, dist_name.c_str(), mkl_vsl_name.c_str());
    }

    private:
    Vector<result_type> buffer_;
    MKL_INT buffer_size_;
    MKL_INT index_;

    std::string mkl_vsl_name_prefix(MKL_INT) { return "vi"; }
    std::string mkl_vsl_name_prefix(unsigned) { return "vi"; }
    std::string mkl_vsl_name_prefix(unsigned MKL_INT64) { return "vi"; }
    std::string mkl_vsl_name_prefix(float) { return "vs"; }
    std::string mkl_vsl_name_prefix(double) { return "vd"; }
}; // class MKLDistribution

/// \brief MKL uniform bits distribution (32-bits)
/// \ingroup MKLRNG
class MKLUniformBits32Distribution
    : public MKLDistribution<unsigned, MKLUniformBits32Distribution>
{
    public:
    using result_type = unsigned;

    template <MKL_INT BRNG>
    void generate(MKLStream<BRNG> &stream, MKL_INT n, result_type *r)
    {
        int status = ::viRngUniformBits32(
            VSL_RNG_METHOD_UNIFORMBITS32_STD, stream.ptr(), n, r);
        this->template generate_error_check<BRNG>(status, "UniformBits32");
    }
}; // class MKLUniformBits32Distribution

/// \brief MKL uniform bits distribution (64-bits)
/// \ingroup MKLRNG
class MKLUniformBits64Distribution
    : public MKLDistribution<unsigned MKL_INT64, MKLUniformBits64Distribution>
{
    public:
    using result_type = unsigned MKL_INT64;

    template <MKL_INT BRNG>
    void generate(MKLStream<BRNG> &stream, MKL_INT n, result_type *r)
    {
        int status = ::viRngUniformBits64(
            VSL_RNG_METHOD_UNIFORMBITS64_STD, stream.ptr(), n, r);
        this->template generate_error_check<BRNG>(status, "UniformBits64");
    }
}; // class MKLUniformBits64Distribution

/// \brief MKL Uniform distribution
/// \ingroup MKLRNG
template <typename ResultType, MKL_INT Method>
class MKLUniformDistribution : public MKLDistribution<ResultType,
                                   MKLUniformDistribution<ResultType, Method>>
{
    public:
    using result_type = ResultType;

    explicit MKLUniformDistribution(result_type a = 0, result_type b = 1)
        : a_(a), b_(b)
    {
    }

    template <MKL_INT BRNG>
    void generate(MKLStream<BRNG> &stream, MKL_INT n, result_type *r)
    {
        int status = generate(stream.ptr(), n, r);
        this->template generate_error_check<BRNG>(status, "Uniform");
    }

    private:
    result_type a_;
    result_type b_;

    int generate(VSLStreamStatePtr ptr, MKL_INT n, MKL_INT *r)
    {
        return ::viRngUniform(Method, ptr, n, r, a_, b_);
    }

    int generate(VSLStreamStatePtr ptr, MKL_INT n, float *r)
    {
        return ::vsRngUniform(Method, ptr, n, r, a_, b_);
    }

    int generate(VSLStreamStatePtr ptr, MKL_INT n, double *r)
    {
        return ::vdRngUniform(Method, ptr, n, r, a_, b_);
    }
}; // class MKLUniformDistribution

/// \brief MKL Bernoulli distribution
/// \ingroup MKLRNG
template <MKL_INT Method>
class MKLBernoulliDistribution
    : public MKLDistribution<MKL_INT, MKLBernoulliDistribution<Method>>
{
    public:
    using result_type = MKL_INT;

    explicit MKLBernoulliDistribution(double p = 0.5) : p_(p) {}

    template <MKL_INT BRNG>
    void generate(MKLStream<BRNG> &stream, MKL_INT n, result_type *r)
    {
        int status = ::viRngBernoulli(Method, stream.ptr(), n, r, p_);
        this->template generate_error_check<BRNG>(status, "Bernoulli");
    }

    private:
    double p_;
}; // class MKLBernoulliDistribution

/// \brief MKL Geometric distribution
/// \ingroup MKLRNG
template <MKL_INT Method>
class MKLGeometricDistribution
    : public MKLDistribution<MKL_INT, MKLGeometricDistribution<Method>>
{
    public:
    using result_type = MKL_INT;

    explicit MKLGeometricDistribution(double p = 0.5) : p_(p) {}

    template <MKL_INT BRNG>
    void generate(MKLStream<BRNG> &stream, MKL_INT n, result_type *r)
    {
        int status = ::viRngGeometric(Method, stream.ptr(), n, r, p_);
        this->template generate_error_check<BRNG>(status, "Geometric");
    }

    private:
    double p_;
}; // class MKLGeometricDistribution

/// \brief MKL Binomial distribution
/// \ingroup MKLRNG
template <MKL_INT Method>
class MKLBinomialDistribution
    : public MKLDistribution<MKL_INT, MKLBinomialDistribution<Method>>
{
    public:
    using result_type = MKL_INT;

    explicit MKLBinomialDistribution(result_type ntrial = 1, double p = 0.5)
        : ntrial_(ntrial), p_(p)
    {
    }

    template <MKL_INT BRNG>
    void generate(MKLStream<BRNG> &stream, MKL_INT n, result_type *r)
    {
        int status = ::viRngBinomial(Method, stream.ptr(), n, r, ntrial_, p_);
        this->template generate_error_check<BRNG>(status, "Binomial");
    }

    private:
    result_type ntrial_;
    double p_;
}; // class MKLBinomialDistribution

/// \brief MKL Hypergeometric distribution
/// \ingroup MKLRNG
template <MKL_INT Method>
class MKLHypergeometricDistribution
    : public MKLDistribution<MKL_INT, MKLHypergeometricDistribution<Method>>
{
    public:
    using result_type = MKL_INT;

    MKLHypergeometricDistribution(
        result_type population, result_type sample, result_type mask)
        : l_(population), s_(sample), m_(mask)
    {
    }

    template <MKL_INT BRNG>
    void generate(MKLStream<BRNG> &stream, MKL_INT n, result_type *r)
    {
        int status =
            ::viRngHypergeometric(Method, stream.ptr(), n, r, l_, s_, m_);
        this->template generate_error_check<BRNG>(status, "Hypergeometric");
    }

    private:
    result_type l_;
    result_type s_;
    result_type m_;
}; // class MKLHypergeometricDistribution

/// \brief MKL Poisson distribution
/// \ingroup MKLRNG
template <MKL_INT Method>
class MKLPoissonDistribution
    : public MKLDistribution<MKL_INT, MKLPoissonDistribution<Method>>
{
    public:
    using result_type = MKL_INT;

    explicit MKLPoissonDistribution(double lambda = 1) : lambda_(lambda) {}

    template <MKL_INT BRNG>
    void generate(MKLStream<BRNG> &stream, MKL_INT n, result_type *r)
    {
        int status = ::viRngPoisson(Method, stream.ptr(), n, r, lambda_);
        this->template generate_error_check<BRNG>(status, "Poisson");
    }

    private:
    double lambda_;
}; // class MKLPoissonDistribution

/// \brief MKL NegBinomial distribution
/// \ingroup MKLRNG
template <MKL_INT Method>
class MKLNegBinomialDistribution
    : public MKLDistribution<MKL_INT, MKLNegBinomialDistribution<Method>>
{
    public:
    using result_type = MKL_INT;

    explicit MKLNegBinomialDistribution(double ntrial = 1, double p = 0.5)
        : ntrial_(ntrial), p_(p)
    {
    }

    template <MKL_INT BRNG>
    void generate(MKLStream<BRNG> &stream, MKL_INT n, result_type *r)
    {
        int status =
            ::viRngNegBinomial(Method, stream.ptr(), n, r, ntrial_, p_);
        this->template generate_error_check<BRNG>(status, "NegBinomial");
    }

    private:
    double ntrial_;
    double p_;
}; // class MKLNegBinomialDistribution

/// \brief MKL Gaussian distribution
/// \ingroup MKLRNG
template <typename RealType, MKL_INT Method>
class MKLGaussianDistribution : public MKLDistribution<RealType,
                                    MKLGaussianDistribution<RealType, Method>>
{
    public:
    using result_type = RealType;

    explicit MKLGaussianDistribution(result_type mean = 0, result_type sd = 1)
        : mean_(mean), sd_(sd)
    {
        VSMC_STATIC_ASSERT_RNG_MKL_VSL_DISTRIBUTION_FP_TYPE(
            RealType, Gaussian);
    }

    template <MKL_INT BRNG>
    void generate(MKLStream<BRNG> &stream, MKL_INT n, result_type *r)
    {
        int status = generate(stream.ptr(), n, r);
        this->template generate_error_check<BRNG>(status, "Gaussian");
    }

    private:
    result_type mean_;
    result_type sd_;

    int generate(VSLStreamStatePtr ptr, MKL_INT n, float *r)
    {
        return ::vsRngGaussian(Method, ptr, n, r, mean_, sd_);
    }

    int generate(VSLStreamStatePtr ptr, MKL_INT n, double *r)
    {
        return ::vdRngGaussian(Method, ptr, n, r, mean_, sd_);
    }
}; // class MKLGaussianDistribution

/// \brief MKL Exponential distribution
/// \ingroup MKLRNG
template <typename RealType, MKL_INT Method>
class MKLExponentialDistribution
    : public MKLDistribution<RealType,
          MKLExponentialDistribution<RealType, Method>>
{
    public:
    using result_type = RealType;

    explicit MKLExponentialDistribution(
        result_type displacement = 0, result_type scale = 1)
        : disp_(displacement), scale_(scale)
    {
        VSMC_STATIC_ASSERT_RNG_MKL_VSL_DISTRIBUTION_FP_TYPE(
            RealType, Exponential);
    }

    template <MKL_INT BRNG>
    void generate(MKLStream<BRNG> &stream, MKL_INT n, result_type *r)
    {
        int status = generate(stream.ptr(), n, r);
        this->template generate_error_check<BRNG>(status, "Exponential");
    }

    private:
    result_type disp_;
    result_type scale_;

    int generate(VSLStreamStatePtr ptr, MKL_INT n, float *r)
    {
        return ::vsRngExponential(Method, ptr, n, r, disp_, scale_);
    }

    int generate(VSLStreamStatePtr ptr, MKL_INT n, double *r)
    {
        return ::vdRngExponential(Method, ptr, n, r, disp_, scale_);
    }
}; // class MKLExponentialDistribution

/// \brief MKL Laplace distribution
/// \ingroup MKLRNG
template <typename RealType, MKL_INT Method>
class MKLLaplaceDistribution : public MKLDistribution<RealType,
                                   MKLLaplaceDistribution<RealType, Method>>
{
    public:
    using result_type = RealType;

    explicit MKLLaplaceDistribution(
        result_type mean = 0, result_type scale = 1)
        : mean_(mean), scale_(scale)
    {
        VSMC_STATIC_ASSERT_RNG_MKL_VSL_DISTRIBUTION_FP_TYPE(RealType, Laplace);
    }

    template <MKL_INT BRNG>
    void generate(MKLStream<BRNG> &stream, MKL_INT n, result_type *r)
    {
        int status = generate(stream.ptr(), n, r);
        this->template generate_error_check<BRNG>(status, "Laplace");
    }

    private:
    result_type mean_;
    result_type scale_;

    int generate(VSLStreamStatePtr ptr, MKL_INT n, float *r)
    {
        return ::vsRngLaplace(Method, ptr, n, r, mean_, scale_);
    }

    int generate(VSLStreamStatePtr ptr, MKL_INT n, double *r)
    {
        return ::vdRngLaplace(Method, ptr, n, r, mean_, scale_);
    }
}; // class MKLLaplaceDistribution

/// \brief MKL Weibull distribution
/// \ingroup MKLRNG
template <typename RealType, MKL_INT Method>
class MKLWeibullDistribution : public MKLDistribution<RealType,
                                   MKLWeibullDistribution<RealType, Method>>
{
    public:
    using result_type = RealType;

    explicit MKLWeibullDistribution(result_type shape = 1,
        result_type displacement = 0, result_type scale = 1)
        : shape_(shape), disp_(displacement), scale_(scale)
    {
        VSMC_STATIC_ASSERT_RNG_MKL_VSL_DISTRIBUTION_FP_TYPE(RealType, Weibull);
    }

    template <MKL_INT BRNG>
    void generate(MKLStream<BRNG> &stream, MKL_INT n, result_type *r)
    {
        int status = generate(stream.ptr(), n, r);
        this->template generate_error_check<BRNG>(status, "Weibull");
    }

    private:
    result_type shape_;
    result_type disp_;
    result_type scale_;

    int generate(VSLStreamStatePtr ptr, MKL_INT n, float *r)
    {
        return ::vsRngWeibull(Method, ptr, n, r, shape_, disp_, scale_);
    }

    int generate(VSLStreamStatePtr ptr, MKL_INT n, double *r)
    {
        return ::vdRngWeibull(Method, ptr, n, r, shape_, disp_, scale_);
    }
}; // class MKLWeibullDistribution

/// \brief MKL Cauchy distribution
/// \ingroup MKLRNG
template <typename RealType, MKL_INT Method>
class MKLCauchyDistribution
    : public MKLDistribution<RealType, MKLCauchyDistribution<RealType, Method>>
{
    public:
    using result_type = RealType;

    explicit MKLCauchyDistribution(
        result_type displacement = 0, result_type scale = 1)
        : disp_(displacement), scale_(scale)
    {
        VSMC_STATIC_ASSERT_RNG_MKL_VSL_DISTRIBUTION_FP_TYPE(RealType, Cauchy);
    }

    template <MKL_INT BRNG>
    void generate(MKLStream<BRNG> &stream, MKL_INT n, result_type *r)
    {
        int status = generate(stream.ptr(), n, r);
        this->template generate_error_check<BRNG>(status, "Cauchy");
    }

    private:
    result_type disp_;
    result_type scale_;

    int generate(VSLStreamStatePtr ptr, MKL_INT n, float *r)
    {
        return ::vsRngCauchy(Method, ptr, n, r, disp_, scale_);
    }

    int generate(VSLStreamStatePtr ptr, MKL_INT n, double *r)
    {
        return ::vdRngCauchy(Method, ptr, n, r, disp_, scale_);
    }
}; // class MKLCauchyDistribution

/// \brief MKL Rayleigh distribution
/// \ingroup MKLRNG
template <typename RealType, MKL_INT Method>
class MKLRayleighDistribution : public MKLDistribution<RealType,
                                    MKLRayleighDistribution<RealType, Method>>
{
    public:
    using result_type = RealType;

    explicit MKLRayleighDistribution(
        result_type displacement = 0, result_type scale = 1)
        : disp_(displacement), scale_(scale)
    {
        VSMC_STATIC_ASSERT_RNG_MKL_VSL_DISTRIBUTION_FP_TYPE(
            RealType, Rayleigh);
    }

    template <MKL_INT BRNG>
    void generate(MKLStream<BRNG> &stream, MKL_INT n, result_type *r)
    {
        int status = generate(stream.ptr(), n, r);
        this->template generate_error_check<BRNG>(status, "Rayleigh");
    }

    private:
    result_type disp_;
    result_type scale_;

    int generate(VSLStreamStatePtr ptr, MKL_INT n, float *r)
    {
        return ::vsRngRayleigh(Method, ptr, n, r, disp_, scale_);
    }

    int generate(VSLStreamStatePtr ptr, MKL_INT n, double *r)
    {
        return ::vdRngRayleigh(Method, ptr, n, r, disp_, scale_);
    }
}; // class MKLRayleighDistribution

/// \brief MKL Lognormal distribution
/// \ingroup MKLRNG
template <typename RealType, MKL_INT Method>
class MKLLognormalDistribution
    : public MKLDistribution<RealType,
          MKLLognormalDistribution<RealType, Method>>
{
    public:
    using result_type = RealType;

    explicit MKLLognormalDistribution(result_type mean = 0, result_type sd = 1,
        result_type displacement = 0, result_type scale = 1)
        : mean_(mean), sd_(sd), disp_(displacement), scale_(scale)
    {
        VSMC_STATIC_ASSERT_RNG_MKL_VSL_DISTRIBUTION_FP_TYPE(
            RealType, Lognormal);
    }

    template <MKL_INT BRNG>
    void generate(MKLStream<BRNG> &stream, MKL_INT n, result_type *r)
    {
        int status = generate(stream.ptr(), n, r);
        this->template generate_error_check<BRNG>(status, "Lognormal");
    }

    private:
    result_type mean_;
    result_type sd_;
    result_type disp_;
    result_type scale_;

    int generate(VSLStreamStatePtr ptr, MKL_INT n, float *r)
    {
        return ::vsRngLognormal(Method, ptr, n, r, mean_, sd_, disp_, scale_);
    }

    int generate(VSLStreamStatePtr ptr, MKL_INT n, double *r)
    {
        return ::vdRngLognormal(Method, ptr, n, r, mean_, sd_, disp_, scale_);
    }
}; // class MKLLognormalDistribution

/// \brief MKL Gumbel distribution
/// \ingroup MKLRNG
template <typename RealType, MKL_INT Method>
class MKLGumbelDistribution
    : public MKLDistribution<RealType, MKLGumbelDistribution<RealType, Method>>
{
    public:
    using result_type = RealType;

    explicit MKLGumbelDistribution(
        result_type displacement = 0, result_type scale = 1)
        : disp_(displacement), scale_(scale)
    {
        VSMC_STATIC_ASSERT_RNG_MKL_VSL_DISTRIBUTION_FP_TYPE(RealType, Gumbel);
    }

    template <MKL_INT BRNG>
    void generate(MKLStream<BRNG> &stream, MKL_INT n, result_type *r)
    {
        int status = generate(stream.ptr(), n, r);
        this->template generate_error_check<BRNG>(status, "Gumbel");
    }

    private:
    result_type disp_;
    result_type scale_;

    int generate(VSLStreamStatePtr ptr, MKL_INT n, float *r)
    {
        return ::vsRngGumbel(Method, ptr, n, r, disp_, scale_);
    }

    int generate(VSLStreamStatePtr ptr, MKL_INT n, double *r)
    {
        return ::vdRngGumbel(Method, ptr, n, r, disp_, scale_);
    }
}; // class MKLGumbelDistribution

/// \brief MKL Gamma distribution
/// \ingroup MKLRNG
template <typename RealType, MKL_INT Method>
class MKLGammaDistribution
    : public MKLDistribution<RealType, MKLGammaDistribution<RealType, Method>>
{
    public:
    using result_type = RealType;

    explicit MKLGammaDistribution(result_type shape = 1,
        result_type displacement = 0, result_type scale = 1)
        : shape_(shape), disp_(displacement), scale_(scale)
    {
        VSMC_STATIC_ASSERT_RNG_MKL_VSL_DISTRIBUTION_FP_TYPE(RealType, Gamma);
    }

    template <MKL_INT BRNG>
    void generate(MKLStream<BRNG> &stream, MKL_INT n, result_type *r)
    {
        int status = generate(stream.ptr(), n, r);
        this->template generate_error_check<BRNG>(status, "Gamma");
    }

    private:
    result_type shape_;
    result_type disp_;
    result_type scale_;

    int generate(VSLStreamStatePtr ptr, MKL_INT n, float *r)
    {
        return ::vsRngGamma(Method, ptr, n, r, shape_, disp_, scale_);
    }

    int generate(VSLStreamStatePtr ptr, MKL_INT n, double *r)
    {
        return ::vdRngGamma(Method, ptr, n, r, shape_, disp_, scale_);
    }
}; // class MKLGammaDistribution

/// \brief MKL Beta distribution
/// \ingroup MKLRNG
template <typename RealType, MKL_INT Method>
class MKLBetaDistribution
    : public MKLDistribution<RealType, MKLBetaDistribution<RealType, Method>>
{
    public:
    using result_type = RealType;

    explicit MKLBetaDistribution(result_type shape1 = 1,
        result_type shape2 = 1, result_type displacement = 0,
        result_type scale = 1)
        : shape1_(shape1), shape2_(shape2), disp_(displacement), scale_(scale)
    {
        VSMC_STATIC_ASSERT_RNG_MKL_VSL_DISTRIBUTION_FP_TYPE(RealType, Beta);
    }

    template <MKL_INT BRNG>
    void generate(MKLStream<BRNG> &stream, MKL_INT n, result_type *r)
    {
        int status = generate(stream.ptr(), n, r);
        this->template generate_error_check<BRNG>(status, "Beta");
    }

    private:
    result_type shape1_;
    result_type shape2_;
    result_type disp_;
    result_type scale_;

    int generate(VSLStreamStatePtr ptr, MKL_INT n, float *r)
    {
        return ::vsRngBeta(Method, ptr, n, r, shape1_, shape2_, disp_, scale_);
    }

    int generate(VSLStreamStatePtr ptr, MKL_INT n, double *r)
    {
        return ::vdRngBeta(Method, ptr, n, r, shape1_, shape2_, disp_, scale_);
    }
}; // class MKLBetaDistribution

} // namespace vsmc

#endif // VSMC_RNG_MKL_HPP
