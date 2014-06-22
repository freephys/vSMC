#ifndef VSMC_CORE_SAMPLER_HPP
#define VSMC_CORE_SAMPLER_HPP

#include <vsmc/core/monitor.hpp>
#include <vsmc/core/particle.hpp>
#include <vsmc/core/path.hpp>
#include <vsmc/cxx11/functional.hpp>
#include <vsmc/utility/backup.hpp>
#include <limits>
#include <map>
#include <string>
#include <vector>

#define VSMC_RUNTIME_ASSERT_CORE_SAMPLER_MONITOR_NAME(iter, map, func) \
    VSMC_RUNTIME_ASSERT((iter != map.end()),                                 \
            ("**Sampler::"#func"** INVALID MONITOR NAME"))

#define VSMC_RUNTIME_ASSERT_CORE_SAMPLER_FUNCTOR(func, caller, name) \
    VSMC_RUNTIME_ASSERT(bool(func),                                          \
            ("**Sampler::"#caller"** INVALID "#name" OBJECT"))               \

#define VSMC_RUNTIME_WARNING_CORE_SAMPLER_INIT_BY_ITER \
    VSMC_RUNTIME_WARNING((!bool(init_)),                                     \
            ("**Sampler::initialize** A VALID INITIALIZATION OBJECT IS SET " \
             "BUT INITILIALIZED BY ITERATING"))

namespace vsmc {

/// \brief SMC Sampler
/// \ingroup Core
template <typename T>
class Sampler
{
    public :

    typedef typename Particle<T>::size_type size_type;
    typedef typename Particle<T>::resample_type resample_type;
    typedef T value_type;
    typedef cxx11::function<std::size_t (Particle<T> &, void *)> init_type;
    typedef cxx11::function<std::size_t (std::size_t, Particle<T> &)>
        move_type;
    typedef cxx11::function<std::size_t (std::size_t, Particle<T> &)>
        mcmc_type;
    typedef std::map<std::string, Monitor<T> > monitor_map_type;

    /// \brief Construct a Sampler without selection of resampling method
    ///
    /// \details
    /// If no resampling method, either built-in ones or user defined are
    /// provided, then it is assumed that no resamling shall be performed at
    /// all. And the threshold is set to `resample_threshold_never()`. If
    /// resampling is consdiered, then use the other two versions of the
    /// constructor to make the intention clear to the library.
    explicit Sampler (size_type N) :
        init_by_iter_(false), resample_threshold_(resample_threshold_never()),
        particle_(N), iter_num_(0), path_(typename Path<T>::eval_type())
    {resample_scheme(Stratified);}

    /// \brief Construct a Sampler with a built-in resampling scheme
    ///
    /// \details
    /// If a built-in scheme is chosen, then it is assumed that the user always
    /// want to perform resampling.
    Sampler (size_type N, ResampleScheme scheme) :
        init_by_iter_(false), resample_threshold_(resample_threshold_always()),
        particle_(N), iter_num_(0), path_(typename Path<T>::eval_type())
    {resample_scheme(scheme);}

    /// \brief Construct a Sampler with a built-in resampling scheme and a
    /// threshold for resampling
    ///
    /// \details
    /// If a built-in scheme is chosen, then it is assumed that at least the
    /// user want to perform resampling at least sometime. So the threshold is
    /// set to 0.5 if not provided as the third parameter.
    Sampler (size_type N, ResampleScheme scheme, double resample_threshold) :
        init_by_iter_(false), resample_threshold_(resample_threshold),
        particle_(N), iter_num_(0), path_(typename Path<T>::eval_type())
    {resample_scheme(scheme);}

    /// \brief Construct a Sampler with a user defined resampling operation
    ///
    /// \details
    /// If a user defined resampling operation is set, then it is assumed that
    /// at least the user want to perform resampling at least sometime. So the
    /// threshold is set to 0.5 if not provided as the third parameter.
    Sampler (size_type N, const resample_type &res_op,
            double resample_threshold = 0.5) :
        init_by_iter_(false), resample_threshold_(resample_threshold),
        particle_(N), iter_num_(0), path_(typename Path<T>::eval_type())
    {resample_scheme(res_op);}

    /// \brief Number of particles
    size_type size () const {return particle_.size();}

    /// \brief Reserve space for a specified number of iterations
    void reserve (std::size_t num)
    {
        ess_history_.reserve(num);
        resampled_history_.reserve(num);
        for (std::size_t i = 0; i != accept_history_.size(); ++i)
            accept_history_[i].reserve(num);
        if (!path_.empty())
            path_.reserve(num);
        for (typename monitor_map_type::iterator
                m = monitor_.begin(); m != monitor_.end(); ++m) {
            if (!m->second.empty())
                m->second.reserve(num);
        }
    }

    /// \brief Number of iterations (including initialization)
    std::size_t iter_size () const {return ess_history_.size();}

    /// \brief Current iteration number (initialization count as zero)
    ///
    /// \details
    /// The `iter_size() - iter_num()` is always 1. `iter_size` emphasize that it
    /// returns the total number of iterations. `iter_num` is more like an
    /// index of the sampler.
    std::size_t iter_num () const {return iter_num_;}

    /// \brief Force resample
    Sampler<T> &resample ()
    {
        particle_.resample(resample_op_,
                std::numeric_limits<double>::max VSMC_MNE ());

        return *this;
    }

    /// \brief Set resampling method by a resample_type object
    Sampler<T> &resample_scheme (const resample_type &res_op)
    {resample_op_ = res_op; return *this;}

    /// \brief Set resampling method by a built-in ResampleScheme scheme
    /// name
    Sampler<T> &resample_scheme (ResampleScheme scheme)
    {
        switch (scheme) {
            case Multinomial :
                resample_op_ = ResampleType<Multinomial>::type();
                break;
            case Residual :
                resample_op_ = ResampleType<Residual>::type();
                break;
            case Stratified :
                resample_op_ = ResampleType<Stratified>::type();
                break;
            case Systematic :
                resample_op_ = ResampleType<Systematic>::type();
                break;
            case ResidualStratified :
                resample_op_ = ResampleType<ResidualStratified>::type();
                break;
            case ResidualSystematic :
                resample_op_ = ResampleType<ResidualSystematic>::type();
                break;
        }

        return *this;
    }

    /// \brief Get resampling threshold
    double resample_threshold () const {return resample_threshold_;}

    /// \brief Set resampling threshold
    Sampler<T> &resample_threshold (double threshold)
    {resample_threshold_ = threshold; return *this;}

    /// \brief Special value of resampling threshold that indicate no
    /// resampling will be ever performed
    static double resample_threshold_never ()
    {return -std::numeric_limits<double>::infinity();}

    /// \brief Special value of resampling threshold that indicate no
    /// resampling will always be performed
    static double resample_threshold_always ()
    {return std::numeric_limits<double>::infinity();}

    /// \brief Get ESS of a given iteration, initialization count as iter 0
    double ess_history (std::size_t iter) const {return ess_history_[iter];}

    /// \brief Read ESS history through an output iterator
    template <typename OutputIter>
    OutputIter read_ess_history (OutputIter first) const
    {
        for (std::size_t i = 0; i != ess_history_.size(); ++i, ++first)
            *first = ess_history_[i];

        return first;
    }

    /// \brief Get resampling indicator of a given iteration
    bool resampled_history (std::size_t iter) const
    {return resampled_history_[iter];}

    /// \brief Read resampling indicator history through an output iterator
    template <typename OutputIter>
    OutputIter read_resampled_history (OutputIter first) const
    {
        for (std::size_t i = 0; i != resampled_history_.size(); ++i, ++first)
            *first = resampled_history_[i];

        return first;
    }

    /// \brief Get the accept count of a given move id and the iteration
    std::size_t accept_history (std::size_t id, std::size_t iter) const
    {return accept_history_[id][iter];}

    /// \brief Read and write access to the Particle<T> object
    Particle<T> &particle () {return particle_;}

    /// \brief Read only access to the Particle<T> object
    const Particle<T> &particle () const {return particle_;}

    /// \brief Set the initialization object of type init_type
    Sampler<T> &init (const init_type &new_init)
    {
        VSMC_RUNTIME_ASSERT_CORE_SAMPLER_FUNCTOR(new_init, init, INITIALIZE);

        init_ = new_init;

        return *this;
    }

    /// \brief Set if initialization should use the move and mcmc queue
    ///
    /// \details
    /// If set to `false`, then the initialization step use the initialization
    /// object if it is not empty. Otherwise, it perform the same steps as the
    /// iteration step.
    Sampler<T> &init_by_iter (bool initialize_by_iterate)
    {
        init_by_iter_ = initialize_by_iterate;

        return *this;
    }

    /// \brief Set the initialization object with a type move_type object
    ///
    /// \details
    /// When called, the iteration parameter passed to this object will be 0
    /// and the `void *` parameter will be ignored.
    Sampler<T> &init_by_move (const move_type &new_init)
    {
        VSMC_RUNTIME_ASSERT_CORE_SAMPLER_FUNCTOR(new_init, init_by_move, MOVE);

        class init_op
        {
            public :

            init_op (const move_type &new_move) : move_(new_move) {}

            std::size_t operator() (Particle<T> &particle, void *)
            {move_(0, particle);}

            private :

            move_type move_;
        }; // class init_op

        init_ = init_op(new_init);

        return *this;
    }

    /// \brief Clear the move queue
    Sampler<T> &move_queue_clear () {move_queue_.clear(); return *this;}

    /// \brief Check if move queue is empty
    bool move_queue_empty () const {return move_queue_.empty();}

    /// \brief Check the size of the move queue
    std::size_t move_queue_size () const {return move_queue_.size();}

    /// \brief Add a new move
    Sampler<T> &move (const move_type &new_move, bool append)
    {
        VSMC_RUNTIME_ASSERT_CORE_SAMPLER_FUNCTOR(new_move, move, MOVE);

        if (!append)
            move_queue_.clear();
        move_queue_.push_back(new_move);

        return *this;
    }

    /// \brief Add a sequence of new moves
    template <typename InputIter>
    Sampler<T> &move (InputIter first, InputIter last, bool append)
    {
        if (!append)
            move_queue_.clear();
        while (first != last) {
            VSMC_RUNTIME_ASSERT_CORE_SAMPLER_FUNCTOR(*first, move, MOVE);
            move_queue_.push_back(*first);
            ++first;
        }

        return *this;
    }

    /// \brief Clear the mcmc queue
    Sampler<T> &mcmc_queue_clear () {mcmc_queue_.clear(); return *this;}

    /// \brief Check if mcmc queue is empty
    bool mcmc_queue_empty () const {return mcmc_queue_.empty();}

    /// \brief Check the size of the mcmc queue
    std::size_t mcmc_queue_size () const {return mcmc_queue_.size();}

    /// \brief Add a new mcmc
    Sampler<T> &mcmc (const mcmc_type &new_mcmc, bool append)
    {
        VSMC_RUNTIME_ASSERT_CORE_SAMPLER_FUNCTOR(new_mcmc, mcmc, MCMC);

        if (!append)
            mcmc_queue_.clear();
        mcmc_queue_.push_back(new_mcmc);

        return *this;
    }

    /// \brief Add a sequence of new mcmcs
    template <typename InputIter>
    Sampler<T> &mcmc (InputIter first, InputIter last, bool append)
    {
        if (!append)
            mcmc_queue_.clear();
        while (first != last) {
            VSMC_RUNTIME_ASSERT_CORE_SAMPLER_FUNCTOR(*first, mcmc, MCMC);
            mcmc_queue_.push_back(*first);
            ++first;
        }

        return *this;
    }

    /// \brief Initialization
    ///
    /// \param param Additional parameters passed to the initialization object
    /// of type init_type
    ///
    /// All histories (ESS, resampled, accept, Monitor and Path) are clared
    /// before callling the initialization object. Monitors and Path's
    /// evaluation objects are untouched.
    Sampler<T> &initialize (void *param = VSMC_NULLPTR)
    {
        VSMC_RUNTIME_ASSERT_CORE_SAMPLER_FUNCTOR(
                init_, initialize, INITIALIZE);

        if (init_by_iter_) {
            VSMC_RUNTIME_WARNING_CORE_SAMPLER_INIT_BY_ITER;
            do_init();
            do_iter();
            do_monitor();
        } else {
            do_init();
            accept_history_.push_back(std::vector<std::size_t>(1,
                        init_(particle_, param)));
            do_resample();
            do_monitor();
        }

        return *this;
    }

    /// \brief Iteration
    ///
    /// \details
    /// Moves performed first. Then ESS/N is compared to the threshold and
    /// possible resampling is performed. Then mcmcs are performed. Then
    /// monitors and Path are computed
    Sampler<T> &iterate (std::size_t num = 1)
    {
        if (num > 1)
            reserve(iter_size() + num);

        if (accept_history_.size() < move_queue_.size() + mcmc_queue_.size()) {
            std::size_t diff = move_queue_.size() + mcmc_queue_.size() -
                accept_history_.size();
            for (std::size_t i = 0; i != diff; ++i) {
                accept_history_.push_back(
                        std::vector<std::size_t>(iter_size(), 0));
            }
        }

        for (std::size_t i = 0; i != num; ++i) {
            ++iter_num_;
            do_iter();
            do_monitor();
        }

        return *this;
    }

    /// \brief Read and write access to the Path sampling monitor
    Path<T> &path () {return path_;}

    /// \brief Read only access to the Path sampling monitor
    const Path<T> &path () const {return path_;}

    /// \brief Set the Path sampling evaluation object
    Sampler<T> &path_sampling (const typename Path<T>::eval_type &eval)
    {path_.set_eval(eval); return *this;}

    /// \brief Path sampling estimate of the logarithm of normalizing constants
    /// ratio
    double path_sampling () const {return path_.log_zconst();}

    /// \brief Add a monitor
    ///
    /// \param name The name of the monitor
    /// \param mon The new monitor to be added
    Sampler<T> &monitor (const std::string &name, const Monitor<T> &mon)
    {monitor_.insert(std::make_pair(name, mon)); return *this;}

    /// \brief Add a monitor with an evaluation object
    ///
    /// \param name The name of the monitor
    /// \param dim The dimension of the monitor, i.e., the number of variables
    /// \param eval The evaluation object of type Monitor::eval_type
    Sampler<T> &monitor (const std::string &name, std::size_t dim,
            const typename Monitor<T>::eval_type &eval)
    {
        monitor_.insert(typename monitor_map_type::value_type(
                    name, Monitor<T>(dim, eval)));

        return *this;
    }

    /// \brief Read and write access to a named monitor
    Monitor<T> &monitor (const std::string &name)
    {
        typename monitor_map_type::iterator iter = monitor_.find(name);

        VSMC_RUNTIME_ASSERT_CORE_SAMPLER_MONITOR_NAME(
                iter, monitor_, monitor);

        return iter->second;
    }

    /// \brief Read only access to a named monitor
    const Monitor<T> &monitor (const std::string &name) const
    {
        typename monitor_map_type::const_iterator citer = monitor_.find(name);

        VSMC_RUNTIME_ASSERT_CORE_SAMPLER_MONITOR_NAME(
                citer, monitor_, monitor);

        return citer->second;
    }

    /// \brief Read and write access to all monitors to the monitor_map_type
    /// object
    monitor_map_type &monitor () {return monitor_;}

    /// \brief Read only access to all monitors to the the monitor_map_type
    /// object
    const monitor_map_type &monitor () const {return monitor_;}

    /// \brief Erase a named monitor
    bool clear_monitor (const std::string &name)
    {
        return monitor_.erase(name) ==
            static_cast<typename monitor_map_type::size_type>(1);
    }

    /// \brief Erase all monitors
    Sampler<T> &clear_monitor () {monitor_.clear(); return *this;}

    /// \brief The size of Sampler summary header
    std::size_t summary_header_size () const
    {
        if (iter_size() == 0)
            return 0;

        std::size_t header_size = 1; // ESS
        header_size += accept_history_.size();
        if (path_.iter_size() > 0)
            header_size +=  2;
        for (typename monitor_map_type::const_iterator
                m = monitor_.begin(); m != monitor_.end(); ++m) {
            if (m->second.iter_size() > 0)
                header_size += m->second.dim();
        }

        return header_size;
    }

    /// \brief Sampler summary header
    template <typename OutputIter>
    OutputIter summary_header (OutputIter first) const
    {
        if (summary_header_size() == 0)
            return first;

        *first = std::string("ESS");
        ++first;

        char acc_name[32];
        unsigned accd = static_cast<unsigned>(accept_history_.size());
        for (unsigned i = 0; i != accd; ++i) {
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4996)
#endif
            std::sprintf(acc_name, "Accept.%u", i + 1);
#ifdef _MSC_VER
#pragma warning(pop)
#endif
            *first = std::string(acc_name);
            ++first;
        }

        if (path_.iter_size() > 0) {
            *first = std::string("Path.Integrand");
            ++first;
            *first = std::string("Path.Grid");
            ++first;
        }

        for (typename monitor_map_type::const_iterator
                m = monitor_.begin(); m != monitor_.end(); ++m) {
            if (m->second.iter_size() > 0) {
                char *mon_name = new char[m->first.size() + 32];
                unsigned mond = static_cast<unsigned>(m->second.dim());
                for (unsigned i = 0; i != mond; ++i, ++first) {
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4996)
#endif
                    std::sprintf(mon_name, "%s.%u", m->first.c_str(), i + 1);
#ifdef _MSC_VER
#pragma warning(pop)
#endif
                    *first = std::string(mon_name);
                }
                delete [] mon_name;
            }
        }

        return first;
    }

    /// \brief The size of Sampler summary data
    std::size_t summary_data_size () const
    {return summary_header_size() * iter_size();}

    /// \brief Sampler summary data
    ///
    /// \param first The beginning of the output
    template <MatrixOrder Order, typename OutputIter>
    OutputIter summary_data (OutputIter first) const
    {
        if (summary_data_size() == 0)
            return first;

        if (Order == RowMajor)
            first = summary_data_row(first);

        if (Order == ColMajor)
            first = summary_data_col(first);

        return first;
    }

    /// \brief Print the history of the Sampler
    ///
    /// \param sampler_id The ID of the sampler
    /// \param os The ostream to which the contents are printed
    template<typename CharT, typename Traits>
    void print (std::size_t sampler_id,
            std::basic_ostream<CharT, Traits> &os) const
    {
        if (!os.good())
            return;

        std::size_t var_num = summary_header_size();
        std::size_t dat_num = var_num * iter_size();
        std::vector<std::string> header(var_num);
        std::vector<double> data(dat_num);
        summary_header(header.begin());
        summary_data<RowMajor>(data.begin());

        os << "Sampler.ID Iteration Resampled";
        for (std::size_t i = 0; i != header.size(); ++i)
            os << ' ' << header[i];
        if (iter_size() > 0)
            os << '\n';

        std::size_t data_offset = 0;
        for (std::size_t iter = 0; iter != iter_size(); ++iter) {
            os << sampler_id;
            os << ' ' << iter;
            os << ' ' << resampled_history_[iter];
            for (std::size_t i = 0; i != var_num; ++i)
                os << ' ' << data[data_offset++];
            os << '\n';
        }
    }

    void print (std::size_t sampler_id = 0) const
    {print(sampler_id, std::cout);}

    template<typename CharT, typename Traits>
    friend inline std::basic_ostream<CharT, Traits> &operator<< (
            std::basic_ostream<CharT, Traits> &os, const Sampler<T> &sampler)
    {
        sampler.print(0, os);

        return os;
    }

    private :

    bool init_by_iter_;
    init_type init_;
    std::vector<move_type> move_queue_;
    std::vector<mcmc_type> mcmc_queue_;

    resample_type resample_op_;
    double resample_threshold_;

    Particle<T> particle_;
    std::size_t iter_num_;
    std::vector<double> ess_history_;
    std::vector<bool> resampled_history_;
    std::vector<std::vector<std::size_t> > accept_history_;

    Path<T> path_;
    monitor_map_type monitor_;

    void do_init ()
    {
        ess_history_.clear();
        resampled_history_.clear();
        accept_history_.clear();
        path_.clear();
        for (typename monitor_map_type::iterator
                m = monitor_.begin(); m != monitor_.end(); ++m)
            m->second.clear();

        iter_num_ = 0;
        particle_.set_equal_weight();
    }

    void do_iter ()
    {
        std::size_t ia = 0;
        ia = do_move(ia);
        do_resample();
        ia = do_mcmc(ia);
        for (; ia != accept_history_.size(); ++ia)
            accept_history_[ia].push_back(0);
    }

    std::size_t do_move (std::size_t ia)
    {
        for (typename std::vector<move_type>::iterator
                m = move_queue_.begin(); m != move_queue_.end(); ++m) {
            accept_history_[ia].push_back((*m)(iter_num_, particle_));
            ++ia;
        }

        return ia;
    }

    std::size_t do_mcmc (std::size_t ia)
    {
        for (typename std::vector<mcmc_type>::iterator
                m = mcmc_queue_.begin(); m != mcmc_queue_.end(); ++m) {
            accept_history_[ia].push_back((*m)(iter_num_, particle_));
            ++ia;
        }

        return ia;
    }

    void do_resample ()
    {
        bool resampled = particle_.resample(resample_op_, resample_threshold_);
        ess_history_.push_back(particle_.ess());
        resampled_history_.push_back(resampled);
    }

    void do_monitor ()
    {
        if (!path_.empty())
            path_.eval(iter_num_, particle_);

        for (typename monitor_map_type::iterator
                m = monitor_.begin(); m != monitor_.end(); ++m) {
            if (!m->second.empty())
                m->second.eval(iter_num_, particle_);
        }
    }

    template <typename OutputIter>
    OutputIter summary_data_row (OutputIter first) const
    {
        double missing_data = std::numeric_limits<double>::quiet_NaN();

        std::size_t piter = 0;
        std::vector<std::size_t> miter(monitor_.size(), 0);
        for (std::size_t iter = 0; iter != iter_size(); ++iter) {
            *first = ess_history_[iter] / size();
            ++first;
            for (std::size_t i = 0; i != accept_history_.size(); ++i) {
                *first = accept_history_[i][iter] /
                    static_cast<double>(size());
                ++first;
            }
            if (path_.iter_size() > 0) {
                if (piter == path_.iter_size() ||iter != path_.index(piter)) {
                    *first = missing_data;
                    ++first;
                    *first = missing_data;
                    ++first;
                } else {
                    *first = path_.integrand(piter);
                    ++first;
                    *first = path_.grid(piter);
                    ++first;
                    ++piter;
                }
            }
            std::size_t mm = 0;
            for (typename monitor_map_type::const_iterator
                    m = monitor_.begin(); m != monitor_.end(); ++m, ++mm) {
                if (m->second.iter_size() > 0) {
                    if (miter[mm] == m->second.iter_size()
                            || iter != m->second.index(miter[mm])) {
                        for (std::size_t i = 0; i != m->second.dim();
                                ++i, ++first)
                            *first = missing_data;
                    } else {
                        for (std::size_t i = 0; i != m->second.dim();
                                ++i, ++first)
                            *first = m->second.record(i, miter[mm]);
                        ++miter[mm];
                    }
                }
            }
        }

        return first;
    }

    template <typename OutputIter>
    OutputIter summary_data_col (OutputIter first) const
    {
        double missing_data = std::numeric_limits<double>::quiet_NaN();

        for (std::size_t iter = 0; iter != iter_size(); ++iter, ++first)
            *first = ess_history_[iter] / size();
        for (std::size_t i = 0; i != accept_history_.size(); ++i) {
            for (std::size_t iter = 0; iter != iter_size(); ++iter, ++first)
                *first = accept_history_[i][iter] /
                    static_cast<double>(size());
        }
        if (path_.iter_size() > 0) {
            std::size_t piter;
            piter = 0;
            for (std::size_t iter = 0; iter != iter_size(); ++iter, ++first) {
                if (piter == path_.iter_size() ||iter != path_.index(piter)) {
                    *first = missing_data;
                } else {
                    *first = path_.integrand(piter);
                    ++piter;
                }
            }
            piter = 0;
            for (std::size_t iter = 0; iter != iter_size(); ++iter, ++first) {
                if (piter == path_.iter_size() ||iter != path_.index(piter)) {
                    *first = missing_data;
                } else {
                    *first = path_.grid(piter);
                    ++piter;
                }
            }
        }
        for (typename monitor_map_type::const_iterator
                m = monitor_.begin(); m != monitor_.end(); ++m) {
            if (m->second.iter_size() > 0) {
                for (std::size_t i = 0; i != m->second.dim(); ++i) {
                    std::size_t miter = 0;
                    for (std::size_t iter = 0; iter != iter_size();
                            ++iter, ++first) {
                        if (miter == m->second.iter_size()
                                || iter != m->second.index(miter)) {
                            *first = missing_data;
                        } else {
                            *first = m->second.record(i, miter);
                            ++miter;
                        }
                    }
                }
            }
        }

        return first;
    }
}; // class Sampler

} // namespace vsmc

#endif // VSMC_CORE_SAMPLER_HPP
