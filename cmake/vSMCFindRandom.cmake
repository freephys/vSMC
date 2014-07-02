# ============================================================================
#  cmake/vSMCFindRandom.cmake
# ----------------------------------------------------------------------------
#
#                          vSMC: Scalable Monte Carlo
#
#  This file is distribured under the 2-clauses BSD License.
#  See LICENSE for details.
# ============================================================================

IF (DEFINED VSMC_RANDOM_FOUND)
    RETURN ()
ENDIF (DEFINED VSMC_RANDOM_FOUND)

FILE (READ ${CMAKE_CURRENT_LIST_DIR}/vSMCFindRandom.cpp
    VSMC_RANDOM_TEST_SOURCE)

INCLUDE (CheckCXXSourceRuns)
SET (SAFE_CMAKE_REQUIRED_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS})
SET (SAFE_CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES})

IF (VSMC_ENABLE_CXX11)
    SET (CMAKE_REQUIRED_DEFINITIONS ${SAFE_CMAKE_REQUIRED_DEFINITIONS}
        -DVSMC_HAS_CXX11LIB_RANDOM=1 -DVSMC_USE_RANDOM123=0)
    CHECK_CXX_SOURCE_RUNS ("${VSMC_RANDOM_TEST_SOURCE}"
        VSMC_RANDOM_STD_FOUND)
ENDIF (VSMC_ENABLE_CXX11)
IF (NOT VSMC_RANDOM_STD_FOUND)
    SET (CMAKE_REQUIRED_DEFINITIONS ${SAFE_CMAKE_REQUIRED_DEFINITIONS}
        -DVSMC_HAS_CXX11LIB_RANDOM=0 -DVSMC_USE_RANDOM123=0)
    CHECK_CXX_SOURCE_RUNS ("${VSMC_RANDOM_TEST_SOURCE}"
        VSMC_RANDOM_BOOST_FOUND)
ENDIF (NOT VSMC_RANDOM_STD_FOUND)
IF (VSMC_RANDOM_STD_FOUND OR VSMC_RANDOM_BOOST_FOUND)
    SET (VSMC_RANDOM_FOUND TRUE CACHE BOOL "Found random")
ELSE (VSMC_RANDOM_STD_FOUND OR VSMC_RANDOM_BOOST_FOUND)
    SET (VSMC_RANDOM_FOUND FALSE CACHE BOOL "NOT Found random")
ENDIF (VSMC_RANDOM_STD_FOUND OR VSMC_RANDOM_BOOST_FOUND)

INCLUDE (FindRandom123)
IF (RANDOM123_FOUND AND VSMC_RANDOM_FOUND)
    UNSET (VSMC_RANDOM123_STD_FOUND CACHE)
    UNSET (VSMC_RANDOM123_BOOST_FOUND CACHE)
    SET (CMAKE_REQUIRED_INCLUDES ${SAFE_CMAKE_REQUIRED_INCLUDES}
        ${Random123_INCLUDE_DIR})
    IF (VSMC_RANDOM_STD_FOUND)
        SET (CMAKE_REQUIRED_DEFINITIONS ${SAFE_CMAKE_REQUIRED_DEFINITIONS}
            -DVSMC_HAS_CXX11LIB_RANDOM=1 -DVSMC_USE_RANDOM123=1)
        CHECK_CXX_SOURCE_RUNS ("${VSMC_RANDOM_TEST_SOURCE}"
            VSMC_RANDOM123_STD_FOUND)
    ENDIF (VSMC_RANDOM_STD_FOUND)
    IF (NOT VSMC_RANDOM123_STD_FOUND)
        SET (CMAKE_REQUIRED_DEFINITIONS ${SAFE_CMAKE_REQUIRED_DEFINITIONS}
            -DVSMC_HAS_CXX11LIB_RANDOM=0 -DVSMC_USE_RANDOM123=1)
        CHECK_CXX_SOURCE_RUNS ("${VSMC_RANDOM_TEST_SOURCE}"
            VSMC_RANDOM123_BOOST_FOUND)
    ENDIF (NOT VSMC_RANDOM123_STD_FOUND)
    IF (VSMC_RANDOM123_STD_FOUND OR VSMC_RANDOM123_BOOST_FOUND)
        SET (VSMC_RANDOM123_FOUND TRUE CACHE BOOL "Found Random123")
    ELSE (VSMC_RANDOM123_STD_FOUND OR VSMC_RANDOM123_BOOST_FOUND)
        SET (VSMC_RANDOM123_FOUND FALSE CACHE BOOL "NOT Found Random123")
    ENDIF (VSMC_RANDOM123_STD_FOUND OR VSMC_RANDOM123_BOOST_FOUND)
ENDIF (RANDOM123_FOUND AND VSMC_RANDOM_FOUND)

SET (CMAKE_REQUIRED_DEFINITIONS ${SAFE_CMAKE_REQUIRED_DEFINITIONS})
SET (CMAKE_REQUIRED_INCLUDES ${SAFE_CMAKE_REQUIRED_INCLUDES})
