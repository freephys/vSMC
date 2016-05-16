//============================================================================
// vSMC/lib/src/core/sampler.cpp
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

#include <vsmc/core/core.h>
#include "libvsmc.hpp"

namespace vsmc
{

inline SamplerC::eval_type cast(vsmc_sampler_eval_type fptr)
{
    return [fptr](std::size_t iter, ParticleC &particle) {
        vsmc_particle particle_c = {&particle};
        return fptr(iter, particle_c);
    };
}

} // namespace vsmc

extern "C" {

vsmc_sampler vsmc_sampler_new(size_t n, size_t dim)
{
    return {new ::vsmc::SamplerC(n, dim)};
}

void vsmc_sampler_delete(vsmc_sampler *sampler_ptr)
{
    delete ::vsmc::cast(sampler_ptr);
    sampler_ptr->ptr = nullptr;
}

void vsmc_sampler_assign(vsmc_sampler sampler, vsmc_sampler other)
{
    ::vsmc::cast(sampler) = ::vsmc::cast(other);
}

vsmc_sampler vsmc_sampler_clone(vsmc_sampler sampler)
{
    vsmc_sampler clone = vsmc_sampler_new(0, 0);
    ::vsmc::cast(clone) = ::vsmc::cast(sampler).clone();

    return clone;
}

size_t vsmc_sampler_size(vsmc_sampler sampler)
{
    return ::vsmc::cast(sampler).size();
}

void vsmc_sampler_reserve(vsmc_sampler sampler, size_t num)
{
    ::vsmc::cast(sampler).reserve(num);
}

size_t vsmc_sampler_iter_size(vsmc_sampler sampler)
{
    return ::vsmc::cast(sampler).iter_size();
}

size_t vsmc_sampler_iter_num(vsmc_sampler sampler)
{
    return ::vsmc::cast(sampler).iter_num();
}

void vsmc_sampler_reset(vsmc_sampler sampler)
{
    ::vsmc::cast(sampler).reset();
}

void vsmc_sampler_clear(vsmc_sampler sampler)
{
    ::vsmc::cast(sampler).clear();
}

void vsmc_sampler_eval_clear(vsmc_sampler sampler)
{
    ::vsmc::cast(sampler).eval_clear();
}

void vsmc_sampler_eval(vsmc_sampler sampler, vsmc_sampler_eval_type new_eval,
    vSMCSamplerStage stage, int append)
{
    ::vsmc::cast(sampler).eval(::vsmc::cast(new_eval),
        static_cast<::vsmc::SamplerStage>(stage), append != 0);
}

void vsmc_sampler_resample_scheme(
    vsmc_sampler sampler, vSMCResampleScheme scheme, double threshold)
{
    ::vsmc::cast(sampler).resample_method(
        static_cast<::vsmc::ResampleScheme>(scheme), threshold);
}

void vsmc_sampler_resample_eval(
    vsmc_sampler sampler, vsmc_sampler_eval_type res_eval, double threshold)
{
    ::vsmc::cast(sampler).resample_method(::vsmc::cast(res_eval), threshold);
}

double vsmc_sampler_get_threshold(vsmc_sampler sampler)
{
    return ::vsmc::cast(sampler).resample_threshold();
}

void vsmc_sampler_set_threshold(vsmc_sampler sampler, double threshold)
{
    ::vsmc::cast(sampler).resample_threshold(threshold);
}

double vsmc_sampler_resample_threshold_never()
{
    return ::vsmc::SamplerC::resample_threshold_never();
}

double vsmc_sampler_resample_threshold_always()
{
    return ::vsmc::SamplerC::resample_threshold_always();
}

void vsmc_sampler_set_monitor(
    vsmc_sampler sampler, const char *name, vsmc_monitor mon)
{
    ::vsmc::cast(sampler).monitor(name, ::vsmc::cast(mon));
}

vsmc_monitor vsmc_sampler_get_monitor(vsmc_sampler sampler, const char *name)
{
    vsmc_monitor monitor = {&::vsmc::cast(sampler).monitor(name)};

    return monitor;
}

void vsmc_sampler_monitor_clear(vsmc_sampler sampler, const char *name)
{
    ::vsmc::cast(sampler).monitor_clear(name);
}

void vsmc_sampler_monitor_clear_all(vsmc_sampler sampler)
{
    ::vsmc::cast(sampler).monitor_clear();
}

void vsmc_sampler_initialize(vsmc_sampler sampler)
{
    ::vsmc::cast(sampler).initialize();
}

void vsmc_sampler_iterate(vsmc_sampler sampler, size_t num)
{
    ::vsmc::cast(sampler).iterate(num);
}

vsmc_particle vsmc_sampler_particle(vsmc_sampler sampler)
{
    vsmc_particle particle = {&::vsmc::cast(sampler).particle()};

    return particle;
}

size_t vsmc_sampler_size_history(vsmc_sampler sampler, size_t iter)
{
    return ::vsmc::cast(sampler).size_history(iter);
}

void vsmc_sampler_read_size_history(vsmc_sampler sampler, size_t *first)
{
    ::vsmc::cast(sampler).read_size_history(first);
}

double vsmc_sampler_ess_history(vsmc_sampler sampler, size_t iter)
{
    return ::vsmc::cast(sampler).ess_history(static_cast<std::size_t>(iter));
}

void vsmc_sampler_read_ess_history(vsmc_sampler sampler, double *first)
{
    ::vsmc::cast(sampler).read_ess_history(first);
}

int vsmc_sampler_resampled_history(vsmc_sampler sampler, size_t iter)
{
    return ::vsmc::cast(sampler).resampled_history(iter);
}

void vsmc_sampler_read_resampled_history(vsmc_sampler sampler, int *first)
{
    ::vsmc::cast(sampler).read_resampled_history(first);
}

size_t vsmc_sampler_print(vsmc_sampler sampler, char *buf, char sepchar)
{
    std::stringstream ss;
    ::vsmc::cast(sampler).print(ss, sepchar);
    std::string str(ss.str());
    std::size_t size = (str.size() + 1) * sizeof(char);
    if (buf != nullptr)
        std::memcpy(buf, str.c_str(), size);

    return size;
}

void vsmc_sampler_print_f(
    vsmc_sampler sampler, const char *filename, char sepchar)
{
    std::ofstream os(filename);
    ::vsmc::cast(sampler).print(os, sepchar);
    os.close();
}

} // extern "C"
