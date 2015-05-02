//============================================================================
// vSMC/include/vsmc/opencl/cl_manip.hpp
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

#ifndef VSMC_OPENCL_CL_MANIP_HPP
#define VSMC_OPENCL_CL_MANIP_HPP

#include <vsmc/opencl/internal/common.hpp>

namespace vsmc
{

/// \brief Query the preferred factor of local size
/// \ingroup OpenCL
///
/// \param kern An OpenCL kernel
/// \param dev An OpenCL device
/// \param factor Multiplier factor of local size for optimzied performance
/// \param lmax Maximum of the local size
/// \param mmax Maximum of the multiplier of the factor
inline void cl_minmax_local_size(::cl_kernel kern, ::cl_device_id dev,
    std::size_t &factor, std::size_t &lmax, std::size_t &mmax)
{
    ::cl_int status = CL_SUCCESS;

    status = ::clGetKernelWorkGroupInfo(kern, dev,
        CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, sizeof(std::size_t),
        &factor, nullptr);
    if (status != CL_SUCCESS) {
        factor = lmax = mmax = 0;
        return;
    }

    status = ::clGetKernelWorkGroupInfo(kern, dev, CL_KERNEL_WORK_GROUP_SIZE,
        sizeof(std::size_t), &lmax, nullptr);
    if (status != CL_SUCCESS) {
        factor = lmax = mmax = 0;
        return;
    }

    if (factor == 0 || factor > lmax) {
        factor = lmax = mmax = 0;
        return;
    }

    mmax = lmax / factor;
}

/// \brief The minimum global size that is a multiple of the local size
/// \ingroup OpenCL
inline std::size_t cl_min_global_size(std::size_t N, std::size_t local_size)
{
    if (local_size == 0)
        return N;

    return (local_size && N % local_size) ? (N / local_size + 1) * local_size :
                                            N;
}

/// \brief The preferred global and local size
/// \ingroup OpenCL
///
/// \return The difference between the preferred global size and the N
inline std::size_t cl_preferred_work_size(std::size_t N, ::cl_kernel kern,
    ::cl_device_id dev, std::size_t &global_size, std::size_t &local_size)
{
    ::cl_int status = CL_SUCCESS;

    std::size_t reqd_size[3] = {0};
    status = ::clGetKernelWorkGroupInfo(kern, dev,
        CL_KERNEL_COMPILE_WORK_GROUP_SIZE, sizeof(reqd_size), reqd_size,
        nullptr);
    if (status == CL_SUCCESS && reqd_size[0] != 0) {
        local_size = reqd_size[0];
        global_size = cl_min_global_size(N, local_size);

        return global_size - N;
    }

    std::size_t factor;
    std::size_t lmax;
    std::size_t mmax;
    cl_minmax_local_size(kern, dev, factor, lmax, mmax);
    if (lmax == 0) {
        global_size = N;
        local_size = 0;

        return global_size - N;
    }

    local_size = lmax;
    global_size = cl_min_global_size(N, local_size);
    std::size_t diff_size = global_size - N;
    for (std::size_t m = mmax; m >= 1; --m) {
        std::size_t l = m * factor;
        std::size_t g = cl_min_global_size(N, l);
        std::size_t d = g - N;
        if (d < diff_size) {
            local_size = l;
            global_size = g;
            diff_size = d;
        }
    }

    return diff_size;
}

inline void cl_set_kernel_args(::cl_kernel, ::cl_uint) {}

/// \brief Set OpenCL kernel arguments
/// \ingroup OpenCL
///
/// \details
/// The first argument `arg1` is set with index `offset` and the next with
/// index `offset + 1` and so on. For C++11 implementations that support
/// variadic template, there can be arbitrary number of arguments (or limited
/// by the compiler's implementation). Otherwise this function supports up to
/// 16 arguments.
template <typename Arg1, typename... Args>
inline void cl_set_kernel_args(
    ::cl_kernel kern, ::cl_uint offset, const Arg1 &arg1, const Args &... args)
{
    ::cl_int status = ::clSetKernelArg(kern, offset, sizeof(Arg1), &arg1);
    internal::cl_error_check(status, "cl_set_kernel_args", "::clSetKernelArg");
    cl_set_kernel_args(kern, offset + 1, args...);
}

} // namespace vsmc

#endif // VSMC_OPENCL_CL_MANIP_HPP
