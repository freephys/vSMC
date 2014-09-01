//============================================================================
// include/vsmc/thread/thread_guard.hpp
//----------------------------------------------------------------------------
//
//                         vSMC: Scalable Monte Carlo
//
// This file is distribured under the 2-clauses BSD License.
// See LICENSE for details.
//============================================================================

#ifndef VSMC_THREAD_THREAD_GUARD_HPP
#define VSMC_THREAD_THREAD_GUARD_HPP

#include <vsmc/internal/common.hpp>

namespace vsmc {

/// \brief Strictly scope-based thread ownership wrapper
/// \ingroup Thread
template <typename ThreadType>
class ThreadGuard
{
    public :

    typedef ThreadType thread_type;

    ThreadGuard () noexcept {}

    ThreadGuard (const ThreadGuard &) = delete;

    ThreadGuard &operator= (const ThreadGuard &) = delete;

    ThreadGuard (thread_type &&thr) noexcept :
        thread_(cxx11::move(thr)) {}

    ThreadGuard (ThreadGuard &&other) noexcept :
        thread_(cxx11::move(other.thread_)) {}

    ThreadGuard &operator= (ThreadGuard &&other) noexcept
    {thread_ = cxx11::move(other.thread_); return *this;}

    ~ThreadGuard () noexcept {if (thread_.joinable()) thread_.join();}

    private :

    thread_type thread_;
}; // class ThreadGuard

} // namespace vsmc

#endif // VSMC_THREAD_THREAD_GUARD_HPP