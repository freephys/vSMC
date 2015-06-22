//============================================================================
// vSMC/include/vsmc/rng/mkl_brng.hpp
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

#ifndef VSMC_RNG_MKL_BRNG_HPP
#define VSMC_RNG_MKL_BRNG_HPP

#include <vsmc/vsmc.h>
#include <vsmc/rng/internal/common.hpp>
#include <vsmc/rng/engine.hpp>
#include <vsmc/utility/mkl.hpp>

#define VSMC_DEFINE_RNG_MKL_BRNG(RNGType, name)                               \
    template <>                                                               \
    inline MKL_INT mkl_brng<RNGType>()                                        \
    {                                                                         \
        return static_cast<MKL_INT>(::vsmc_mkl_brng_##name());                \
    }

namespace vsmc
{

/// \brief Register a C++11 RNG engine for use as a MKL BRNG
/// \ingroup MKL
template <typename RNGType>
MKL_INT mkl_brng();

#include <vsmc/rng/internal/mkl_brng_defines.hpp>

} // namespace vsmc

#endif // VSMC_RNG_MKL_BRNG_HPP
