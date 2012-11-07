SET (CXX_MATH_ERF_TEST_SOURCE "
#include <cassert>
#include <cmath>

#ifdef CXX_MATH_ERF_C99_FOUND
#define ERF erf
#endif

#ifdef CXX_MATH_ERF_STD_FOUND
#define ERF std::erf
#endif

#ifdef CXX_MATH_ERF_BOOST_FOUND
#include <boost/math/special_functions/erf.hpp>
#define ERF boost::math::erf
#endif

int main ()
{
    assert(std::abs(ERF(0)) < 1e-10);

    return 0;
}
")

IF (NOT CXX_MATH_ERF_FOUND)
    UNSET (CXX_MATH_ERF_FOUND CACHE)
    UNSET (CXX_MATH_ERF_C99_FOUND CACHE)
    UNSET (CXX_MATH_ERF_STD_FOUND CACHE)
    UNSET (CXX_MATH_ERF_BOOST_FOUND CACHE)
    INCLUDE (FindBoost)
    INCLUDE (CheckCXXSourceRuns)
    SET (SAFE_CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES})
    SET (CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES}
        ${Boost_INCLUDE_DIRS})
    CHECK_CXX_SOURCE_RUNS ("${CXX_MATH_ERF_TEST_SOURCE}"
        CXX_MATH_ERF_C99_FOUND)
    CHECK_CXX_SOURCE_RUNS ("${CXX_MATH_ERF_TEST_SOURCE}"
        CXX_MATH_ERF_STD_FOUND)
    CHECK_CXX_SOURCE_RUNS ("${CXX_MATH_ERF_TEST_SOURCE}"
        CXX_MATH_ERF_BOOST_FOUND)
    IF (CXX_MATH_ERF_C99_FOUND OR CXX_MATH_ERF_STD_FOUND
            OR CXX_MATH_ERF_BOOST_FOUND)
        SET (CXX_MATH_ERF_FOUND TRUE)
    ENDIF (CXX_MATH_ERF_C99_FOUND OR CXX_MATH_ERF_STD_FOUND
        OR CXX_MATH_ERF_BOOST_FOUND)
    SET (CMAKE_REQUIRED_INCLUDES ${SAFE_CMAKE_REQUIRED_INCLUDES})
ENDIF (NOT CXX_MATH_ERF_FOUND)
