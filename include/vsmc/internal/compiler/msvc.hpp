//============================================================================
// vSMC/include/vsmc/internal/compiler/msvc.hpp
//----------------------------------------------------------------------------
//                         vSMC: Scalable Monte Carlo
//----------------------------------------------------------------------------
// Copyright (c) 2013,2014, Yan Zhou
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

#ifndef VSMC_INTERNAL_COMPILER_MSVC_HPP
#define VSMC_INTERNAL_COMPILER_MSVC_HPP

#define VSMC_MSVC_NONEXIST 0xFFFFFFFFUL

#define VSMC_MSVC_VERSION _MSC_VER

#if VSMC_MSVC_VERSION >= 1600
#ifndef VSMC_HAS_CXX11_LONG_LONG
#define VSMC_HAS_CXX11_LONG_LONG 1
#endif
#endif

// C++11 language features

#if VSMC_MSVC_VERSION >= VSMC_MSVC_NONEXIST
#ifndef VSMC_HAS_CXX11_ACCESS_CONTROL_SFINAE
#define VSMC_HAS_CXX11_ACCESS_CONTROL_SFINAE 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1800
#ifndef VSMC_HAS_CXX11_ALIAS_TEMPLATES
#define VSMC_HAS_CXX11_ALIAS_TEMPLATES 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1900
#ifndef VSMC_HAS_CXX11_ALIGNAS
#define VSMC_HAS_CXX11_ALIGNAS 1
#endif
#endif

#if VSMC_MSVC_VERSION >= VSMC_MSVC_NONEXIST
#ifndef VSMC_HAS_CXX11_ATTRIBUTES
#define VSMC_HAS_CXX11_ATTRIBUTES 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1600
#ifndef VSMC_HAS_CXX11_AUTO_TYPE
#define VSMC_HAS_CXX11_AUTO_TYPE 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1900
#ifndef VSMC_HAS_CXX11_CONSTEXPR
#define VSMC_HAS_CXX11_CONSTEXPR 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1600
#ifndef VSMC_HAS_CXX11_DECLTYPE
#define VSMC_HAS_CXX11_DECLTYPE 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1800
#ifndef VSMC_HAS_CXX11_DEFAULT_FUNCTION_TEMPLATE_ARGS
#define VSMC_HAS_CXX11_DEFAULT_FUNCTION_TEMPLATE_ARGS 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1900
#ifndef VSMC_HAS_CXX11_DEFAULTED_FUNCTIONS
#define VSMC_HAS_CXX11_DEFAULTED_FUNCTIONS 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1800
#ifndef VSMC_HAS_CXX11_DELEGATING_CONSTRUCTORS
#define VSMC_HAS_CXX11_DELEGATING_CONSTRUCTORS 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1900
#ifndef VSMC_HAS_CXX11_DELETED_FUNCTIONS
#define VSMC_HAS_CXX11_DELETED_FUNCTIONS 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1800
#ifndef VSMC_HAS_CXX11_EXPLICIT_CONVERSIONS
#define VSMC_HAS_CXX11_EXPLICIT_CONVERSIONS 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1800
#ifndef VSMC_HAS_CXX11_GENERALIZED_INITIALIZERS
#define VSMC_HAS_CXX11_GENERALIZED_INITIALIZERS 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1900
#ifndef VSMC_HAS_CXX11_IMPLICIT_MOVES
#define VSMC_HAS_CXX11_IMPLICIT_MOVES 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1900
#ifndef VSMC_HAS_CXX11_INHERITING_CONSTRUCTORS
#define VSMC_HAS_CXX11_INHERITING_CONSTRUCTORS 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1900
#ifndef VSMC_HAS_CXX11_INLINE_NAMESPACES
#define VSMC_HAS_CXX11_INLINE_NAMESPACES 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1600
#ifndef VSMC_HAS_CXX11_LAMBDAS
#define VSMC_HAS_CXX11_LAMBDAS 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1600
#ifndef VSMC_HAS_CXX11_LOCAL_TYPE_TEMPLATE_ARGS
#define VSMC_HAS_CXX11_LOCAL_TYPE_TEMPLATE_ARGS 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1900
#ifndef VSMC_HAS_CXX11_NOEXCEPT
#define VSMC_HAS_CXX11_NOEXCEPT 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1800
#ifndef VSMC_HAS_CXX11_NONSTATIC_MEMBER_INIT
#define VSMC_HAS_CXX11_NONSTATIC_MEMBER_INIT 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1600
#ifndef VSMC_HAS_CXX11_NULLPTR
#define VSMC_HAS_CXX11_NULLPTR 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1700
#ifndef VSMC_HAS_CXX11_OVERRIDE_CONTROL
#define VSMC_HAS_CXX11_OVERRIDE_CONTROL 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1700
#ifndef VSMC_HAS_CXX11_RANGE_FOR
#define VSMC_HAS_CXX11_RANGE_FOR 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1800
#ifndef VSMC_HAS_CXX11_RAW_STRING_LITERALS
#define VSMC_HAS_CXX11_RAW_STRING_LITERALS 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1900
#ifndef VSMC_HAS_CXX11_REFERENCE_QUALIFIED_FUNCTIONS
#define VSMC_HAS_CXX11_REFERENCE_QUALIFIED_FUNCTIONS 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1600
#ifndef VSMC_HAS_CXX11_RVALUE_REFERENCES
#define VSMC_HAS_CXX11_RVALUE_REFERENCES 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1600
#ifndef VSMC_HAS_CXX11_STATIC_ASSERT
#define VSMC_HAS_CXX11_STATIC_ASSERT 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1700
#ifndef VSMC_HAS_CXX11_STRONG_ENUMS
#define VSMC_HAS_CXX11_STRONG_ENUMS 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1900
#ifndef VSMC_HAS_CXX11_THREAD_LOCAL
#define VSMC_HAS_CXX11_THREAD_LOCAL 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1600
#ifndef VSMC_HAS_CXX11_TRAILING_RETURN
#define VSMC_HAS_CXX11_TRAILING_RETURN 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1900
#ifndef VSMC_HAS_CXX11_UNICODE_LITERALS
#define VSMC_HAS_CXX11_UNICODE_LITERALS 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1900
#ifndef VSMC_HAS_CXX11_UNRESTRICTED_UNIONS
#define VSMC_HAS_CXX11_UNRESTRICTED_UNIONS 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1900
#ifndef VSMC_HAS_CXX11_USER_LITERALS
#define VSMC_HAS_CXX11_USER_LITERALS 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1800
#ifndef VSMC_HAS_CXX11_VARIADIC_TEMPLATES
#define VSMC_HAS_CXX11_VARIADIC_TEMPLATES 1
#endif
#endif

// C++11 library features

#if VSMC_MSVC_VERSION >= 1600
#ifndef VSMC_HAS_CXX11LIB_ALGORITHM
#define VSMC_HAS_CXX11LIB_ALGORITHM 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1900
#ifndef VSMC_HAS_CXX11LIB_CHRONO
#define VSMC_HAS_CXX11LIB_CHRONO 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1800
#ifndef VSMC_HAS_CXX11LIB_CMATH
#define VSMC_HAS_CXX11LIB_CMATH 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1700
#ifndef VSMC_HAS_CXX11LIB_FUNCTIONAL
#define VSMC_HAS_CXX11LIB_FUNCTIONAL 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1700
#ifndef VSMC_HAS_CXX11LIB_FUTURE
#define VSMC_HAS_CXX11LIB_FUTURE 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1700
#ifndef VSMC_HAS_CXX11LIB_MUTEX
#define VSMC_HAS_CXX11LIB_MUTEX 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1700
#ifndef VSMC_HAS_CXX11LIB_RANDOM
#define VSMC_HAS_CXX11LIB_RANDOM 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1700
#ifndef VSMC_HAS_CXX11LIB_THREAD
#define VSMC_HAS_CXX11LIB_THREAD 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1800
#ifndef VSMC_HAS_CXX11LIB_TUPLE
#define VSMC_HAS_CXX11LIB_TUPLE 1
#endif
#endif

// C++14 language features

#if VSMC_MSVC_VERSION >= 1900
#ifndef VSMC_HAS_CXX14_BINARY_LITERALS
#define VSMC_HAS_CXX14_BINARY_LITERALS 1
#endif
#endif

#if VSMC_MSVC_VERSION >= VSMC_MSVC_NONEXIST
#ifndef VSMC_HAS_CXX14_CONTEXTUAL_CONVERSIONS
#define VSMC_HAS_CXX14_CONTEXTUAL_CONVERSIONS 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1900
#ifndef VSMC_HAS_CXX14_DECLTYPE_AUTO
#define VSMC_HAS_CXX14_DECLTYPE_AUTO 1
#endif
#endif

#if VSMC_MSVC_VERSION >= VSMC_MSVC_NONEXIST
#ifndef VSMC_HAS_CXX14_DIGIT_SEPERATORS
#define VSMC_HAS_CXX14_DIGIT_SEPERATORS 1
#endif
#endif

#if VSMC_MSVC_VERSION >= VSMC_MSVC_NONEXIST
#ifndef VSMC_HAS_CXX14_AGGREGATE_NSDMI
#define VSMC_HAS_CXX14_AGGREGATE_NSDMI 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1900
#ifndef VSMC_HAS_CXX14_INIT_CAPTURES
#define VSMC_HAS_CXX14_INIT_CAPTURES 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1900
#ifndef VSMC_HAS_CXX14_GENERIC_LAMBDAS
#define VSMC_HAS_CXX14_GENERIC_LAMBDAS 1
#endif
#endif

#if VSMC_MSVC_VERSION >= VSMC_NONEXIST
#ifndef VSMC_HAS_CXX14_RELAXED_CONSTEXPR
#define VSMC_HAS_CXX14_RELAXED_CONSTEXPR 1
#endif
#endif

#if VSMC_MSVC_VERSION >= 1900
#ifndef VSMC_HAS_CXX14_RETURN_TYPE_DEDUCTION
#define VSMC_HAS_CXX14_RETURN_TYPE_DEDUCTION 1
#endif
#endif

#if VSMC_MSVC_VERSION >= VSMC_MSVC_NONEXIST
#ifndef VSMC_HAS_CXX14_RUNTIME_SIZED_ARRAYS
#define VSMC_HAS_CXX14_RUNTIME_SIZED_ARRAYS 1
#endif
#endif

#if VSMC_MSVC_VERSION >= VSMC_MSVC_NONEXIST
#ifndef VSMC_HAS_CXX14_VARIABLE_TEMPLATES
#define VSMC_HAS_CXX14_VARIABLE_TEMPLATES 1
#endif
#endif

// C99 library features

#if VSMC_MSVC_VERSION >= 1800
#ifndef VSMC_HAS_C99LIB_MATH
#define VSMC_HAS_C99LIB_MATH 1
#endif
#endif

// Compiler features

#ifndef VSMC_STRONG_INLINE
#define VSMC_STRONG_INLINE __foceinline
#endif

// Target specific features

#ifndef VSMC_HAS_SSE2
#if _M_IX86_FP == 2 || defined(__AVX__)
#define VSMC_HAS_SSE2 1
#endif
#endif

// Parallelization features

#if VSMC_MSVC_VERSION >= 1700
#ifndef VSMC_HAS_PPL
#define VSMC_HAS_PPL 1
#endif
#endif

#endif // VSMC_INTERNAL_COMPILER_MSVC_HPP
