#ifndef VSMC_UTILITY_STDTBB_HPP
#define VSMC_UTILITY_STDTBB_HPP

#include <atomic>
#include <cstdlib>
#include <functional>
#include <memory>
#include <numeric>
#include <utility>
#include <thread>
#include <vector>

#include <vsmc/internal/config.hpp>
#include <vsmc/internal/assert.hpp>
#include <vsmc/internal/defines.hpp>
#include <vsmc/internal/forward.hpp>

#if VSMC_HAS_CXX11LIB_FUTURE
#include <future>
#endif

namespace vsmc { namespace thread {

/// \brief Blocked range
/// \ingroup STDTBB
template <typename SizeType>
class BlockedRange
{
    public :

    typedef SizeType size_type;

    BlockedRange (size_type begin, size_type end) : begin_(begin), end_(end)
    {
        VSMC_RUNTIME_ASSERT((begin < end), "INVALID RANG **BlockedRange**");
    }

    size_type begin () const
    {
        return begin_;
    }

    size_type end () const
    {
        return end_;
    }

    private :

    size_type begin_;
    size_type end_;
}; // class BlockedRange

/// \brief C++11 Thread safe lock-based stack
/// \ingroup STDTBB
template <typename T>
class LockBasedStack
{
}; // class LockBasedStack

/// \brief C++11 Thread safe lock-free stack
/// \ingroup STDTBB
template <typename T>
class LockFreeStack
{
}; // class LockFreeStack

/// \brief C++11 Thread safe lock-based queue
/// \ingroup STDTBB
template <typename T>
class LockBasedQueue
{
}; // class LockBasedQuque

/// \brief C++11 Thread safe lock-free queue
/// \ingroup STDTBB
template <typename T>
class LockFreeQueue
{
}; // class LockFreeQueue

/// \brief C++11 Thread guard
/// \ingroup STDTBB
class ThreadGuard
{
#if VSMC_HAS_CXX11_DELETED_FUNCTIONS
    public :

    ThreadGuard (const ThreadGuard &) = delete;
    ThreadGuard &operator= (const ThreadGuard &) = delete;
#else
    private :

    ThreadGuard (const ThreadGuard &);
    ThreadGuard &operator= (const ThreadGuard &);
#endif

    public :

    ThreadGuard () VSMC_NOEXCEPT {}

    ThreadGuard (ThreadGuard &&other) VSMC_NOEXCEPT :
        thread_(std::move(other.thread_)) {}

    ThreadGuard &operator= (ThreadGuard &&other) VSMC_NOEXCEPT
    {
        thread_ = std::move(other.thread_);

        return *this;
    }

    ThreadGuard (std::thread &&thr) VSMC_NOEXCEPT : thread_(std::move(thr)) {}

    ~ThreadGuard () VSMC_NOEXCEPT
    {
        if (thread_.joinable())
            thread_.join();
    }

    private :

    std::thread thread_;
}; // class ThreadGuard

/// \brief C++11 Thread informations
/// \ingroup STDTBB
class ThreadInfo
{
    public :

    static ThreadInfo &instance ()
    {
        static ThreadInfo info;

        return info;
    }

    unsigned thread_num () const
    {
        return thread_num_;
    }

    void thread_num (unsigned num)
    {
        thread_num_ = num;
    }

    template <typename SizeType>
    std::vector<BlockedRange<SizeType> > partition (
            const BlockedRange<SizeType> &range) const
    {
        SizeType N = range.end() - range.begin();
        SizeType tn = static_cast<SizeType>(thread_num());
        SizeType block_size =  0;

        if (N < tn)
            block_size = 1;
        else if (N % tn)
            block_size = N / tn + 1;
        else
            block_size = N / tn;

        std::vector<BlockedRange<SizeType> > range_vec;
        range_vec.reserve(thread_num());
        SizeType B = range.begin();
        while (N > 0) {
            SizeType next = std::min VSMC_MINMAX_NO_EXPANSION (
                    N, block_size);
            range_vec.push_back(BlockedRange<SizeType>(B, B + next));
            B += next;
            N -= next;
        }
        VSMC_RUNTIME_ASSERT((range_vec.back().end() == range.end()),
                "INVALID PARTITION **ThreadInfo::partition**");

        return range_vec;
    }

    private :

    unsigned thread_num_;

    ThreadInfo () : thread_num_(std::max VSMC_MINMAX_NO_EXPANSION (1U,
             static_cast<unsigned>(std::thread::hardware_concurrency())))
    {
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4996)
#endif
        const char *num_str = std::getenv("VSMC_STD_NUM_THREADS");
#ifdef _MSC_VER
#pragma warning(pop)
#endif
        if (num_str) {
            unsigned num = std::atoi(num_str);
            if (num)
                thread_num_ = num;
        }
    }

    ThreadInfo (const ThreadInfo &);
    ThreadInfo &operator= (const ThreadInfo &);
}; // class ThreadInfo

/// \brief C++11 Thread pool
/// \ingroup STDTBB
class ThreadPool
{
    public :

    static ThreadPool &instance ()
    {
        static ThreadPool pool;

        return pool;
    }

    void submit ();
    void barrier ();

    private :

    ThreadPool ();
    ThreadPool (const ThreadPool &);
    ThreadPool &operator= (const ThreadPool &);
}; // class ThreadPool

/// \brief Parallel for using C++11 thread
/// \ingroup STDTBB
///
/// \details
/// Requirement: WorkType:
/// void work (const thread::BlockedRange<size_type> &range);
template <typename SizeType, typename WorkType>
void parallel_for (const BlockedRange<SizeType> &range, WorkType &&work)
{
    std::vector<BlockedRange<SizeType> > range_vec(
            ThreadInfo::instance().partition(range));
#if VSMC_HAS_CXX11LIB_FUTURE
    std::vector<std::future<void> > wg;
    for (typename std::vector<BlockedRange<SizeType> >::iterator
            r = range_vec.begin(); r != range_vec.end(); ++r) {
        wg.push_back(std::async(std::forward<WorkType>(work), *r));
    }
    for (std::vector<std::future<void> >::iterator
            w = wg.begin(); w != wg.end(); ++w) { w->get(); }
#else
    // start parallelization
    {
        std::vector<ThreadGuard> tg;
        for (typename std::vector<BlockedRange<SizeType> >::iterator
                r = range_vec.begin(); r != range_vec.end(); ++r) {
            tg.push_back(ThreadGuard(std::thread(std::forward<WorkType>(work),
                            *r)));
        }
    }
    // stop parallelization
    // for (typename std::vector<BlockedRange<SizeType> >::iterator
    //         r = range_vec.begin(); r != range_vec.end(); ++r) {
    //     ThreadPool::instance().submit(std::forward<WorkType>(work), *r);
    // }
    // ThreadPool::instance().barrier();
#endif
}

/// \brief Parallel accumulate using C++11 thread
/// \ingroup STDTBB
///
/// \details
/// Requirement: WorkType:
/// void work (const thread::BlockedRange<size_type> &range, T &res);
template <typename SizeType, typename T, typename WorkType>
T parallel_accumulate (const BlockedRange<SizeType> &range, WorkType &&work,
        T init)
{
    std::vector<BlockedRange<SizeType> > range_vec(
            ThreadInfo::instance().partition(range));
    std::vector<T> result(range_vec.size());
    unsigned i = 0;
#if VSMC_HAS_CXX11LIB_FUTURE
    std::vector<std::future<void> > wg;
    for (typename std::vector<BlockedRange<SizeType> >::iterator
            r = range_vec.begin(); r != range_vec.end(); ++r, ++i) {
        wg.push_back(std::async(std::forward<WorkType>(work),
                    *r, std::ref(result[i])));
    }
    for (std::vector<std::future<void> >::iterator
            w = wg.begin(); w != wg.end(); ++w) { w->get(); }
#else
    // start parallelization
    {
        std::vector<ThreadGuard> tg;
        for (typename std::vector<BlockedRange<SizeType> >::iterator
                r = range_vec.begin(); r != range_vec.end(); ++r, ++i) {
            tg.push_back(ThreadGuard(std::thread(std::forward<WorkType>(work),
                            *r, std::ref(result[i]))));
        }
    }
    // stop parallelization
    // for (typename std::vector<BlockedRange<SizeType> >::iterator
    //         r = range_vec.begin(); r != range_vec.end(); ++r) {
    //     ThreadPool::instance().submit(std::forward<WorkType>(work), *r,
    //             std::ref(result[i]));
    // }
    // ThreadPool::instance().barrier();
#endif

    return std::accumulate(result.begin(), result.end(), init);
}

/// \brief Parallel accumulate using C++11 thread
/// \ingroup STDTBB
///
/// \details
/// Requirement: WorkType:
/// void work (const thread::BlockedRange<size_type> &range, T &res);
template <typename SizeType, typename T, typename BinOp, typename WorkType>
T parallel_accumulate (const BlockedRange<SizeType> &range, WorkType &&work,
        T init, BinOp bin_op)
{
    std::vector<BlockedRange<SizeType> > range_vec(
            ThreadInfo::instance().partition(range));
    std::vector<T> result(range_vec.size());
    unsigned i =0;
#if VSMC_HAS_CXX11LIB_FUTURE
    std::vector<std::future<void> > wg;
    for (typename std::vector<BlockedRange<SizeType> >::iterator
            r = range_vec.begin(); r != range_vec.end(); ++r, ++i) {
        wg.push_back(std::async(std::forward<WorkType>(work),
                    *r, std::ref(result[i])));
    }
    for (std::vector<std::future<void> >::iterator
            w = wg.begin(); w != wg.end(); ++w) { w->get(); }
#else
    // start parallelization
    {
        std::vector<ThreadGuard> tg;
        for (typename std::vector<BlockedRange<SizeType> >::iterator
                r = range_vec.begin(); r != range_vec.end(); ++r, ++i) {
            tg.push_back(ThreadGuard(std::thread(std::forward<WorkType>(work),
                            *r, std::ref(result[i]))));
        }
    }
    // stop parallelization
    // for (typename std::vector<BlockedRange<SizeType> >::iterator
    //         r = range_vec.begin(); r != range_vec.end(); ++r) {
    //     ThreadPool::instance().submit(std::forward<WorkType>(work), *r,
    //             std::ref(result[i]));
    // }
    // ThreadPool::instance().barrier();
#endif

    return std::accumulate(result.begin(), result.end(), init, bin_op);
}

} } // namespace vsmc::thread

#endif // VSMC_UTILITY_STDTBB_HPP
