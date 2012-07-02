#ifndef VSMC_CORE_RNG_HPP
#define VSMC_CORE_RNG_HPP

#include <vsmc/internal/common.hpp>
#include <vsmc/rng/random.hpp>

namespace vsmc {

/// \brief Sequential RNG set class
/// \ingroup Core
class RngSetSeq
{
    public :

    /// The type of the random number generator C++11 engine
    typedef VSMC_SEQRNG_TYPE rng_type;

    /// The type of the seed sequence
    typedef rng::Seed<rng_type> seed_type;

    template <typename SizeType>
    RngSetSeq (SizeType N) : rng_(static_cast<rng_type::result_type>(
                seed_type::create().get())) {}

    /// \brief Get a C++11 RNG engine
    ///
    /// \param id The position of the particle, 0 to size() - 1
    ///
    /// \return A reference to a C++11 RNG engine unique to particle at
    /// position id, and independent of others
    template <typename SizeType>
    rng_type &rng (SizeType id)
    {
        return rng_;
    }

    private :

    rng_type rng_;
}; // class RngSetSeq

/// \brief Parallel RNG set class
/// \ingroup Core
class RngSetPrl
{
    public :

    /// The type of the random number generator C++11 engine
    typedef VSMC_PRLRNG_TYPE rng_type;

    /// The type of the seed sequence
    typedef rng::Seed<rng_type> seed_type;

    template <typename SizeType>
    RngSetPrl (SizeType N)
    {
        seed_type &seed = rng::Seed<rng_type>::create();
        for (SizeType i = 0; i != N; ++i)
            rng_.push_back(
                    rng_type(static_cast<rng_type::result_type>(seed.get())));
    }

    /// \brief Get a C++11 RNG engine
    ///
    /// \param id The position of the particle, 0 to size() - 1
    ///
    /// \return A reference to a C++11 RNG engine unique to particle at
    /// position id, and independent of others
    template <typename SizeType>
    rng_type &rng (SizeType id)
    {
        return rng_[id];
    }

    private :

    std::vector<rng_type> rng_;
}; // class RngSetPrl

} // namespace vsmc

#if VSMC_USE_RANDOM123
VSMC_DEFINE_TYPE_DISPATCH_TRAIT(RngSetType, rng_set_type, RngSetPrl);
#else
VSMC_DEFINE_TYPE_DISPATCH_TRAIT(RngSetType, rng_set_type, RngSetSeq);
#endif

#endif // VSMC_CORE_RNG_HPP
