#ifndef VSMC_UTILITY_STATIC_VECTOR_HPP
#define VSMC_UTILITY_STATIC_VECTOR_HPP

#include <vsmc/internal/common.hpp>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4351)
#endif

#define VSMC_STATIC_ASSERT_UTILITY_STATIC_VECTOR_RANGE(Pos, N) \
    VSMC_STATIC_ASSERT((Pos < N),                                            \
            USE_StaticVector_WITH_AN_INDEX_OUT_OF_RANGE)

#define VSMC_STATIC_ASSERT_UTILITY_STATIC_VECTOR_SLICE_BEGIN(Begin, N) \
    VSMC_STATIC_ASSERT((Begin < N),                                          \
            USE_StaticVector_SLICE_WITH_FIRST_INDEX_TOO_LARGE)

#define VSMC_STATIC_ASSERT_UTILITY_STATIC_VECTOR_SLICE_END(End, N) \
    VSMC_STATIC_ASSERT((End < N),                                            \
            USE_StaticVector_WITH_SLICE_WITH_SECOND_INDEX_TOO_LARGE)

#define VSMC_RUNTIME_ASSERT_UTILITY_STATIC_VECTOR_RANGE(i, N) \
    VSMC_RUNTIME_ASSERT((i < N),                                             \
            ("**StaticVector** USED WITH AN INDEX OUT OF RANGE"))

namespace vsmc {

namespace internal {

template <typename, std::size_t, bool> class StaticVectorStorage;

template <typename T, std::size_t N>
class StaticVectorStorage<T, N, true>
{
    public :

    T &operator[] (std::size_t i) {return data_[i];}

    const T &operator[] (std::size_t i) const {return data_[i];}

    T *data () {return data_;}

    const T *data () const {return data_;}

    void swap_data (StaticVectorStorage<T, N, true> &other)
    {
        using std::swap;

        for (std::size_t i = 0; i != N; ++i)
            swap(data_[i], other.data_[i]);
    }

    private :

    T data_[N];
}; // class StaticVectorStorage

template <typename T, std::size_t N>
class StaticVectorStorage<T, N, false>
{
    public :

    StaticVectorStorage () : data_(new T[N]()) {}

    StaticVectorStorage (const StaticVectorStorage<T, N, false> &other) :
        data_(new T[N]())
    {
        for (std::size_t i = 0; i != N; ++i)
            data_[i] = other.data_[i];
    }

    StaticVectorStorage<T, N, false> &operator= (
            const StaticVectorStorage<T, N, false> &other)
    {
        if (this != &other) {
            for (std::size_t i = 0; i != N; ++i)
                data_[i] = other.data_[i];
        }

        return *this;
    }

#if VSMC_HAS_CXX11_RVALUE_REFERENCES
    StaticVectorStorage (StaticVectorStorage<T, N, false> &&other) :
        data_(other.data_) {other.data_ = VSMC_NULLPTR;}

    StaticVectorStorage<T, N, false> &operator= (
            StaticVectorStorage<T, N, false> &&other)
    {
        if (this != &other) {
            if (data_ != VSMC_NULLPTR)
                delete [] data_;
            data_ = other.data_;
            other.data_ = VSMC_NULLPTR;
        }

        return *this;
    }
#endif

    ~StaticVectorStorage ()
    {
        if (data_ != VSMC_NULLPTR)
            delete [] data_;
    }

    T &operator[] (std::size_t i) {return data_[i];}

    const T &operator[] (std::size_t i) const {return data_[i];}

    T *data () {return data_;}

    const T *data () const {return data_;}

    void swap_data (StaticVectorStorage<T, N, false> &other)
    {std::swap(data_, other.data_);}

    private :

    T *data_;
}; // class StaticVectorStorage

} // namespace vsmc::internal

namespace traits {

/// \brief Default traits of StaticVector
/// \ingroup Traits
template <typename T> struct StaticVectorTrait
{static VSMC_CONSTEXPR const std::size_t max_static_size = 1024;};

} // namespace vsmc::traits

/// \brief Container class with static size but possible dynamic memory
/// allocation
/// \ingroup StaticVector
///
/// \details
/// Array and C++11 `std::array` are efficient when the size is known at
/// compile time. However, whent the size is large, the benefits can be
/// negligible. And in the worst case, it can even cause stack overflow or
/// degenerate the performance. There are other undesired effects of
/// `std::array`. For example, the `swap` operation has a complexity linear to
/// the size while with `std::vector`, it has a constant complexity.
///
/// The StaticVector container is sort of a hybrid of `std::array` and
/// `std::vector`.  When the size is small, it uses array to allocate the
/// memory and therefore if the object is allocated on the stack, then no
/// dynamic memory allocation happens at all. And if a sequence of StaticVector
/// is allocated, the internal storage is allocated next to each other. When
/// the size is large, it uses dynamic memory allocation and it still behaves
/// much like a `std::array` except one does not need to worry that very large
/// stack allocation will happen.
///
/// The interface is almost identical to that of `std::array` with a few
/// differences.
/// - There is no `max_size()` member function
/// - The `size()` and `empty()` member functions are `static` and can be used
/// as constant expression if `constexpr` is supported
/// - There are additional `at<Pos>()` and `at(Position<Pos>())` member
/// functions, which perform static assertions of the index instead of runtime
/// assertion. They can also be used to write loop unrolling functions for the
/// container. They are preferred way to access elements. They are (slightly)
/// more efficient than `operator[]` and also safer.
/// - The non-member function `get` and `swap` are defined in the namespace
/// `vsmc`. Use the swap idiom,
/// ~~~{.cpp}
/// using std::swap;
/// swap(obj1, obj2);
/// ~~~
/// instead of calling `std::swap(obj1, obj2)`. See "Effective C++".
/// - The helper classes `std::tuple_size` and `std::tuple_element` are not
/// defined for StaticVector
template <typename T, std::size_t N,
         typename Traits = traits::StaticVectorTrait<T> >
class StaticVector : public internal::StaticVectorStorage<T, N,
    (sizeof(T) * N <= Traits::max_static_size)>
{
    typedef internal::StaticVectorStorage<T, N,
    (sizeof(T) * N <= Traits::max_static_size)> storage_type;

    public :

    typedef T value_type;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    typedef value_type & reference;
    typedef const value_type & const_reference;
    typedef T * pointer;
    typedef const T * const_pointer;

    typedef pointer iterator;
    typedef const_pointer const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    iterator begin () {return this->data();}

    iterator end () {return this->data() + N;}

    const_iterator begin () const {return this->data();}

    const_iterator end () const {return this->data() + N;}

    const_iterator cbegin () const {return this->data();}

    const_iterator cend () const {return this->data() + N;}

    reverse_iterator rbegin () {return reverse_iterator(end());}

    reverse_iterator rend () {return reverse_iterator(begin());}

    const_reverse_iterator rbegin () const
    {return const_reverse_iterator(end());}

    const_reverse_iterator rend () const
    {return const_reverse_iterator(begin());}

    const_reverse_iterator crbegin () const
    {return const_reverse_iterator(cend());}

    const_reverse_iterator crend () const
    {return const_reverse_iterator(cbegin());}

    reference at (size_type i)
    {
        VSMC_RUNTIME_ASSERT_UTILITY_STATIC_VECTOR_RANGE(i, N);
        return this->operator[](i);
    }

    const_reference at (size_type i) const
    {
        VSMC_RUNTIME_ASSERT_UTILITY_STATIC_VECTOR_RANGE(i, N);
        return this->operator[](i);
    }

    template <size_type Pos>
    reference at ()
    {
        VSMC_STATIC_ASSERT_UTILITY_STATIC_VECTOR_RANGE(Pos, N);
        return this->operator[](Pos);
    }

    template <size_type Pos>
    const_reference at () const
    {
        VSMC_STATIC_ASSERT_UTILITY_STATIC_VECTOR_RANGE(Pos, N);
        return this->operator[](Pos);
    }

    template <size_type Pos>
    reference at (Position<Pos>)
    {
        VSMC_STATIC_ASSERT_UTILITY_STATIC_VECTOR_RANGE(Pos, N);
        return this->operator[](Pos);
    }

    template <size_type Pos>
    const_reference at (Position<Pos>) const
    {
        VSMC_STATIC_ASSERT_UTILITY_STATIC_VECTOR_RANGE(Pos, N);
        return this->operator[](Pos);
    }

    reference operator[] (size_type i)
    {return this->operator[](i);}

    const_reference operator[] (size_type i) const
    {return this->operator[](i);}

    template <size_type Pos>
    reference operator[] (Position<Pos>) {return at<Pos>();}

    template <size_type Pos>
    const_reference operator[] (Position<Pos>) const {return at<Pos>();}

    reference front () {return at<0>();}

    const_reference front () const {return at<0>();}

    reference back () {return at<N - 1>();}

    const_reference back () const {return at<N - 1>();}

    static VSMC_CONSTEXPR bool empty () {return N == 0;}

    static VSMC_CONSTEXPR size_type size () {return N;}

    void fill (const T &value)
    {
        for (size_type i = 0; i != N; ++i)
            this->operator[](i) = value;
    }

    void swap (StaticVector<T, N, Traits> &other) {this->swap_data(other);}

    /// \brief Get a new vector with a different size
    template <std::size_t Size>
    StaticVector<T, Size, Traits> resize () const
    {return resize<Size>(cxx11::integral_constant<bool, Size == N>());}

    /// \brief Get a slice of the vector, [Begin, End)
    template <std::size_t Begin, std::size_t End>
    StaticVector<T, End - Begin, Traits> slice () const
    {
        return slice<Begin, End>(
                cxx11::integral_constant<bool, Begin == 0 && End == N>());
    }

    /// \brief Expand the vector, starting with Begin and size N
    template <std::size_t Begin, std::size_t Size>
    StaticVector<T, Size, Traits> expand () const
    {return expand<Begin, Size>(cxx11::integral_constant<bool, Begin == 0>());}

    friend inline bool operator== (
            const StaticVector<T, N, Traits> &sv1,
            const StaticVector<T, N, Traits> &sv2)
    {
        for (std::size_t i = 0; i != N; ++i)
            if (sv1[i] != sv2[i])
                return false;

        return true;
    }

    friend inline bool operator!= (
            const StaticVector<T, N, Traits> &sv1,
            const StaticVector<T, N, Traits> &sv2)
    {return !(sv1 == sv2);}

    friend inline bool operator< (
            const StaticVector<T, N, Traits> &sv1,
            const StaticVector<T, N, Traits> &sv2)
    {
        for (std::size_t i = 0; i != N; ++i) {
            if (sv1[i] < sv2[i])
                return true;
            if (sv2[i] < sv1[i])
                return false;
        }

        return false;
    }

    friend inline bool operator<= (
            const StaticVector<T, N, Traits> &sv1,
            const StaticVector<T, N, Traits> &sv2)
    {
        for (std::size_t i = 0; i != N; ++i) {
            if (sv1[i] < sv2[i])
                return true;
            if (sv2[i] < sv1[i])
                return false;
        }

        return true;
    }

    friend inline bool operator> (
            const StaticVector<T, N, Traits> &sv1,
            const StaticVector<T, N, Traits> &sv2)
    {return !(sv1 <= sv2);}

    friend inline bool operator>= (
            const StaticVector<T, N, Traits> &sv1,
            const StaticVector<T, N, Traits> &sv2)
    {return !(sv1 < sv2);}

    template <typename CharT, typename CharTraits>
    friend inline std::basic_ostream<CharT, CharTraits> &operator<< (
            std::basic_ostream<CharT, CharTraits> &os,
            const StaticVector<T, N, Traits> &sv)
    {
        if (!os.good())
            return os;

        for (std::size_t i = 0; i < N - 1; ++i)
            os << sv[i] << ' ';
        os << sv[N - 1];

        return os;
    }

    template <typename CharT, typename CharTraits>
    friend inline std::basic_istream<CharT, CharTraits> &operator>> (
            std::basic_istream<CharT, CharTraits> &is,
            StaticVector<T, N, Traits> &sv)
    {
        if (!is.good())
            return is;

        StaticVector<T, N, Traits> sv_tmp;
        for (std::size_t i = 0; i != N; ++i)
            is >> std::ws >> sv_tmp[i];

        if (is.good()) {
#if VSMC_HAS_CXX11_RVALUE_REFERENCES
            sv = cxx11::move(sv_tmp);
#else
            sv = sv_tmp;
#endif
        }
    }

    private :

    template <std::size_t Size>
    StaticVector<T, Size, Traits> resize (cxx11::true_type) const
    {return *this;}

    template <std::size_t Size>
    StaticVector<T, Size, Traits> resize (cxx11::false_type) const
    {
        StaticVector<T, Size, Traits> res;
        for (std::size_t i = 0, j = 0; i != N && j != Size; ++i, ++j)
            res[j] = this->operator[](i);

        return res;
    }

    template <std::size_t Begin, std::size_t End>
    StaticVector<T, End - Begin, Traits> slice (cxx11::true_type) const
    {return *this;}

    template <std::size_t Begin, std::size_t End>
    StaticVector<T, End - Begin, Traits> slice (cxx11::false_type) const
    {
        VSMC_STATIC_ASSERT_UTILITY_STATIC_VECTOR_SLICE_BEGIN(Begin, N);
        VSMC_STATIC_ASSERT_UTILITY_STATIC_VECTOR_SLICE_END(End, N);
        StaticVector<T, End - Begin, Traits> res;
        for (std::size_t i = Begin, j = 0; i != End; ++i, ++j)
            res[j] = this->operator[](i);

        return res;
    }

    template <std::size_t Begin, std::size_t Size>
    StaticVector<T, Size, Traits> expand (cxx11::true_type) const
    {return resize<Size>();}

    template <std::size_t Begin, std::size_t Size>
    StaticVector<T, Size, Traits> expand (cxx11::false_type) const
    {
        VSMC_STATIC_ASSERT_UTILITY_STATIC_VECTOR_SLICE_BEGIN(Begin, N);
        StaticVector<T, Size, Traits> res;
        for (std::size_t i = Begin, j = 0; i != N && j != Size; ++i, ++j)
            res[j] = this->operator[](i);

        return res;
    }
}; // class StaticVector

/// \brief StaticVector ADL of swap
/// \ingroup StaticVector
template <typename T, std::size_t N, typename Traits>
inline void swap (
        StaticVector<T, N, Traits> &sv1,
        StaticVector<T, N, Traits> &sv2)
{sv1.swap(sv2);}

/// \brief StaticVector ADL of get
/// \ingroup StaticVector
template <std::size_t I, typename T, std::size_t N, typename Traits>
inline T &get (StaticVector<T, N, Traits> &sv)
{return sv.template at<I>();}

/// \brief StaticVector ADL of get
/// \ingroup StaticVector
template <std::size_t I, typename T, std::size_t N, typename Traits>
inline const T &get (const StaticVector<T, N, Traits> &sv)
{return sv.template at<I>();}

#if VSMC_HAS_CXX11_RVALUE_REFERENCES
/// \brief StaticVector ADL of get
/// \ingroup StaticVector
template <std::size_t I, typename T, std::size_t N, typename Traits>
inline T &&get (StaticVector<T, N, Traits> &&sv)
{return cxx11::move(sv.template at<I>());}
#endif

namespace internal {

template <typename T, bool> struct StaticCounterMask;

template <typename T>
struct StaticCounterMask<T, true>
{
    static VSMC_CONSTEXPR const T max_val =
        static_cast<T>(~(static_cast<T>(0)));

    static VSMC_CONSTEXPR const T mask_hi =
        static_cast<T>(static_cast<T>(max_val << 8) >> 8);

    static VSMC_CONSTEXPR const T mask_lo =
        static_cast<T>(mask_hi^max_val);
}; // struct StaticCounterMask

} // namespace vsmc::internal

template <typename> class StaticCounter;

/// \brief Using StaticVector of unsigned integers as counters
/// \ingroup StaticVector
///
/// \details
/// This class provides methods for using StaticVector of unsigned integers as
/// large integer counters.
///
/// It deals with two types of counters. The first type is in the form
/// `StaticVector<T, K, Traits>` where `T` is an unsigned integer type, treated
/// as a `sizeof(T) * K * 8` bits integer. For example,
/// `StaticVector<uint32_t, 4>` is treated as an 128-bits integer. The counter
/// start with all elements being zero. The value of the integer can be
/// calculated as, \f$c_0 + c_1 M + c_2 M^2 +\cdots + c_{K-1} M^{K - 1}\f$,
/// where \f$c_i\f$ is the \f$i\f$th element in the StaticVector and has a
/// range from zero to \f$M - 1\f$, \f$M\f$ is the largest number of type `T`
/// plus one, that is \f$2^n\f$ with \f$n\f$ being the number of bits in type
/// `T`.
///
/// The second type is blocks of counters of the first type. For example,
/// `StaticVector<ctr_type, Blocks, BlockTraits>`, where `ctr_type` is a
/// counter of the first type. When set and incremented using methods in this
/// class, all `Blocks` counters are maintained such that, they are always
/// distinctive. This is done by reserving eight bits as counter IDs.
/// Therefore, there can be at most 256 blocks. The eight bits reserved are the
/// 8 higher bits in the last element of each counter. In a little-endian
/// representation, such as on x86, the last bytes in the memory of counter is
/// reserved. The increment works by increment each counter the same way as in
/// the first type, except that the last element, \f$c_{K-1}\f$ has a range
/// from zero to \f$2^{n - 8} - 1\f$ where \f$n\f$ is the number of bits in
/// type `T`. Therefore, in the extreme case where `ctr_type` is
/// `StaticVector<uint8_t, 1>`, increment won't change the counter at all.
template <typename T, std::size_t K, typename Traits>
class StaticCounter<StaticVector<T, K, Traits> >
{
    public :

    typedef StaticVector<T, K, Traits> ctr_type;

    /// \brief Set the counter to a given value
    static void set (ctr_type &ctr, const ctr_type &c) {ctr = c;}

    /// \brief Set a block of counters given the value of the first counter
    template <std::size_t Blocks, typename BlockTraits>
    static void set (StaticVector<ctr_type, Blocks, BlockTraits> &ctr,
            const ctr_type &c)
    {
        ctr.front() = c;
        set_block<1>(ctr, cxx11::integral_constant<bool, 1 < Blocks>());
    }

    /// \brief Reset a counter to zero
    static void reset (ctr_type &ctr)
    {std::memset(static_cast<void *>(ctr.data()), 0, sizeof(T) * K);}

    /// \brief Reset a block of counters with the first set to zero
    template <std::size_t Blocks, typename BlockTraits>
    static void reset (StaticVector<ctr_type, Blocks, BlockTraits> &ctr)
    {
        reset(ctr.front());
        set_block<1>(ctr, cxx11::integral_constant<bool, 1 < Blocks>());
    }

    /// \brief Increment the counter by one
    static void increment (ctr_type &ctr)
    {increment_single<0>(ctr, cxx11::integral_constant<bool, 1 < K>());}

    /// \brief Increment each counter in a block by one
    template <std::size_t Blocks, typename BlockTraits>
    static void increment (StaticVector<ctr_type, Blocks, BlockTraits> &ctr)
    {increment_block<0>(ctr, cxx11::true_type());}

    /// \brief Increment a counter by a given value
    static void increment (ctr_type &ctr, T nskip)
    {
        if (nskip == 0)
            return;

        if (K == 1) {
            ctr.front() += nskip;
            return;
        }

        if (nskip == 1) {
            increment(ctr);
            return;
        }

        if (ctr.front() <= max_ - nskip) {
            ctr.front() += nskip;
            return;
        }

        nskip -= max_ - ctr.front();
        ctr.front() = max_;
        increment(ctr);
        ctr.front() = nskip - 1;
    }

    /// \brief Increment each counter in a block by a given value
    template <std::size_t Blocks, typename BlockTraits>
    static void increment (StaticVector<ctr_type, Blocks, BlockTraits> &ctr,
            T nskip)
    {
        if (nskip == 0)
            return;

        if (nskip == 1) {
            increment(ctr);
            return;
        }

        increment_block<0>(ctr, nskip, cxx11::true_type());
    }

    private :

    static VSMC_CONSTEXPR const T max_ =
        internal::StaticCounterMask<T, cxx11::is_unsigned<T>::value>::max_val;

    static VSMC_CONSTEXPR const T mask_hi_ =
        internal::StaticCounterMask<T, cxx11::is_unsigned<T>::value>::mask_hi;

    static VSMC_CONSTEXPR const T mask_lo_ =
        internal::StaticCounterMask<T, cxx11::is_unsigned<T>::value>::mask_lo;

    template <std::size_t>
    static void increment_single (ctr_type &ctr, cxx11::false_type)
    {++ctr.back();}

    template <std::size_t N>
    static void increment_single (ctr_type &ctr, cxx11::true_type)
    {
        if (++ctr[Position<N>()] != 0)
            return;

        increment_single<N + 1>(ctr,
                cxx11::integral_constant<bool, N + 2 < K>());
    }

    template <std::size_t, std::size_t Blocks, typename BlockTraits>
    static void set_block (StaticVector<ctr_type, Blocks, BlockTraits> &,
            cxx11::false_type) {}

    template <std::size_t B, std::size_t Blocks, typename BlockTraits>
    static void set_block (StaticVector<ctr_type, Blocks, BlockTraits> &ctr,
            cxx11::true_type)
    {
        T m = ctr[Position<B - 1>()].back() & mask_lo_;
        m >>= sizeof(T) * 8 - 8;
        ++m;
        m <<= sizeof(T) * 8 - 8;

        ctr[Position<B>()] = ctr[Position<B - 1>()];
        ctr[Position<B>()].back() &= mask_hi_;
        ctr[Position<B>()].back() ^= m;
        set_block<B + 1>(ctr,
                cxx11::integral_constant<bool, B + 1 < Blocks>());
    }

    template <std::size_t, std::size_t Blocks, typename BlockTraits>
    static void increment_block (
            StaticVector<ctr_type, Blocks, BlockTraits> &,
            cxx11::false_type) {}

    template <std::size_t B, std::size_t Blocks, typename BlockTraits>
    static void increment_block (
            StaticVector<ctr_type, Blocks, BlockTraits> &ctr,
            cxx11::true_type)
    {
        increment_block_ctr(ctr[Position<B>()]);
        increment_block<B + 1>(ctr,
                cxx11::integral_constant<bool, B + 1 < Blocks>());
    }

    template <std::size_t, std::size_t Blocks, typename BlockTraits>
    static void increment_block (
            StaticVector<ctr_type, Blocks, BlockTraits> &,
            T, cxx11::false_type) {}

    template <std::size_t B, std::size_t Blocks, typename BlockTraits>
    static void increment_block (
            StaticVector<ctr_type, Blocks, BlockTraits> &ctr,
            T nskip, cxx11::true_type)
    {
        increment_block_ctr(ctr[Position<B>()], nskip);
        increment_block<B + 1>(ctr, nskip,
                cxx11::integral_constant<bool, B + 1 < Blocks>());
    }

    static void increment_block_ctr (ctr_type &ctr)
    {increment_block_single<0>(ctr, cxx11::integral_constant<bool, 1 < K>());}

    static void increment_block_ctr (ctr_type &ctr, T nskip)
    {
        increment_block_nskip(ctr, nskip,
                cxx11::integral_constant<bool, 1 < K>());
    }

    template <std::size_t>
    static void increment_block_single (ctr_type &ctr, cxx11::false_type)
    {
        T m = ctr.back() & mask_lo_;
        ctr.back() <<= 8;
        ctr.back() >>= 8;
        ++ctr.back();
        ctr.back() &= mask_hi_;
        ctr.back() ^= m;
    }

    template <std::size_t N>
    static void increment_block_single (ctr_type &ctr, cxx11::true_type)
    {
        if (++ctr[Position<N>()] != 0)
            return;

        increment_block_single<N + 1>(ctr,
                cxx11::integral_constant<bool, N + 2 < K>());
    }

    static void increment_block_nskip (ctr_type &ctr, T nskip,
            cxx11::false_type)
    {
        T m = ctr.back() & mask_lo_;
        T b = ctr.back();
        b <<= 8;
        b >>= 8;

        if (nskip <= mask_hi_ - b) {
            b += nskip;
            b &= mask_hi_;
            b ^= m;
            ctr.back() = b;
            return;
        }

        nskip -= mask_hi_ - b - 1;
        while (nskip > mask_hi_)
            nskip -= mask_hi_;
        b = nskip;
        b &= mask_hi_;
        b ^= m;
        ctr.back() = b;
    }

    static void increment_block_nskip (ctr_type &ctr, T nskip,
            cxx11::true_type)
    {
        if (nskip == 0)
            return;

        if (nskip == 1) {
            increment_block_ctr(ctr);
            return;
        }

        if (ctr.front() <= max_ - nskip) {
            ctr.front() += nskip;
            return;
        }

        nskip -= max_ - ctr.front();
        ctr.front() = max_;
        increment_block_ctr(ctr);
        ctr.front() = nskip - 1;
    }
}; // struct StaticCounter

} // namespace vsmc

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // VSMC_UTILITY_STATIC_VECTOR_HPP
