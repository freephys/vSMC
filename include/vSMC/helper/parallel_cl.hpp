#ifndef V_SMC_HELPER_PARALLEL_CL_HPP
#define V_SMC_HELPER_PARALLEL_CL_HPP

#define __CL_ENABLE_EXCEPTIONS

#include <vSMC/internal/common.hpp>
#include <vSMC/helper/parallel_cl/cl.hpp>

namespace vSMC {

/// Called before resampling in an OpenCL sampler
/// \ingroup OpenCL
template <typename T>
void cl_pre_resampling (T &state)
{
    state.pre_resampling();
}

/// Called after resampling in an OpenCL sampler
/// \ingroup OpenCL
template <typename T>
void cl_post_resampling (T &state)
{
    state.post_resampling();
}

/// \brief Particle::value_type subtype
/// \ingroup OpenCL
///
/// \tparam Dim The dimension of the state parameter vector
/// \tparam T The type of the value of the state parameter vector
///
/// \note If the compiler generated copy construtor and assignement operator
/// are not fine for the derived class, the derived class has to copy this base
/// class itself.
template <unsigned Dim, typename T>
class StateCL
{
    public :

    /// The type of the size of the particle set
    typedef V_SMC_INDEX_TYPE size_type;

    /// The type of state parameters (cl_float or cl_double)
    typedef T state_type;

    /// The type of the matrix of states returned by state_host()
    typedef Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> state_mat_type;

    /// The type of the vector of weights returned by weight_host()
    typedef Eigen::Matrix<T, Eigen::Dynamic, 1> weight_vec_type;

    /// The type of the vector of accept counts returned by accept_host()
    typedef Eigen::Matrix<cl_uint, Eigen::Dynamic, 1> accept_vec_type;

    /// The type of the vector of copy sources
    typedef Eigen::Matrix<cl_uint, Eigen::Dynamic, 1> copy_vec_type;

    explicit StateCL (size_type N) :
        size_(N),
        platform_created_(false), context_created_(false),
        device_created_(false), command_queue_created_(false),
        program_created_(false), build_(false),
        global_size_(N), local_size_(0),
        state_host_(Dim, N), weight_host_(N), accept_host_(N), copy_host_(N)
    {}

    virtual ~StateCL () {}

    /// \brief The dimension of the problem
    ///
    /// \return The dimension of the parameter vector
    static unsigned dim ()
    {
        return Dim;
    }

    /// \brief The dimension of the problem
    ///
    /// \return The dimension of the parameter vector
    size_type size () const
    {
        return size_;
    }

    const std::vector<cl::Platform> &platform () const
    {
        return platform_;
    }

    void platform (const std::vector<cl::Platform> &plat)
    {
        platform_ = plat;
        platform_created_ = true;
    }

    bool platform_created () const
    {
        return platform_created_;
    }

    const cl::Context &context () const
    {
        return context_;
    }

    void context (const cl::Context &ctx)
    {
        context_ = ctx;
        setup_buffer();
        context_created_ = true;
    }

    const bool context_created () const
    {
        return context_created_;
    }

    const std::vector<cl::Device> &device () const
    {
        return device_;
    }

    void device (const std::vector<cl::Device> &dev)
    {
        device_ = dev;
    }

    bool device_created () const
    {
        return device_created_;
    }

    const cl::CommandQueue &command_queue () const
    {
        return command_queue_;
    }

    void command_queue (const cl::CommandQueue &queue)
    {
        command_queue_ = queue;
        command_queue_created_ = true;
    }

    bool command_queue_created () const
    {
        return command_queue_created_;
    }

    const cl::Program &program () const
    {
        return program_;
    }

    void program (const cl::Program &prg)
    {
        program_ = prg;
        program_created_ = true;
    }

    bool program_created () const
    {
        return program_created_;
    }

    /// \brief Set a new local group size
    ///
    /// \param lsize The size of the work group.
    void local_size (std::size_t lsize)
    {
        local_size_ = lsize;
        if (local_size_ && size_ % local_size_)
            global_size_ = (size_ / local_size_ + 1) * local_size_;
        else
            global_size_ = size_;
    }

    /// \brief The global cl::NDRange used by kernel calls
    ///
    /// \return A cl::NDRange object that is valid for the current setting of
    /// local group size and larger than the particle set size.
    cl::NDRange global_nd_range () const
    {
        return cl::NDRange(global_size_);
    }

    /// \brief The local cl::NDRange used by kernel calls
    ///
    /// \return A cl::NDRange object that can be used as the group size when
    /// call a kernel. It is the same size of set by local_size() or
    /// cl::NullRange if local_size() is set with zero.
    cl::NDRange local_nd_range () const
    {
        if (local_size_)
            return cl::NDRange(local_size_);
        else
            return cl::NullRange;
    }

    /// \brief Read only access to the memory buffer on the device that stores
    /// the states
    ///
    /// \return A const reference to the cl::Buffer object. It is read only
    /// only with regard to the host code. The device code have read and write
    /// access to the buffer.
    const cl::Buffer &state_device () const
    {
        return state_device_;
    }

    /// \brief Read only access to the states
    ///
    /// \return A const reference to a matrix of the states which have the same
    /// contents as stored in the device memory buffer of state_device().
    ///
    /// \note This call will read the buffer from the device first, and
    /// therefore is expensive.
    const state_mat_type &state_host () const
    {
        command_queue_.enqueueReadBuffer(state_device_, 1, 0,
                sizeof(T) * size_ * Dim, (void *) state_host_.data());
        return state_host_;
    }

    /// \brief Read only access to the memory buffer on the device that stores
    /// the weights
    ///
    /// \return A const reference to the cl::Buffer object. It is read only
    /// only with regard to the host code. The device code have read and write
    /// access to the buffer.
    ///
    /// \note Despite the names, this is not the current weights of the
    /// particle system. It is intended to be used by clients to store and
    /// manipulate the weights, for example the incremental weights. For access
    /// to actual weights and log weights, always use the interface of
    /// Particle.
    const cl::Buffer &weight_device () const
    {
        return weight_device_;
    }

    /// \brief Read only access to the weights
    ///
    /// \return A const reference to a matrix of the states which have the same
    /// contents as stored in the device memory buffer of weight_device().
    ///
    /// \note This call will read the buffer from the device first, and
    /// therefore is expensive.
    const weight_vec_type &weight_host () const
    {
        command_queue_.enqueueReadBuffer(weight_device_, 1, 0,
                sizeof(T) * size_, (void *) weight_host_.data());
        return weight_host_;
    }

    /// \brief Read only access to the memory buffer on the device that stores
    /// the accept counts
    ///
    /// \return A const reference to the cl::Buffer object. It is read only
    /// only with regard to the host code. The device code have read and write
    /// access to the buffer.
    ///
    /// \note Despite the names, this is not the current accept counts of the
    /// particle system. It is intended to be used by clients to store and
    /// manipulate the accept counts.
    const cl::Buffer &accept_device () const
    {
        return accept_device_;
    }

    /// \brief Read only access to the accept counts
    ///
    /// \return A const reference to a matrix of the states which have the same
    /// contents as stored in the device memory buffer of accept_device().
    ///
    /// \note This call will read the buffer from the device first, and
    /// therefore is expensive.
    const accept_vec_type &accept_host () const
    {
        command_queue_.enqueueReadBuffer(accept_device_, 1, 0,
                sizeof(cl_uint) * size_, (void *) accept_host_.data());
        return accept_host_;
    }

    /// \brief Setup the OpenCL environment
    ///
    /// \param dev_type The type of the device intended to use
    void setup (cl_device_type dev_type = CL_DEVICE_TYPE_GPU)
    {
        cl::Platform::get(&platform_);
        platform_created_ = true;

        cl_context_properties context_properties[] = {
            CL_CONTEXT_PLATFORM, (cl_context_properties)(platform_[0])(), 0
        };
        context_ = cl::Context(dev_type, context_properties);
        setup_buffer();
        context_created_ = true;

        device_= context_.getInfo<CL_CONTEXT_DEVICES>();
        device_created_ = true;

        command_queue_ = cl::CommandQueue(context_, device_[0], 0);
        command_queue_created_ = true;
    }

    /// \brief Build the program
    ///
    /// \param source The source in C-Style string format (NOT THE FILE NAME)
    /// \param flags The compiler flags passed to the OpenCL compiler
    /// \param sampler A reference to a sampler which will manipulate this
    /// particle set.
    ///
    /// \note The call to build() will call setup() unless the user have set
    /// platform, context, device and command_queue correctly himself. If these
    /// are set by the user, than the library does not check for its validity.
    ///
    /// \note When building the program, the user can assume the following
    /// things,
    /// \li The Rand123 library's \c philox.h, \c threefry.h, and \c u01.h
    /// headers are included before the source.
    /// \li A type \c state_type which is the same as the host \c state_type
    /// are defined before the source.
    /// \li A type \c state_struct which looks like the following are defined
    /// before the souce.
    /// \code
    /// typedef {
    ///     state_type param1;
    ///     state_type param2;
    ///     ...
    ///     state_type paramDim;
    /// } state_struct;
    /// \endcode
    /// where The last \c paramDim will clearly be replaced by the proper
    /// number.
    /// \li A constant \c Dim of type \c uint is defined before the source
    /// which is the same as the dimension of this StateCL object.
    template <typename State>
    void build (const char *source, const char *flags, Sampler<State> &sampler)
    {
        if (!(platform_created_ && context_created_ && device_created_ &&
                    command_queue_created_))
        {
            setup();
        }

        if (!program_created_) {
            std::stringstream ss;
            ss << "__constant uint Dim = " << Dim << ";\n";
            if (sizeof(T) == sizeof(cl_float))
                ss << "typedef float state_type;\n";
            else if (sizeof(T) == sizeof(cl_double))
                ss << "typedef double state_type;\n";
            ss << "typedef struct {\n";
            for (unsigned d = 0; d != Dim; ++d)
                ss << "state_type param" << d + 1 << ";\n";
            ss << "} state_struct;\n";
            ss << "#include <vSMC/helper/parallel_cl/common.cl>\n";
            ss << source << '\n';
            cl::Program::Sources src(1, std::make_pair(ss.str().c_str(), 0));
            program_ = cl::Program(context_, src);
            program_created_ = true;
        }

        program_.build(device_, flags);

        kernel_copy_ = cl::Kernel(program_, "copy");
        sampler.particle().pre_resampling(
                cl_pre_resampling<StateCL<Dim, T> >);
        sampler.particle().post_resampling(
                cl_post_resampling<StateCL<Dim, T> >);
        build_ = true;
    }

    /// \brief Check the status of build
    ///
    /// \return \b true if the last call to build is successful.
    bool build () const
    {
        return build_;
    }

    virtual void copy (size_type from, size_type to)
    {
        copy_host_[to] = from;
    }

    void pre_resampling ()
    {
        for (size_type i = 0; i != size_; ++i)
            copy_host_[i] = i;
    }

    void post_resampling ()
    {
        assert(build_);

        command_queue_.enqueueWriteBuffer(copy_device_, 1, 0,
                sizeof(cl_uint) * size_, (void *) copy_host_.data());
        kernel_copy_.setArg(0, (std::size_t) size_);
        kernel_copy_.setArg(1, state_device_);
        kernel_copy_.setArg(2, copy_device_);
        command_queue_.enqueueNDRangeKernel(kernel_copy_, cl::NullRange,
                global_nd_range(), local_nd_range());
    }

    private :

    size_type size_;

    std::vector<cl::Platform> platform_;
    cl::Context context_;
    std::vector<cl::Device> device_;
    cl::CommandQueue command_queue_;
    cl::Program program_;
    cl::Kernel kernel_copy_;

    bool platform_created_;
    bool context_created_;
    bool device_created_;
    bool command_queue_created_;
    bool program_created_;
    bool build_;

    std::size_t global_size_;
    std::size_t local_size_;

    cl::Buffer state_device_;
    cl::Buffer weight_device_;
    cl::Buffer accept_device_;
    cl::Buffer copy_device_;

    mutable state_mat_type state_host_;
    mutable weight_vec_type weight_host_;
    mutable accept_vec_type accept_host_;
    mutable copy_vec_type copy_host_;

    void setup_buffer ()
    {
        state_device_ = cl::Buffer(context_, CL_MEM_READ_WRITE,
                sizeof(T) * size_ * Dim);
        weight_device_ = cl::Buffer(context_, CL_MEM_READ_WRITE,
                sizeof(T) * size_);
        accept_device_ = cl::Buffer(context_, CL_MEM_READ_WRITE,
                sizeof(cl_uint) * size_);
        copy_device_ = cl::Buffer(context_, CL_MEM_READ_WRITE,
                sizeof(cl_uint) * size_);
    }
}; // class StateCL

/// \brief Sampler::init_type subtype
/// \ingroup OpenCL
///
/// \tparam T A subtype of StateCL
template <typename T>
class InitializeCL
{
    public :

    /// \brief Construct a InitializeCL object from the name of the kernel
    ///
    /// \param kernel_name The name of the kernel in the source file build by
    /// StateCL::build. Note that the first four arguments to the kernel are
    /// set automatically and are size of the particle set, the states, output
    /// buffer for the log_weights, and output buffer for the accept counts,
    /// respectively.
    ///
    /// A valid kernel declaration looks like
    /// \code
    /// __kernel void init (
    ///     size_t size, state_struct *state,
    ///     state_type *weight, uint *accept)
    /// \endcode
    explicit InitializeCL (const char *kernel_name) :
        kernel_name_(kernel_name), kernel_created_(false) {}

    InitializeCL (const InitializeCL<T> &other) :
        kernel_(other.kernel_), kernel_name_(other.kernel_name_),
        kernel_created_(other.kernel_created_) {}

    const InitializeCL<T> &operator= (const InitializeCL<T> &other)
    {
        if (this != &other) {
            kernel_ = other.kernel_;
            kernel_name_ = other.kernel_name_;
            kernel_created_ = other.kernel_created_;
        }

        return *this;
    }

    virtual ~InitializeCL () {}

    virtual unsigned operator() (Particle<T> &particle, void *param)
    {
        create_kernel(particle);
        initialize_param(particle, param);
        pre_processor(particle);
        // TODO more control over local size
        particle.value().command_queue().enqueueNDRangeKernel(kernel_,
                cl::NullRange,
                particle.value().global_nd_range(),
                particle.value().local_nd_range());
        // TODO more efficient weight copying
        const typename T::weight_vec_type &weight =
            particle.value().weight_host();
        if (sizeof(typename T::state_type) == sizeof(double)) {
            particle.set_log_weight(
                    reinterpret_cast<const double *>(weight.data()));
        } else {
            weight_.resize(particle.size());
            for (typename Particle<T>::size_type i = 0;
                    i != particle.size(); ++i) {
                weight_[i] = weight[i];
            }
            particle.set_log_weight(weight_);
        }
        post_processor(particle);

        return particle.value().accept_host().sum();
    }

    virtual void initialize_param (Particle<T> &particle, void *param) {}
    virtual void pre_processor (Particle<T> &particle) {}
    virtual void post_processor (Particle<T> &particle) {}

    cl::Kernel &kernel ()
    {
        return kernel_;
    }

    const cl::Kernel &kernel () const
    {
        return kernel_;
    }

    void create_kernel (const Particle<T> &particle)
    {
        assert(particle.value().build());

        if (!kernel_created_) {
            kernel_ = cl::Kernel(
                    particle.value().program(), kernel_name_.c_str());
            kernel_created_ = true;
        }

        kernel_.setArg(0, (std::size_t) particle.size());
        kernel_.setArg(1, particle.value().state_device());
        kernel_.setArg(2, particle.value().weight_device());
        kernel_.setArg(3, particle.value().accept_device());
    }

    private :

    cl::Kernel kernel_;
    std::string kernel_name_;
    bool kernel_created_;
    typename Particle<T>::weight_type weight_;
};

/// \brief Sampler::move_type subtype
/// \ingroup OpenCL
///
/// \tparam T A subtype of StateCL
template <typename T>
class MoveCL
{
    public :

    /// \brief Construct a MoveCL object from the name of the kernel
    ///
    /// \param kernel_name The name of the kernel in the source file build by
    /// StateCL::build. Note that the first five arguments to the kernel are
    /// set automatically and are size of the particle set, the iteration
    /// number, the states, output buffer for the log_weights, and output
    /// buffer for the accept counts, respectively.
    ///
    /// A valid kernel declaration looks like
    /// \code
    /// __kernel void move (
    ///     size_t size, uint iter, state_struct *state,
    ///     state_type *weight, uint *accept)
    /// \endcode
    explicit MoveCL (const char *kernel_name) :
        kernel_name_(kernel_name), kernel_created_(false) {}

    MoveCL (const MoveCL<T> &other) :
        kernel_(other.kernel_), kernel_name_(other.kernel_name_),
        kernel_created_(other.kernel_created_) {}

    const MoveCL<T> &operator= (const MoveCL<T> &other)
    {
        if (this != &other) {
            kernel_ = other.kernel_;
            kernel_name_ = other.kernel_name_;
            kernel_created_ = other.kernel_created_;
        }

        return *this;
    }

    virtual ~MoveCL () {}

    virtual unsigned operator() (unsigned iter, Particle<T> &particle)
    {
        create_kernel(iter, particle);
        pre_processor(iter, particle);
        // TODO more control over local size
        particle.value().command_queue().enqueueNDRangeKernel(kernel_,
                cl::NullRange,
                particle.value().global_nd_range(),
                particle.value().local_nd_range());
        // TODO more efficient weight copying
        const typename T::weight_vec_type &weight =
            particle.value().weight_host();
        if (sizeof(typename T::state_type) == sizeof(double)) {
            particle.add_log_weight(
                    reinterpret_cast<const double *>(weight.data()));
        } else {
            weight_.resize(particle.size());
            for (typename Particle<T>::size_type i = 0;
                    i != particle.size(); ++i) {
                weight_[i] = weight[i];
            }
            particle.add_log_weight(weight_);
        }
        post_processor(iter, particle);

        return particle.value().accept_host().sum();
    }

    virtual void pre_processor (unsigned iter, Particle<T> &particle) {}
    virtual void post_processor (unsigned iter, Particle<T> &particle) {}

    cl::Kernel &kernel ()
    {
        return kernel_;
    }

    const cl::Kernel &kernel () const
    {
        return kernel_;
    }

    void create_kernel (unsigned iter, const Particle<T> &particle)
    {
        assert(particle.value().build());

        if (!kernel_created_) {
            kernel_ = cl::Kernel(
                    particle.value().program(), kernel_name_.c_str());
            kernel_created_ = true;
        }

        kernel_.setArg(0, (std::size_t) particle.size());
        kernel_.setArg(1, (cl_uint) iter);
        kernel_.setArg(2, particle.value().state_device());
        kernel_.setArg(3, particle.value().weight_device());
        kernel_.setArg(4, particle.value().accept_device());
    }

    private :

    cl::Kernel kernel_;
    std::string kernel_name_;
    bool kernel_created_;
    typename Particle<T>::weight_type weight_;
}; // class MoveCL

/// \brief Non-direct Monitor::eval_type subtype
/// \ingroup OpenCL
///
/// \tparam T A Subtype of StateCL
/// \tparam Dim The dimension of the monitor
///
/// \note Currently Dim cannot be larger than particle set size
template <typename T, unsigned Dim>
class MonitorCL
{
    public :

    /// \brief Construct a MonitorCL object from the name of the kernel
    ///
    /// \param kernel_name The name of the kernel in the source file build by
    /// StateCL::build. Note that the first five arguments to the kernel are
    /// set automatically and are size of the particle set, the iteration
    /// number, the dimension of the monitor, the states, and output buffer for
    /// results, respectively.
    ///
    /// A valid kernel declaration looks like
    /// \code
    /// __kernel void monitor_eval (
    ///     size_t size, uint iter, uint dim, state_struct *state,
    ///     state_type *buffer)
    /// \endcode
    /// where \c buffer is a row major \c dim by \c size matrix
    explicit MonitorCL (const char *kernel_name) :
        kernel_name_(kernel_name), kernel_created_(false) {}

    MonitorCL (const MonitorCL<T, Dim> &other) :
        kernel_(other.kernel_), kernel_name_(other.kernel_name_),
        kernel_created_(other.kernel_created_),
        buffer_device_(other.buffer_device_) {}

    const MonitorCL<T, Dim> &operator= (const MonitorCL<T, Dim> &other)
    {
        if (this != &other) {
            kernel_         = other.kernel_;
            kernel_name_    = other.kernel_name_;
            kernel_created_ = other.kernel_created_;
            buffer_device_  = other.buffer_device_;
        }

        return *this;
    }

    virtual ~MonitorCL () {}

    virtual void operator() (unsigned iter, const Particle<T> &particle,
        typename Monitor<T>::integrand_mat_type &res)
    {
        create_kernel(iter, particle);
        pre_processor(iter, particle);
        particle.value().command_queue().enqueueNDRangeKernel(kernel_,
                cl::NullRange,
                particle.value().global_nd_range(),
                particle.value().local_nd_range());
        if (sizeof(typename T::state_type) == sizeof(double)) {
            particle.value().command_queue().enqueueReadBuffer(buffer_device_,
                    1, 0,
                    sizeof(typename T::state_type) * particle.size() * Dim,
                    (void *) res.data());
        } else {
            buffer_host_.resize(Dim, particle.size());
            particle.value().command_queue().enqueueReadBuffer(buffer_device_,
                    1, 0,
                    sizeof(typename T::state_type) * particle.size() * Dim,
                    (void *) buffer_host_.data());
            for (unsigned d = 0; d != Dim; ++d) {
                for (typename Particle<T>::size_type i = 0;
                        i != particle.size(); ++i)
                    res(d, i) = buffer_host_(d, i);
            }
        }
        post_processor(iter, particle);
    }

    virtual void pre_processor (unsigned iter, const Particle<T> &particle) {}
    virtual void post_processor (unsigned iter, const Particle<T> &particle) {}

    static unsigned dim ()
    {
        return Dim;
    }

    cl::Kernel kernel ()
    {
        return kernel_;
    }

    const cl::Kernel kernel () const
    {
        return kernel_;
    }

    void create_kernel (unsigned iter, const Particle<T> &particle)
    {
        assert(particle.value().build());

        if (!kernel_created_) {
            kernel_ = cl::Kernel(
                    particle.value().program(), kernel_name_.c_str());
            buffer_device_ = cl::Buffer(particle.value().context(),
                    CL_MEM_READ_WRITE,
                    sizeof(typename T::state_type) * Dim * particle.size());
            kernel_created_ = true;
        }

        kernel_.setArg(0, (std::size_t) particle.size());
        kernel_.setArg(1, (cl_uint) iter);
        kernel_.setArg(2, (cl_uint) Dim);
        kernel_.setArg(3, particle.value().state_device());
        kernel_.setArg(4, buffer_device_);
    }

    private :

    cl::Kernel kernel_;
    std::string kernel_name_;
    bool kernel_created_;
    cl::Buffer buffer_device_;
    typename T::state_mat_type buffer_host_;
}; // class MonitorCL

/// \brief Non-direct Path::eval_type subtype
/// \ingroup OpenCL
///
/// \tparam T A subtype of StateCL
template <typename T>
class PathCL
{
    public :

    /// \brief Construct a PathCL object from the name of the kernel
    ///
    /// \param kernel_name The name of the kernel in the source file build by
    /// StateCL::build. Note that the first four arguments to the kernel are
    /// set automatically and are size of the particle set, the iteration
    /// number, the states, and output buffer for results, respectively.
    ///
    /// A valid kernel declaration looks like
    /// \code
    /// __kernel void path_eval (
    ///     size_t size, uint iter, state_struct *state,
    ///     state_type *buffer)
    /// \endcode
    explicit PathCL (const char *kernel_name) :
        kernel_name_(kernel_name), kernel_created_(false) {}

    PathCL (const PathCL<T> &other) :
        kernel_(other.kernel_), kernel_name_(other.kernel_name_),
        kernel_created_(other.kernel_created_),
        buffer_device_(other.buffer_device_) {}

    const PathCL<T> &operator= (const PathCL<T> &other)
    {
        if (this != &other) {
            kernel_         = other.kernel_;
            kernel_name_    = other.kernel_name_;
            kernel_created_ = other.kernel_created_;
            buffer_device_  = other.buffer_device_;
        }

        return *this;
    }

    virtual ~PathCL () {}

    virtual double operator() (unsigned iter, const Particle<T> &particle,
        double *res)
    {
        create_kernel(iter, particle);
        pre_processor(iter, particle);
        particle.value().command_queue().enqueueNDRangeKernel(kernel_,
                cl::NullRange,
                particle.value().global_nd_range(),
                particle.value().local_nd_range());
        if (sizeof(typename T::state_type) == sizeof(double)) {
            particle.value().command_queue().enqueueReadBuffer(buffer_device_,
                    1, 0, sizeof(typename T::state_type) * particle.size(),
                    (void *) res);
        } else {
            buffer_host_.resize(particle.size());
            particle.value().command_queue().enqueueReadBuffer(buffer_device_,
                    1, 0, sizeof(typename T::state_type) * particle.size(),
                    (void *) buffer_host_.data());
            for (typename Particle<T>::size_type i = 0;
                        i != particle.size(); ++i) {
                    res[i] = buffer_host_[i];
            }
        }
        post_processor(iter, particle);

        return this->width_state(iter, particle);
    }

    virtual double width_state (unsigned iter,
            const Particle<T> &particle) = 0;
    virtual void pre_processor (unsigned iter, const Particle<T> &particle) {}
    virtual void post_processor (unsigned iter, const Particle<T> &particle) {}

    cl::Kernel kernel ()
    {
        return kernel_;
    }

    const cl::Kernel kernel () const
    {
        return kernel_;
    }

    void create_kernel (unsigned iter, const Particle<T> &particle)
    {
        assert(particle.value().build());

        if (!kernel_created_) {
            kernel_ = cl::Kernel(
                    particle.value().program(), kernel_name_.c_str());
            buffer_device_ = cl::Buffer(particle.value().context(),
                    CL_MEM_READ_WRITE,
                    sizeof(typename T::state_type) * particle.size());
            kernel_created_ = true;
        }

        kernel_.setArg(0, (std::size_t) particle.size());
        kernel_.setArg(1, (cl_uint) iter);
        kernel_.setArg(2, particle.value().state_device());
        kernel_.setArg(3, buffer_device_);
    }

    private :

    cl::Kernel kernel_;
    std::string kernel_name_;
    bool kernel_created_;
    cl::Buffer buffer_device_;
    typename T::weight_vec_type buffer_host_;
}; // class PathCL

} // namespace vSMC

#endif // V_SMC_HELPER_PARALLEL_CL_HPP
