//============================================================================
// vSMC/include/vsmc/mpi/backend_mpi.hpp
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

#ifndef VSMC_MPI_BACKEND_MPI_HPP
#define VSMC_MPI_BACKEND_MPI_HPP

#include <vsmc/mpi/internal/common.hpp>
#include <vsmc/core/weight_set.hpp>
#include <vsmc/mpi/mpi_datatype.hpp>
#include <vsmc/mpi/mpi_manager.hpp>

#define VSMC_RUNTIME_ASSERT_MPI_BACKEND_MPI_COPY_SIZE_MISMATCH                \
    VSMC_RUNTIME_ASSERT(                                                      \
        (N == global_size_), "**StateMPI::copy** SIZE MISMATCH")

namespace vsmc
{

/// \brief Particle::weight_set_type subtype using MPI
/// \ingroup MPI
template <typename WeightSetBase, typename ID = MPIDefault>
class WeightSetMPI : public WeightSetBase
{
    public:
    typedef SizeType<WeightSetBase> size_type;
    typedef ID mpi_id;

    explicit WeightSetMPI(size_type N)
        : WeightSetBase(N)
        , world_(MPICommunicator<ID>::instance().get(),
              ::boost::mpi::comm_duplicate)
        , resample_size_(0)
    {
        ::boost::mpi::all_reduce(
            world_, N, resample_size_, std::plus<size_type>());
        this->set_ess(static_cast<double>(resample_size_));
    }

    /// \brief A duplicated MPI communicator for this weight set object
    const ::boost::mpi::communicator &world() const { return world_; }

    size_type resample_size() const { return resample_size_; }

    void read_resample_weight(double *first) const
    {
        gather_resample_weight();
        if (world_.rank() == 0) {
            const std::size_t S = static_cast<std::size_t>(world_.size());
            for (std::size_t r = 0; r != S; ++r) {
                first = std::copy(
                    weight_all_[r].begin(), weight_all_[r].end(), first);
            }
        }
    }

    const double *resample_weight_data() const
    {
        resample_weight_.resize(resample_size_);
        read_resample_weight(resample_weight_.data());

        return world_.rank() == 0 ? resample_weight_.data() : nullptr;
    }

    void normalize_log_weight()
    {
        const size_type N = static_cast<size_type>(this->size());
        double *const lwptr = this->mutable_log_weight_data();

        double lmax_weight = *(std::max_element(lwptr, lwptr + N));
        double gmax_weight = 0;
        ::boost::mpi::all_reduce(
            world_, lmax_weight, gmax_weight, ::boost::mpi::maximum<double>());
        for (size_type i = 0; i != N; ++i)
            lwptr[i] -= gmax_weight;
    }

    void normalize_weight()
    {
        const size_type N = static_cast<size_type>(this->size());
        double *const wptr = this->mutable_weight_data();

        double lcoeff = math::asum(N, wptr, 1);
        double gcoeff = 0;
        ::boost::mpi::all_reduce(world_, lcoeff, gcoeff, std::plus<double>());
        gcoeff = 1 / gcoeff;
        math::scal(N, gcoeff, wptr, 1);

        double less = math::nrm2(N, wptr, 1);
        less = less * less;
        double gess = 0;
        ::boost::mpi::all_reduce(world_, less, gess, std::plus<double>());
        gess = 1 / gess;
        this->set_ess(gess);
    }

    double compute_ess(const double *first, bool use_log) const
    {
        const size_type N = static_cast<size_type>(this->size());
        Vector<double> buffer(N);
        double *const bptr = buffer.data();

        if (use_log) {
            const double *const lwptr = this->log_weight_data();
            for (size_type i = 0; i != N; ++i)
                bptr[i] = lwptr[i] + first[i];
            double lmax_weight = bptr[0];
            for (size_type i = 0; i != N; ++i)
                if (lmax_weight < bptr[i])
                    lmax_weight = bptr[i];
            double gmax_weight = 0;
            ::boost::mpi::all_reduce(world_, lmax_weight, gmax_weight,
                ::boost::mpi::maximum<double>());
            for (size_type i = 0; i != N; ++i)
                bptr[i] -= gmax_weight;
            for (size_type i = 0; i != N; ++i)
                bptr[i] = std::exp(bptr[i]);
        } else {
            const double *const wptr = this->weight_data();
            for (size_type i = 0; i != N; ++i)
                bptr[i] = wptr[i] * first[i];
        }

        double lcoeff = 0;
        for (size_type i = 0; i != N; ++i)
            lcoeff += bptr[i];
        double gcoeff = 0;
        ::boost::mpi::all_reduce(world_, lcoeff, gcoeff, std::plus<double>());
        gcoeff = 1 / gcoeff;
        for (size_type i = 0; i != N; ++i)
            bptr[i] *= gcoeff;

        double less = 0;
        for (size_type i = 0; i != N; ++i)
            less += bptr[i] * bptr[i];
        double gess = 0;
        ::boost::mpi::all_reduce(world_, less, gess, std::plus<double>());

        return 1 / gess;
    }

    double compute_cess(const double *first, bool use_log) const
    {
        const size_type N = static_cast<size_type>(this->size());
        const double *bptr = first;
        const double *const wptr = this->weight_data();
        Vector<double> buffer;
        if (use_log) {
            buffer.resize(N);
            double *const cptr = buffer.data();
            for (size_type i = 0; i != N; ++i)
                cptr[i] = std::exp(first[i]);
            bptr = cptr;
        }

        double labove = 0;
        double lbelow = 0;
        for (size_type i = 0; i != N; ++i) {
            double wb = wptr[i] * bptr[i];
            labove += wb;
            lbelow += wb * bptr[i];
        }
        double gabove = 0;
        double gbelow = 0;
        ::boost::mpi::all_reduce(world_, labove, gabove, std::plus<double>());
        ::boost::mpi::all_reduce(world_, lbelow, gbelow, std::plus<double>());

        return gabove * gabove / gbelow;
    }

    private:
    ::boost::mpi::communicator world_;
    size_type resample_size_;
    mutable std::vector<double> resample_weight_;
    mutable std::vector<double> weight_;
    mutable std::vector<std::vector<double>> weight_all_;

    void gather_resample_weight() const
    {
        weight_.resize(this->size());
        this->read_weight(weight_.data());
        if (world_.rank() == 0)
            ::boost::mpi::gather(world_, weight_, weight_all_, 0);
        else
            ::boost::mpi::gather(world_, weight_, 0);
    }
}; // class WeightSetMPI

/// \brief Particle::value_type subtype using MPI
/// \ingroup MPI
template <typename StateBase, typename ID = MPIDefault>
class StateMPI : public StateBase
{
    public:
    typedef SizeType<StateBase> size_type;
    typedef WeightSetMPI<WeightSetType<StateBase>, ID> weight_set_type;
    typedef ID mpi_id;

    explicit StateMPI(size_type N)
        : StateBase(N)
        , world_(MPICommunicator<ID>::instance().get(),
              ::boost::mpi::comm_duplicate)
        , offset_(0)
        , global_size_(0)
        , size_equal_(true)
        , copy_tag_(::boost::mpi::environment::max_tag())
    {
        ::boost::mpi::all_gather(world_, N, size_all_);
        const std::size_t R = static_cast<std::size_t>(world_.rank());
        const std::size_t S = static_cast<std::size_t>(world_.size());
        for (std::size_t i = 0; i != R; ++i) {
            offset_ += size_all_[i];
            global_size_ += size_all_[i];
            size_equal_ = size_equal_ && N == size_all_[i];
        }
        for (std::size_t i = R; i != S; ++i) {
            global_size_ += size_all_[i];
            size_equal_ = size_equal_ && N == size_all_[i];
        }
    }

    /// \brief Copy particles
    ///
    /// \param N The number of particles on all nodes
    /// \param copy_from A vector of length `N`, for each particle with global
    /// id `to`, `copy_from[to]` is the global id of the particle it shall
    /// copy.
    ///
    /// \details
    /// The `StateBase` type is required to have the following members -
    /// `state_pack_type`: A type that used to pack state values. It shall be
    /// serializable. That is, a `state_pack_type` object is acceptable by
    /// `boost::mpi::communicator::send` etc.  StateMatrix::state_pack_type
    /// satisfies this requirement if their template type parameter types are
    /// serializable. For user defined types, see document of Boost.Serialize
    /// of how to serialize a class object.
    /// - `state_pack`
    /// ~~~{.cpp}
    /// state_pack_type state_pack (size_type id) const;
    /// ~~~
    /// Given a local particle id on this node, pack the state values into a
    /// `state_pack_type` object.
    /// - `state_unpack`
    /// ~~~{.cpp}
    /// void state_unpack (size_type id, const state_pack_type &pack);
    /// ~~~
    /// Given a local particle id and a `state_pack_type` object, unpack it
    /// into the given position on this node. If C++11 rvalue reference is
    /// supported, then an rvalue version of this function can be defined to
    /// improved the performance as `pack` will be passed as an rvalue. e.g.,
    /// ~~~{.cpp}
    /// void state_unpack (size_type id, state_pack_type &&pack);
    /// ~~~
    /// As usual, if `state_pack_type` needs to explicitly define move
    /// constructor or assignment operator, care shall be taken to make sure
    /// that after the move `pack` is still a valid, assignable object.
    ///
    /// In vSMC, the resampling algorithms generate the number of replications
    /// of each particle. Particles with replication zero need to copy other
    /// particles. The vector of the number of replications is transfered to
    /// `copy_from` by Particle::resample, and it is generated in such a way
    /// that each particle will copy from somewhere close to itself.
    /// Therefore,
    /// transferring between nodes is minimized.
    ///
    /// This default implementation perform three stages of copy.
    /// - Stage one: Generate a local duplicate of `copy_from` on node `0` and
    /// broadcast it to all nodes.
    /// - Stage two: Perform local copy, copy those particles where the
    /// destination and source are both on this node. This is performed in
    /// parallel on each node.
    /// - Stage three: copy particles that need message passing between nodes.
    ///
    /// A derived class can override this `copy` method. For the following
    /// possible reasons,
    /// - Stage one is not needed or too expansive
    /// - Stage three is too expansive. The default implementation assumes
    /// `this->state_pack(id)` and `this->state_unpack(id, pack) is not too
    /// expansive, and inter-node copy is rare anyway. If this is not the
    /// case,
    /// then it can be a performance bottle neck.
    template <typename IntType>
    void copy(size_type N, const IntType *copy_from)
    {
        VSMC_RUNTIME_ASSERT_MPI_BACKEND_MPI_COPY_SIZE_MISMATCH;

        copy_pre_processor_dispatch(has_copy_pre_processor_<StateBase>());
        copy_from_.resize(N);
        if (world_.rank() == 0)
            std::copy(copy_from, copy_from + N, copy_from_.begin());
        ::boost::mpi::broadcast(world_, copy_from_, 0);
        copy_this_node(N, copy_from_.data(), copy_recv_, copy_send_);
        copy_inter_node(copy_recv_, copy_send_);
        copy_post_processor_dispatch(has_copy_post_processor_<StateBase>());
    }

    /// \brief A duplicated MPI communicator for this state value object
    const ::boost::mpi::communicator &world() const { return world_; }

    /// \brief The number of particles on all nodes
    size_type global_size() const { return global_size_; }

    /// \brief The number of particles on nodes with ranks less than the rank
    /// of this node.
    size_type offset() const { return offset_; }

    /// \brief Given a global particle id return the rank of the node it
    /// belongs
    int rank(size_type global_id) const
    {
        if (size_equal_)
            return static_cast<int>(global_id / this->size());

        std::size_t r = 0;
        size_type g = size_all_[0];
        while (g <= global_id) {
            ++r;
            g += size_all_[r];
        }

        return static_cast<int>(r);
    }

    /// \brief Given a global particle id check if it is on this `node`
    bool is_local(size_type global_id) const
    {
        return global_id >= offset_ && global_id < this->size() + offset_;
    }

    /// \brief Transfer a global particle id into a local particle id
    /// (possibly not on this node, use `rank` to get the rank of its node)
    size_type local_id(size_type global_id) const
    {
        if (size_equal_) {
            return global_id -
                this->size() * static_cast<size_type>(rank(global_id));
        }

        std::size_t r = 0;
        size_type g = size_all_[0];
        while (g <= global_id) {
            ++r;
            g += size_all_[r];
        }
        g -= size_all_[r];

        return global_id - g;
    }

    /// \brief Transfer a local particle id *on this node* into a global
    /// particle id
    size_type global_id(size_type local_id) const
    {
        return local_id + offset_;
    }

    protected:
    /// \brief The MPI recv/send tag used by `copy_inter_node`
    int copy_tag() const { return copy_tag_; }

    /// \brief Perform local copy
    ///
    /// \param N The number of particles on all nodes
    /// \param copy_from The beginning of the copy_from vector
    /// \param copy_recv All particles that shall be received at this node
    /// \param copy_send All particles that shall be send from this node
    ///
    /// \details
    /// `copy_from` is a pointer that can access a vector of size `N`. For
    /// each
    /// `to` in the range `0` to `N - 1`
    /// - If both `to` and `from = copy_from[to]` are particles on this node,
    /// use `StateBase::copy` to copy the parties. Otherwise,
    /// - If `to` is a particle on this node, insert a pair into `copy_recv`,
    /// whose values are the rank of the node from which this node shall
    /// receive the particle and the particle id *on this node* where the
    /// particle received shall be unpacked. Otherwise,
    /// - If `from = copy_from[to]` is a particle on this node, insert a pair
    /// into `copy_send`, whose values are the rank of the node to which this
    /// node shall send the particle and the particle id *on this node* where
    /// the particle sent shall be packed. Otherwise do nothing.
    ///
    /// It is important the the vector accessed through `copy_from_first` is
    /// the same for all nodes. Otherwise the behavior is undefined.
    void copy_this_node(size_type N, const size_type *copy_from,
        std::vector<std::pair<int, size_type>> &copy_recv,
        std::vector<std::pair<int, size_type>> &copy_send)
    {
        using std::advance;

        const int rank_this = world_.rank();

        copy_from_this_.resize(this->size());
        const size_type *first = copy_from + offset_;
        for (size_type to = 0; to != this->size(); ++to, ++first) {
            size_type from = *first;
            copy_from_this_[to] =
                rank_this == rank(from) ? local_id(from) : to;
        }
        StateBase::copy(this->size(), copy_from_this_.data());

        copy_recv.clear();
        copy_send.clear();
        for (size_type to = 0; to != N; ++to, ++copy_from) {
            size_type from = *copy_from;
            int rank_recv = rank(to);
            int rank_send = rank(from);
            size_type id_recv = local_id(to);
            size_type id_send = local_id(from);
            if (rank_this == rank_recv && rank_this == rank_send) {
                continue;
            } else if (rank_this == rank_recv) {
                copy_recv.push_back(std::make_pair(rank_send, id_recv));
            } else if (rank_this == rank_send) {
                copy_send.push_back(std::make_pair(rank_recv, id_send));
            }
        }
    }

    /// \brief Perform global copy
    ///
    /// \param copy_recv The output vector `copy_recv` from `copy_this_node`
    /// \param copy_send The output vector `copy_send` from `copy_this_node`
    void copy_inter_node(
        const std::vector<std::pair<int, size_type>> &copy_recv,
        const std::vector<std::pair<int, size_type>> &copy_send)
    {
        const int rank_this = world_.rank();
        for (int r = 0; r != world_.size(); ++r) {
            if (rank_this == r) {
                for (std::size_t i = 0; i != copy_recv.size(); ++i) {
                    world_.recv(copy_recv[i].first, copy_tag_, pack_recv_);
                    this->state_unpack(
                        copy_recv[i].second, std::move(pack_recv_));
                }
            } else {
                for (std::size_t i = 0; i != copy_send.size(); ++i) {
                    if (copy_send[i].first == r) {
                        pack_send_ = this->state_pack(copy_send[i].second);
                        world_.send(copy_send[i].first, copy_tag_, pack_send_);
                    }
                }
            }
        }
    }

    private:
    ::boost::mpi::communicator world_;
    size_type offset_;
    size_type global_size_;
    bool size_equal_;
    std::vector<size_type> size_all_;
    int copy_tag_;
    std::vector<size_type> copy_from_;
    std::vector<size_type> copy_from_this_;
    std::vector<std::pair<int, size_type>> copy_recv_;
    std::vector<std::pair<int, size_type>> copy_send_;
    typename StateBase::state_pack_type pack_recv_;
    typename StateBase::state_pack_type pack_send_;

    VSMC_DEFINE_METHOD_CHECKER(copy_pre_processor, void, ())
    VSMC_DEFINE_METHOD_CHECKER(copy_post_processor, void, ())

    void copy_pre_processor_dispatch(std::true_type)
    {
        StateBase::copy_pre_processor();
    }

    void copy_pre_processor_dispatch(std::false_type) {}

    void copy_post_processor_dispatch(std::true_type)
    {
        StateBase::copy_post_processor();
    }

    void copy_post_processor_dispatch(std::false_type) {}
}; // class StateMPI

} // namespace vsmc

#endif // VSMC_MPI_BACKEND_MPI_HPP
