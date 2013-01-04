#ifndef VSMC_HPP
#define VSMC_HPP

#include <vsmc/internal/config.hpp>

#include <vsmc/core/sampler.hpp>
#include <vsmc/core/particle.hpp>
#include <vsmc/core/monitor.hpp>
#include <vsmc/core/path.hpp>

#include <vsmc/smp/sequential.hpp>
#include <vsmc/smp/adapter.hpp>

#if VSMC_USE_CILK
#include <vsmc/smp/parallel_cilk.hpp>
#endif

#if VSMC_USE_OMP
#include <vsmc/smp/parallel_omp.hpp>
#endif

#if VSMC_USE_TBB
#include <vsmc/smp/parallel_tbb.hpp>
#endif

#if VSMC_USE_STD
#include <vsmc/smp/parallel_std.hpp>
#endif

#if VSMC_USE_CL
#include <vsmc/gpgpu/parallel_cl.hpp>
#endif

#endif // VSMC_HPP

/// \defgroup Core Core
/// \brief Constructing samplers with operations on the whole particle set

/// \defgroup Resampling Resampling
/// \ingroup Core
/// \brief Various resampling methods

/// \defgroup SMP Symmetric Multiprocessing
/// \brief Single threaded and parallel samplers using SMP implementations

/// \defgroup Base Dispatcher
/// \ingroup SMP
/// \brief Base class templates that dispatch computing tasks based on
/// implementations

/// \defgroup Adapter Adapter
/// \ingroup SMP
/// \brief Adapter class templates for constructing concrete objects

/// \defgroup Implementation Implementation
/// \ingroup SMP
/// \brief Implementation class templates that parallelize user defined
/// computing tasks

/// \defgroup Sequential Sequential
/// \ingroup Implementation
/// \brief Single threaded sampler

/// \defgroup STD C++11 Multithread Support
/// \ingroup Implementation
/// \brief Parallelized samplers with C++11 multithread support

/// \defgroup TBB Intel Threading Buidling Block
/// \ingroup Implementation
/// \brief Parallelized samplers with Intel TBB

/// \defgroup CILK Intel Cilk Plus
/// \ingroup Implementation
/// \brief Parallelized samplers with Intel Cilk Plus

/// \defgroup OMP OpenMP
/// \ingroup Implementation
/// \brief Parallelized samplers with OpenMP

/// \defgroup CL OpenCL
/// \brief Parallelized sampler with OpenCL

/// \defgroup Utility Utility
/// \brief Utilities

/// \defgroup Thread Thread
/// \ingroup Utility
/// \brief C++11 Thread utilities

/// \defgroup OpenCL OpenCL
/// \ingroup Utility
/// \brief OpenCL device management utilities

/// \defgroup Random Random
/// \ingroup Utility
/// \brief Generating distribution random variates using Random123 CBRNG
