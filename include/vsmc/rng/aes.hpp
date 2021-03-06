//============================================================================
// vSMC/include/vsmc/rng/aes.hpp
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

#ifndef VSMC_RNG_AES_HPP
#define VSMC_RNG_AES_HPP

#include <vsmc/rng/internal/common.hpp>
#include <vsmc/rng/aes_ni.hpp>

#define VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(N, val)                            \
    template <>                                                               \
    inline __m128i AESKeyGenAssist<N>(__m128i xmm)                    \
    {                                                                         \
        return _mm_aeskeygenassist_si128(xmm, val);                           \
    }

/// \brief AESEngine default blocks
/// \ingroup Config
#ifndef VSMC_RNG_AES_BLOCKS
#define VSMC_RNG_AES_BLOCKS 4
#endif

namespace vsmc
{

namespace internal
{

template <std::size_t>
inline __m128i AESKeyGenAssist(__m128i) ;

VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x00, 0x8D)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x01, 0x01)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x02, 0x02)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x03, 0x04)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x04, 0x08)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x05, 0x10)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x06, 0x20)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x07, 0x40)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x08, 0x80)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x09, 0x1B)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x0A, 0x36)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x0B, 0x6C)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x0C, 0xD8)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x0D, 0xAB)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x0E, 0x4D)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x0F, 0x9A)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x10, 0x2F)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x11, 0x5E)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x12, 0xBC)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x13, 0x63)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x14, 0xC6)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x15, 0x97)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x16, 0x35)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x17, 0x6A)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x18, 0xD4)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x19, 0xB3)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x1A, 0x7D)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x1B, 0xFA)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x1C, 0xEF)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x1D, 0xC5)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x1E, 0x91)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x1F, 0x39)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x20, 0x72)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x21, 0xE4)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x22, 0xD3)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x23, 0xBD)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x24, 0x61)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x25, 0xC2)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x26, 0x9F)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x27, 0x25)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x28, 0x4A)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x29, 0x94)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x2A, 0x33)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x2B, 0x66)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x2C, 0xCC)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x2D, 0x83)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x2E, 0x1D)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x2F, 0x3A)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x30, 0x74)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x31, 0xE8)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x32, 0xCB)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x33, 0x8D)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x34, 0x01)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x35, 0x02)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x36, 0x04)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x37, 0x08)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x38, 0x10)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x39, 0x20)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x3A, 0x40)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x3B, 0x80)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x3C, 0x1B)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x3D, 0x36)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x3E, 0x6C)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x3F, 0xD8)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x40, 0xAB)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x41, 0x4D)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x42, 0x9A)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x43, 0x2F)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x44, 0x5E)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x45, 0xBC)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x46, 0x63)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x47, 0xC6)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x48, 0x97)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x49, 0x35)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x4A, 0x6A)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x4B, 0xD4)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x4C, 0xB3)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x4D, 0x7D)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x4E, 0xFA)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x4F, 0xEF)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x50, 0xC5)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x51, 0x91)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x52, 0x39)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x53, 0x72)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x54, 0xE4)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x55, 0xD3)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x56, 0xBD)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x57, 0x61)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x58, 0xC2)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x59, 0x9F)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x5A, 0x25)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x5B, 0x4A)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x5C, 0x94)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x5D, 0x33)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x5E, 0x66)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x5F, 0xCC)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x60, 0x83)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x61, 0x1D)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x62, 0x3A)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x63, 0x74)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x64, 0xE8)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x65, 0xCB)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x66, 0x8D)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x67, 0x01)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x68, 0x02)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x69, 0x04)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x6A, 0x08)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x6B, 0x10)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x6C, 0x20)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x6D, 0x40)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x6E, 0x80)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x6F, 0x1B)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x70, 0x36)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x71, 0x6C)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x72, 0xD8)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x73, 0xAB)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x74, 0x4D)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x75, 0x9A)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x76, 0x2F)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x77, 0x5E)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x78, 0xBC)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x79, 0x63)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x7A, 0xC6)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x7B, 0x97)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x7C, 0x35)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x7D, 0x6A)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x7E, 0xD4)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x7F, 0xB3)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x80, 0x7D)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x81, 0xFA)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x82, 0xEF)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x83, 0xC5)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x84, 0x91)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x85, 0x39)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x86, 0x72)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x87, 0xE4)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x88, 0xD3)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x89, 0xBD)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x8A, 0x61)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x8B, 0xC2)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x8C, 0x9F)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x8D, 0x25)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x8E, 0x4A)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x8F, 0x94)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x90, 0x33)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x91, 0x66)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x92, 0xCC)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x93, 0x83)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x94, 0x1D)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x95, 0x3A)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x96, 0x74)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x97, 0xE8)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x98, 0xCB)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x99, 0x8D)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x9A, 0x01)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x9B, 0x02)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x9C, 0x04)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x9D, 0x08)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x9E, 0x10)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0x9F, 0x20)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xA0, 0x40)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xA1, 0x80)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xA2, 0x1B)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xA3, 0x36)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xA4, 0x6C)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xA5, 0xD8)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xA6, 0xAB)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xA7, 0x4D)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xA8, 0x9A)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xA9, 0x2F)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xAA, 0x5E)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xAB, 0xBC)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xAC, 0x63)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xAD, 0xC6)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xAE, 0x97)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xAF, 0x35)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xB0, 0x6A)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xB1, 0xD4)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xB2, 0xB3)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xB3, 0x7D)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xB4, 0xFA)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xB5, 0xEF)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xB6, 0xC5)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xB7, 0x91)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xB8, 0x39)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xB9, 0x72)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xBA, 0xE4)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xBB, 0xD3)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xBC, 0xBD)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xBD, 0x61)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xBE, 0xC2)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xBF, 0x9F)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xC0, 0x25)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xC1, 0x4A)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xC2, 0x94)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xC3, 0x33)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xC4, 0x66)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xC5, 0xCC)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xC6, 0x83)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xC7, 0x1D)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xC8, 0x3A)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xC9, 0x74)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xCA, 0xE8)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xCB, 0xCB)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xCC, 0x8D)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xCD, 0x01)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xCE, 0x02)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xCF, 0x04)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xD0, 0x08)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xD1, 0x10)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xD2, 0x20)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xD3, 0x40)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xD4, 0x80)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xD5, 0x1B)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xD6, 0x36)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xD7, 0x6C)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xD8, 0xD8)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xD9, 0xAB)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xDA, 0x4D)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xDB, 0x9A)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xDC, 0x2F)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xDD, 0x5E)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xDE, 0xBC)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xDF, 0x63)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xE0, 0xC6)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xE1, 0x97)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xE2, 0x35)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xE3, 0x6A)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xE4, 0xD4)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xE5, 0xB3)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xE6, 0x7D)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xE7, 0xFA)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xE8, 0xEF)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xE9, 0xC5)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xEA, 0x91)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xEB, 0x39)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xEC, 0x72)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xED, 0xE4)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xEE, 0xD3)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xEF, 0xBD)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xF0, 0x61)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xF1, 0xC2)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xF2, 0x9F)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xF3, 0x25)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xF4, 0x4A)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xF5, 0x94)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xF6, 0x33)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xF7, 0x66)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xF8, 0xCC)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xF9, 0x83)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xFA, 0x1D)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xFB, 0x3A)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xFC, 0x74)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xFD, 0xE8)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xFE, 0xCB)
VSMC_DEFINE_RNG_AES_KEY_GEN_ASSIST(0xFF, 0x8D)

class AESKeyInit
{
    public:
    template <std::size_t Offset, std::size_t N, typename T,
        std::size_t KeySize, std::size_t Rp1>
    static void eval(const std::array<T, KeySize> &key,
        std::array<__m128i, Rp1> &ks, __m128i &xmm) 
    {
        init<Offset, N>(key, ks, xmm, std::integral_constant < bool, N<Rp1>());
    }

    private:
    template <std::size_t, std::size_t, typename T, std::size_t KeySize,
        std::size_t Rp1>
    static void init(const std::array<T, KeySize> &,
        std::array<__m128i, Rp1> &, __m128i &, std::false_type) 
    {
    }

    template <std::size_t Offset, std::size_t N, typename T,
        std::size_t KeySize, std::size_t Rp1>
    static void init(const std::array<T, KeySize> &key,
        std::array<__m128i, Rp1> &ks, __m128i &xmm, std::true_type) 
    {
        M128I<> tmp;
        tmp.load(key.data());
        std::get<N>(ks) = xmm = tmp.value();
    }
}; // class AESKeyInit

template <typename T, std::size_t Rounds, typename KeySeqGenerator>
class AESKeySeq
{
    public:
    using key_type = typename KeySeqGenerator::key_type;

    void reset(const key_type &key) 
    {
        KeySeqGenerator generator;
        generator(key, key_seq_);
    }

    template <typename U>
    void operator()(
        const key_type &, std::array<M128I<U>, Rounds + 1> &rk) const 
    {
        rk = key_seq_;
    }

    private:
    std::array<M128I<>, Rounds + 1> key_seq_;
}; // class AESKeySeq

template <typename T>
class AES128KeySeqGenerator
{
    public:
    using key_type = std::array<T, 16 / sizeof(T)>;

    template <std::size_t Rp1>
    void operator()(
        const key_type &key, std::array<M128I<>, Rp1> &key_seq) 
    {
        std::array<__m128i, Rp1> ks;
        AESKeyInit::eval<0, 0>(key, ks, xmm1_);
        generate_seq<1>(ks, std::integral_constant<bool, 1 < Rp1>());
        std::memcpy(key_seq.data(), ks.data(), sizeof(__m128i) * Rp1);
    }

    private:
    __m128i xmm1_;
    __m128i xmm2_;
    __m128i xmm3_;

    template <std::size_t, std::size_t Rp1>
    void generate_seq(std::array<__m128i, Rp1> &, std::false_type) 
    {
    }

    template <std::size_t N, std::size_t Rp1>
    void generate_seq(std::array<__m128i, Rp1> &ks, std::true_type) 
    {
        xmm2_ = AESKeyGenAssist<N>(xmm1_);
        expand_key();
        std::get<N>(ks) = xmm1_;
        generate_seq<N + 1>(ks, std::integral_constant<bool, N + 1 < Rp1>());
    }

    void expand_key() 
    {
        xmm2_ = _mm_shuffle_epi32(xmm2_, 0xFF); // pshufd xmm2, xmm2, 0xFF
        xmm3_ = _mm_slli_si128(xmm1_, 0x04);    // pshufb xmm3, xmm5
        xmm1_ = _mm_xor_si128(xmm1_, xmm3_);    // pxor   xmm1, xmm3
        xmm3_ = _mm_slli_si128(xmm3_, 0x04);    // pslldq xmm3, 0x04
        xmm1_ = _mm_xor_si128(xmm1_, xmm3_);    // pxor   xmm1, xmm3
        xmm3_ = _mm_slli_si128(xmm3_, 0x04);    // pslldq xmm3, 0x04
        xmm1_ = _mm_xor_si128(xmm1_, xmm3_);    // pxor   xmm1, xmm3
        xmm1_ = _mm_xor_si128(xmm1_, xmm2_);    // pxor   xmm1, xmm2
    }
}; // class AES128KeySeq

template <typename T>
class AES192KeySeqGenerator
{
    public:
    using key_type = std::array<T, 24 / sizeof(T)>;

    template <std::size_t Rp1>
    void operator()(
        const key_type &key, std::array<M128I<>, Rp1> &key_seq) 
    {
        std::array<__m128i, Rp1> ks;

        std::array<std::uint64_t, 3> key_tmp;
        std::memcpy(key_tmp.data(), key.data(), 24);
        AESKeyInit::eval<0, 0>(key_tmp, ks, xmm1_);
        std::get<0>(key_tmp) = std::get<2>(key_tmp);
        std::get<1>(key_tmp) = 0;
        AESKeyInit::eval<0, 1>(key_tmp, ks, xmm7_);

        xmm3_ = _mm_setzero_si128();
        xmm6_ = _mm_setzero_si128();
        xmm4_ = _mm_shuffle_epi32(xmm7_, 0x4F); // pshufd xmm4, xmm7, 0x4F

        std::array<unsigned char, Rp1 * 16 + 16> ks_tmp;
        generate_seq<1, Rp1>(
            ks_tmp.data(), std::integral_constant<bool, 24 < Rp1 * 16>());
        copy_key(
            ks, ks_tmp.data(), std::integral_constant<bool, 24 < Rp1 * 16>());

        std::memcpy(key_seq.data(), ks.data(), sizeof(__m128i) * Rp1);
    }

    private:
    __m128i xmm1_;
    __m128i xmm2_;
    __m128i xmm3_;
    __m128i xmm4_;
    __m128i xmm5_;
    __m128i xmm6_;
    __m128i xmm7_;

    template <std::size_t, std::size_t>
    void generate_seq(unsigned char *, std::false_type) 
    {
    }

    template <std::size_t N, std::size_t Rp1>
    void generate_seq(unsigned char *ks_ptr, std::true_type) 
    {
        generate_key<N>(ks_ptr);
        complete_key<N>(
            ks_ptr, std::integral_constant<bool, N * 24 + 16 < Rp1 * 16>());
        generate_seq<N + 1, Rp1>(
            ks_ptr, std::integral_constant<bool, N * 24 + 24 < Rp1 * 16>());
    }

    template <std::size_t N>
    void generate_key(unsigned char *ks_ptr) 
    {
        // In entry, N * 24 < Rp1 * 16
        // Required Storage: N * 24 + 16;

        xmm2_ = AESKeyGenAssist<N>(xmm4_);
        generate_key_expansion();
        _mm_storeu_si128(reinterpret_cast<__m128i *>(ks_ptr + N * 24), xmm1_);
    }

    template <std::size_t>
    void complete_key(unsigned char *, std::false_type) 
    {
    }

    template <std::size_t N>
    void complete_key(unsigned char *ks_ptr, std::true_type) 
    {
        // In entry, N * 24 + 16 < Rp1 * 16
        // Required storage: N * 24 + 32

        complete_key_expansion();
        _mm_storeu_si128(
            reinterpret_cast<__m128i *>(ks_ptr + N * 24 + 16), xmm7_);
    }

    void generate_key_expansion() 
    {
        xmm2_ = _mm_shuffle_epi32(xmm2_, 0xFF);  // pshufd xmm2, xmm2, 0xFF
        xmm3_ = _mm_castps_si128(_mm_shuffle_ps( // shufps xmm3, xmm1, 0x10
            _mm_castsi128_ps(xmm3_), _mm_castsi128_ps(xmm1_), 0x10));
        xmm1_ = _mm_xor_si128(xmm1_, xmm3_);     // pxor   xmm1, xmm3
        xmm3_ = _mm_castps_si128(_mm_shuffle_ps( // shufps xmm3, xmm1, 0x10
            _mm_castsi128_ps(xmm3_), _mm_castsi128_ps(xmm1_), 0x8C));
        xmm1_ = _mm_xor_si128(xmm1_, xmm3_); // pxor   xmm1, xmm3
        xmm1_ = _mm_xor_si128(xmm1_, xmm2_); // pxor   xmm1, xmm2
    }

    void complete_key_expansion() 
    {
        xmm5_ = _mm_load_si128(&xmm4_);          // movdqa xmm5, xmm4
        xmm5_ = _mm_slli_si128(xmm5_, 0x04);     // pslldq xmm5, 0x04
        xmm6_ = _mm_castps_si128(_mm_shuffle_ps( // shufps xmm6, xmm1, 0x10
            _mm_castsi128_ps(xmm6_), _mm_castsi128_ps(xmm1_), 0xF0));
        xmm6_ = _mm_xor_si128(xmm6_, xmm5_);    // pxor   xmm6, xmm5
        xmm4_ = _mm_xor_si128(xmm4_, xmm6_);    // pxor   xmm4, xmm6
        xmm7_ = _mm_shuffle_epi32(xmm4_, 0x0E); // pshufd xmm7, xmm4, 0x0E
    }

    template <std::size_t Rp1>
    void copy_key(std::array<__m128i, Rp1> &, const unsigned char *,
        std::false_type) 
    {
    }

    template <std::size_t Rp1>
    void copy_key(std::array<__m128i, Rp1> &ks, const unsigned char *ks_ptr,
        std::true_type) 
    {
        unsigned char *dst = reinterpret_cast<unsigned char *>(ks.data());
        std::memcpy(dst + 24, ks_ptr + 24, Rp1 * 16 - 24);
    }
}; // class AES192KeySeq

template <typename T>
class AES256KeySeqGenerator
{
    public:
    using key_type = std::array<T, 32 / sizeof(T)>;

    template <std::size_t Rp1>
    void operator()(
        const key_type &key, std::array<M128I<>, Rp1> &key_seq) 
    {
        std::array<__m128i, Rp1> ks;
        AESKeyInit::eval<0, 0>(key, ks, xmm1_);
        AESKeyInit::eval<16 / sizeof(T), 1>(key, ks, xmm3_);
        generate_seq<2>(ks, std::integral_constant<bool, 2 < Rp1>());
        std::memcpy(key_seq.data(), ks.data(), sizeof(__m128i) * Rp1);
    }

    private:
    __m128i xmm1_;
    __m128i xmm2_;
    __m128i xmm3_;
    __m128i xmm4_;

    template <std::size_t, std::size_t Rp1>
    void generate_seq(std::array<__m128i, Rp1> &, std::false_type) 
    {
    }

    template <std::size_t N, std::size_t Rp1>
    void generate_seq(std::array<__m128i, Rp1> &ks, std::true_type) 
    {
        generate_key<N>(ks, std::integral_constant<bool, N % 2 == 0>());
        generate_seq<N + 1>(ks, std::integral_constant<bool, N + 1 < Rp1>());
    }

    template <std::size_t N, std::size_t Rp1>
    void generate_key(std::array<__m128i, Rp1> &ks, std::true_type) 
    {
        xmm2_ = AESKeyGenAssist<N / 2>(xmm3_);
        expand_key(std::true_type());
        std::get<N>(ks) = xmm1_;
    }

    template <std::size_t N, std::size_t Rp1>
    void generate_key(std::array<__m128i, Rp1> &ks, std::false_type) 
    {
        xmm4_ = _mm_aeskeygenassist_si128(xmm1_, 0);
        expand_key(std::false_type());
        std::get<N>(ks) = xmm3_;
    }

    void expand_key(std::true_type) 
    {
        xmm2_ = _mm_shuffle_epi32(xmm2_, 0xFF); // pshufd xmm2, xmm2, 0xFF
        xmm4_ = _mm_slli_si128(xmm1_, 0x04);    // pshufb xmm4, xmm5
        xmm1_ = _mm_xor_si128(xmm1_, xmm4_);    // pxor   xmm1, xmm4
        xmm4_ = _mm_slli_si128(xmm4_, 0x04);    // pslldq xmm4, 0x04
        xmm1_ = _mm_xor_si128(xmm1_, xmm4_);    // pxor   xmm1, xmm4
        xmm4_ = _mm_slli_si128(xmm4_, 0x04);    // pslldq xmm4, 0x04
        xmm1_ = _mm_xor_si128(xmm1_, xmm4_);    // pxor   xmm1, xmm4
        xmm1_ = _mm_xor_si128(xmm1_, xmm2_);    // pxor   xmm1, xmm2
    }

    void expand_key(std::false_type) 
    {
        xmm2_ = _mm_shuffle_epi32(xmm4_, 0xAA); // pshufd xmm2, xmm4, 0xAA
        xmm4_ = _mm_slli_si128(xmm3_, 0x04);    // pshufb xmm4, xmm5
        xmm3_ = _mm_xor_si128(xmm3_, xmm4_);    // pxor   xmm3, xmm4
        xmm4_ = _mm_slli_si128(xmm4_, 0x04);    // pslldq xmm4, 0x04
        xmm3_ = _mm_xor_si128(xmm3_, xmm4_);    // pxor   xmm3, xmm4
        xmm4_ = _mm_slli_si128(xmm4_, 0x04);    // pslldq xmm4, 0x04
        xmm3_ = _mm_xor_si128(xmm3_, xmm4_);    // pxor   xmm3, xmm4
        xmm3_ = _mm_xor_si128(xmm3_, xmm2_);    // pxor   xmm1, xmm2
    }
}; // class AESKey256

} // namespace vsmc::internal

/// \brief AES128Engine key sequence generator
/// \ingroup AESNIRNG
template <typename ResultType, std::size_t Rounds>
using AES128KeySeq = internal::AESKeySeq<ResultType, Rounds,
    internal::AES128KeySeqGenerator<ResultType>>;

/// \brief AES192Engine key sequence generator
/// \ingroup AESNIRNG
template <typename ResultType, std::size_t Rounds>
using AES192KeySeq = internal::AESKeySeq<ResultType, Rounds,
    internal::AES192KeySeqGenerator<ResultType>>;

/// \brief AES256Engine key sequence generator
/// \ingroup AESNIRNG
template <typename ResultType, std::size_t Rounds>
using AES256KeySeq = internal::AESKeySeq<ResultType, Rounds,
    internal::AES256KeySeqGenerator<ResultType>>;

/// \brief AES-128 RNG engine
/// \ingroup AESNIRNG
template <typename ResultType, std::size_t Blocks = VSMC_RNG_AES_BLOCKS>
using AES128Engine =
    AESNIEngine<ResultType, AES128KeySeq<ResultType, 10>, 10, Blocks>;

/// \brief AES-192 RNG engine
/// \ingroup AESNIRNG
template <typename ResultType, std::size_t Blocks = VSMC_RNG_AES_BLOCKS>
using AES192Engine =
    AESNIEngine<ResultType, AES192KeySeq<ResultType, 12>, 12, Blocks>;

/// \brief AES-256 RNG engine
/// \ingroup AESNIRNG
template <typename ResultType, std::size_t Blocks = VSMC_RNG_AES_BLOCKS>
using AES256Engine =
    AESNIEngine<ResultType, AES256KeySeq<ResultType, 14>, 14, Blocks>;

/// \brief AES-128 RNG engine with 32-bits integers output and 1 block
/// \ingroup AESNIRNG
using AES128_1x32 = AES128Engine<std::uint32_t, 1>;

/// \brief AES-128 RNG engine with 32-bits integers output and 2 blocks
/// \ingroup AESNIRNG
using AES128_2x32 = AES128Engine<std::uint32_t, 2>;

/// \brief AES-128 RNG engine with 32-bits integers output and 4 blocks
/// \ingroup AESNIRNG
using AES128_4x32 = AES128Engine<std::uint32_t, 4>;

/// \brief AES-128 RNG engine with 32-bits integers output and 8 blocks
/// \ingroup AESNIRNG
using AES128_8x32 = AES128Engine<std::uint32_t, 8>;

/// \brief AES-128 RNG engine with 64-bits integers output and 1 block
/// \ingroup AESNIRNG
using AES128_1x64 = AES128Engine<std::uint64_t, 1>;

/// \brief AES-128 RNG engine with 64-bits integers output and 2 blocks
/// \ingroup AESNIRNG
using AES128_2x64 = AES128Engine<std::uint64_t, 2>;

/// \brief AES-128 RNG engine with 64-bits integers output and 4 blocks
/// \ingroup AESNIRNG
using AES128_4x64 = AES128Engine<std::uint64_t, 4>;

/// \brief AES-128 RNG engine with 64-bits integers output and 8 blocks
/// \ingroup AESNIRNG
using AES128_8x64 = AES128Engine<std::uint64_t, 8>;

/// \brief AES-128 RNG engine with 32-bits integers output and default blocks
/// \ingroup AESNIRNG
using AES128 = AES128Engine<std::uint32_t>;

/// \brief AES-128 RNG engine with 64-bits integers output and default blocks
/// \ingroup AESNIRNG
using AES128_64 = AES128Engine<std::uint64_t>;

/// \brief AES-192 RNG engine with 32-bits integers output and 1 block
/// \ingroup AESNIRNG
using AES192_1x32 = AES192Engine<std::uint32_t, 1>;

/// \brief AES-192 RNG engine with 32-bits integers output and 2 blocks
/// \ingroup AESNIRNG
using AES192_2x32 = AES192Engine<std::uint32_t, 2>;

/// \brief AES-192 RNG engine with 32-bits integers output and 4 blocks
/// \ingroup AESNIRNG
using AES192_4x32 = AES192Engine<std::uint32_t, 4>;

/// \brief AES-192 RNG engine with 32-bits integers output and 8 blocks
/// \ingroup AESNIRNG
using AES192_8x32 = AES192Engine<std::uint32_t, 8>;

/// \brief AES-192 RNG engine with 64-bits integers output and 1 block
/// \ingroup AESNIRNG
using AES192_1x64 = AES192Engine<std::uint64_t, 1>;

/// \brief AES-192 RNG engine with 64-bits integers output and 2 blocks
/// \ingroup AESNIRNG
using AES192_2x64 = AES192Engine<std::uint64_t, 2>;

/// \brief AES-192 RNG engine with 64-bits integers output and 4 blocks
/// \ingroup AESNIRNG
using AES192_4x64 = AES192Engine<std::uint64_t, 4>;

/// \brief AES-192 RNG engine with 64-bits integers output and 8 blocks
/// \ingroup AESNIRNG
using AES192_8x64 = AES192Engine<std::uint64_t, 8>;

/// \brief AES-192 RNG engine with 32-bits integers output and default blocks
/// \ingroup AESNIRNG
using AES192 = AES192Engine<std::uint32_t>;

/// \brief AES-192 RNG engine with 64-bits integers output and default blocks
/// \ingroup AESNIRNG
using AES192_64 = AES192Engine<std::uint64_t>;

/// \brief AES-256 RNG engine with 32-bits integers output and 1 block
/// \ingroup AESNIRNG
using AES256_1x32 = AES256Engine<std::uint32_t, 1>;

/// \brief AES-256 RNG engine with 32-bits integers output and 2 blocks
/// \ingroup AESNIRNG
using AES256_2x32 = AES256Engine<std::uint32_t, 2>;

/// \brief AES-256 RNG engine with 32-bits integers output and 4 blocks
/// \ingroup AESNIRNG
using AES256_4x32 = AES256Engine<std::uint32_t, 4>;

/// \brief AES-256 RNG engine with 32-bits integers output and 8 blocks
/// \ingroup AESNIRNG
using AES256_8x32 = AES256Engine<std::uint32_t, 8>;

/// \brief AES-256 RNG engine with 64-bits integers output and 1 block
/// \ingroup AESNIRNG
using AES256_1x64 = AES256Engine<std::uint64_t, 1>;

/// \brief AES-256 RNG engine with 64-bits integers output and 2 blocks
/// \ingroup AESNIRNG
using AES256_2x64 = AES256Engine<std::uint64_t, 2>;

/// \brief AES-256 RNG engine with 64-bits integers output and 4 blocks
/// \ingroup AESNIRNG
using AES256_4x64 = AES256Engine<std::uint64_t, 4>;

/// \brief AES-256 RNG engine with 64-bits integers output and 8 blocks
/// \ingroup AESNIRNG
using AES256_8x64 = AES256Engine<std::uint64_t, 8>;

/// \brief AES-256 RNG engine with 32-bits integers output and default blocks
/// \ingroup AESNIRNG
using AES256 = AES256Engine<std::uint32_t>;

/// \brief AES-256 RNG engine with 64-bits integers output and default blocks
/// \ingroup AESNIRNG
using AES256_64 = AES256Engine<std::uint64_t>;

} // namespace vsmc

#endif // VSMC_RNG_AES_HPP
