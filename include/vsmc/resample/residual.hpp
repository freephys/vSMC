//============================================================================
// vSMC/include/vsmc/resample/residual.hpp
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

#ifndef VSMC_RESAMPLE_RESIDUAL_HPP
#define VSMC_RESAMPLE_RESIDUAL_HPP

#include <vsmc/resample/internal/common.hpp>
#include <vsmc/resample/transform.hpp>

namespace vsmc
{

/// \brief Residual resampling
/// \ingroup Resample
class ResampleResidual : public ResampleBase<ResampleResidual>
{
    public:
    /// \brief Generate replication numbers from normalized weights
    ///
    /// \param N Sample size before resampling
    /// \param M Sample size after resampling
    /// \param rng An RNG engine
    /// \param weight N-vector of normalized weights
    /// \param replication N-vector of replication numbers
    template <typename IntType, typename RNGType>
    static void eval(std::size_t N, std::size_t M, RNGType &rng,
        const double *weight, IntType *replication)
    {
        Vector<IntType> integ(N);
        Vector<double> resid(N);
        std::size_t R =
            resample_trans_residual(N, M, weight, resid.data(), integ.data());
        U01SequenceSorted<RNGType, double> u01seq(R, rng);
        resample_trans_u01_rep(N, R, resid.data(), u01seq, replication);
        add(N, replication, integ.data(), replication);
    }
}; // ResampleResidual

/// \brief Type trait of Residual scheme
/// \ingroup Resample
template <>
class ResampleTypeTrait<Residual>
{
    public:
    using type = ResampleResidual;
}; // class ResampleTypeTrait

} // namespace vsmc

#endif // VSMC_RESAMPLE_RESIDUAL_HPP
