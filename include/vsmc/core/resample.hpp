#ifndef VSMC_CORE_RESAMPLE_HPP
#define VSMC_CORE_RESAMPLE_HPP

#include <vsmc/internal/common.hpp>

namespace vsmc {

namespace internal {

template <typename SizeType, typename RngSetType>
inline void weight2replication (SizeType N, SizeType S, RngSetType &rng_set,
        const double *weight, SizeType *replication)
{
        double sum_w = 0;
        double acc_w = 0;
        SizeType acc_s = 0;

        for (SizeType i = 0; i != N; ++i) {
            replication[i] = 0;
            sum_w += weight[i];
        }

        for (SizeType i = 0; i != N; ++i) {
            if (acc_s < S && weight[i] > 0) {
                typedef typename cxx11::make_signed<SizeType>::type s_t;
                s_t s = S - acc_s;
                double p = weight[i] / (sum_w - acc_w);
                if (p < 0) {
                    p = 0;
                    assert(p > -1e-6);
                }
                if (p > 1) {
                    p = 1;
                    assert(p - 1 < 1e-6);
                }
                cxx11::binomial_distribution<s_t> binom(s, p);
                replication[i] = binom(rng_set.rng(i));
            }
            acc_w += weight[i];
            acc_s += replication[i];
        }
}

} // namespace vsmc::internal

/// \defgroup Resampling Resampling
/// \ingroup Core
/// \brief Various resampling methods

/// \brief Resample scheme
/// \ingroup Resampling
enum ResampleScheme {
    MULTINOMIAL,         ///< Multinomial resampling
    RESIDUAL,            ///< Reisudal resampling
    STRATIFIED,          ///< Startified resampling
    SYSTEMATIC,          ///< Systematic resampling
    RESIDUAL_STRATIFIED, ///< Stratified resampling on the residuals
    RESIDUAL_SYSTEMATIC  ///< Systematic resampling on the residuals
}; // enum ResamleScheme

/// \brief Int-to-Type struct template for resampling scheme
/// \ingroup Resampling
template <typename EnumType, EnumType S> struct ResampleType {};

/// \brief Resample class template
/// \ingroup Resampling
template <typename ResType, typename SizeType, typename RngSetType>
class Resample {};

/// \brief Multinomial resampling
/// \ingroup Resampling
template <typename SizeType, typename RngSetType>
class Resample<ResampleType<ResampleScheme, MULTINOMIAL>,
      SizeType, RngSetType>
{
    public :

    void operator() (SizeType N, RngSetType &rng_set,
            const double *weight, SizeType *replication)
    {
        internal::weight2replication(N, N, rng_set, weight, replication);
    }
}; // Mulitnomial resampling

/// \brief Residual resampling
/// \ingroup Resampling
template <typename SizeType, typename RngSetType>
class Resample<ResampleType<ResampleScheme, RESIDUAL>,
      SizeType, RngSetType>
{
    public :

    void operator() (SizeType N, RngSetType &rng_set,
            const double *weight, SizeType *replication)
    {
        using std::modf;

        residual_.resize(N);
        integral_.resize(N);
        for (SizeType i = 0; i != N; ++i)
            residual_[i] = modf(N * weight[i], &integral_[i]);
        double dsize = std::accumulate(residual_.begin(), residual_.end(),
                static_cast<double>(0));
        SizeType size = static_cast<SizeType>(dsize);
        for (SizeType i = 0; i != N; ++i)
            residual_[i] /= dsize;
        internal::weight2replication(N, size, rng_set, &residual_[0],
                replication);
        for (SizeType i = 0; i != N; ++i)
            replication[i] += static_cast<SizeType>(integral_[i]);
    }

    private :

    std::vector<double> residual_;
    std::vector<double> integral_;
}; // Residual resampling

/// \brief Stratified resampling
/// \ingroup Resampling
template <typename SizeType, typename RngSetType>
class Resample<ResampleType<ResampleScheme, STRATIFIED>,
      SizeType, RngSetType>
{
    public :

    void operator() (SizeType N, RngSetType &rng_set,
            const double *weight, SizeType *replication)
    {
        for (SizeType i = 0; i != N; ++i)
            replication[i] = 0;

        SizeType j = 0;
        SizeType k = 0;
        cxx11::uniform_real_distribution<double> unif(0,1);
        double u = unif(rng_set.rng(0));
        double cw = weight[0];
        while (j != N) {
            while (j < cw * N - u && j != N) {
                ++replication[k];
                u = unif(rng_set.rng(j));
                ++j;
            }
            if (k == N - 1)
                break;
            cw += weight[++k];
        }
    }
}; // Stratified resampling

/// \brief Systematic resampling
/// \ingroup Resampling
template <typename SizeType, typename RngSetType>
class Resample<ResampleType<ResampleScheme, SYSTEMATIC>,
      SizeType, RngSetType>
{
    public :

    void operator() (SizeType N, RngSetType &rng_set,
            const double *weight, SizeType *replication)
    {
        for (SizeType i = 0; i != N; ++i)
            replication[i] = 0;

        SizeType j = 0;
        SizeType k = 0;
        cxx11::uniform_real_distribution<double> unif(0,1);
        double u = unif(rng_set.rng(0));
        double cw = weight[0];
        while (j != N) {
            while (j < cw * N - u && j != N) {
                ++replication[k];
                ++j;
            }
            if (k == N - 1)
                break;
            cw += weight[++k];
        }
    }
}; // Systematic resampling

/// \brief Residual stratified resampling
/// \ingroup Resampling
template <typename SizeType, typename RngSetType>
class Resample<ResampleType<ResampleScheme, RESIDUAL_STRATIFIED>,
      SizeType, RngSetType>
{
    public :

    void operator() (SizeType N, RngSetType &rng_set,
            const double *weight, SizeType *replication)
    {
        using std::modf;

        residual_.resize(N);
        integral_.resize(N);
        for (SizeType i = 0; i != N; ++i) {
            replication[i] = 0;
            residual_[i] = modf(N * weight[i], &integral_[i]);
        }

        double dsize = std::accumulate(residual_.begin(), residual_.end(),
                static_cast<double>(0));
        SizeType size = static_cast<SizeType>(dsize);
        for (SizeType i = 0; i != N; ++i)
            residual_[i] /= dsize;
        SizeType j = 0;
        SizeType k = 0;
        cxx11::uniform_real_distribution<double> unif(0,1);
        double u = unif(rng_set.rng(0));
        double cw = residual_[0];
        while (j != size) {
            while (j < cw * size - u && j != size) {
                ++replication[k];
                u = unif(rng_set.rng(j));
                ++j;
            }
            if (k == N - 1)
                break;
            cw += residual_[++k];
        }
        for (SizeType i = 0; i != N; ++i)
            replication[i] += static_cast<SizeType>(integral_[i]);
    }

    private :

    std::vector<double> residual_;
    std::vector<double> integral_;
}; // Residual stratified resampling

/// \brief Residual systematic resampling
/// \ingroup Resampling
template <typename SizeType, typename RngSetType>
class Resample<ResampleType<ResampleScheme, RESIDUAL_SYSTEMATIC>,
      SizeType, RngSetType>
{
    public :

    void operator() (SizeType N, RngSetType &rng_set,
            const double *weight, SizeType *replication)
    {
        using std::modf;

        residual_.resize(N);
        integral_.resize(N);
        for (SizeType i = 0; i != N; ++i) {
            replication[i] = 0;
            residual_[i] = modf(N * weight[i], &integral_[i]);
        }

        double dsize = std::accumulate(residual_.begin(), residual_.end(),
                static_cast<double>(0));
        SizeType size = static_cast<SizeType>(dsize);
        for (SizeType i = 0; i != N; ++i)
            residual_[i] /= dsize;
        SizeType j = 0;
        SizeType k = 0;
        cxx11::uniform_real_distribution<double> unif(0,1);
        double u = unif(rng_set.rng(0));
        double cw = residual_[0];
        while (j != size) {
            while (j < cw * size - u && j != size) {
                ++replication[k];
                ++j;
            }
            if (k == N - 1)
                break;
            cw += residual_[++k];
        }
        for (SizeType i = 0; i != N; ++i)
            replication[i] += static_cast<SizeType>(integral_[i]);
    }

    private :

    std::vector<double> residual_;
    std::vector<double> integral_;
}; // Residual systematic resampling

} // namespace vsmc

#endif // VSMC_CORE_RESAMPLE_HPP
