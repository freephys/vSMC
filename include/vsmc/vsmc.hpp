//============================================================================
// vSMC/include/vsmc/vsmc.hpp
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

#ifndef VSMC_HPP
#define VSMC_HPP

/// \defgroup Config Configuration
/// \brief Configuration macros and default values if undefined

/// \defgroup Definitions Enumerators, placeholders and macros
/// \brief Enumerator, placeholder and macro definitions

/// \defgroup Traits Traits
/// \brief Trait classes

/// \defgroup Core Core
/// \brief Constructing samplers with operations on the whole particle set

/// \defgroup Dispatch Grand Central Dispatch
/// \brief C++ wrapper of Apple GCD

/// \defgroup Resample Resampling algorithms
/// \brief Resampling algorithm functor classes

/// \defgroup Adapter Adapter
/// \brief Adapter class templates for constructing concrete objects

/// \defgroup MPI Message Passing Interface
/// \brief Parallel samplers using MPI

/// \defgroup OpenCL OpenCL
/// \brief Parallel sampler using OpenCL

/// \defgroup SMP Symmetric Multiprocessing
/// \brief Parallel samplers using multi-threading on SMP architecture

/// \defgroup CILK Intel Cilk Plus
/// \ingroup SMP
/// \brief Parallel samplers using Intel Cilk Plus

/// \defgroup GCD Grand Central Dispatch
/// \ingroup SMP
/// \brief Parallel samplers using Apple GCD

/// \defgroup OMP OpenMP
/// \ingroup SMP
/// \brief Parallel samplers using OpenMP

/// \defgroup PPL Parallel Pattern Library
/// \ingroup SMP
/// \brief Parallel samplers using Microsoft PPL

/// \defgroup SEQ Sequential
/// \ingroup SMP
/// \brief Sequential samplers

/// \defgroup STD C++11 concurrency
/// \ingroup SMP
/// \brief Parallel samplers using C++11 concurrency

/// \defgroup TBB Intel Threading Building Blocks
/// \ingroup SMP
/// \brief Parallel samplers using Intel TBB

/// \defgroup Math Mathematics
/// \brief Mathematical utilities

/// \defgroup CBLAS C BLAS
/// \ingroup Math
/// \brief Selected C BLAS like routines

/// \defgroup Constants Constants
/// \ingroup Math
/// \brief Mathematical constants

/// \defgroup vMath Vector math functions
/// \ingroup Math
/// \brief Math functions on vectors

/// \defgroup RNG Random number generating
/// \brief Random number generating engines and utilities

/// \defgroup AESNIRNG AES-NI
/// \ingroup RNG
/// \brief Random number generating using AES-NI

/// \defgroup CRNG C API
/// \ingroup RNG
/// \brief Random number generating in C interface

/// \defgroup Distribution Distribution
/// \ingroup RNG
/// \brief Distribution random varaites

/// \defgroup MKLRNG Intel Math Kernel Library
/// \ingroup RNG
/// \brief Random number generating using MKL RNG

/// \defgroup R123RNG Random123
/// \ingroup RNG
/// \brief Random number generating using Random123 RNG

/// \defgroup RDRAND Intel DRNG
/// \ingroup RNG
/// \brief Random number generating using Intel RDRAND instructions

/// \defgroup Xorshift Xorshift
/// \ingroup RNG
/// \brief Random number generating using the Xorshift algorithm

/// \defgroup Thread Thread
/// \brief C++11 threading support

/// \defgroup Utility Utility
/// \brief Utilities independent of other part of the library

/// \defgroup AlignedMemory Memory alignment
/// \ingroup Utility
/// \brief Memory allocation with alignment requirement

/// \defgroup CPUID CPUID
/// \ingroup Utility
/// \brief Query CPUID information

/// \defgroup Counter Counter
/// \ingroup Utility
/// \brief Use std::array of unsinged integers as counters

/// \defgroup HDF5IO HDF5 objects IO
/// \ingroup Utility
/// \brief Load and store objects in the HDF5 format

/// \defgroup MKL Intel Math Kernel Library
/// \ingroup Utility
/// \brief Resource management for Intel Math Kernel Library

/// \defgroup Option Program option
/// \ingroup Utility
/// \brief Process program command line options

/// \defgroup Progress Progress
/// \ingroup Utility
/// \brief Display progress while algorithms proceed

/// \defgroup RDTSC RDTSC
/// \ingroup Utility
/// \brief CPU clock cycles count using RDTSC and RDTSCP

/// \defgroup StopWatch Stop watch
/// \ingroup Utility
/// \brief Time measurement

#include <vsmc/internal/config.hpp>

#include <vsmc/core/core.hpp>
#include <vsmc/math/math.hpp>
#include <vsmc/resample/resample.hpp>
#include <vsmc/rng/rng.hpp>
#include <vsmc/smp/smp.hpp>
#include <vsmc/thread/thread.hpp>
#include <vsmc/utility/utility.hpp>

#if VSMC_HAS_GCD
#include <vsmc/gcd/gcd.hpp>
#endif

#if VSMC_HAS_MPI
#include <vsmc/mpi/mpi.hpp>
#endif

#if VSMC_HAS_OPENCL
#include <vsmc/opencl/opencl.hpp>
#endif

#endif // VSMC_HPP
