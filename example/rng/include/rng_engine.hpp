//============================================================================
// vSMC/example/rng/include/rng_engine.hpp
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

#ifndef VSMC_EXAMPLE_RNG_ENGINE_HPP
#define VSMC_EXAMPLE_RNG_ENGINE_HPP

#include <vsmc/rng/engine.hpp>
#include <vsmc/utility/stop_watch.hpp>

inline std::string rng_engine_test(bool pass)
{
    return pass ? "Passed" : "Failed";
}

inline std::uint64_t rng_engine_rdtsc()
{
#if defined(VSMC_CLANG) || defined(VSMC_GCC) || defined(VSMC_INTEL)
    unsigned hi = 0;
    unsigned lo = 0;
#if VSMC_HAS_X86_64
    asm volatile("CPUID\n\t"
                 "RDTSC\n\t"
                 "mov %%edx, %0\n\t"
                 "mov %%eax, %1\n\t"
                 : "=r"(hi), "=r"(lo)::"%rax", "%rbx", "%rcx", "%rdx");
#elif VSMC_HAS_X86
    asm volatile("CPUID\n\t"
                 "RDTSC\n\t"
                 "mov %%edx, %0\n\t"
                 "mov %%eax, %1\n\t"
                 : "=r"(lo), "=r"(lo)::"%eax", "%ebx", "%ecx", "%edx");
#endif // VSMC_HAS_X86
    return (static_cast<std::uint64_t>(hi) << 32) + lo;
#else  // defined(VSMC_CLANG) || defined(VSMC_GCC) || defined(VSMC_INTEL)
    return 0;
#endif // defined(VSMC_CLANG) || defined(VSMC_GCC) || defined(VSMC_INTEL)
}

template <typename RNGType>
inline bool rng_engine_kat(const RNGType &)
{
    return true;
}

template <typename ResultType, typename T, std::size_t K, std::size_t Rounds>
inline bool rng_engine_kat(vsmc::PhiloxEngine<ResultType, T, K, Rounds> &rng)
{
    std::string filename("rng_engine_kat_Philox");
    filename += std::to_string(K) + "x";
    filename += std::to_string(std::numeric_limits<T>::digits) + ".txt";
    std::ifstream kat(filename);
    vsmc::Vector<T> k;
    k.reserve(1000);
    while (kat) {
        T x;
        kat >> x;
        k.push_back(x);
    }
    kat.close();

    vsmc::Vector<ResultType> r(256);
    rng(256, r.data());

    return std::memcmp(k.data(), r.data(), sizeof(ResultType) * 256) == 0;
}

template <typename ResultType, typename T, std::size_t K, std::size_t Rounds>
inline bool rng_engine_kat(vsmc::ThreefryEngine<ResultType, T, K, Rounds> &rng)
{
    std::string filename("rng_engine_kat_Threefry");
    filename += std::to_string(K) + "x";
    filename += std::to_string(std::numeric_limits<T>::digits) + ".txt";
    std::ifstream kat(filename);

    vsmc::Vector<T> k;
    k.reserve(1000);
    while (kat) {
        T x;
        kat >> x;
        k.push_back(x);
    }
    kat.close();

    vsmc::Vector<ResultType> r(256);
    rng(256, r.data());

    return std::memcmp(k.data(), r.data(), sizeof(ResultType) * 256) == 0;
}

#if VSMC_HAS_AES_NI

template <typename ResultType, std::size_t Blocks>
inline bool rng_engine_kat(vsmc::AES128Engine<ResultType, Blocks> &rng)
{
    std::ifstream kat("rng_engine_kat_AES128.txt");
    vsmc::Vector<std::uint64_t> k;
    k.reserve(1000);
    while (kat) {
        std::uint64_t x;
        kat >> x;
        k.push_back(x);
    }
    kat.close();

    vsmc::Vector<ResultType> r(256);
    rng(256, r.data());

    return std::memcmp(k.data(), r.data(), sizeof(ResultType) * 256) == 0;
}

template <typename ResultType, std::size_t Blocks>
inline bool rng_engine_kat(vsmc::AES192Engine<ResultType, Blocks> &rng)
{
    std::ifstream kat("rng_engine_kat_AES192.txt");
    vsmc::Vector<std::uint64_t> k;
    k.reserve(1000);
    while (kat) {
        std::uint64_t x;
        kat >> x;
        k.push_back(x);
    }
    kat.close();

    vsmc::Vector<ResultType> r(256);
    rng(256, r.data());

    return std::memcmp(k.data(), r.data(), sizeof(ResultType) * 256) == 0;
}

template <typename ResultType, std::size_t Blocks>
inline bool rng_engine_kat(vsmc::AES256Engine<ResultType, Blocks> &rng)
{
    std::ifstream kat("rng_engine_kat_AES256.txt");
    vsmc::Vector<std::uint64_t> k;
    k.reserve(1000);
    while (kat) {
        std::uint64_t x;
        kat >> x;
        k.push_back(x);
    }
    kat.close();

    vsmc::Vector<ResultType> r(256);
    rng(256, r.data());

    return std::memcmp(k.data(), r.data(), sizeof(ResultType) * 256) == 0;
}

template <typename ResultType, std::size_t Rounds, std::size_t Blocks>
inline bool rng_engine_kat(vsmc::ARSEngine<ResultType, Rounds, Blocks> &rng)
{
    std::ifstream kat("rng_engine_kat_ARS.txt");
    vsmc::Vector<std::uint64_t> k;
    k.reserve(1000);
    while (kat) {
        std::uint64_t x;
        kat >> x;
        k.push_back(x);
    }
    kat.close();

    vsmc::Vector<ResultType> r(256);
    rng(256, r.data());

    return std::memcmp(k.data(), r.data(), sizeof(ResultType) * 256) == 0;
}

#endif // VSMC_HAS_AES_NI

template <typename RNGType>
inline void rng_engine(std::size_t N, std::size_t M, int nwid, int swid,
    int twid, const std::string &name)
{
    RNGType rng;
    RNGType rng1;
    RNGType rng2;
    vsmc::Vector<typename RNGType::result_type> r1;
    vsmc::Vector<typename RNGType::result_type> r2;
    r1.reserve(N);
    r2.reserve(N);
    vsmc::StopWatch watch1;
    vsmc::StopWatch watch2;
    std::uint64_t cycle1 = 0;
    std::uint64_t cycle2 = 0;
    bool pass = rng_engine_kat(rng);

    std::uniform_int_distribution<std::size_t> runif(N / 2, N);
    std::size_t num = 0;
    for (std::size_t i = 0; i != M; ++i) {
        std::size_t K = runif(rng);
        num += K;
        r1.resize(K);
        r2.resize(K);

        watch1.start();
        std::uint64_t b1 = rng_engine_rdtsc();
        for (std::size_t j = 0; j != K; ++j)
            r1[j] = rng1();
        std::uint64_t e1 = rng_engine_rdtsc();
        watch1.stop();
        cycle1 += e1 - b1;

        watch2.start();
        std::uint64_t b2 = rng_engine_rdtsc();
        vsmc::rng_rand(rng2, K, r2.data());
        std::uint64_t e2 = rng_engine_rdtsc();
        watch2.stop();
        cycle2 += e2 - b2;
        pass = pass && (r1 == r2 || rng != rng);

        std::stringstream ss;
        ss << rng;
        vsmc::rng_rand(rng, K, r1.data());
        ss >> rng;
        vsmc::rng_rand(rng, K, r2.data());
        pass = pass && (r1 == r2 || rng != rng);

        rng1.discard(static_cast<unsigned>(K));
        typename RNGType::result_type next = rng1();
        for (std::size_t j = 0; j != K; ++j)
            rng2();
        bool find = false;
        for (std::size_t j = 0; j != 2; ++j)
            find = find || rng2() == next;
        pass = pass && (find || rng != rng);
    }

    double s1 = sizeof(typename RNGType::result_type) * num;
    double s2 = sizeof(typename RNGType::result_type) * num;
    double g1 = s1 / watch1.nanoseconds();
    double g2 = s2 / watch2.nanoseconds();
    std::string cpB1 = cycle1 == 0 ? "N/A" : std::to_string(cycle1 / s1);
    std::string cpB2 = cycle2 == 0 ? "N/A" : std::to_string(cycle2 / s2);

    std::cout << std::left << std::setw(nwid) << name;
    std::cout << std::right << std::setw(swid) << sizeof(RNGType);
    std::cout << std::right << std::setw(swid) << alignof(RNGType);
    std::cout << std::right << std::setw(twid) << std::fixed << g1;
    std::cout << std::right << std::setw(twid) << std::fixed << g2;
    std::cout << std::right << std::setw(twid) << std::fixed << cpB1;
    std::cout << std::right << std::setw(twid) << std::fixed << cpB2;
    std::cout << std::right << std::setw(twid) << (pass ? "Passed" : "Failed");
    std::cout << std::endl;
}

inline void rng_engine(std::size_t N, std::size_t M)
{
    const int nwid = 20;
    const int swid = 6;
    const int twid = 15;
    const std::size_t lwid = nwid + swid * 2 + twid * 5;

    std::cout << std::string(lwid, '=') << std::endl;
    std::cout << std::left << std::setw(nwid) << "RNGType";
    std::cout << std::right << std::setw(swid) << "Size";
    std::cout << std::right << std::setw(swid) << "Align";
    std::cout << std::right << std::setw(twid) << "GB/s (Loop)";
    std::cout << std::right << std::setw(twid) << "GB/s (Batch)";
    std::cout << std::right << std::setw(twid) << "cpB (Loop)";
    std::cout << std::right << std::setw(twid) << "cpB (Batch)";
    std::cout << std::right << std::setw(twid) << "Deterministics";
    std::cout << std::endl;
    std::cout << std::string(lwid, '-') << std::endl;

#ifdef VSMC_RNG_DEFINE_MACRO
#undef VSMC_RNG_DEFINE_MACRO
#endif

#ifdef VSMC_RNG_DEFINE_MACRO_MKL
#undef VSMC_RNG_DEFINE_MACRO_MKL
#endif

#define VSMC_RNG_DEFINE_MACRO(RNGType, Name, name)                            \
    rng_engine<RNGType>(N, M, nwid, swid, twid, #Name);

#include <vsmc/rng/internal/rng_define_macro.hpp>
#include <vsmc/rng/internal/rng_define_macro_mkl.hpp>

    std::cout << std::string(lwid, '-') << std::endl;
}

#endif // VSMC_EXAMPLE_RNG_ENGINE_HPP
