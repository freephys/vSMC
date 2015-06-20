//============================================================================
// vSMC/example/rng/include/rng_cl.hpp
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

#ifndef VSMC_EXAMPLE_RNG_OPENCL_HPP
#define VSMC_EXAMPLE_RNG_OPENCL_HPP

#include <vsmc/opencl/cl_buffer.hpp>
#include <vsmc/opencl/cl_manager.hpp>
#include <vsmc/opencl/cl_manip.hpp>
#include <vsmc/rng/philox.hpp>
#include <vsmc/rng/threefry.hpp>
#include <vsmc/rngc/rngc.h>
#include <vsmc/utility/aligned_memory.hpp>
#include <vsmc/utility/stop_watch.hpp>
#if VSMC_HAS_HDF5
#include <vsmc/utility/hdf5io.hpp>
#endif

template <typename RNG, typename CRNG, typename CRNGInit, typename CRNGRand>
inline void rng_cl_rng(std::size_t N, std::size_t M,
    const vsmc::CLProgram &program, CRNGInit &cinit, CRNGRand &crand,
    const std::string &name)
{
    vsmc::StopWatch watch;

    typedef typename RNG::result_type result_type;

    vsmc::Vector<result_type> cpp(N);
    RNG rng(1);
    watch.reset();
    watch.start();
    for (std::size_t i = 0; i != N; ++i)
        cpp[i] = rng();
    watch.stop();
    double tcpp = watch.milliseconds();

    vsmc::Vector<result_type> c(N);
    CRNG crng;
    cinit(&crng, 1);
    watch.reset();
    watch.start();
    for (std::size_t i = 0; i != N; ++i)
        c[i] = crand(&crng);
    watch.stop();
    double tc = watch.milliseconds();
    for (std::size_t i = 0; i != N; ++i) {
        if (cpp[i] != c[i]) {
            std::cout << "Failure: C      " << name << std::endl;
            break;
        }
    }

    std::size_t n = N / M + 1;
    vsmc::Vector<result_type> cl(n * M);
    vsmc::CLBuffer<result_type> buffer(n * M);
    vsmc::CLKernel kernel(program, "kernel_" + name);
    vsmc::cl_set_kernel_args(
        kernel, 0, static_cast<cl_ulong>(n), buffer.data());
    vsmc::CLManager<>::instance().run_kernel(kernel, n);
    vsmc::CLManager<>::instance().read_buffer(buffer.data(), n * M, cl.data());
    watch.reset();
    watch.start();
    vsmc::CLManager<>::instance().run_kernel(kernel, n);
    watch.stop();
    double tcl = watch.milliseconds();
    for (std::size_t i = 0; i != N; ++i) {
        if (cpp[i] != cl[i]) {
            std::cout << "Failure: OpenCL " << name << std::endl;
            break;
        }
    }

    std::cout << std::setw(20) << std::left << name;
    std::cout << std::setw(20) << std::fixed << std::right << tcpp;
    std::cout << std::setw(20) << std::fixed << std::right << tc;
    std::cout << std::setw(20) << std::fixed << std::right << tcl;
    std::cout << std::setw(20) << std::fixed << std::right << tcpp / tc;
    std::cout << std::setw(20) << std::fixed << std::right << tcpp / tcl;
    std::cout << std::endl;
}

inline double rng_cl_normal01(std::size_t N, float *c)
{
    vsmc::StopWatch watch;

    vsmc_crng rng;
    vsmc_crng_init(&rng, 1);
    vsmc_normal01_f32 norm01;
    vsmc_normal01_f32_init(&norm01, &rng);
    watch.reset();
    watch.start();
    for (std::size_t i = 0; i != N; ++i)
        c[i] = vsmc_normal01_f32_rand(&norm01, &rng);
    watch.stop();

    return watch.milliseconds();
}

inline double rng_cl_normal01(std::size_t N, double *c)
{
    vsmc::StopWatch watch;

    vsmc_crng rng;
    vsmc_crng_init(&rng, 1);
    vsmc_normal01_f64 norm01;
    vsmc_normal01_f64_init(&norm01, &rng);
    watch.reset();
    watch.start();
    for (std::size_t i = 0; i != N; ++i)
        c[i] = vsmc_normal01_f64_rand(&norm01, &rng);
    watch.stop();

    return watch.milliseconds();
}

template <typename FPType>
inline void rng_cl_normal01(std::size_t N, const vsmc::CLProgram &program,
    vsmc::Vector<vsmc::Vector<FPType>> &result)
{
    vsmc::StopWatch watch;

    vsmc::Vector<FPType> cpp(N);
    vsmc::Philox2x32 rng(1);
    std::normal_distribution<FPType> rnorm(0, 1);
    watch.reset();
    watch.start();
    for (std::size_t i = 0; i != N; ++i)
        cpp[i] = rnorm(rng);
    watch.stop();
    double tcpp = watch.milliseconds();

    vsmc::Vector<FPType> c(N);
    double tc = rng_cl_normal01(N, c.data());

    vsmc::Vector<FPType> cl(N);
    vsmc::CLBuffer<FPType> buffer(N);
    vsmc::CLKernel kernel(program, "kernel_Normal01");
    vsmc::cl_set_kernel_args(
        kernel, 0, static_cast<cl_ulong>(N), buffer.data());
    vsmc::CLManager<>::instance().run_kernel(kernel, N);
    watch.reset();
    watch.start();
    vsmc::CLManager<>::instance().run_kernel(kernel, N);
    watch.stop();
    vsmc::CLManager<>::instance().read_buffer(buffer.data(), N, cl.data());
    double tcl = watch.milliseconds();

    std::cout << std::setw(20) << std::left << "Normal(0, 1)";
    std::cout << std::setw(20) << std::fixed << std::right << tcpp;
    std::cout << std::setw(20) << std::fixed << std::right << tc;
    std::cout << std::setw(20) << std::fixed << std::right << tcl;
    std::cout << std::setw(20) << std::fixed << std::right << tcpp / tc;
    std::cout << std::setw(20) << std::fixed << std::right << tcpp / tcl;
    std::cout << std::endl;

    result.push_back(std::move(cpp));
    result.push_back(std::move(c));
    result.push_back(std::move(cl));
}

inline double rng_cl_gammak1(std::size_t N, float *c, float shape)
{
    vsmc::StopWatch watch;

    vsmc_crng rng;
    vsmc_crng_init(&rng, 1);
    vsmc_gammak1_f32 gammak1;
    vsmc_gammak1_f32_init(&gammak1, &rng, shape);
    watch.reset();
    watch.start();
    for (std::size_t i = 0; i != N; ++i)
        c[i] = vsmc_gammak1_f32_rand(&gammak1, &rng);
    watch.stop();

    return watch.milliseconds();
}

inline double rng_cl_gammak1(std::size_t N, double *c, double shape)
{
    vsmc::StopWatch watch;

    vsmc_crng rng;
    vsmc_crng_init(&rng, 1);
    vsmc_gammak1_f64 gammak1;
    vsmc_gammak1_f64_init(&gammak1, &rng, shape);
    watch.reset();
    watch.start();
    for (std::size_t i = 0; i != N; ++i)
        c[i] = vsmc_gammak1_f64_rand(&gammak1, &rng);
    watch.stop();

    return watch.milliseconds();
}

template <typename FPType>
inline void rng_cl_gammak1(std::size_t N, const vsmc::CLProgram &program,
    FPType shape, vsmc::Vector<vsmc::Vector<FPType>> &result)
{
    vsmc::StopWatch watch;

    vsmc::Vector<FPType> cpp(N);
    vsmc::Philox2x32 rng(1);
    std::gamma_distribution<FPType> rgamma(shape, 1);
    watch.reset();
    watch.start();
    for (std::size_t i = 0; i != N; ++i)
        cpp[i] = rgamma(rng);
    watch.stop();
    double tcpp = watch.milliseconds();

    vsmc::Vector<FPType> c(N);
    double tc = rng_cl_gammak1(N, c.data(), shape);

    vsmc::Vector<FPType> cl(N);
    vsmc::CLBuffer<FPType> buffer(N);
    vsmc::CLKernel kernel(program, "kernel_GammaK1");
    vsmc::cl_set_kernel_args(
        kernel, 0, static_cast<cl_ulong>(N), buffer.data(), shape);
    vsmc::CLManager<>::instance().run_kernel(kernel, N);
    watch.reset();
    watch.start();
    vsmc::CLManager<>::instance().run_kernel(kernel, N);
    watch.stop();
    vsmc::CLManager<>::instance().read_buffer(buffer.data(), N, cl.data());
    double tcl = watch.milliseconds();

    std::stringstream ss;
    ss << "Gamma(" << shape << ", 1)";
    std::cout << std::setw(20) << std::left << ss.str();
    std::cout << std::setw(20) << std::fixed << std::right << tcpp;
    std::cout << std::setw(20) << std::fixed << std::right << tc;
    std::cout << std::setw(20) << std::fixed << std::right << tcl;
    std::cout << std::setw(20) << std::fixed << std::right << tcpp / tc;
    std::cout << std::setw(20) << std::fixed << std::right << tcpp / tcl;
    std::cout << std::endl;

    result.push_back(std::move(cpp));
    result.push_back(std::move(c));
    result.push_back(std::move(cl));
}

template <typename FPType>
inline void rng_cl(std::size_t N, const vsmc::CLProgram &program)
{
    std::cout << std::string(120, '=') << std::endl;
    std::cout << std::setw(20) << std::left << "Test name";
    std::cout << std::setw(20) << std::right << "Time (ms, C++)";
    std::cout << std::setw(20) << std::right << "Time (ms, C)";
    std::cout << std::setw(20) << std::right << "Time (ms, OpenCL)";
    std::cout << std::setw(20) << std::right << "Speedup (C)";
    std::cout << std::setw(20) << std::right << "Speedup (OpenCL)";
    std::cout << std::endl;

    std::cout << std::string(120, '-') << std::endl;
    rng_cl_rng<vsmc::Philox2x32, vsmc_philox2x32>(N, 2, program,
        vsmc_philox2x32_init, vsmc_philox2x32_rand, "Philox2x32");
    rng_cl_rng<vsmc::Philox4x32, vsmc_philox4x32>(N, 4, program,
        vsmc_philox4x32_init, vsmc_philox4x32_rand, "Philox4x32");
    rng_cl_rng<vsmc::Threefry2x32, vsmc_threefry2x32>(N, 2, program,
        vsmc_threefry2x32_init, vsmc_threefry2x32_rand, "Threefry2x32");
    rng_cl_rng<vsmc::Threefry4x32, vsmc_threefry4x32>(N, 4, program,
        vsmc_threefry4x32_init, vsmc_threefry4x32_rand, "Threefry4x32");
    rng_cl_rng<vsmc::Threefry2x64, vsmc_threefry2x64>(N, 2, program,
        vsmc_threefry2x64_init, vsmc_threefry2x64_rand, "Threefry2x64");
    rng_cl_rng<vsmc::Threefry4x64, vsmc_threefry4x64>(N, 4, program,
        vsmc_threefry4x64_init, vsmc_threefry4x64_rand, "Threefry4x64");

    vsmc::Vector<vsmc::Vector<FPType>> result;
    std::cout << std::string(120, '-') << std::endl;
    rng_cl_normal01<FPType>(N, program, result);
    std::cout << std::string(120, '-') << std::endl;
    rng_cl_gammak1<FPType>(N, program, static_cast<FPType>(0.01), result);
    rng_cl_gammak1<FPType>(N, program, static_cast<FPType>(0.1), result);
    rng_cl_gammak1<FPType>(N, program, static_cast<FPType>(1), result);
    rng_cl_gammak1<FPType>(N, program, static_cast<FPType>(10), result);
    rng_cl_gammak1<FPType>(N, program, static_cast<FPType>(100), result);
    std::cout << std::string(120, '=') << std::endl;

#if VSMC_HAS_HDF5
    std::size_t nrow = N;
    std::size_t ncol = result.size();
    vsmc::Vector<FPType> mat(nrow * ncol);
    FPType *first = mat.data();
    for (std::size_t i = 0; i != ncol; ++i)
        first = std::copy(result[i].begin(), result[i].end(), first);
    vsmc::hdf5store_matrix<vsmc::ColMajor, FPType>(
        nrow, ncol, "rng_cl.h5", "result", mat.data());
#else
    std::ofstream rng_cl_txt("rng_cl.txt");
    for (std::size_t i = 0; i != N; ++i) {
        for (std::size_t j = 0; j != result.size(); ++j)
            rng_cl_txt << result[j][i] << '\t';
        rng_cl_txt << '\n';
    }
    rng_cl_txt.close();
#endif
}

#endif // VSMC_EXAMPLE_RNG_OPENCL_HPP
