//============================================================================
// vSMC/include/vsmc/utility/mkl.hpp
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

#ifndef VSMC_UTILITY_MKL_HPP
#define VSMC_UTILITY_MKL_HPP

#include <vsmc/rng/internal/common.hpp>
#include <mkl.h>

#define VSMC_STATIC_ASSERT_UTILITY_MKL_SS_TASK_RESULT_TYPE(ResultType)        \
    VSMC_STATIC_ASSERT((std::is_same<ResultType, float>::value ||             \
                           std::is_same<ResultType, double>::value),          \
        "**MKLSSTask** USED WITH A ResultType OTHER THAN float OR double")

#define VSMC_STATIC_ASSERT_UTILITY_MKL_CONV_TASK_RESULT_TYPE(ResultType)      \
    VSMC_STATIC_ASSERT((std::is_same<ResultType, float>::value ||             \
                           std::is_same<ResultType, double>::value ||         \
                           std::is_same<ResultType, MKL_Complex8>::value ||   \
                           std::is_same<ResultType, MKL_Complex16>::value),   \
        "**MKLConvTask** USED WITH A ResultType OTHER THAN float, double, "   \
        "MKL_Complex8, OR MKL_Complex16")

#define VSMC_STATIC_ASSERT_UTILITY_MKL_CORR_TASK_RESULT_TYPE(ResultType)      \
    VSMC_STATIC_ASSERT((std::is_same<ResultType, float>::value ||             \
                           std::is_same<ResultType, double>::value ||         \
                           std::is_same<ResultType, MKL_Complex8>::value ||   \
                           std::is_same<ResultType, MKL_Complex16>::value),   \
        "**MKLCorrTask** USED WITH A ResultType OTHER THAN float, double, "   \
        "MKL_Complex8, OR MKL_Complex16")

#define VSMC_STATIC_ASSERT_UTILITY_MKL_DF_TASK_RESULT_TYPE(ResultType)        \
    VSMC_STATIC_ASSERT((std::is_same<ResultType, float>::value ||             \
                           std::is_same<ResultType, double>::value),          \
        "**MKLDFTask** USED WITH A ResultType OTHER THAN float OR double")

#define VSMC_RUNTIME_ASSERT_UTILITY_MKL_VSL_OFFSET(offset)                    \
    VSMC_RUNTIME_ASSERT((offset < max VSMC_MNE()),                            \
        "**MKLOffsetDynamic** "                                               \
        "EXCESS MAXIMUM NUMBER OF INDEPDENT RNG STREAMS")

namespace vsmc
{

namespace internal
{

#if VSMC_NO_RUNTIME_ASSERT
inline void mkl_error_check(int, const char *, const char *) {}
#else
inline void mkl_error_check(int status, const char *func, const char *mklf)
{
    if (status == 0)
        return;

    std::string msg("**vsmc::");
    msg += func;
    msg += "** failure";
    msg += "; MKL function: ";
    msg += mklf;
    msg += "; Error code: ";
    msg += itos(status);

    VSMC_RUNTIME_ASSERT((status == 0), msg.c_str());
}
#endif

struct MKLOffsetZero {
    static constexpr MKL_INT min VSMC_MNE() { return 0; }
    static constexpr MKL_INT max VSMC_MNE() { return 0; }
    static void offset(MKL_INT) {}
    static constexpr MKL_INT offset() { return 0; }
}; // struct OffsetZero

template <MKL_INT MaxOffset>
struct MKLOffsetDynamic {
    MKLOffsetDynamic() : offset_(0) {}

    static constexpr MKL_INT min VSMC_MNE() { return 0; }
    static constexpr MKL_INT max VSMC_MNE() { return MaxOffset; }

    void offset(MKL_INT n)
    {
        VSMC_RUNTIME_ASSERT_UTILITY_MKL_VSL_OFFSET(n);
        offset_ = n;
    }

    MKL_INT offset() const { return offset_; }

    private:
    MKL_INT offset_;
}; // struct OffsetDynamic

template <MKL_INT>
struct MKLOffset {
    typedef MKLOffsetZero type;
};

template <>
struct MKLOffset<VSL_BRNG_MT2203> {
    typedef MKLOffsetDynamic<6024> type;
};

template <>
struct MKLOffset<VSL_BRNG_WH> {
    typedef MKLOffsetDynamic<273> type;
};

} // namespace vsmc::internal

namespace traits
{

/// \brief Default seed for MKL RNG
/// \ingroup Traits
template <MKL_INT>
struct MKLSeedTrait : public std::integral_constant<MKL_UINT, 1> {
};

/// \brief Default seed for MKL Sobol quasi-RNG
template <>
struct MKLSeedTrait<VSL_BRNG_SOBOL>
    : public std::integral_constant<MKL_UINT, 10> {
};

/// \brief Default seed for MKL Niederr quasi-RNG
template <>
struct MKLSeedTrait<VSL_BRNG_NIEDERR>
    : public std::integral_constant<MKL_UINT, 10> {
};

} // namespace traits

/// \brief MKL resource management base class
/// \ingroup MKL
template <typename MKLPtr, typename Derived>
class MKLBase
{
    public:
    typedef MKLPtr pointer;
    typedef typename std::remove_pointer<MKLPtr>::type element_type;

    class deleter_type
    {
        public:
        void operator()(MKLPtr ptr) { status_ = Derived::release(&ptr); }

        int status() const { return status_; }

        private:
        int status_;
    };

    MKLBase(const MKLBase<MKLPtr, Derived> &) = delete;
    MKLBase<MKLPtr, Derived> &operator=(
        const MKLBase<MKLPtr, Derived> &) = delete;
    MKLBase(MKLBase<MKLPtr, Derived> &&) = default;
    MKLBase<MKLPtr, Derived> &operator=(MKLBase<MKLPtr, Derived> &&) = default;

    int release() { return Derived::release(ptr_.get()); }

    void reset(pointer ptr = nullptr) { ptr_.reset(ptr); }

    void swap(MKLBase<MKLPtr, Derived> &other) { ptr_.swap(other.ptr_); }

    pointer get() { return ptr_.get(); }

    pointer ptr() { return ptr_.get(); }

    deleter_type &get_deleter() { return ptr_.get_deleter(); }
    const deleter_type &get_deleter() const { return ptr_.get_deleter(); }

    explicit operator bool() const { return bool(ptr_); }

    private:
    std::unique_ptr<element_type, deleter_type> ptr_;
}; // class MKLBase

/// \brief Comparison of equality of two MKLBase objects
/// \ingroup MKL
template <typename MKLPtr, typename Derived>
inline bool operator==(
    const MKLBase<MKLPtr, Derived> &ptr1, const MKLBase<MKLPtr, Derived> &ptr2)
{
    return ptr1.get() == ptr2.get();
}

/// \brief Comparison of inequality of two MKLBase objects
/// \ingroup MKL
template <typename MKLPtr, typename Derived>
inline bool operator!=(
    const MKLBase<MKLPtr, Derived> &ptr1, const MKLBase<MKLPtr, Derived> &ptr2)
{
    return ptr1.get() == ptr2.get();
}

/// \brief Swap two MKLBase objects
/// \ingroup MKL
template <typename MKLPtr, typename Derived>
inline void swap(
    const MKLBase<MKLPtr, Derived> &ptr1, const MKLBase<MKLPtr, Derived> &ptr2)
{
    ptr1.swap(ptr2);
}

/// \brief MKL `VSLStreamStatePtr`
/// \ingroup MKL
template <MKL_INT BRNG>
class MKLStream : public internal::MKLOffset<BRNG>::type
{
    public:
    explicit MKLStream(
        MKL_UINT s = traits::MKLSeedTrait<BRNG>::value, MKL_INT offset = 0)
    {
        this->offset(offset);
        VSLStreamStatePtr ptr = nullptr;
        internal::mkl_error_check(
            ::vslNewStream(&ptr, BRNG + this->offset(), s),
            "MKLStream::MKLStream", "::vslNewStream");
        stream_ptr_.reset(ptr);
    }

    template <typename SeedSeq>
    explicit MKLStream(
        SeedSeq &seq, typename std::enable_if<internal::is_seed_seq<SeedSeq,
                          MKL_UINT, MKLStream<BRNG>>::value>::type * = nullptr)
    {
        MKL_UINT s = 0;
        seq.generate(&s, &s + 1);
        VSLStreamStatePtr ptr = nullptr;
        internal::mkl_error_check(
            ::vslNewStream(&ptr, BRNG + this->offset(), s),
            "MKLStream::MKLStream", "::vslNewStream");
        stream_ptr_.reset(ptr);
    }

    MKLStream(const MKLStream<BRNG> &other)
        : internal::MKLOffset<BRNG>::type(other)
    {
        VSLStreamStatePtr ptr = nullptr;
        internal::mkl_error_check(
            ::vslCopyStream(&ptr, other.stream_ptr_.get()),
            "MKLStream::MKLStream", "::vslCopyStream");
        stream_ptr_.reset(ptr);
    }

    MKLStream<BRNG> &operator=(const MKLStream<BRNG> &other)
    {
        if (this != &other) {
            internal::MKLOffset<BRNG>::type::operator=(other);
            internal::mkl_error_check(::vslCopyStreamState(stream_ptr_.get(),
                                          other.stream_ptr_.get()),
                "MKLStream::operator=", "::vslCopyStreamState");
        }

        return *this;
    }

    MKLStream(MKLStream<BRNG> &&) = default;
    MKLStream<BRNG> &operator=(MKLStream<BRNG> &&) = default;

    void seed(MKL_UINT s)
    {
        VSLStreamStatePtr ptr = nullptr;
        internal::mkl_error_check(
            ::vslNewStream(&ptr, BRNG + this->offset(), s), "MKLStream::seed",
            "::vslNewStream");
        stream_ptr_.reset(ptr);
    }

    template <typename SeedSeq>
    void seed(
        SeedSeq &seq, typename std::enable_if<internal::is_seed_seq<SeedSeq,
                          MKL_UINT, MKLStream<BRNG>>::value>::type * = nullptr)
    {
        MKL_UINT s = 0;
        seq.generate(&s, &s + 1);
        seed(s);
    }

    VSLStreamStatePtr ptr() const { return stream_ptr_.get(); }

    private:
    struct deleter {
        void operator()(VSLStreamStatePtr ptr)
        {
            internal::mkl_error_check(::vslDeleteStream(&ptr),
                "MKLStream::~MKLStream", "::vslDeleteStream");
        }
    };

    MKL_UINT seed_;
    std::unique_ptr<std::remove_pointer<VSLStreamStatePtr>::type, deleter>
        stream_ptr_;
}; // class MKLStream

/// \brief MKL `VSLSSTaskPtr`
/// \ingroup MKL
template <typename ResultType = double>
class MKLSSTask : public MKLBase<VSLSSTaskPtr, MKLSSTask<ResultType>>
{
    public:
    typedef ResultType result_type;

    MKLSSTask()
    {
        VSMC_STATIC_ASSERT_UTILITY_MKL_SS_TASK_RESULT_TYPE(ResultType);
    }

    MKLSSTask(const MKL_INT *p, const MKL_INT *n, const MKL_INT *xstorage,
        const result_type *x, const result_type *w = nullptr,
        const MKL_INT *indices = nullptr)
    {
        VSMC_STATIC_ASSERT_UTILITY_MKL_SS_TASK_RESULT_TYPE(ResultType);
        reset(p, n, xstorage, x, w, indices);
    }

    static int release(VSLSSTaskPtr ptr)
    {
        int status = ::vslSSDeleteTask(&ptr);
        internal::mkl_error_check(
            status, "MKLSSTask::release", "::vslDeleteTask");

        return status;
    }

    int reset(const MKL_INT *p, const MKL_INT *n, const MKL_INT *xstorage,
        const result_type *x, const result_type *w = nullptr,
        const MKL_INT *indices = nullptr)
    {
        VSLSSTaskPtr ptr = nullptr;
        int status = reset_dispatch(&ptr, p, n, xstorage, x, w, indices);
        this->reset(ptr);

        return status;
    }

    private:
    static int reset_dispatch(VSLSSTaskPtr *task, const MKL_INT *p,
        const MKL_INT *n, const MKL_INT *xstorage, const float *x,
        const float *w, const MKL_INT *indices)
    {
        int status = ::vslsSSNewTask(task, p, n, xstorage, x, w, indices);
        internal::mkl_error_check(
            status, "MKLSSTask::reset", "::vslsSSNewTask");

        return status;
    }

    static int reset_dispatch(VSLSSTaskPtr *task, const MKL_INT *p,
        const MKL_INT *n, const MKL_INT *xstorage, const double *x,
        const double *w, const MKL_INT *indices)
    {
        int status = ::vsldSSNewTask(task, p, n, xstorage, x, w, indices);
        internal::mkl_error_check(
            status, "MKLSSTask::reset", "::vsldSSNewTask");

        return status;
    }
}; // class MKLSSTask

/// \brief MKL `VSLConvTaskPtr`
/// \ingroup MKL
template <typename ResultType = double>
class MKLConvTask : public MKLBase<VSLConvTaskPtr, MKLConvTask<ResultType>>
{
    public:
    typedef ResultType result_type;

    MKLConvTask()
    {
        VSMC_STATIC_ASSERT_UTILITY_MKL_CONV_TASK_RESULT_TYPE(ResultType);
    }

    /// \brief `vslConvNewTask`
    MKLConvTask(MKL_INT mode, MKL_INT dims, const MKL_INT *xshape,
        const MKL_INT *yshape, const MKL_INT *zshape)
    {
        VSMC_STATIC_ASSERT_UTILITY_MKL_CONV_TASK_RESULT_TYPE(ResultType);
        reset(mode, dims, xshape, yshape, zshape);
    }

    /// \brief `vslConvNewTask1D`
    MKLConvTask(
        MKL_INT mode, const MKL_INT xshape, MKL_INT yshape, MKL_INT zshape)
    {
        VSMC_STATIC_ASSERT_UTILITY_MKL_CONV_TASK_RESULT_TYPE(ResultType);
        reset(mode, xshape, yshape, zshape);
    }

    /// \brief `vslConvNewTaskX`
    MKLConvTask(MKL_INT mode, MKL_INT dims, const MKL_INT *xshape,
        const MKL_INT *yshape, const MKL_INT *zshape, const result_type *x,
        const MKL_INT *xstride)
    {
        VSMC_STATIC_ASSERT_UTILITY_MKL_CONV_TASK_RESULT_TYPE(ResultType);
        reset(mode, dims, xshape, yshape, zshape, x, xstride);
    }

    /// \brief `vslConvNewTaskX1D`
    MKLConvTask(MKL_INT mode, MKL_INT xshape, MKL_INT yshape, MKL_INT zshape,
        const result_type *x, const MKL_INT xstride)
    {
        VSMC_STATIC_ASSERT_UTILITY_MKL_CONV_TASK_RESULT_TYPE(ResultType);
        reset(mode, xshape, yshape, zshape, x, xstride);
    }

    MKLConvTask(const MKLConvTask<ResultType> &other)
    {
        VSMC_STATIC_ASSERT_UTILITY_MKL_CONV_TASK_RESULT_TYPE(ResultType);

        VSLConvTaskPtr ptr = nullptr;
        internal::mkl_error_check(::vslConvCopyTask(&ptr, other.get()),
            "MKLConvTask::MKLConvTask", "::vslConvCopyTask");
        this->reset(ptr);
    }

    MKLConvTask<ResultType> &operator=(const MKLConvTask<ResultType> &other)
    {
        if (this != &other) {
            VSLConvTaskPtr ptr = nullptr;
            internal::mkl_error_check(::vslConvCopyTask(&ptr, other.get()),
                "MKLConvTask::operator=", "::vslConvCopyTask");
            this->reset(ptr);
        }

        return *this;
    }

    MKLConvTask(MKLConvTask<ResultType> &&) = default;
    MKLConvTask<ResultType> &operator=(MKLConvTask<ResultType> &&) = default;

    static int release(VSLConvTaskPtr ptr)
    {
        int status = ::vslConvDeleteTask(&ptr);
        internal::mkl_error_check(
            status, "MKLConvTask::release", "::vslConvDeleteTask");

        return status;
    }

    /// \brief `vslConvNewTask`
    int reset(MKL_INT mode, MKL_INT dims, const MKL_INT *xshape,
        const MKL_INT *yshape, const MKL_INT *zshape)
    {
        VSLConvTaskPtr ptr = nullptr;
        int status = reset_dispatch(&ptr, mode, dims, xshape, yshape, zshape);
        this->reset(ptr);

        return status;
    }

    /// \brief `vslConvNewTask1D`
    int reset(
        MKL_INT mode, const MKL_INT xshape, MKL_INT yshape, MKL_INT zshape)
    {
        VSLConvTaskPtr ptr = nullptr;
        int status = reset_dispatch(&ptr, mode, xshape, yshape, zshape);
        this->reset(ptr);

        return status;
    }

    /// \brief `vslConvNewTaskX`
    int reset(MKL_INT mode, MKL_INT dims, const MKL_INT *xshape,
        const MKL_INT *yshape, const MKL_INT *zshape, const result_type *x,
        const MKL_INT *xstride)
    {
        VSLConvTaskPtr ptr = nullptr;
        int status = reset_dispatch(
            &ptr, mode, dims, xshape, yshape, zshape, x, xstride);
        this->reset(ptr);

        return status;
    }

    /// \brief `vslConvNewTaskX1D`
    int reset(MKL_INT mode, MKL_INT xshape, MKL_INT yshape, MKL_INT zshape,
        const result_type *x, const MKL_INT xstride)
    {
        VSLConvTaskPtr ptr = nullptr;
        int status =
            reset_dispatch(&ptr, mode, xshape, yshape, zshape, x, xstride);
        this->reset(ptr);

        return status;
    }

    private:
    static int reset_dispatch(VSLConvTaskPtr *task, const MKL_INT mode,
        MKL_INT dims, const MKL_INT *xshape, const MKL_INT *yshape,
        const MKL_INT *zshape, float *)
    {
        internal::mkl_error_check(
            ::vslsConvNewTask(task, mode, dims, xshape, yshape, zshape),
            "MKLConvTask::reset", "::vslsConvNewTask");
    }

    static int reset_dispatch(VSLConvTaskPtr *task, const MKL_INT mode,
        MKL_INT dims, const MKL_INT *xshape, const MKL_INT *yshape,
        const MKL_INT *zshape, double *)
    {
        internal::mkl_error_check(
            ::vsldConvNewTask(task, mode, dims, xshape, yshape, zshape),
            "MKLConvTask::reset", "::vsldConvNewTask");
    }

    static int reset_dispatch(VSLConvTaskPtr *task, const MKL_INT mode,
        MKL_INT dims, const MKL_INT *xshape, const MKL_INT *yshape,
        const MKL_INT *zshape, MKL_Complex8 *)
    {
        internal::mkl_error_check(
            ::vslcConvNewTask(task, mode, dims, xshape, yshape, zshape),
            "MKLConvTask::reset", "::vslcConvNewTask");
    }

    static int reset_dispatch(VSLConvTaskPtr *task, const MKL_INT mode,
        MKL_INT dims, const MKL_INT *xshape, const MKL_INT *yshape,
        const MKL_INT *zshape, MKL_Complex16 *)
    {
        internal::mkl_error_check(
            ::vslzConvNewTask(task, mode, dims, xshape, yshape, zshape),
            "MKLConvTask::reset", "::vslzConvNewTask");
    }

    static int reset_dispatch(VSLConvTaskPtr *task, const MKL_INT mode,
        const MKL_INT xshape, const MKL_INT yshape, const MKL_INT zshape,
        float *)
    {
        internal::mkl_error_check(
            ::vslsConvNewTask1D(task, mode, xshape, yshape, zshape),
            "MKLConvTask::reset", "::vslsConvNewTask1D");
    }

    static int reset_dispatch(VSLConvTaskPtr *task, const MKL_INT mode,
        const MKL_INT xshape, const MKL_INT yshape, const MKL_INT zshape,
        double *)
    {
        internal::mkl_error_check(
            ::vsldConvNewTask1D(task, mode, xshape, yshape, zshape),
            "MKLConvTask::reset", "::vsldConvNewTask1D");
    }

    static int reset_dispatch(VSLConvTaskPtr *task, const MKL_INT mode,
        const MKL_INT xshape, const MKL_INT yshape, const MKL_INT zshape,
        MKL_Complex8 *)
    {
        internal::mkl_error_check(
            ::vslcConvNewTask1D(task, mode, xshape, yshape, zshape),
            "MKLConvTask::reset", "::vslcConvNewTask1D");
    }

    static int reset_dispatch(VSLConvTaskPtr *task, const MKL_INT mode,
        const MKL_INT xshape, const MKL_INT yshape, const MKL_INT zshape,
        MKL_Complex16 *)
    {
        internal::mkl_error_check(
            ::vslzConvNewTask1D(task, mode, xshape, yshape, zshape),
            "MKLConvTask::reset", "::vslzConvNewTask1D");
    }

    static int reset_dispatch(VSLConvTaskPtr *task, const MKL_INT mode,
        MKL_INT dims, const MKL_INT *xshape, const MKL_INT *yshape,
        const MKL_INT *zshape, const float *x, const MKL_INT *xstride)
    {
        internal::mkl_error_check(::vslsConvNewTaskX(task, mode, dims, xshape,
                                      yshape, zshape, x, xstride),
            "MKLConvTask::reset", "::vslsConvNewTaskX");
    }

    static int reset_dispatch(VSLConvTaskPtr *task, const MKL_INT mode,
        MKL_INT dims, const MKL_INT *xshape, const MKL_INT *yshape,
        const MKL_INT *zshape, const double *x, const MKL_INT *xstride)
    {
        internal::mkl_error_check(::vsldConvNewTaskX(task, mode, dims, xshape,
                                      yshape, zshape, x, xstride),
            "MKLConvTask::reset", "::vsldConvNewTaskX");
    }

    static int reset_dispatch(VSLConvTaskPtr *task, const MKL_INT mode,
        MKL_INT dims, const MKL_INT *xshape, const MKL_INT *yshape,
        const MKL_INT *zshape, const MKL_Complex8 *x, const MKL_INT *xstride)
    {
        internal::mkl_error_check(::vslcConvNewTaskX(task, mode, dims, xshape,
                                      yshape, zshape, x, xstride),
            "MKLConvTask::reset", "::vslcConvNewTaskX");
    }

    static int reset_dispatch(VSLConvTaskPtr *task, const MKL_INT mode,
        MKL_INT dims, const MKL_INT *xshape, const MKL_INT *yshape,
        const MKL_INT *zshape, const MKL_Complex16 *x, const MKL_INT *xstride)
    {
        internal::mkl_error_check(::vslzConvNewTaskX(task, mode, dims, xshape,
                                      yshape, zshape, x, xstride),
            "MKLConvTask::reset", "::vslzConvNewTaskX");
    }

    static int reset_dispatch(VSLConvTaskPtr *task, const MKL_INT mode,
        const MKL_INT xshape, const MKL_INT yshape, const MKL_INT zshape,
        const float *x, const MKL_INT xstride)
    {
        internal::mkl_error_check(::vslsConvNewTaskX1D(task, mode, xshape,
                                      yshape, zshape, x, xstride),
            "MKLConvTask::reset", "::vslsConvNewTaskX1D");
    }

    static int reset_dispatch(VSLConvTaskPtr *task, const MKL_INT mode,
        const MKL_INT xshape, const MKL_INT yshape, const MKL_INT zshape,
        const double *x, const MKL_INT xstride)
    {
        internal::mkl_error_check(::vsldConvNewTaskX1D(task, mode, xshape,
                                      yshape, zshape, x, xstride),
            "MKLConvTask::reset", "::vsldConvNewTaskX1D");
    }

    static int reset_dispatch(VSLConvTaskPtr *task, const MKL_INT mode,
        const MKL_INT xshape, const MKL_INT yshape, const MKL_INT zshape,
        const MKL_Complex8 *x, const MKL_INT xstride)
    {
        internal::mkl_error_check(::vslcConvNewTaskX1D(task, mode, xshape,
                                      yshape, zshape, x, xstride),
            "MKLConvTask::reset", "::vslcConvNewTaskX1D");
    }

    static int reset_dispatch(VSLConvTaskPtr *task, const MKL_INT mode,
        const MKL_INT xshape, const MKL_INT yshape, const MKL_INT zshape,
        const MKL_Complex16 *x, const MKL_INT xstride)
    {
        internal::mkl_error_check(::vslzConvNewTaskX1D(task, mode, xshape,
                                      yshape, zshape, x, xstride),
            "MKLConvTask::reset", "::vslzConvNewTaskX1D");
    }
}; // class MKLConvTask

/// \brief MKL `VSLCorrTaskPtr`
/// \ingroup MKL
template <typename ResultType = double>
class MKLCorrTask : public MKLBase<VSLCorrTaskPtr, MKLCorrTask<ResultType>>
{
    public:
    typedef ResultType result_type;

    MKLCorrTask()
    {
        VSMC_STATIC_ASSERT_UTILITY_MKL_CORR_TASK_RESULT_TYPE(ResultType);
    }

    /// \brief `vslCorrNewTask`
    MKLCorrTask(MKL_INT mode, MKL_INT dims, const MKL_INT *xshape,
        const MKL_INT *yshape, const MKL_INT *zshape)
    {
        VSMC_STATIC_ASSERT_UTILITY_MKL_CORR_TASK_RESULT_TYPE(ResultType);
        reset(mode, dims, xshape, yshape, zshape);
    }

    /// \brief `vslCorrNewTask1D`
    MKLCorrTask(
        MKL_INT mode, const MKL_INT xshape, MKL_INT yshape, MKL_INT zshape)
    {
        VSMC_STATIC_ASSERT_UTILITY_MKL_CORR_TASK_RESULT_TYPE(ResultType);
        reset(mode, xshape, yshape, zshape);
    }

    /// \brief `vslCorrNewTaskX`
    MKLCorrTask(MKL_INT mode, MKL_INT dims, const MKL_INT *xshape,
        const MKL_INT *yshape, const MKL_INT *zshape, const result_type *x,
        const MKL_INT *xstride)
    {
        VSMC_STATIC_ASSERT_UTILITY_MKL_CORR_TASK_RESULT_TYPE(ResultType);
        reset(mode, dims, xshape, yshape, zshape, x, xstride);
    }

    /// \brief `vslCorrNewTaskX1D`
    MKLCorrTask(MKL_INT mode, MKL_INT xshape, MKL_INT yshape, MKL_INT zshape,
        const result_type *x, const MKL_INT xstride)
    {
        VSMC_STATIC_ASSERT_UTILITY_MKL_CORR_TASK_RESULT_TYPE(ResultType);
        reset(mode, xshape, yshape, zshape, x, xstride);
    }

    MKLCorrTask(const MKLCorrTask<ResultType> &other)
    {
        VSMC_STATIC_ASSERT_UTILITY_MKL_CORR_TASK_RESULT_TYPE(ResultType);

        VSLCorrTaskPtr ptr = nullptr;
        internal::mkl_error_check(::vslCorrCopyTask(&ptr, other.get()),
            "MKLCorrTask::MKLCorrTask", "::vslCorrCopyTask");
        this->reset(ptr);
    }

    MKLCorrTask<ResultType> &operator=(const MKLCorrTask<ResultType> &other)
    {
        if (this != &other) {
            VSLCorrTaskPtr ptr = nullptr;
            internal::mkl_error_check(::vslCorrCopyTask(&ptr, other.get()),
                "MKLCorrTask::operator=", "::vslCorrCopyTask");
            this->reset(ptr);
        }

        return *this;
    }

    MKLCorrTask(MKLCorrTask<ResultType> &&) = default;
    MKLCorrTask<ResultType> &operator=(MKLCorrTask<ResultType> &&) = default;

    static int release(VSLCorrTaskPtr ptr)
    {
        int status = ::vslCorrDeleteTask(&ptr);
        internal::mkl_error_check(
            status, "MKLCorrTask::release", "::vslCorrDeleteTask");

        return status;
    }

    /// \brief `vslCorrNewTask`
    int reset(MKL_INT mode, MKL_INT dims, const MKL_INT *xshape,
        const MKL_INT *yshape, const MKL_INT *zshape)
    {
        VSLCorrTaskPtr ptr = nullptr;
        int status = reset_dispatch(&ptr, mode, dims, xshape, yshape, zshape);
        this->reset(ptr);

        return status;
    }

    /// \brief `vslCorrNewTask1D`
    int reset(
        MKL_INT mode, const MKL_INT xshape, MKL_INT yshape, MKL_INT zshape)
    {
        VSLCorrTaskPtr ptr = nullptr;
        int status = reset_dispatch(&ptr, mode, xshape, yshape, zshape);
        this->reset(ptr);

        return status;
    }

    /// \brief `vslCorrNewTaskX`
    int reset(MKL_INT mode, MKL_INT dims, const MKL_INT *xshape,
        const MKL_INT *yshape, const MKL_INT *zshape, const result_type *x,
        const MKL_INT *xstride)
    {
        VSLCorrTaskPtr ptr = nullptr;
        int status = reset_dispatch(
            &ptr, mode, dims, xshape, yshape, zshape, x, xstride);
        this->reset(ptr);

        return status;
    }

    /// \brief `vslCorrNewTaskX1D`
    int reset(MKL_INT mode, MKL_INT xshape, MKL_INT yshape, MKL_INT zshape,
        const result_type *x, const MKL_INT xstride)
    {
        VSLCorrTaskPtr ptr = nullptr;
        int status =
            reset_dispatch(&ptr, mode, xshape, yshape, zshape, x, xstride);
        this->reset(ptr);

        return status;
    }

    private:
    static int reset_dispatch(VSLCorrTaskPtr *task, const MKL_INT mode,
        MKL_INT dims, const MKL_INT *xshape, const MKL_INT *yshape,
        const MKL_INT *zshape, float *)
    {
        internal::mkl_error_check(
            ::vslsCorrNewTask(task, mode, dims, xshape, yshape, zshape),
            "MKLCorrTask::reset", "::vslsCorrNewTask");
    }

    static int reset_dispatch(VSLCorrTaskPtr *task, const MKL_INT mode,
        MKL_INT dims, const MKL_INT *xshape, const MKL_INT *yshape,
        const MKL_INT *zshape, double *)
    {
        internal::mkl_error_check(
            ::vsldCorrNewTask(task, mode, dims, xshape, yshape, zshape),
            "MKLCorrTask::reset", "::vsldCorrNewTask");
    }

    static int reset_dispatch(VSLCorrTaskPtr *task, const MKL_INT mode,
        MKL_INT dims, const MKL_INT *xshape, const MKL_INT *yshape,
        const MKL_INT *zshape, MKL_Complex8 *)
    {
        internal::mkl_error_check(
            ::vslcCorrNewTask(task, mode, dims, xshape, yshape, zshape),
            "MKLCorrTask::reset", "::vslcCorrNewTask");
    }

    static int reset_dispatch(VSLCorrTaskPtr *task, const MKL_INT mode,
        MKL_INT dims, const MKL_INT *xshape, const MKL_INT *yshape,
        const MKL_INT *zshape, MKL_Complex16 *)
    {
        internal::mkl_error_check(
            ::vslzCorrNewTask(task, mode, dims, xshape, yshape, zshape),
            "MKLCorrTask::reset", "::vslzCorrNewTask");
    }

    static int reset_dispatch(VSLCorrTaskPtr *task, const MKL_INT mode,
        const MKL_INT xshape, const MKL_INT yshape, const MKL_INT zshape,
        float *)
    {
        internal::mkl_error_check(
            ::vslsCorrNewTask1D(task, mode, xshape, yshape, zshape),
            "MKLCorrTask::reset", "::vslsCorrNewTask1D");
    }

    static int reset_dispatch(VSLCorrTaskPtr *task, const MKL_INT mode,
        const MKL_INT xshape, const MKL_INT yshape, const MKL_INT zshape,
        double *)
    {
        internal::mkl_error_check(
            ::vsldCorrNewTask1D(task, mode, xshape, yshape, zshape),
            "MKLCorrTask::reset", "::vsldCorrNewTask1D");
    }

    static int reset_dispatch(VSLCorrTaskPtr *task, const MKL_INT mode,
        const MKL_INT xshape, const MKL_INT yshape, const MKL_INT zshape,
        MKL_Complex8 *)
    {
        internal::mkl_error_check(
            ::vslcCorrNewTask1D(task, mode, xshape, yshape, zshape),
            "MKLCorrTask::reset", "::vslcCorrNewTask1D");
    }

    static int reset_dispatch(VSLCorrTaskPtr *task, const MKL_INT mode,
        const MKL_INT xshape, const MKL_INT yshape, const MKL_INT zshape,
        MKL_Complex16 *)
    {
        internal::mkl_error_check(
            ::vslzCorrNewTask1D(task, mode, xshape, yshape, zshape),
            "MKLCorrTask::reset", "::vslzCorrNewTask1D");
    }

    static int reset_dispatch(VSLCorrTaskPtr *task, const MKL_INT mode,
        MKL_INT dims, const MKL_INT *xshape, const MKL_INT *yshape,
        const MKL_INT *zshape, const float *x, const MKL_INT *xstride)
    {
        internal::mkl_error_check(::vslsCorrNewTaskX(task, mode, dims, xshape,
                                      yshape, zshape, x, xstride),
            "MKLCorrTask::reset", "::vslsCorrNewTaskX");
    }

    static int reset_dispatch(VSLCorrTaskPtr *task, const MKL_INT mode,
        MKL_INT dims, const MKL_INT *xshape, const MKL_INT *yshape,
        const MKL_INT *zshape, const double *x, const MKL_INT *xstride)
    {
        internal::mkl_error_check(::vsldCorrNewTaskX(task, mode, dims, xshape,
                                      yshape, zshape, x, xstride),
            "MKLCorrTask::reset", "::vsldCorrNewTaskX");
    }

    static int reset_dispatch(VSLCorrTaskPtr *task, const MKL_INT mode,
        MKL_INT dims, const MKL_INT *xshape, const MKL_INT *yshape,
        const MKL_INT *zshape, const MKL_Complex8 *x, const MKL_INT *xstride)
    {
        internal::mkl_error_check(::vslcCorrNewTaskX(task, mode, dims, xshape,
                                      yshape, zshape, x, xstride),
            "MKLCorrTask::reset", "::vslcCorrNewTaskX");
    }

    static int reset_dispatch(VSLCorrTaskPtr *task, const MKL_INT mode,
        MKL_INT dims, const MKL_INT *xshape, const MKL_INT *yshape,
        const MKL_INT *zshape, const MKL_Complex16 *x, const MKL_INT *xstride)
    {
        internal::mkl_error_check(::vslzCorrNewTaskX(task, mode, dims, xshape,
                                      yshape, zshape, x, xstride),
            "MKLCorrTask::reset", "::vslzCorrNewTaskX");
    }

    static int reset_dispatch(VSLCorrTaskPtr *task, const MKL_INT mode,
        const MKL_INT xshape, const MKL_INT yshape, const MKL_INT zshape,
        const float *x, const MKL_INT xstride)
    {
        internal::mkl_error_check(::vslsCorrNewTaskX1D(task, mode, xshape,
                                      yshape, zshape, x, xstride),
            "MKLCorrTask::reset", "::vslsCorrNewTaskX1D");
    }

    static int reset_dispatch(VSLCorrTaskPtr *task, const MKL_INT mode,
        const MKL_INT xshape, const MKL_INT yshape, const MKL_INT zshape,
        const double *x, const MKL_INT xstride)
    {
        internal::mkl_error_check(::vsldCorrNewTaskX1D(task, mode, xshape,
                                      yshape, zshape, x, xstride),
            "MKLCorrTask::reset", "::vsldCorrNewTaskX1D");
    }

    static int reset_dispatch(VSLCorrTaskPtr *task, const MKL_INT mode,
        const MKL_INT xshape, const MKL_INT yshape, const MKL_INT zshape,
        const MKL_Complex8 *x, const MKL_INT xstride)
    {
        internal::mkl_error_check(::vslcCorrNewTaskX1D(task, mode, xshape,
                                      yshape, zshape, x, xstride),
            "MKLCorrTask::reset", "::vslcCorrNewTaskX1D");
    }

    static int reset_dispatch(VSLCorrTaskPtr *task, const MKL_INT mode,
        const MKL_INT xshape, const MKL_INT yshape, const MKL_INT zshape,
        const MKL_Complex16 *x, const MKL_INT xstride)
    {
        internal::mkl_error_check(::vslzCorrNewTaskX1D(task, mode, xshape,
                                      yshape, zshape, x, xstride),
            "MKLCorrTask::reset", "::vslzCorrNewTaskX1D");
    }
}; // class MKLCorrTask

/// \brief MKL `DFTaskPtr`
/// \ingroup MKL
template <typename ResultType = double>
class MKLDFTask
{
    public:
    typedef ResultType result_type;

    MKLDFTask()
    {
        VSMC_STATIC_ASSERT_UTILITY_MKL_DF_TASK_RESULT_TYPE(ResultType);
    }

    MKLDFTask(MKL_INT nx, const result_type *x, MKL_INT xhint, MKL_INT ny,
        const result_type *y, MKL_INT yhint)
    {
        VSMC_STATIC_ASSERT_UTILITY_MKL_DF_TASK_RESULT_TYPE(ResultType);
        reset(nx, x, xhint, ny, y, yhint);
    }

    static int release(DFTaskPtr ptr)
    {
        int status = ::dfDeleteTask(&ptr);
        internal::mkl_error_check(
            status, "MKLDFTask::release", "::dfDeleteTask");

        return status;
    }

    int reset(MKL_INT nx, const result_type *x, MKL_INT xhint, MKL_INT ny,
        const result_type *y, MKL_INT yhint)
    {
        DFTaskPtr ptr = nullptr;
        int status = reset_dispatch(&ptr, nx, x, xhint, ny, y, yhint);
        this->reset(ptr);

        return status;
    }

    private:
    static int reset_dispatch(DFTaskPtr *task, MKL_INT nx, const float *x,
        MKL_INT xhint, MKL_INT ny, const float *y, MKL_INT yhint)
    {
        int status = ::dfsNewTask1D(task, nx, x, xhint, ny, y, yhint);
        internal::mkl_error_check(
            status, "MKLDFTask::reset", "::dfsNewTask1D");

        return status;
    }

    static int reset_dispatch(DFTaskPtr *task, MKL_INT nx, const double *x,
        MKL_INT xhint, MKL_INT ny, const double *y, MKL_INT yhint)
    {
        int status = ::dfdNewTask1D(task, nx, x, xhint, ny, y, yhint);
        internal::mkl_error_check(
            status, "MKLDFTask::reset", "::dfdNewTask1D");
    }
}; // class MKLDFTask

} // namespace vsmc

#endif // VSMC_UTILITY_MKL_HPP