//============================================================================
// vSMC/include/vsmc/internal/config.hpp
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

#ifndef VSMC_INTERNAL_CONFIG_HPP
#define VSMC_INTERNAL_CONFIG_HPP

#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif

#include <vsmc/internal/compiler.hpp>

#ifndef VSMC_NO_RUNTIME_ASSERT
#ifndef NDEBUG
#define VSMC_NO_RUNTIME_ASSERT 0
#else
#define VSMC_NO_RUNTIME_ASSERT 1
#endif
#endif

#ifndef VSMC_NO_RUNTIME_WARNING
#ifndef NDEBUG
#define VSMC_NO_RUNTIME_WARNING 0
#else
#define VSMC_NO_RUNTIME_WARNING 1
#endif
#endif

/// \brief Turn vSMC runtime assertions into exceptions
/// \ingroup Config
#ifndef VSMC_RUNTIME_ASSERT_AS_EXCEPTION
#define VSMC_RUNTIME_ASSERT_AS_EXCEPTION 0
#endif

/// \brief Turn vSMC runtime warnings into exceptions
/// \ingroup Config
#ifndef VSMC_RUNTIME_WARNING_AS_EXCEPTION
#define VSMC_RUNTIME_WARNING_AS_EXCEPTION 0
#endif

// Parallelization features

#ifndef VSMC_HAS_OMP
#define VSMC_HAS_OMP 0
#endif

#ifndef VSMC_HAS_TBB
#define VSMC_HAS_TBB 0
#endif

#ifndef VSMC_USE_TBB
#define VSMC_USE_TBB VSMC_HAS_TBB
#endif

#ifndef VSMC_HAS_TBB_MALLOC
#define VSMC_HAS_TBB_MALLOC VSMC_HAS_TBB
#endif

#ifndef VSMC_HAS_MPI
#define VSMC_HAS_MPI 0
#endif

#ifndef VSMC_HAS_OPENCL
#define VSMC_HAS_OPENCL 0
#endif

// Optional libraries

#ifndef VSMC_HAS_CBLAS
#define VSMC_HAS_CBLAS 0
#endif

#ifndef VSMC_HAS_HDF5
#define VSMC_HAS_HDF5 0
#endif

#ifndef VSMC_HAS_MKL
#define VSMC_HAS_MKL 0
#endif

#ifndef VSMC_USE_MKL_CBLAS
#define VSMC_USE_MKL_CBLAS VSMC_HAS_MKL
#endif

#ifndef VSMC_USE_MKL_VML
#define VSMC_USE_MKL_VML VSMC_HAS_MKL
#endif

#ifndef VSMC_USE_MKL_VSL
#define VSMC_USE_MKL_VSL VSMC_HAS_MKL
#endif

#endif // VSMC_INTERNAL_CONFIG_HPP
