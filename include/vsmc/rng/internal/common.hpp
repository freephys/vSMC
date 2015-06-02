//============================================================================
// vSMC/include/vsmc/rng/internal/common.hpp
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

#ifndef VSMC_RNG_INTERNAL_COMMON_HPP
#define VSMC_RNG_INTERNAL_COMMON_HPP

#include <vsmc/internal/common.hpp>

#ifndef UINT64_C
#error __STDC_CONSTANT_MACROS not defined before #<stdint.h>
#endif

namespace vsmc
{

namespace internal
{

VSMC_DEFINE_TYPE_DISPATCH_TRAIT(KeyType, key_type, NullType)

template <typename SeedSeq, typename U, typename V = U, typename W = V>
struct is_seed_seq
    : public std::integral_constant<bool,
          !std::is_convertible<SeedSeq, U>::value &&
              !std::is_convertible<SeedSeq, V>::value &&
              !std::is_convertible<SeedSeq, W>::value &&
              !std::is_same<typename std::remove_cv<SeedSeq>::type,
                  U>::value &&
              !std::is_same<typename std::remove_cv<SeedSeq>::type,
                  V>::value &&
              !std::is_same<typename std::remove_cv<SeedSeq>::type,
                  W>::value> {
};

template <std::size_t, typename T, std::size_t K>
inline static void increment_single(std::array<T, K> &ctr, std::false_type)
{
    ++ctr.back();
}

template <std::size_t N, typename T, std::size_t K>
inline static void increment_single(std::array<T, K> &ctr, std::true_type)
{
    if (++std::get<N>(ctr) != 0)
        return;

    increment_single<N + 1>(ctr, std::integral_constant<bool, N + 2 < K>());
}

template <typename T, std::size_t K>
inline void increment(std::array<T, K> &ctr)
{
    increment_single<0>(ctr, std::integral_constant<bool, 1 < K>());
}

template <typename T, std::size_t K, T NSkip>
inline void increment(std::array<T, K> &ctr, std::integral_constant<T, NSkip>)
{
    T old = ctr.front();
    ctr.front() += NSkip;

    if (ctr.front() < old && old != 0)
        increment_single<1>(ctr, std::integral_constant<bool, 2 < K>());
}

template <typename T, std::size_t K>
inline static void increment(std::array<T, K> &ctr, T nskip)
{
    T old = ctr.front();
    ctr.front() += nskip;

    if (ctr.front() < old && old != 0)
        increment_single<1>(ctr, std::integral_constant<bool, 2 < K>());
}

template <std::size_t Blocks, std::size_t, typename T, std::size_t K>
inline static void increment_block(std::array<T, K> &,
    std::array<std::array<T, K>, Blocks> &, std::false_type)
{
}

template <std::size_t Blocks, std::size_t B, typename T, std::size_t K>
inline static void increment_block(std::array<T, K> &ctr,
    std::array<std::array<T, K>, Blocks> &ctr_block, std::true_type)
{
    std::get<B>(ctr_block) = ctr;
    increment(std::get<B>(ctr_block), std::integral_constant<T, B + 1>());
    increment_block<Blocks, B + 1>(
        ctr, ctr_block, std::integral_constant<bool, B + 1 < Blocks>());
}

template <std::size_t Blocks, typename T, std::size_t K>
static void increment(
    std::array<T, K> &ctr, std::array<std::array<T, K>, Blocks> &ctr_block)
{
    increment_block<Blocks, 0>(
        ctr, ctr_block, std::integral_constant<bool, 0 < Blocks>());
    ctr = ctr_block.back();
}

} // namespace vsmc::internal

} // namespace vsmc

#endif // VSMC_RNG_INTERNAL_COMMON_HPP
