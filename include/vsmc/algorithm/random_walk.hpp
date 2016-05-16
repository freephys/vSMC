//============================================================================
// vSMC/include/vsmc/algorithm/random_walk.hpp
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

#ifndef VSMC_ALGORITHM_RANDOM_WALK_HPP
#define VSMC_ALGORITHM_RANDOM_WALK_HPP

#include <vsmc/internal/common.hpp>
#include <vsmc/rng/normal_distribution.hpp>
#include <vsmc/rng/normal_mv_distribution.hpp>
#include <vsmc/rng/u01_distribution.hpp>

#define VSMC_RUNTIME_ASSERT_ALGORITHM_RANDOM_WALK_PROPOSAL_PARAM(flag, Name)  \
    VSMC_RUNTIME_ASSERT(                                                      \
        (flag), "**" #Name "Proposal** CONSTRUCTED WITH INVALID PARAMETERS")

#define VSMC_RUNTIME_ASSERT_ALGORITHM_RANDOM_WALK_NORMAL_MV_LOGIT_DIM(dim)    \
    VSMC_RUNTIME_ASSERT((dim > 1), "**NormalMVLogitProposal** CONSTRUCTED "   \
                                   "WITH DIMENSION LESS THAN 2")

namespace vsmc
{

/// \brief Random walk MCMC update
/// \ingroup RandomWalk
template <typename ResultType = double, std::size_t Dim = Dynamic>
class RandomWalk
{
    public:
    using result_type = ResultType;

    /// \brief Only usable when `Dim != Dynamic`
    RandomWalk()
    {
        static_assert(Dim != Dynamic,
            "**RandomWalk** OBJECT DECLARED WITH DYNAMIC DIMENSION");
    }

    /// \brief Only usable when `Dim == Dynamic`
    RandomWalk(std::size_t dim) : x_(dim), y_(dim)
    {
        static_assert(Dim == Dynamic,
            "**RandomWalk** OBJECT DECLARED WITH FIXED DIMENSION");
    }

    std::size_t dim() const { return x_.size(); }

    /// \brief One-step random walk update
    ///
    /// \param rng RNG engine
    /// \param x The current state value. It will be updated to the new value
    /// after the MCMC move.
    /// \param ltx If it is a non-null pointer, then it points to the value of
    /// the \f$\log\gamma(x)\f$. It will be updated to the new value if the
    /// MCMC move is accepted and left unchanged otherwise. If it is a null
    /// pointer, then it is ignored. Use this pointer to save
    /// \f$\log\gamma(x)\f$ between updates if it is expensive to calculate.
    /// \param log_target The log-target fucntion
    /// ~~~{.cpp}
    /// double log_target(std::size_t dim_x, const result_type *x );
    /// ~~~
    /// It accepts the lengths of the state vector and a pointer to the storage
    /// of the state value. It return the value of log-target function
    /// \f$\log\gamma(x)\f$.
    /// \param proposal The proposal function. It takes the form,
    /// ~~~{.cpp}
    /// double proposal(RNGType &rng, std::size_t dim,
    ///     const result_type *x, result_type *y);
    /// ~~~
    /// After the call, the function return the proposed value in `y` and
    /// return the value \f$\log(q(y, x) / q(x, y))\f$.
    ///
    /// \return Acceptance count
    template <typename RNGType, typename LogTargetType, typename ProposalType>
    std::size_t operator()(RNGType &rng, result_type *x, double *ltx,
        LogTargetType &&log_target, ProposalType &&proposal)
    {
        U01Distribution<double> u01;
        double s = ltx == nullptr ? log_target(dim(), x) : *ltx;
        double q = proposal(rng, dim(), x, y_.data());
        double t = log_target(dim(), y_.data());
        double p = t - s + q;
        double u = std::log(u01(rng));

        if (u < p) {
            std::copy(y_.begin(), y_.end(), x);
            if (ltx != nullptr)
                *ltx = t;
            return 1;
        }
        return 0;
    }

    /// \brief Multi-step random walk update
    template <typename RNGType, typename LogTargetType, typename ProposalType>
    std::size_t operator()(std::size_t n, RNGType &rng, result_type *x,
        double *ltx, LogTargetType &&log_target, ProposalType &&proposal)
    {
        std::copy_n(x, dim(), x_.data());
        U01Distribution<double> u01;
        std::size_t accept = 0;
        double s = ltx == nullptr ? log_target(dim(), x) : *ltx;
        for (std::size_t i = 0; i != n; ++i) {
            double q = proposal(rng, dim(), x_.data(), y_.data());
            double t = log_target(dim(), y_.data());
            double p = t - s + q;
            double u = std::log(u01(rng));
            if (u < p) {
                ++accept;
                s = t;
                x_.swap(y_);
            }
        }
        if (accept != 0) {
            std::copy(x_.begin(), x_.end(), x);
            if (ltx != nullptr)
                *ltx = s;
        }

        return accept;
    }

    private:
    internal::StaticVector<ResultType, Dim> x_;
    internal::StaticVector<ResultType, Dim> y_;
}; // class RandomWalk

namespace internal
{

template <typename RealType>
inline bool normal_proposal_check_param(RealType a, RealType b)
{
    return a < b;
}

template <typename RealType>
inline bool normal_mv_proposal_check_param(
    std::size_t dim, const RealType *a, const RealType *b)
{
    for (std::size_t i = 0; i != dim; ++i)
        if (a[i] >= b[i])
            return false;
    return true;
}

template <typename RealType>
inline double normal_proposal_q(RealType x, RealType &y, RealType z)
{
    y = x + z;

    return 0;
}

template <typename RealType>
inline double normal_proposal_qa(
    RealType x, RealType &y, RealType z, RealType a)
{
    y = a + (x - a) * std::exp(z);

    return static_cast<double>(z);
}

template <typename RealType>
inline double normal_proposal_qb(
    RealType x, RealType &y, RealType z, RealType b)
{
    y = b - (b - x) * std::exp(z);

    return static_cast<double>(z);
}

template <typename RealType>
inline double normal_proposal_qab(
    RealType x, RealType &y, RealType z, RealType a, RealType b)
{
    RealType r = std::exp(z) * (x - a) / (b - x);
    y = (a + b * r) / (1 + r);

    return std::log(
        static_cast<double>(((y - a) / (x - a)) * ((b - y) / (b - x))));
}

} // namespace vsmc::internal

/// \brief Normal random walk proposal
/// \ingroup RandomWalk
template <typename RealType = double>
class NormalProposal
{
    public:
    using result_type = RealType;

    /// \brief Construct a Normal random walk proposal
    ///
    /// \param stddev The standard deviation (scale) of the proposal
    /// \param a The lower bound of the support of the target distribution
    /// \param b The upper bound of the support of the target distribution
    NormalProposal(result_type stddev, result_type a, result_type b)
        : normal_(static_cast<result_type>(0), stddev), a_(a), b_(b), flag_(0)
    {
        unsigned lower = std::isfinite(a) ? 1 : 0;
        unsigned upper = std::isfinite(b) ? 1 : 0;
        flag_ = (lower << 1) + upper;
        VSMC_RUNTIME_ASSERT_ALGORITHM_RANDOM_WALK_PROPOSAL_PARAM(
            internal::normal_proposal_check_param(a, b), Normal);
    }

    /// \brief Propose new value `y` and return \f$\log(q(y, x) / q(x, y))\f$.
    template <typename RNGType>
    double operator()(
        RNGType &rng, std::size_t, const result_type *x, result_type *y)
    {
        result_type z = normal_(rng);
        switch (flag_) {
            case 0: return internal::normal_proposal_q(*x, *y, z);
            case 1: return internal::normal_proposal_qb(*x, *y, z, b_);
            case 2: return internal::normal_proposal_qa(*x, *y, z, a_);
            case 3: return internal::normal_proposal_qab(*x, *y, z, a_, b_);
            default: return 0;
        }
    }

    private:
    NormalDistribution<RealType> normal_;
    result_type a_;
    result_type b_;
    unsigned flag_;
}; // class NormalProposal

/// \brief Multivariate Normal random walk proposal
/// \ingroup RandomWalk
template <typename RealType = double, std::size_t Dim = Dynamic>
class NormalMVProposal
{
    public:
    using result_type = RealType;

    /// \brief Only usable when `Dim != Dynamic`
    NormalMVProposal(result_type chol, result_type a, result_type b)
        : normal_mv_(static_cast<result_type>(0), chol)
    {
        static_assert(Dim != Dynamic,
            "**NormalMVProposal** OBJECT DECLARED WITH DYNAMIC DIMENSION");
        init_a(a);
        init_b(b);
        init_flag();
    }

    /// \brief Only usable when `Dim != Dynamic`
    NormalMVProposal(result_type chol, result_type a, const result_type *b)
        : normal_mv_(static_cast<result_type>(0), chol)
    {
        static_assert(Dim != Dynamic,
            "**NormalMVProposal** OBJECT DECLARED WITH DYNAMIC DIMENSION");
        init_a(a);
        init_b(b);
        init_flag();
    }

    /// \brief Only usable when `Dim != Dynamic`
    NormalMVProposal(result_type chol, const result_type *a, result_type b)
        : normal_mv_(static_cast<result_type>(0), chol)
    {
        static_assert(Dim != Dynamic,
            "**NormalMVProposal** OBJECT DECLARED WITH DYNAMIC DIMENSION");
        init_a(a);
        init_b(b);
        init_flag();
    }

    /// \brief Only usable when `Dim != Dynamic`
    NormalMVProposal(
        result_type chol, const result_type *a, const result_type *b)
        : normal_mv_(static_cast<result_type>(0), chol)
    {
        static_assert(Dim != Dynamic,
            "**NormalMVProposal** OBJECT DECLARED WITH DYNAMIC DIMENSION");
        init_a(a);
        init_b(b);
        init_flag();
    }

    /// \brief Only usable when `Dim != Dynamic`
    NormalMVProposal(const result_type *chol, result_type a, result_type b)
        : normal_mv_(static_cast<result_type>(0), chol)
    {
        static_assert(Dim != Dynamic,
            "**NormalMVProposal** OBJECT DECLARED WITH DYNAMIC DIMENSION");
        init_a(a);
        init_b(b);
        init_flag();
    }

    /// \brief Only usable when `Dim != Dynamic`
    NormalMVProposal(
        const result_type *chol, result_type a, const result_type *b)
        : normal_mv_(static_cast<result_type>(0), chol)
    {
        static_assert(Dim != Dynamic,
            "**NormalMVProposal** OBJECT DECLARED WITH DYNAMIC DIMENSION");
        init_a(a);
        init_b(b);
        init_flag();
    }

    /// \brief Only usable when `Dim != Dynamic`
    NormalMVProposal(
        const result_type *chol, const result_type *a, result_type b)
        : normal_mv_(static_cast<result_type>(0), chol)
    {
        static_assert(Dim != Dynamic,
            "**NormalMVProposal** OBJECT DECLARED WITH DYNAMIC DIMENSION");
        init_a(a);
        init_b(b);
        init_flag();
    }

    /// \brief Only usable when `Dim != Dynamic`
    NormalMVProposal(
        const result_type *chol, const result_type *a, const result_type *b)
        : normal_mv_(static_cast<result_type>(0), chol)
    {
        static_assert(Dim != Dynamic,
            "**NormalMVProposal** OBJECT DECLARED WITH DYNAMIC DIMENSION");
        init_a(a);
        init_b(b);
        init_flag();
    }

    /// \brief Only usable when `Dim == Dynamic`
    NormalMVProposal(
        std::size_t dim, result_type chol, result_type a, result_type b)
        : normal_mv_(dim, static_cast<result_type>(0), chol)
        , a_(dim)
        , b_(dim)
        , z_(dim)
        , flag_(dim)
    {
        static_assert(Dim == Dynamic,
            "**NormalMVProposal** OBJECT DECLARED WITH FIXED DIMENSION");
        init_a(a);
        init_b(b);
        init_flag();
    }

    /// \brief Only usable when `Dim == Dynamic`
    NormalMVProposal(
        std::size_t dim, result_type chol, result_type a, const result_type *b)
        : normal_mv_(dim, static_cast<result_type>(0), chol)
        , a_(dim)
        , b_(dim)
        , z_(dim)
        , flag_(dim)
    {
        static_assert(Dim == Dynamic,
            "**NormalMVProposal** OBJECT DECLARED WITH FIXED DIMENSION");
        init_a(a);
        init_b(b);
        init_flag();
    }

    /// \brief Only usable when `Dim == Dynamic`
    NormalMVProposal(
        std::size_t dim, result_type chol, const result_type *a, result_type b)
        : normal_mv_(dim, static_cast<result_type>(0), chol)
        , a_(dim)
        , b_(dim)
        , z_(dim)
        , flag_(dim)
    {
        static_assert(Dim == Dynamic,
            "**NormalMVProposal** OBJECT DECLARED WITH FIXED DIMENSION");
        init_a(a);
        init_b(b);
        init_flag();
    }

    /// \brief Only usable when `Dim == Dynamic`
    NormalMVProposal(std::size_t dim, result_type chol, const result_type *a,
        const result_type *b)
        : normal_mv_(dim, static_cast<result_type>(0), chol)
        , a_(dim)
        , b_(dim)
        , z_(dim)
        , flag_(dim)
    {
        static_assert(Dim == Dynamic,
            "**NormalMVProposal** OBJECT DECLARED WITH FIXED DIMENSION");
        init_a(a);
        init_b(b);
        init_flag();
    }

    /// \brief Only usable when `Dim == Dynamic`
    NormalMVProposal(
        std::size_t dim, const result_type *chol, result_type a, result_type b)
        : normal_mv_(dim, static_cast<result_type>(0), chol)
        , a_(dim)
        , b_(dim)
        , z_(dim)
        , flag_(dim)
    {
        static_assert(Dim == Dynamic,
            "**NormalMVProposal** OBJECT DECLARED WITH FIXED DIMENSION");
        init_a(a);
        init_b(b);
        init_flag();
    }

    /// \brief Only usable when `Dim == Dynamic`
    NormalMVProposal(std::size_t dim, const result_type *chol, result_type a,
        const result_type *b)
        : normal_mv_(dim, static_cast<result_type>(0), chol)
        , a_(dim)
        , b_(dim)
        , z_(dim)
        , flag_(dim)
    {
        static_assert(Dim == Dynamic,
            "**NormalMVProposal** OBJECT DECLARED WITH FIXED DIMENSION");
        init_a(a);
        init_b(b);
        init_flag();
    }

    /// \brief Only usable when `Dim == Dynamic`
    NormalMVProposal(std::size_t dim, const result_type *chol,
        const result_type *a, result_type b)
        : normal_mv_(dim, static_cast<result_type>(0), chol)
        , a_(dim)
        , b_(dim)
        , z_(dim)
        , flag_(dim)
    {
        static_assert(Dim == Dynamic,
            "**NormalMVProposal** OBJECT DECLARED WITH FIXED DIMENSION");
        init_a(a);
        init_b(b);
        init_flag();
    }

    /// \brief Only usable when `Dim == Dynamic`
    NormalMVProposal(std::size_t dim, const result_type *chol,
        const result_type *a, const result_type *b)
        : normal_mv_(dim, static_cast<result_type>(0), chol)
        , a_(dim)
        , b_(dim)
        , z_(dim)
        , flag_(dim)
    {
        static_assert(Dim == Dynamic,
            "**NormalMVProposal** OBJECT DECLARED WITH FIXED DIMENSION");
        init_a(a);
        init_b(b);
        init_flag();
    }

    template <typename RNGType>
    result_type operator()(
        RNGType &rng, std::size_t, const result_type *x, result_type *y)
    {
        normal_mv_(rng, z_.data());
        result_type q = 0;
        for (std::size_t i = 0; i != z_.size(); ++i) {
            switch (flag_[i]) {
                case 0:
                    q += internal::normal_proposal_q(x[i], y[i], z_[i]);
                    break;
                case 1:
                    q +=
                        internal::normal_proposal_qb(x[i], y[i], z_[i], b_[i]);
                    break;
                case 2:
                    q +=
                        internal::normal_proposal_qa(x[i], y[i], z_[i], a_[i]);
                    break;
                case 3:
                    q += internal::normal_proposal_qab(
                        x[i], y[i], z_[i], a_[i], b_[i]);
                    break;
                default: break;
            }
        }

        return q;
    }

    private:
    NormalMVDistribution<RealType, Dim> normal_mv_;
    internal::StaticVector<RealType, Dim> a_;
    internal::StaticVector<RealType, Dim> b_;
    internal::StaticVector<RealType, Dim> z_;
    internal::StaticVector<unsigned, Dim> flag_;

    void init_a(result_type a) { std::fill(a_.begin(), a_.end(), a); }

    void init_a(const result_type *a)
    {
        std::copy_n(a, a_.size(), a_.begin());
    }

    void init_b(result_type b) { std::fill(b_.begin(), b_.end(), b); }

    void init_b(const result_type *b)
    {
        std::copy_n(b, b_.size(), b_.begin());
    }

    void init_flag()
    {

        for (std::size_t i = 0; i != flag_.size(); ++i) {
            unsigned lower = std::isfinite(a_[i]) ? 1 : 0;
            unsigned upper = std::isfinite(b_[i]) ? 1 : 0;
            flag_[i] = (lower << 1) + upper;
        }

        VSMC_RUNTIME_ASSERT_ALGORITHM_RANDOM_WALK_PROPOSAL_PARAM(
            internal::normal_mv_proposal_check_param(
                flag_.size(), a_.data(), b_.data()),
            NormalMV);
    }
}; // class NormalMVProposal

/// \brief Multivariate Normal random walk proposal on logit scale
/// \ingroup RandomWalk
template <typename RealType = double, std::size_t Dim = Dynamic>
class NormalMVLogitProposal
{
    public:
    using result_type = RealType;

    /// \brief Only usable when `Dim > 1`
    NormalMVLogitProposal(result_type chol)
        : normal_mv_(static_cast<result_type>(0), chol)
    {
        static_assert(Dim > 1, "**NormalMVLogitProposal** OBJECT DECLARED "
                               "WITH DIMENSION LESS THAN 2");
    }

    /// \brief Only usable when `Dim > 1`
    NormalMVLogitProposal(const result_type *chol)
        : normal_mv_(static_cast<result_type>(0), chol)
    {
        static_assert(Dim > 1, "**NormalMVLogitProposal** OBJECT DECLARED "
                               "WITH DIMENSION LESS THAN 2");
    }

    /// \brief Only usable when `Dim == Dynamic`
    NormalMVLogitProposal(std::size_t dim, result_type chol)
        : normal_mv_(dim, static_cast<result_type>(0), chol), z_(dim)
    {
        static_assert(Dim == Dynamic,
            "**NormalMVLogitProposal** OBJECT DECLARED WITH FIXED DIMENSION");
        VSMC_RUNTIME_ASSERT_ALGORITHM_RANDOM_WALK_NORMAL_MV_LOGIT_DIM(dim);
    }

    /// \brief Only usable when `Dim == Dynamic`
    NormalMVLogitProposal(std::size_t dim, const result_type *chol)
        : normal_mv_(dim, static_cast<result_type>(0), chol), z_(dim)
    {
        static_assert(Dim == Dynamic,
            "**NormalMVLogitProposal** OBJECT DECLARED WITH FIXED DIMENSION");
        VSMC_RUNTIME_ASSERT_ALGORITHM_RANDOM_WALK_NORMAL_MV_LOGIT_DIM(dim);
    }

    template <typename RNGType>
    double operator()(
        RNGType &rng, std::size_t, const result_type *x, result_type *y)
    {
        const std::size_t d = dim();
        normal_mv_(rng, z_.data());
        exp(d - 1, z_.data(), y);
        mul(d - 1, x, y, y);
        mul(d - 1, 1 / x[d - 1], y, y);
        y[d - 1] = 1;
        mul(d, 1 / std::accumulate(y, y + d, 0.0), y, y);

        return static_cast<double>(q(y) - q(x));
    }

    private:
    NormalMVDistribution<RealType, Dim> normal_mv_;
    internal::StaticVector<RealType, Dim> z_;

    result_type q(const result_type *x)
    {
        const std::size_t d = dim();

        result_type slw = 1;
        result_type sllw = 0;
        const result_type w = x[d - 1];
        for (std::size_t i = 0; i != d - 1; ++i) {
            double v = x[i] / w;
            slw += v;
            sllw += std::log(v);
        }
        sllw -= d * std::log(slw);

        return sllw;
    }
}; // class NormalMVLogitProposal

} // namespace vsmc

#endif // VSMC_ALGORITHM_RANDOM_WALK_HPP
