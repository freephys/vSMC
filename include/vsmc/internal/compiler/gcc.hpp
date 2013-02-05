#ifndef VSMC_INTERNAL_COMPILER_GCC_HPP
#define VSMC_INTERNAL_COMPILER_GCC_HPP

#define VSMC_GNUC_NONEXIST 1000000UL

#ifndef VSMC_STATIC_INLINE
#define VSMC_STATIC_INLINE static inline
#endif

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L

// C++11 language features

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= VSMC_GNUC_NONEXIST
#ifndef VSMC_HAS_CXX11_ACCESS_CONTROL_SFINAE
#define VSMC_HAS_CXX11_ACCESS_CONTROL_SFINAE 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 7
#ifndef VSMC_HAS_CXX11_ALIAS_TEMPLATES
#define VSMC_HAS_CXX11_ALIAS_TEMPLATES 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= VSMC_GNUC_NONEXIST
#ifndef VSMC_HAS_CXX11_ALIGNAS
#define VSMC_HAS_CXX11_ALIGNAS 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= VSMC_GNUC_NONEXIST
#ifndef VSMC_HAS_CXX11_ATTRIBUTES
#define VSMC_HAS_CXX11_ATTRIBUTES 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 4
#ifndef VSMC_HAS_CXX11_AUTO_TYPE
#define VSMC_HAS_CXX11_AUTO_TYPE 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
#ifndef VSMC_HAS_CXX11_CONSTEXPR
#define VSMC_HAS_CXX11_CONSTEXPR 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 3
#ifndef VSMC_HAS_CXX11_DECLTYPE
#define VSMC_HAS_CXX11_DECLTYPE 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 3
#ifndef VSMC_HAS_CXX11_DEFAULT_FUNCTION_TEMPLATE_ARGS
#define VSMC_HAS_CXX11_DEFAULT_FUNCTION_TEMPLATE_ARGS 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 4
#ifndef VSMC_HAS_CXX11_DEFAULTED_FUNCTIONS
#define VSMC_HAS_CXX11_DEFAULTED_FUNCTIONS 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 7
#ifndef VSMC_HAS_CXX11_DELEGATING_CONSTRUCTORS
#define VSMC_HAS_CXX11_DELEGATING_CONSTRUCTORS 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 4
#ifndef VSMC_HAS_CXX11_DELETED_FUNCTIONS
#define VSMC_HAS_CXX11_DELETED_FUNCTIONS 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 5
#ifndef VSMC_HAS_CXX11_EXPLICIT_CONVERSIONS
#define VSMC_HAS_CXX11_EXPLICIT_CONVERSIONS 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 4
#ifndef VSMC_HAS_CXX11_GENERALIZED_INITIALIZERS
#define VSMC_HAS_CXX11_GENERALIZED_INITIALIZERS 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
#ifndef VSMC_HAS_CXX11_IMPLICIT_MOVES
#define VSMC_HAS_CXX11_IMPLICIT_MOVES 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= VSMC_GNUC_NONEXIST
#ifndef VSMC_HAS_CXX11_INHERITING_CONSTRUCTORS
#define VSMC_HAS_CXX11_INHERITING_CONSTRUCTORS 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 4
#ifndef VSMC_HAS_CXX11_INLINE_NAMESPACES
#define VSMC_HAS_CXX11_INLINE_NAMESPACES 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 5
#ifndef VSMC_HAS_CXX11_LAMBDAS
#define VSMC_HAS_CXX11_LAMBDAS 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 5
#ifndef VSMC_HAS_CXX11_LOCAL_TYPE_TEMPLATE_ARGS
#define VSMC_HAS_CXX11_LOCAL_TYPE_TEMPLATE_ARGS 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
#ifndef VSMC_HAS_CXX11_NOEXCEPT
#define VSMC_HAS_CXX11_NOEXCEPT 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 7
#ifndef VSMC_HAS_CXX11_NONSTATIC_MEMBER_INIT
#define VSMC_HAS_CXX11_NONSTATIC_MEMBER_INIT 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
#ifndef VSMC_HAS_CXX11_NULLPTR
#define VSMC_HAS_CXX11_NULLPTR 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 7
#ifndef VSMC_HAS_CXX11_OVERRIDE_CONTROL
#define VSMC_HAS_CXX11_OVERRIDE_CONTROL 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
#ifndef VSMC_HAS_CXX11_RANGE_FOR
#define VSMC_HAS_CXX11_RANGE_FOR 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 5
#ifndef VSMC_HAS_CXX11_RAW_STRING_LITERALS
#define VSMC_HAS_CXX11_RAW_STRING_LITERALS 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= VSMC_GNUC_NONEXIST
#ifndef VSMC_HAS_CXX11_REFERENCE_QUALIFIED_FUNCTIONS
#define VSMC_HAS_CXX11_REFERENCE_QUALIFIED_FUNCTIONS 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 3
#ifndef VSMC_HAS_CXX11_RVALUE_REFERENCES
#define VSMC_HAS_CXX11_RVALUE_REFERENCES 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 3
#ifndef VSMC_HAS_CXX11_STATIC_ASSERT
#define VSMC_HAS_CXX11_STATIC_ASSERT 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 4
#ifndef VSMC_HAS_CXX11_STRONG_ENUMS
#define VSMC_HAS_CXX11_STRONG_ENUMS 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 4
#ifndef VSMC_HAS_CXX11_TRAILING_RETURN
#define VSMC_HAS_CXX11_TRAILING_RETURN 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 5
#ifndef VSMC_HAS_CXX11_UNICODE_LITERALS
#define VSMC_HAS_CXX11_UNICODE_LITERALS 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
#ifndef VSMC_HAS_CXX11_UNRESTRICTED_UNIONS
#define VSMC_HAS_CXX11_UNRESTRICTED_UNIONS 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 7
#ifndef VSMC_HAS_CXX11_USER_LITERALS
#define VSMC_HAS_CXX11_USER_LITERALS 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 4
#ifndef VSMC_HAS_CXX11_VARIADIC_TEMPLATES
#define VSMC_HAS_CXX11_VARIADIC_TEMPLATES 1
#endif
#endif

// C++11 library features

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 4
#ifndef VSMC_HAS_CXX11LIB_CHRONO
#define VSMC_HAS_CXX11LIB_CHRONO 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 4
#ifndef VSMC_HAS_CXX11LIB_FUNCTIONAL
#define VSMC_HAS_CXX11LIB_FUNCTIONAL 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 5
#ifndef VSMC_HAS_CXX11LIB_FUTURE
#define VSMC_HAS_CXX11LIB_FUTURE 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 5
#ifndef VSMC_HAS_CXX11LIB_RANDOM
#define VSMC_HAS_CXX11LIB_RANDOM 1
#endif
#endif

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 5
#ifndef VSMC_HAS_CXX11LIB_THREAD
#define VSMC_HAS_CXX11LIB_THREAD 1
#endif
#endif

#endif // defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L

#endif // VSMC_INTERNAL_COMPILER_GCC_HPP
