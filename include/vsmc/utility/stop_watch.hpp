#ifndef VSMC_UTILITY_STOP_WATCH_HPP
#define VSMC_UTILITY_STOP_WATCH_HPP

#include <vsmc/internal/config.hpp>
#include <vsmc/internal/assert.hpp>
#include <vsmc/internal/defines.hpp>
#include <vsmc/internal/forward.hpp>

#if VSMC_HAS_CXX11LIB_CHRONO
#define VSMC_STOP_WATCH_DEFINED

#include <chrono>

namespace vsmc {

template <typename ClockType>
class StopWatchChrono
{
    public :

    StopWatchChrono () : elapsed_(0) {}

    void start () const
    {
        start_time_ = ClockType::now();
    }

    void stop () const
    {
        typename ClockType::time_point stop_time = ClockType::now();
        elapsed_ += stop_time - start_time_;
    }

    void reset () const
    {
        elapsed_ = ClockType::duration(0);
    }

    double nanoseconds () const
    {
        return std::chrono::duration_cast<std::chrono::duration<
            double, std::nano> >(elapsed_).count();
    }

    double microseconds () const
    {
        return std::chrono::duration_cast<std::chrono::duration<
            double, std::micro> >(elapsed_).count();
    }

    double milliseconds () const
    {
        return std::chrono::duration_cast<std::chrono::duration<
            double, std::milli> >(elapsed_).count();
    }

    double seconds () const
    {
        return std::chrono::duration_cast<std::chrono::duration<
            double, std::ratio<1> > >(elapsed_).count();
    }

    double minutes () const
    {
        return std::chrono::duration_cast<std::chrono::duration<
            double, std::ratio<60> > >(elapsed_).count();
    }

    double hours () const
    {
        return std::chrono::duration_cast<std::chrono::duration<
            double, std::ratio<3600> > >(elapsed_).count();
    }

    private :

    mutable typename ClockType::duration elapsed_;
    mutable typename ClockType::time_point start_time_;
}; // class StopWatch

typedef StopWatchChrono<std::chrono::high_resolution_clock> StopWatch;

} // namespace vsmc

#elif defined(__APPLE__) || defined(__MACOSX)
#define VSMC_STOP_WATCH_DEFINED

#include <mach/mach_time.h>

namespace vsmc {

class StopWatch
{
    public :

    StopWatch ()
    {
        reset();
    }

    void start () const
    {
        start_time_ = mach_absolute_time();
    }

    void stop () const
    {
        uint64_t stop_time = mach_absolute_time();
        uint64_t elapsed_abs = stop_time - start_time_;
        uint64_t elapsed_nano = elapsed_abs *
            timebase_.numer / timebase_.denom;
        uint64_t sec = elapsed_nano / ratio_;
        elapsed_sec_ += sec;
        elapsed_nsec_ += elapsed_nano - sec * ratio_;
    }

    void reset () const
    {
        elapsed_sec_ = 0;
        elapsed_nsec_ = 0;
        mach_timebase_info(&timebase_);
    }

    double nanoseconds () const
    {
        return static_cast<double>(elapsed_sec_) * 1e9 +
            static_cast<double>(elapsed_nsec_);
    }

    double microseconds () const
    {
        return static_cast<double>(elapsed_sec_) * 1e6 +
            static_cast<double>(elapsed_nsec_) / 1e3;
    }

    double milliseconds () const
    {
        return static_cast<double>(elapsed_sec_) * 1e3 +
            static_cast<double>(elapsed_nsec_) / 1e6;
    }

    double seconds () const
    {
        return static_cast<double>(elapsed_sec_) +
            static_cast<double>(elapsed_nsec_) / 1e9;
    }

    double minutes () const
    {
        return static_cast<double>(elapsed_sec_) / 60.0+
            static_cast<double>(elapsed_nsec_) / 1e9 / 60.0;
    }

    double hours () const
    {
        return static_cast<double>(elapsed_sec_) / 3600.0+
            static_cast<double>(elapsed_nsec_) / 1e9 / 3600.0;
    }

    private :

    mutable uint64_t elapsed_sec_;
    mutable uint64_t elapsed_nsec_;
    mutable uint64_t start_time_;
    mutable mach_timebase_info_data_t timebase_;
    static const uint64_t ratio_ = 1000000000L; // 9 zero
}; // class StopWatch

} // namespace vsmc

#elif defined(__linux__)

#include <time.h>
#include <features.h>

#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 199309L
#define VSMC_STOP_WATCH_DEFINED

namespace vsmc {

class StopWatch
{
    public :

    StopWatch ()
    {
        reset();
    }

    void start () const
    {
        clock_gettime(CLOCK_REALTIME, &start_time_);
    }

    void stop () const
    {
        timespec stop_time;
        clock_gettime(CLOCK_REALTIME, &stop_time);
        time_t sec = stop_time.tv_sec - start_time_.tv_sec;
        long nsec = stop_time.tv_nsec - start_time_.tv_nsec;

        elapsed_.tv_sec += sec;
        elapsed_.tv_nsec += nsec;
        long inc_sec = elapsed_.tv_nsec / ratio_;
        elapsed_.tv_sec += static_cast<time_t>(inc_sec);
        elapsed_.tv_nsec -= inc_sec * ratio_;
    }

    void reset () const
    {
        elapsed_.tv_sec = 0;
        elapsed_.tv_nsec = 0;
    }

    double nanoseconds () const
    {
        return static_cast<double>(elapsed_.tv_sec) * 1e9 +
            static_cast<double>(elapsed_.tv_nsec);
    }

    double microseconds () const
    {
        return static_cast<double>(elapsed_.tv_sec) * 1e6 +
            static_cast<double>(elapsed_.tv_nsec) / 1e3;
    }

    double milliseconds () const
    {
        return static_cast<double>(elapsed_.tv_sec) * 1e3 +
            static_cast<double>(elapsed_.tv_nsec) / 1e6;
    }

    double seconds () const
    {
        return static_cast<double>(elapsed_.tv_sec) +
            static_cast<double>(elapsed_.tv_nsec) / 1e9;
    }

    double minutes () const
    {
        return static_cast<double>(elapsed_.tv_sec) / 60.0+
            static_cast<double>(elapsed_.tv_nsec) / 1e9 / 60.0;
    }

    double hours () const
    {
        return static_cast<double>(elapsed_.tv_sec) / 3600.0+
            static_cast<double>(elapsed_.tv_nsec) / 1e9 / 3600.0;
    }

    private :

    mutable timespec elapsed_;
    mutable timespec start_time_;
    static const long ratio_ = 1000000000L; // 9 zero
}; // class StopWatch

} // namespace vsmc

#endif // _POSIX_C_SOURCE

#endif // VSMC_HAS_CXX11LIB_CHRONO

#ifndef VSMC_STOP_WATCH_DEFINED

namespace vsmc {

class StopWatch
{
    public :

    void start () const {}
    void stop () const {}
    void reset () const {}

    double nanoseconds () const { return 0; }
    double microseconds () const { return 0; }
    double milliseconds () const { return 0; }
    double seconds () const { return 0; }
    double minutes () const { return 0; }
    double hours () const { return 0; }
}; // class StopWatch

} // namespace vsmc

#endif // VSMC_STOP_WATCH_DEFINED

#ifdef VSMC_STOP_WATCH_DEFINED
#undef VSMC_STOP_WATCH_DEFINED
#endif

#endif // VSMC_UTILITY_STOP_WATCH_HPP
