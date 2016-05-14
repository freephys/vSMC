//============================================================================
// vSMC/include/vsmc/rng/normal_mv_distribution.hpp
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

#ifndef VSMC_RNG_NORMAL_MV_DISTRIBUTION_HPP
#define VSMC_RNG_NORMAL_MV_DISTRIBUTION_HPP

#include <vsmc/rng/internal/common.hpp>
#include <vsmc/rng/normal_distribution.hpp>

namespace vsmc
{

/// \brief Multivariate Normal distribution
/// \ingroup Distribution
///
/// \details
/// The distribution is parameterized by its mean vector and the lower
/// triangular elements of the Cholesky decomposition of the covaraince matrix,
/// packed row by row.
///
/// \tparam RealType Only `float` and `double` are supported.
/// \tparam Dim If `Dim > 0`, then the distribution has a static size and does
/// not use dynamic memory. If `Dim == Dynamic`, then the dimension of the
/// distribution is specified at runtime.
template <typename RealType, std::size_t Dim>
class NormalMVDistribution
{

    public:
    using result_type = RealType;
    using distribution_type = NormalMVDistribution<RealType, Dim>;

    class param_type
    {
        static_assert(internal::is_one_of<RealType, float, double>::value,
            "**NormalMVDistributon::param_type** USED WITH RealType OTHER "
            "THAN float OR double");

        public:
        using result_type = RealType;
        using distribution_type = NormalMVDistribution<RealType, Dim>;

        param_type(const result_type *mean, const result_type *chol)
            : normal_(0, 1)
            , is_null_mean_(mean == nullptr)
            , is_null_chol_(chol == nullptr)
        {
            static_assert(Dim != Dynamic, "**NormalMVDistribution::param_type*"
                                          "* OBJECT DECLARED WITH DYNAMIC "
                                          "DIMENSION");
            init(mean, chol);
        }

        param_type(
            std::size_t dim, const result_type *mean, const result_type *chol)
            : normal_(0, 1)
            , mean_(dim)
            , chol_(dim * (dim + 1) / 2)
            , is_null_mean_(mean == nullptr)
            , is_null_chol_(chol == nullptr)
        {
            static_assert(Dim == Dynamic, "**NormalMVDistribution::param_type*"
                                          "* OBJECT DECLARED WITH FIXED "
                                          "DIMENSION");
            init(mean, chol);
        }

        std::size_t dim() const { return mean_.size(); }

        const result_type *mean() const { return mean_.data(); }

        const result_type *chol() const { return chol_.data(); }

        friend bool operator==(
            const param_type &param1, const param_type &param2)
        {
            if (param1.norm_ != param2.norm_)
                return false;
            if (param1.mean_ != param2.mean_)
                return false;
            if (param1.chol_ != param2.chol_)
                return false;
            if (param1.is_null_mean_ != param2.is_null_mean_)
                return false;
            if (param1.is_null_chol_ != param2.is_null_chol_)
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
            if (!os)
                return os;

            os << param.norm_ << ' ';
            os << param.mean_ << ' ';
            os << param.chol_ << ' ';
            os << param.is_null_mean_ << ' ';
            os << param.is_null_chol_;

            return os;
        }

        template <typename CharT, typename Traits>
        friend std::basic_istream<CharT, Traits> &operator>>(
            std::basic_istream<CharT, Traits> &is, const param_type &param)
        {
            if (!is)
                return is;

            NormalDistribution<RealType> normal;
            internal::StaticVector<result_type, Dim> mean;
            internal::StaticVector<result_type, Dim *(Dim + 1) / 2> chol;
            bool is_null_mean;
            bool is_null_chol;

            is >> std::ws >> normal;
            is >> std::ws >> mean;
            is >> std::ws >> chol;
            is >> std::ws >> is_null_mean;
            is >> std::ws >> is_null_chol;

            if (is) {
                param.normal_ = std::move(normal);
                param.mean_ = std::move(mean);
                param.chol_ = std::move(chol);
                param.is_null_mean_ = is_null_mean;
                param.is_null_chol_ = is_null_chol;
            } else {
                is.setstate(std::ios_base::failbit);
            }

            return is;
        }

        private:
        NormalDistribution<RealType> normal_;
        internal::StaticVector<result_type, Dim> mean_;
        internal::StaticVector<result_type, Dim *(Dim + 1) / 2> chol_;
        bool is_null_mean_;
        bool is_null_chol_;

        friend distribution_type;

        void init(const result_type *mean, const result_type *chol)
        {
            if (mean == nullptr)
                std::fill(mean_.begin(), mean_.end(), 0);
            else
                std::copy_n(mean, mean_.size(), mean_.begin());

            if (chol == nullptr)
                std::fill(chol_.begin(), chol_.end(), 0);
            else
                std::copy_n(chol, chol_.size(), chol_.begin());

            if (chol == nullptr)
                for (std::size_t i = 0; i != mean_.size(); ++i)
                    chol_[i * (i + 1) / 2 + i] = 1;
        }
    }; // class param_type

    /// \brief Only usable when `Dim > 0`
    ///
    /// \param mean Mean vector, if it is a null pointer, then the mean is a
    /// zero vector
    /// \param chol The lower triangular elements of the Cholesky decomposition
    /// of the covaraince matrix, packed row by row. If it is a nullpointer,
    /// then the covariance is the identicy matrix \f$I\f$
    NormalMVDistribution(const result_type *mean, const result_type *chol)
        : param_(mean, chol)
    {
        internal::size_check<VSMC_BLAS_INT>(
            Dim, "NormalMVDistribution::NormalMVDistribution");
        reset();
    }

    /// \brief Only usable when `Dim == Dynamic`
    NormalMVDistribution(
        std::size_t dim, const result_type *mean, const result_type *chol)
        : param_(dim, mean, chol)
    {
        internal::size_check<VSMC_BLAS_INT>(
            dim, "NormalMVDistribution::NormalMVDistribution");
        reset();
    }

    explicit NormalMVDistribution(const param_type &param) : param_(param)
    {
        internal::size_check<VSMC_BLAS_INT>(
            param.dim_, "NormalMVDistribution::NormalMVDistribution");
        reset();
    }

    explicit NormalMVDistribution(param_type &&param)
        : param_(std::move(param))
    {
        internal::size_check<VSMC_BLAS_INT>(
            param.dim_, "NormalMVDistribution::NormalMVDistribution");
        reset();
    }

    void min(result_type *x) const
    {
        std::fill_n(x, dim(), std::numeric_limits<result_type>::lowest());
    }

    void max(result_type *x) const
    {
        std::fill_n(x, dim(), std::numeric_limits<result_type>::max());
    }

    void reset() { param_.normal_.reset(); }

    std::size_t dim() const { return param_.dim(); }

    const result_type *mean() const { return param_.mean(); }

    const result_type *chol() const { return param_.chol(); }

    const param_type &param() const { return param_; }

    void param(const param_type &param)
    {
        param_ = param;
        reset();
    }

    void param(param_type &&param)
    {
        param_ = std::move(param);
        reset();
    }

    template <typename RNGType>
    void operator()(RNGType &rng, result_type *r)
    {
        operator()(rng, r, param_);
    }

    template <typename RNGType>
    void operator()(RNGType &rng, result_type *r, const param_type &param)
    {
        generate(rng, r, param);
    }

    template <typename RNGType>
    void operator()(RNGType &rng, std::size_t n, result_type *r)
    {
        operator()(rng, n, r, param_);
    }

    template <typename RNGType>
    void operator()(
        RNGType &rng, std::size_t n, result_type *r, const param_type &param)
    {
        normal_mv_distribution(rng, n, r, param.dim(),
            (param.is_null_mean_ ? param.mean() : nullptr),
            (param.is_null_chol_ ? param.chol() : nullptr));
    }

    friend bool operator==(
        const distribution_type &dist1, const distribution_type &dist2)
    {
        if (dist1.param_ != dist2.param_)
            return false;
        return true;
    }

    friend bool operator!=(
        const distribution_type &dist1, const distribution_type &dist2)
    {
        return !(dist1 == dist2);
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os, const distribution_type &dist)
    {
        if (!os)
            return os;

        os << dist.param_;

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &operator>>(
        std::basic_istream<CharT, Traits> &is, distribution_type &dist)
    {
        if (!is)
            return is;

        param_type param;
        is >> std::ws >> param;
        if (is)
            dist.param_ = std::move(param);

        return is;
    }

    private:
    param_type param_;

    template <typename RNGType>
    void generate(RNGType &rng, result_type *r, const param_type &param)
    {
        param_.normal_(rng, param.dim(), r);
        if (!param.is_null_chol_)
            mulchol(r, param);
        if (!param.is_null_mean_)
            add(param.dim(), param.mean(), r, r);
    }

    void mulchol(float *r, const param_type &param)
    {
        internal::cblas_stpmv(internal::CblasRowMajor, internal::CblasLower,
            internal::CblasNoTrans, internal::CblasNonUnit,
            static_cast<VSMC_BLAS_INT>(dim()), param.chol(), r, 1);
    }

    void mulchol(double *r, const param_type &param)
    {
        internal::cblas_dtpmv(internal::CblasRowMajor, internal::CblasLower,
            internal::CblasNoTrans, internal::CblasNonUnit,
            static_cast<VSMC_BLAS_INT>(dim()), param.chol(), r, 1);
    }
}; // class NormalMVDistribution

namespace internal
{

inline void normal_mv_distribution_mulchol(
    std::size_t n, float *r, std::size_t dim, const float *chol)
{
    cblas_strmm(CblasRowMajor, CblasRight, CblasLower, CblasTrans,
        CblasNonUnit, static_cast<VSMC_BLAS_INT>(n),
        static_cast<VSMC_BLAS_INT>(dim), 1, chol,
        static_cast<VSMC_BLAS_INT>(dim), r, static_cast<VSMC_BLAS_INT>(dim));
}

inline void normal_mv_distribution_mulchol(
    std::size_t n, double *r, std::size_t dim, const double *chol)
{
    cblas_dtrmm(CblasRowMajor, CblasRight, CblasLower, CblasTrans,
        CblasNonUnit, static_cast<VSMC_BLAS_INT>(n),
        static_cast<VSMC_BLAS_INT>(dim), 1, chol,
        static_cast<VSMC_BLAS_INT>(dim), r, static_cast<VSMC_BLAS_INT>(dim));
}

} // namespace vsmc::internal

/// \brief Generating multivariate Normal random varaites
/// \ingroup Distribution
template <typename RealType, typename RNGType>
inline void normal_mv_distribution(RNGType &rng, std::size_t n, RealType *r,
    std::size_t dim, const RealType *mean, const RealType *chol)
{
    static_assert(internal::is_one_of<RealType, float, double>::value,
        "**normal_mv_distribution** USED WITH RealType OTHER THAN float OR "
        "double");

    internal::size_check<VSMC_BLAS_INT>(n, "normal_mv_distribution");
    internal::size_check<VSMC_BLAS_INT>(dim, "normal_mv_distribution");

    normal_distribution(
        rng, n * dim, r, static_cast<RealType>(0), static_cast<RealType>(1));
    if (chol != nullptr) {
        Vector<RealType> cholf(dim * dim);
        for (std::size_t i = 0; i != dim; ++i)
            for (std::size_t j = 0; j <= i; ++j)
                cholf[i * dim + j] = *chol++;
        internal::normal_mv_distribution_mulchol(n, r, dim, cholf.data());
    }
    if (mean != nullptr)
        for (std::size_t i = 0; i != n; ++i, r += dim)
            add(dim, mean, r, r);
}

template <typename RealType, std::size_t Dim, typename RNGType>
inline void normal_mv_distribution(RNGType &rng, std::size_t n, RealType *r,
    const typename NormalMVDistribution<RealType, Dim>::param_type &param)
{
    NormalMVDistribution<RealType, Dim> dist(param);
    dist(rng, n, r);
}

template <typename RealType, std::size_t Dim, typename RNGType>
inline void rand(RNGType &rng, NormalMVDistribution<RealType, Dim> &dist,
    std::size_t n, RealType *r)
{
    dist(rng, n, r);
}

} // namespace vsmc

#endif // VSMC_RNG_NORMAL_DISTRIBUTION_HPP
