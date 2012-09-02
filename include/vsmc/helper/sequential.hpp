#ifndef VSMC_HELPER_SEQUENTIAL_HPP
#define VSMC_HELPER_SEQUENTIAL_HPP

#include <vsmc/internal/common.hpp>
#include <vsmc/helper/base.hpp>

/// \defgroup Sequential Sequential
/// \ingroup Helper
/// \brief Single threaded sampler

namespace vsmc {

/// \brief Particle::value_type subtype
/// \ingroup Sequential
///
/// \tparam Dim The dimension of the state parameter vector
/// \tparam T The type of the value of the state parameter vector
template <unsigned Dim, typename T, typename Timer>
class StateSeq : public StateBase<Dim, T, Timer>
{
    public :

    typedef StateBase<Dim, T, Timer> state_base_type;
    typedef typename state_base_type::size_type  size_type;
    typedef typename state_base_type::state_type state_type;
    typedef typename state_base_type::timer_type timer_type;

    explicit StateSeq (size_type N) : StateBase<Dim, T, Timer>(N), size_(N) {}

    template <typename IntType>
    void copy (const IntType *copy_from)
    {
        for (size_type to = 0; to != size_; ++to)
            this->copy_particle(copy_from[to], to);
    }

    private :

    size_type size_;
}; // class StateSeq

/// \brief Sampler<T>::init_type subtype
/// \ingroup Sequential
///
/// \tparam T A subtype of StateBase
template <typename T, typename Derived>
class InitializeSeq : public InitializeBase<T, Derived>
{
    public :

    typedef typename Particle<T>::size_type size_type;
    typedef T value_type;

    unsigned operator() (Particle<T> &particle, void *param)
    {
        VSMC_STATIC_ASSERT_STATE_TYPE(StateSeq, T, InitializeSeq);

        this->initialize_param(particle, param);
        this->pre_processor(particle);
        unsigned accept = 0;
        particle.value().timer().start();
        for (size_type i = 0; i != particle.value().size(); ++i)
            accept += this->initialize_state(SingleParticle<T>(i, &particle));
        particle.value().timer().stop();
        this->post_processor(particle);

        return accept;
    }

    protected :

    InitializeSeq () {}
    InitializeSeq (const InitializeSeq<T, Derived> &) {}
    const InitializeSeq<T, Derived> &operator=
        (const InitializeSeq<T, Derived> &) {return *this;}
    ~InitializeSeq () {}
}; // class InitializeSeq

/// \brief Sampler<T>::move_type subtype
/// \ingroup Sequential
///
/// \tparam T A subtype of StateBase
template <typename T, typename Derived>
class MoveSeq : public MoveBase<T, Derived>
{
    public :

    typedef typename Particle<T>::size_type size_type;
    typedef T value_type;

    unsigned operator() (unsigned iter, Particle<T> &particle)
    {
        VSMC_STATIC_ASSERT_STATE_TYPE(StateSeq, T, MoveSeq);

        this->pre_processor(iter, particle);
        unsigned accept = 0;
        particle.value().timer().start();
        for (size_type i = 0; i != particle.value().size(); ++i)
            accept += this->move_state(iter, SingleParticle<T>(i, &particle));
        particle.value().timer().stop();
        this->post_processor(iter, particle);

        return accept;
    }

    protected :

    MoveSeq () {}
    MoveSeq (const MoveSeq<T, Derived> &) {}
    const MoveSeq<T, Derived> &operator=
        (const MoveSeq<T, Derived> &) {return *this;}
    ~MoveSeq () {}
}; // class MoveSeq

/// \brief Monitor<T>::eval_type subtype
/// \ingroup Sequential
///
/// \tparam T A subtype of StateBase
template <typename T, typename Derived>
class MonitorEvalSeq : public MonitorEvalBase<T, Derived>
{
    public :

    typedef typename Particle<T>::size_type size_type;
    typedef T value_type;

    void operator() (unsigned iter, unsigned dim, const Particle<T> &particle,
            double *res)
    {
        VSMC_STATIC_ASSERT_STATE_TYPE(StateSeq, T, MonitorEvalSeq);

        this->pre_processor(iter, particle);
        particle.value().timer().start();
        for (size_type i = 0; i != particle.value().size(); ++i) {
            this->monitor_state(iter, dim,
                    ConstSingleParticle<T>(i, &particle), res + i * dim);
        }
        particle.value().timer().stop();
        this->post_processor(iter, particle);
    }

    protected :

    MonitorEvalSeq () {}
    MonitorEvalSeq (const MonitorEvalSeq<T, Derived> &) {}
    const MonitorEvalSeq<T, Derived> &operator=
        (const MonitorEvalSeq<T, Derived> &) {return *this;}
    ~MonitorEvalSeq () {}
}; // class MonitorEvalSeq

/// \brief Path<T>::eval_type subtype
/// \ingroup Sequential
///
/// \tparam T A subtype of StateBase
template <typename T, typename Derived>
class PathEvalSeq : public PathEvalBase<T, Derived>
{
    public :

    typedef typename Particle<T>::size_type size_type;
    typedef T value_type;

    double operator() (unsigned iter, const Particle<T> &particle, double *res)
    {
        VSMC_STATIC_ASSERT_STATE_TYPE(StateSeq, T, PathEvalSeq);

        this->pre_processor(iter, particle);
        particle.value().timer().start();
        for (size_type i = 0; i != particle.value().size(); ++i) {
            res[i] = this->path_state(iter,
                    ConstSingleParticle<T>(i, &particle));
        }
        particle.value().timer().stop();
        this->post_processor(iter, particle);

        return this->path_width(iter, particle);
    }

    protected :

    PathEvalSeq () {}
    PathEvalSeq (const PathEvalSeq<T, Derived> &) {}
    const PathEvalSeq<T, Derived> &operator=
        (const PathEvalSeq<T, Derived> &) {return *this;}
    ~PathEvalSeq () {}
}; // class PathEvalSeq

} // namespace vsmc

#endif // VSMC_HELPER_SEQUENTIAL_HPP
