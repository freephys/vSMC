//============================================================================
// vSMC/example/rng/src/rng_fisher_f.cpp
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

#define RNG_DIST_STREAM(stream, n, r, param)                                  \
    {                                                                         \
        const std::size_t k = vsmc::internal::BufferSize<RealType>::value;    \
        const std::size_t p = n / k;                                          \
        const std::size_t q = n % k;                                          \
        vsmc::Array<RealType, k> s;                                           \
        RealType *ptr = r.data();                                             \
        for (std::size_t x = 0; x != p; ++x, ptr += k) {                      \
            stream.gamma(                                                     \
                static_cast<MKL_INT>(k), s.data(), param[0] / 2, 0, 2);       \
            stream.gamma(static_cast<MKL_INT>(k), ptr, param[1] / 2, 0, 2);   \
            vsmc::mul(k, 1 / param[0], s.data(), s.data());                   \
            vsmc::mul(k, 1 / param[1], ptr, ptr);                             \
            vsmc::div(k, s.data(), ptr, ptr);                                 \
        }                                                                     \
        stream.gamma(static_cast<MKL_INT>(q), s.data(), param[0] / 2, 0, 2);  \
        stream.gamma(static_cast<MKL_INT>(q), ptr, param[1] / 2, 0, 2);       \
        vsmc::mul(q, 1 / param[0], s.data(), s.data());                       \
        vsmc::mul(q, 1 / param[1], ptr, ptr);                                 \
        vsmc::div(q, s.data(), ptr, ptr);                                     \
    }

#include <vsmc/rng/fisher_f_distribution.hpp>
#include "rng_dist.hpp"

int main(int argc, char **argv)
{
    vsmc::Vector<std::array<double, 2>> params;
    params.push_back({{0.2, 0.2}});
    params.push_back({{0.2, 1.0}});
    params.push_back({{0.2, 1.5}});
    params.push_back({{0.2, 2.0}});
    params.push_back({{0.2, 3.0}});
    params.push_back({{1.0, 0.2}});
    params.push_back({{1.0, 1.0}});
    params.push_back({{1.0, 1.5}});
    params.push_back({{1.0, 2.0}});
    params.push_back({{1.0, 3.0}});
    params.push_back({{2.0, 0.2}});
    params.push_back({{2.0, 1.0}});
    params.push_back({{2.0, 1.5}});
    params.push_back({{2.0, 2.0}});
    params.push_back({{2.0, 3.0}});
    params.push_back({{3.0, 0.2}});
    params.push_back({{3.0, 1.0}});
    params.push_back({{3.0, 1.5}});
    params.push_back({{3.0, 2.0}});
    params.push_back({{3.0, 3.0}});
    VSMC_RNG_DIST_TEST(2, FisherF, std::fisher_f_distribution);

    return 0;
}
