//============================================================================
// vSMC/include/vsmc/rng/philox.h
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

#ifndef VSMC_RNG_PHILOX_H
#define VSMC_RNG_PHILOX_H

#include <vsmc/rng/internal/common.h>

typedef struct {
    uint32_t v[4];
} vsmc_philox4x32_ctr_t;

typedef struct {
    uint32_t v[2];
} vsmc_philox4x32_key_t;

VSMC_STATIC_INLINE void vsmc_philox4x32_inc(vsmc_philox4x32_ctr_t *ctr)
{
    if (++ctr->v[0] != 0)
        return;
    if (++ctr->v[1] != 0)
        return;
    if (++ctr->v[2] != 0)
        return;
    if (++ctr->v[3] != 0)
        return;
}

VSMC_STATIC_INLINE void vsmc_philox4x32_bumpkey(vsmc_philox4x32_key_t *par)
{
    par->v[0] += 0x9E3779B9U;
    par->v[1] += 0xBB67AE85U;
}

VSMC_STATIC_INLINE void vsmc_philox4x32_round(
    vsmc_philox4x32_ctr_t *state, vsmc_philox4x32_key_t *par)
{
#ifdef __cplusplus
    uint64_t p0 = static_cast<uint64_t>(state->v[0]) * UINT64_C(0xD2511F53);
    uint64_t p2 = static_cast<uint64_t>(state->v[2]) * UINT64_C(0xCD9E8D57);
    uint32_t hi0 = static_cast<uint32_t>(p2 >> 32) ^ par->v[0];
    uint32_t lo1 = static_cast<uint32_t>(p2);
    uint32_t hi2 = static_cast<uint32_t>(p0 >> 32) ^ par->v[1];
    uint32_t lo3 = static_cast<uint32_t>(p0);
#else
    uint64_t p0 = ((uint64_t) state->v[0]) * UINT64_C(0xD2511F53);
    uint64_t p2 = ((uint64_t) state->v[2]) * UINT64_C(0xCD9E8D57);
    uint32_t hi0 = ((uint32_t) p2 >> 32) ^ par->v[0];
    uint32_t lo1 = ((uint32_t) p2);
    uint32_t hi2 = ((uint32_t) p0 >> 32) ^ par->v[1];
    uint32_t lo3 = ((uint32_t) p0);
#endif
    state->v[0] = hi0 ^ state->v[1];
    state->v[1] = lo1;
    state->v[2] = hi2 ^ state->v[3];
    state->v[3] = lo3;
}

/// \brief RNG state structure
/// \ingroup CRNG
typedef struct {
    vsmc_philox4x32_ctr_t state;
    vsmc_philox4x32_ctr_t ctr;
    vsmc_philox4x32_key_t key;
    unsigned char index;
} vsmc_philox4x32;

/// \brief Initialize RNG state
/// \ingroup CRNG
VSMC_STATIC_INLINE void vsmc_philox4x32_init(
    vsmc_philox4x32 *rng, uint32_t seed)
{
    rng->ctr.v[0] = 0;
    rng->ctr.v[1] = 0;
    rng->ctr.v[2] = 0;
    rng->ctr.v[3] = 0;
    rng->key.v[0] = seed;
    rng->index = 4;
}

/// \brief Generate random 32-bits integers
/// \ingroup CRNG
VSMC_STATIC_INLINE uint32_t vsmc_philox4x32_rand(vsmc_philox4x32 *rng)
{
    if (rng->index == 4) {
        rng->index = 0;
        vsmc_philox4x32_inc(&rng->ctr);
        rng->state = rng->ctr;
        vsmc_philox4x32_key_t par = rng->key;

        vsmc_philox4x32_bumpkey(&par);
        vsmc_philox4x32_round(&rng->state, &par); // round 0
        vsmc_philox4x32_bumpkey(&par);
        vsmc_philox4x32_round(&rng->state, &par); // round 1
        vsmc_philox4x32_bumpkey(&par);
        vsmc_philox4x32_round(&rng->state, &par); // round 2
        vsmc_philox4x32_bumpkey(&par);
        vsmc_philox4x32_round(&rng->state, &par); // round 3
        vsmc_philox4x32_bumpkey(&par);
        vsmc_philox4x32_round(&rng->state, &par); // round 4
        vsmc_philox4x32_bumpkey(&par);
        vsmc_philox4x32_round(&rng->state, &par); // round 5
        vsmc_philox4x32_bumpkey(&par);
        vsmc_philox4x32_round(&rng->state, &par); // round 6
        vsmc_philox4x32_bumpkey(&par);
        vsmc_philox4x32_round(&rng->state, &par); // round 7
        vsmc_philox4x32_bumpkey(&par);
        vsmc_philox4x32_round(&rng->state, &par); // round 8
        vsmc_philox4x32_bumpkey(&par);
        vsmc_philox4x32_round(&rng->state, &par); // round 9
    }

    return rng->state.v[rng->index++];
}

#endif // VSMC_RNG_PHILOX_H