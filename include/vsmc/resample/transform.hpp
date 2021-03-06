//============================================================================
// vSMC/include/vsmc/resample/transform.hpp
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

#ifndef VSMC_RESAMPLE_TRANSFORM_HPP
#define VSMC_RESAMPLE_TRANSFORM_HPP

#include <vsmc/resample/internal/common.hpp>

namespace vsmc
{

/// \brief Transform uniform [0, 1] sequence into replication numbers
/// \ingroup Resample
template <typename IntType, typename U01SeqType>
inline void resample_trans_u01_rep(std::size_t M, std::size_t N,
    const double *weight, U01SeqType &&u01seq, IntType *replication)
{
    // Given N sorted U01 random variates
    // Compute M replication numbers based on M weights

    if (M == 0)
        return;

    if (M == 1) {
        *replication = static_cast<IntType>(N);
        return;
    }

    std::memset(replication, 0, sizeof(IntType) * M);
    if (N == 0)
        return;

    double accw = 0;
    std::size_t j = 0;
    for (std::size_t i = 0; i != M - 1; ++i) {
        accw += weight[i];
        while (j != N && u01seq[j] <= accw) {
            ++replication[i];
            ++j;
        }
    }
    replication[M - 1] = static_cast<IntType>(N - j);
}

/// \brief Transform uniform [0, 1] sequence into parent indices
/// \ingroup Resample
template <typename IntType, typename U01SeqType>
inline void resample_trans_u01_index(std::size_t M, std::size_t N,
    const double *weight, U01SeqType &&u01seq, IntType *index)
{
    if (M == 0 || N == 0)
        return;

    std::memset(index, 0, sizeof(IntType) * N);
    if (M == 1)
        return;

    double accw = 0;
    std::size_t j = 0;
    for (std::size_t i = 0; i != M - 1; ++i) {
        accw += weight[i];
        while (j != N && u01seq[j] <= accw)
            index[j++] = static_cast<IntType>(i);
    }
    while (j != N)
        index[j++] = static_cast<IntType>(M - 1);
}

/// \brief Transform replication numbers into parent indices
/// \ingroup Resample
template <typename IntType1, typename IntType2>
inline void resample_trans_rep_index(
    std::size_t M, std::size_t N, const IntType1 *replication, IntType2 *index)
{
    if (M == 0 || N == 0)
        return;

    if (M != N) {
        std::size_t dst = 0;
        for (std::size_t src = 0; src != M; ++src) {
            const IntType1 rep = replication[src];
            for (IntType1 r = 0; r != rep; ++r)
                index[dst++] = static_cast<IntType2>(src);
        }
        return;
    }

    IntType1 time = 0;
    std::size_t src = 0;
    for (std::size_t dst = 0; dst != N; ++dst) {
        if (replication[dst] != 0) {
            index[dst] = static_cast<IntType2>(dst);
        } else {
            // replication[dst] has zero child, copy from elsewhere
            if (replication[src] < time + 2) {
                // only 1 child left on replication[src]
                time = 0;
                do // move src to a position with at least 2 children
                    ++src;
                while (replication[src] < 2);
            }
            index[dst] = static_cast<IntType2>(src);
            ++time;
        }
    }
}

/// \brief Transform parent indices into replication numbers
/// \ingroup Resample
template <typename IntType1, typename IntType2>
inline void resample_trans_index_rep(
    std::size_t M, std::size_t N, const IntType1 *index, IntType2 *replication)
{
    if (M == 0 || N == 0)
        return;

    std::memset(replication, 0, sizeof(IntType2) * M);

    for (std::size_t i = 0; i != N; ++i)
        ++replication[index[i]];
}

/// \brief Transform normalized weights to normalized residual and integrals,
/// and return the number of remaining elements to be resampled
/// \ingroup Resample
template <typename IntType>
inline std::size_t resample_trans_residual(std::size_t M, std::size_t N,
    const double *weight, double *resid, IntType *integ)
{
    double integral = 0;
    const double coeff = static_cast<double>(N);
    for (std::size_t i = 0; i != M; ++i) {
        resid[i] = std::modf(coeff * weight[i], &integral);
        integ[i] = static_cast<IntType>(integral);
    }
    mul(M, 1 / std::accumulate(resid, resid + M, 0.0), resid, resid);
    IntType R = std::accumulate(integ, integ + M, static_cast<IntType>(0));

    return N - static_cast<std::size_t>(R);
}

} // namespace vsmc

#endif // VSMC_RESAMPLE_TRANSFORM_HPP
