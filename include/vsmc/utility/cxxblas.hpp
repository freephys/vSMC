#ifndef VSMC_UTILITY_CXXBLAS_HPP
#define VSMC_UTILITY_CXXBLAS_HPP

#include <vsmc/internal/config.hpp>
#include <vsmc/internal/assert.hpp>
#include <vsmc/internal/defines.hpp>
#include <vsmc/internal/forward.hpp>
#include <vsmc/internal/traits.hpp>

#undef VSMC_USE_CBLAS
#undef VSMC_USE_CXXBLAS
#undef VSMC_CXXBLAS_INT

#if VSMC_USE_MKL // MKL
#include <mkl_cblas.h>
#define VSMC_USE_CBLAS 1
#define VSMC_USE_CXXBLAS 1
#define VSMC_CXXBLAS_INT MKL_INT
#elif VSMC_USE_VECLIB // vecLib
#include <vecLib/cblas.h>
#define VSMC_USE_CBLAS 1
#define VSMC_USE_CXXBLAS 1
#define VSMC_CXXBLAS_INT int
#elif VSMC_USE_GENERIC_CBLAS // Generic CBlas
#define VSMC_USE_CBLAS 1
#define VSMC_USE_CXXBLAS 1
#define VSMC_CXXBLAS_INT int
#elif VSMC_USE_ARMADILLO // Armadillo
#include <armadillo>
#define VSMC_USE_CBLAS 0
#define VSMC_USE_CXXBLAS 0
#define VSMC_CXXBLAS_INT arma::blas_int
#elif VSMC_USE_EIGEN // Eigen
#include <Eigen/Dense>
#define VSMC_USE_CBLAS 0
#define VSMC_USE_CXXBLAS 0
#define VSMC_CXXBLAS_INT EIGEN_DEFAULT_DENSE_INDEX_TYPE
#else // No known CBlas
#define VSMC_USE_CBLAS 0
#define VSMC_USE_CXXBLAS 1
#define VSMC_CXXBLAS_INT VSMC_SIZE_TYPE
#endif

namespace vsmc { namespace cxxblas {

/// \brief C++ wrapper of cblas_ddot
/// \ingroup CXXBLAS
class DDot
{
    public :

    typedef VSMC_CXXBLAS_INT size_type;

    double operator() (const size_type N,
            const double *X, const size_type incX,
            const double *Y, const size_type incY) const
    {
#if VSMC_USE_CBLAS
        return ::cblas_ddot(N, X, incX, Y, incY);
#else // VSMC_USE_CBLAS
        if (N == 0)
            return 0;

        VSMC_RUNTIME_ASSERT((incX > 0),
                "NON-POSITIVE STRIDE OF X IN **vsmc::DDot**");
        VSMC_RUNTIME_ASSERT((incY > 0),
                "NON-POSITIVE STRIDE OF Y IN **vsmc::DDot**");

        if (X == Y && incX == incY) {
            double res = 0;
            if (incX == 1) {
                for (size_type i = 0; i != N; ++i)
                    res += X[i] * X[i];
                return res;
            } else {
                double res = 0;
                for (size_type i = 0; i != N; ++i, X += incX)
                    res += (*X) * (*X);
                return res;
            }
        }

        if (incX == 1 && incY == 1) {
            double res = 0;
            for (size_type i = 0; i != N; ++i, ++X, ++Y)
                res += (*X) * (*Y);
            return res;
        }

        double res = 0;
        for (size_type i = 0; i != N; ++i, X += incX, Y += incY)
            res += (*X) * (*Y);

        return res;
#endif // VSMC_USE_CBLAS
    }
}; // class DDot

/// \brief C++ wrapper of cblas_dgemv
/// \ingroup CXXBLAS
class DGemv
{
    public :

    typedef VSMC_CXXBLAS_INT size_type;

    void operator() (MatrixOrder order, MatrixTranspose trans,
            const size_type M, const size_type N, const double alpha,
            const double *A, const size_type lda,
            const double *X, const size_type incX,
            const double beta, double *Y, const size_type incY) const
    {
#if VSMC_USE_CBLAS
        ::CBLAS_ORDER cblas_order;
        switch (order) {
            case vsmc::RowMajor :
                cblas_order = ::CblasRowMajor;
                break;
            case vsmc::ColMajor :
                cblas_order = ::CblasColMajor;
                break;
            default :
                cblas_order = ::CblasRowMajor;
                break;
        }

        ::CBLAS_TRANSPOSE cblas_trans;
        switch (trans) {
            case vsmc::NoTrans :
                cblas_trans = ::CblasNoTrans;
                break;
            case vsmc::Trans :
                cblas_trans = ::CblasTrans;
                break;
            case vsmc::ConjTrans :
                cblas_trans = ::CblasConjTrans;
                break;
            default :
                cblas_trans = ::CblasNoTrans;
                break;
        }

        ::cblas_dgemv(cblas_order, cblas_trans,
                M, N, alpha, A, lda, X, incX, beta, Y, incY);
#else // VSMC_USE_CBLAS
        if (M == 0 || N == 0)
            return;

        if (alpha == 0 && beta == 0)
            return;

        VSMC_RUNTIME_ASSERT((lda > 0),
                "NON-POSITIVE STRIDE OF A IN **vsmc::DGemv**");
        VSMC_RUNTIME_ASSERT((incX > 0),
                "NON-POSITIVE STRIDE OF X IN **vsmc::DGemv**");
        VSMC_RUNTIME_ASSERT((incY > 0),
                "NON-POSITIVE STRIDE OF Y IN **vsmc::DGemv**");

        const size_type lenX = (trans == NoTrans) ? N : M;
        const size_type lenY = (trans == NoTrans) ? M : N;

        // y := beta * y
        if (beta == 0) {
            size_type iy = 0;
            for (size_type i = 0; i != lenY; ++i, iy += incY)
                Y[iy] = 0;
        } else {
            size_type iy = 0;
            for (size_type i = 0; i != lenY; ++i, iy += incY)
                Y[iy] = beta * Y[iy];
        }

        if (alpha == 0)
            return;
        // y += alpha * AX
        if ((order == RowMajor && trans == NoTrans) ||
                (order == ColMajor && trans == Trans)) {
            size_type iy = 0;
            for (size_type i = 0; i != lenY; ++i, iy += incY) {
                double res = 0;
                size_type ix = 0;
                for (size_type j = 0; j != lenX; ++j, ix += incX)
                    res += X[ix] * A[i * lda + j];
                Y[iy] += alpha * res;
            }
        } else if ((order == RowMajor && trans == Trans) ||
                (order == ColMajor && trans == NoTrans)) {
            size_type ix = 0;
            for (size_type j = 0; j != lenX; ++j, ix += incX) {
                double ax = alpha * X[ix];
                size_type iy = 0;
                for (size_type i = 0; i != lenY; ++i, iy += incY)
                    Y[iy] += ax * A[j * lda + i];
            }
        } else {
            VSMC_RUNTIME_ASSERT(false, "INVALID INPUT TO **vsmc::DGemv**");
        }
#endif // VSMC_USE_CBLAS
    }
}; // class DGemv

/// \brief Check if a pointer is aligned to 16 bytes
/// \ingroup CXXBLAS
inline bool is_sse_aligned (void *ptr)
{
    return ((unsigned long) ptr & 15) == 0;
}

/// \brief Compute the importance sampling integral of univariate variable
/// \ingroup CXXBLAS
class ISIntegral1
{
    public :

    typedef VSMC_CXXBLAS_INT size_type;

    /// \brief Compute the importance sampling integral
    ///
    /// \param N Number of particles
    /// \param hX A `N` vector \f$(h(X_i))\f$
    /// \param W Normalized weights
    /// \return The importance sampling estiamte
    double operator() (size_type N, const double *hX, const double *W) const
    {
        if (N == 0)
            return 0;
#if VSMC_USE_CXXBLAS
        return DDot()(N, hX, 1, W, 1);
#elif VSMC_USE_ARMADILLO
        return arma::dot(arma::vec(hX, N), arma::vec(W, N));
#elif VSMC_USE_EIGEN
        if (is_sse_aligned((void *) hX) && is_sse_aligned((void *) W)) {
            Eigen::Map<const Eigen::VectorXd, Eigen::Aligned> hXEigen(hX, N);
            Eigen::Map<const Eigen::VectorXd, Eigen::Aligned> WEigen(W, N);
            return hXEigen.dot(WEigen);
        } else {
            Eigen::Map<const Eigen::VectorXd> hXEigen(hX, N);
            Eigen::Map<const Eigen::VectorXd> WEigen(W, N);
            return hXEigen.dot(WEigen);
        }
#else
#error No implementation of ISIntegral1
#endif
    }
}; // ISIntegral1


/// \brief Compute the importance sampling integral of multivariate variable
/// \ingroup CXXBLAS
class ISIntegralD
{
    public :

    typedef VSMC_CXXBLAS_INT size_type;

    /// \brief Compute the importance sampling integral
    ///
    /// \param N Number of particles
    /// \param dim Number of variables
    /// \param hX A `N` by `dim` row major matrix, each row `i` contains
    /// \f$h(X_i) = (h_1(X_i), h_2(X_i), \dots, h_d(X_i))\f$
    /// \param W Normalized weights
    /// \param Eh The importance sampling estiamtes of \f$E[h(X)]\f$
    void operator() (size_type N, size_type dim,
            const double *hX, const double *W, double *Eh) const
    {
        if (N == 0)
            return;
#if VSMC_USE_CXXBLAS
        DGemv()(RowMajor, Trans, N, dim, 1, hX, dim, W, 1, 0, Eh, 1);
#elif VSMC_USE_ARMADILLO
        arma::vec res(Eh, dim, false);
        res = arma::mat(hX, dim, N) * arma::vec(W, N);
#elif VSMC_USE_EIGEN
        if (is_sse_aligned((void *) hX) && is_sse_aligned((void *) W) &&
                is_sse_aligned((void *) Eh)) {
            Eigen::Map<const Eigen::Matrix<
                double, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor>,
                Eigen::Aligned> hXEigen(hX, dim, N);
            Eigen::Map<const Eigen::VectorXd, Eigen::Aligned> WEigen(W, N);
            Eigen::Map<Eigen::VectorXd, Eigen::Aligned> res(Eh, dim);
            res = hXEigen * WEigen;
        } else {
            Eigen::Map<const Eigen::Matrix<
                double, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> >
                hXEigen(hX, dim, N);
            Eigen::Map<const Eigen::VectorXd> WEigen(W, N);
            Eigen::Map<Eigen::VectorXd> res(Eh, dim);
            res = hXEigen * WEigen;
        }
#else
#error No implementation of ISIntegralD
#endif
    }
}; // class ISIntegralD

} } // namespace vsmc::cxxblas

#endif // VSMC_UTILITY_CXXBLAS_HPP
