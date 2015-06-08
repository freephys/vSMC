//============================================================================
// vSMC/include/vsmc/utility/aligned_memory.hpp
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

#ifndef VSMC_UTILITY_ALIGNED_MEMORY
#define VSMC_UTILITY_ALIGNED_MEMORY

#include <vsmc/internal/config.hpp>
#include <vsmc/internal/assert.hpp>

#include <cstddef>
#include <cstdlib>
#include <memory>
#include <new>
#include <type_traits>
#include <vector>

#if VSMC_HAS_POSIX
#include <stdlib.h>
#elif defined(VSMC_MSVC)
#include <malloc.h>
#endif

#if VSMC_HAS_TBB_MALLOC
#include <tbb/scalable_allocator.h>
#endif

#if VSMC_HAS_MKL
#include <mkl_service.h>
#endif

/// \brief Default AlignedMemory type
/// \ingroup Config
#ifndef VSMC_ALIGNED_MEMORY_TYPE
#if VSMC_HAS_TBB_MALLOC
#define VSMC_ALIGNED_MEMORY_TYPE ::vsmc::AlignedMemoryTBB
#elif VSMC_HAS_MKL
#define VSMC_ALIGNED_MEMORY_TYPE ::vsmc::AlignedMemoryMKL
#elif VSMC_HAS_POSIX || defined(VSMC_MSVC)
#define VSMC_ALIGNED_MEMORY_TYPE ::vsmc::AlignedMemorySYS
#else
#define VSMC_ALIGNED_MEMORY_TYPE ::vsmc::AlignedMemorySTD
#endif
#endif

/// \brief Defualt alignment
/// \ingroup Config
#ifndef VSMC_ALIGNMENT
#define VSMC_ALIGNMENT 32
#endif

#define VSMC_STATIC_ASSERT_UTILITY_ALIGNED_MEMORY_POWER_OF_TWO(Alignment)     \
    VSMC_STATIC_ASSERT(                                                       \
        (Alignment != 0 && (Alignment & (Alignment - 1)) == 0),               \
        "**AlignedAllocator** USED WITH Alignment NOT A POWER OF TWO")

#define VSMC_STATIC_ASSERT_UTILITY_ALIGNED_MEMORY_SIZEOF_VOID(Alignemnt)      \
    VSMC_STATIC_ASSERT((Alignment >= sizeof(void *)),                         \
        "**AlginedAllocator** USED WITH Alignment LESS THAN sizeof(void *)")

#define VSMC_STATIC_ASSERT_UTILITY_ALIGNED_MEMORY                             \
    VSMC_STATIC_ASSERT_UTILITY_ALIGNED_MEMORY_POWER_OF_TWO(Alignment);        \
    VSMC_STATIC_ASSERT_UTILITY_ALIGNED_MEMORY_SIZEOF_VOID(Alignment);

#define VSMC_RUNTIME_ASSERT_UTILITY_ALIGNED_MEMORY_POWER_OF_TWO(alignment)    \
    VSMC_RUNTIME_ASSERT(                                                      \
        (alignment != 0 && (alignment & (alignment - 1)) == 0),               \
        "**aligned_malloc** USED WITH ALIGNMENT NOT A POWER OF TWO")

#define VSMC_RUNTIME_ASSERT_UTILITY_ALIGNED_MEMORY_SIZEOF_VOID(alignemnt)     \
    VSMC_RUNTIME_ASSERT((alignment >= sizeof(void *)),                        \
        "**aligned_malloc** USED WITH ALIGNMENT LESS THAN sizeof(void *)")

#define VSMC_RUNTIME_ASSERT_UTILITY_ALIGNED_MEMORY                            \
    VSMC_RUNTIME_ASSERT_UTILITY_ALIGNED_MEMORY_POWER_OF_TWO(alignment);       \
    VSMC_RUNTIME_ASSERT_UTILITY_ALIGNED_MEMORY_SIZEOF_VOID(alignment);

namespace vsmc
{

/// \brief Aligned memory using `std::malloc` and `std::free`
/// \ingroup AlignedMemory
///
/// \details
/// Memory allocated through this class is aligned but some bytes might be
/// wasted in each allocation.
class AlignedMemorySTD
{
    public:
    static void *aligned_malloc(std::size_t n, std::size_t alignment)
    {
        VSMC_RUNTIME_ASSERT_UTILITY_ALIGNED_MEMORY;

        if (n == 0)
            return nullptr;

        void *orig_ptr = std::malloc(n + alignment + sizeof(void *));
        if (orig_ptr == nullptr)
            throw std::bad_alloc();

        uintptr_t address = reinterpret_cast<uintptr_t>(orig_ptr);
        uintptr_t offset = alignment - (address + sizeof(void *)) % alignment;
        void *ptr =
            reinterpret_cast<void *>(address + offset + sizeof(void *));
        void **orig = reinterpret_cast<void **>(address + offset);
        *orig = orig_ptr;

        return ptr;
    }

    static void aligned_free(void *ptr)
    {
        std::free(*reinterpret_cast<void **>(
                      reinterpret_cast<uintptr_t>(ptr) - sizeof(void *)));
    }
}; // class AlignedMemmorySTD

#if VSMC_HAS_POSIX

/// \brief Aligned memory using native system aligned memory allocation
/// \ingroup AlignedMemory
///
/// \details
/// This class use `posix_memalign` and `free` on POSIX systems (Mac OS X,
/// Linux, etc.) and `_aligned_malloc` and `_aligned_free` on Windows.
class AlignedMemorySYS
{
    public:
    static void *aligned_malloc(std::size_t n, std::size_t alignment)
    {
        VSMC_RUNTIME_ASSERT_UTILITY_ALIGNED_MEMORY;

        if (n == 0)
            return nullptr;

        void *ptr;
        if (posix_memalign(&ptr, alignment, n) != 0)
            throw std::bad_alloc();

        return ptr;
    }

    static void aligned_free(void *ptr) { free(ptr); }
}; // class AlignedMallocSYS

#elif defined(VSMC_MSVC)

class AlignedMemorySYS
{
    public:
    static void *aligned_malloc(std::size_t n, std::size_t alignment)
    {
        VSMC_RUNTIME_ASSERT_UTILITY_ALIGNED_MEMORY;

        if (n == 0)
            return nullptr;

        void *ptr = _aligned_malloc(n, alignment);
        if (ptr == nullptr)
            throw std::bad_alloc();

        return ptr;
    }

    static void aligned_free(void *ptr) { _aligned_free(ptr); }
}; // class AlignedMemorySYS

#endif // VSMC_HAS_POSIX

#if VSMC_HAS_TBB_MALLOC

/// \brief Aligned memory using Intel TBB `scalable_aligned_malloc` and
/// `scalable_aligned_free`.
/// \ingroup AlignedMemory
class AlignedMemoryTBB
{
    public:
    static void *aligned_malloc(std::size_t n, std::size_t alignment)
    {
        VSMC_RUNTIME_ASSERT_UTILITY_ALIGNED_MEMORY;

        if (n == 0)
            return nullptr;

        void *ptr = scalable_aligned_malloc(n, alignment);
        if (ptr == nullptr)
            throw std::bad_alloc();

        return ptr;
    }

    static void aligned_free(void *ptr) { scalable_aligned_free(ptr); }
}; // class AlignedMemoryTBB

#endif // VSMC_HAS_TBB_MALLOC

#if VSMC_HAS_MKL

/// \brief Aligned memory using Intel MKL `mkl_malloc` and `mkl_free`
/// \ingroup AlignedMemory
class AlignedMemoryMKL
{
    public:
    static void *aligned_malloc(std::size_t n, std::size_t alignment)
    {
        VSMC_RUNTIME_ASSERT_UTILITY_ALIGNED_MEMORY;

        if (n == 0)
            return nullptr;

        void *ptr = mkl_malloc(n, static_cast<int>(alignment));
        if (ptr == nullptr)
            throw std::bad_alloc();

        return ptr;
    }

    static void aligned_free(void *ptr) { mkl_free(ptr); }
}; // class AlignedMemoryMKL

#endif // VSMC_HAS_MKL

/// \brief Default AlignedMemory type
/// \ingroup AlignedMemory
using AlignedMemory = VSMC_ALIGNED_MEMORY_TYPE;

/// \brief Aligned allocator
/// \ingroup AlignedMemory
///
/// \tparam T The value type
/// \tparam Alignment The alignment requirement of memory, must be a power of
/// two and no less than `sizeof(void *)`.
/// \tparam Memory The memory management class. Must provides two member
/// functions, `aligned_malloc` and `aligned_free`. The member function
/// `aligned_malloc` shall behave similar to `std::malloc` but take an
/// additional arguments for alignment. The member function `aligned_free`
/// shall behave just like `std::free`.
template <typename T, std::size_t Alignment = VSMC_ALIGNMENT,
    typename Memory = AlignedMemory>
class AlignedAllocator : public std::allocator<T>
{
    public:
    using size_type = typename std::allocator<T>::size_type;
    using pointer = typename std::allocator<T>::pointer;

    template <typename U>
    class rebind
    {
        public:
        using other = AlignedAllocator<U, Alignment>;
    }; // class rebind

    AlignedAllocator() { VSMC_STATIC_ASSERT_UTILITY_ALIGNED_MEMORY; }

    AlignedAllocator(const AlignedAllocator<T, Alignment, Memory> &other)
        : std::allocator<T>(other)
    {
        VSMC_STATIC_ASSERT_UTILITY_ALIGNED_MEMORY;
    }

    template <typename U>
    AlignedAllocator(const AlignedAllocator<U, Alignment, Memory> &other)
        : std::allocator<T>(static_cast<std::allocator<U>>(other))
    {
        VSMC_STATIC_ASSERT_UTILITY_ALIGNED_MEMORY;
    }

    AlignedAllocator<T, Alignment, Memory> &operator=(
        const AlignedAllocator<T, Alignment, Memory> &) = default;

    AlignedAllocator(AlignedAllocator<T, Alignment, Memory> &&other)
        : std::allocator<T>(std::move(other))
    {
        VSMC_STATIC_ASSERT_UTILITY_ALIGNED_MEMORY;
    }

    template <typename U>
    AlignedAllocator(AlignedAllocator<U, Alignment, Memory> &&other)
        : std::allocator<T>(static_cast<std::allocator<U>>(std::move(other)))
    {
        VSMC_STATIC_ASSERT_UTILITY_ALIGNED_MEMORY;
    }

    AlignedAllocator<T, Alignment, Memory> &operator=(
        AlignedAllocator<T, Alignment, Memory> &&) = default;

    ~AlignedAllocator() {}

    pointer allocate(size_type n, const void * = nullptr)
    {
        if (n == 0)
            return nullptr;

        return static_cast<pointer>(
            memory_.aligned_malloc(sizeof(T) * n, Alignment));
    }

    void deallocate(pointer ptr, size_type)
    {
        if (ptr != nullptr)
            memory_.aligned_free(ptr);
    }

    private:
    Memory memory_;
}; // class AlignedAllocator

/// \brief AlignedAllocator for scalar type and `std::allocator` for others
/// \ingroup AlignedMemory
template <typename T>
using Allocator = typename std::conditional<std::is_scalar<T>::value,
    AlignedAllocator<T>, std::allocator<T>>::type;

/// \brief Vector type using AlignedAllocator
/// \ingroup AlignedMemory
template <typename T>
using AlignedVector = std::vector<T, AlignedAllocator<T>>;

/// \brief AlignedVector for scalar type and `std::vector` for others
/// \ingroup AlignedMemory
template <typename T>
using Vector = typename std::conditional<std::is_scalar<T>::value,
    std::vector<T, AlignedAllocator<T>>, std::vector<T>>::type;

} // namespace vsmc

#endif // VSMC_UTILITY_ALIGNED_MEMORY
