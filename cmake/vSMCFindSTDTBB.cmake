FILE (READ ${PROJECT_SOURCE_DIR}/cmake/vSMCFindSTDTBB.cpp
    VSMC_STDTBB_TEST_SOURCE)

IF (NOT DEFINED VSMC_STDTBB_FOUND AND VSMC_ENABLE_CXX11LIB_THREAD)
    INCLUDE (FindThreads)
    INCLUDE (CheckCXXSourceRuns)
    SET (SAFE_CMAKE_REQUIRED_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS})
    SET (SAFE_CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRAREIS})
    SET (CMAKE_REQUIRED_LIBRARIES ${SAFE_CMAKE_REQUIRED_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT})
    SET (CMAKE_REQUIRED_DEFINITIONS ${SAFE_CMAKE_REQUIRED_DEFINITIONS}
        -DVSMC_HAS_CXX11LIB_FUTURE=0)
    CHECK_CXX_SOURCE_RUNS ("${VSMC_STDTBB_TEST_SOURCE}" VSMC_THREAD_FOUND)
    IF (VSMC_THREAD_FOUND)
        SET (VSMC_THREAD_FOUND TRUE CACHE BOOL "Found C++11 thread")
        SET (VSMC_MUTEX_FOUND TRUE CACHE BOOL "Found C++11 mutex")
    ENDIF (VSMC_THREAD_FOUND)
    IF (VSMC_ENABLE_CXX11LIB_FUTURE)
        SET (CMAKE_REQUIRED_DEFINITIONS ${SAFE_CMAKE_REQUIRED_DEFINITIONS}
            -DVSMC_HAS_CXX11LIB_FUTURE=1)
        CHECK_CXX_SOURCE_RUNS ("${VSMC_STDTBB_TEST_SOURCE}" VSMC_FUTURE_FOUND)
        IF (VSMC_FUTURE_FOUND)
            SET (VSMC_FUTURE_FOUND TRUE CACHE BOOL "Found C++11 future")
            SET (VSMC_MUTEX_FOUND TRUE CACHE BOOL "Found C++11 mutex")
        ENDIF (VSMC_FUTURE_FOUND)
    ENDIF (VSMC_ENABLE_CXX11LIB_FUTURE)
    IF (VSMC_THREAD_FOUND OR VSMC_FUTURE_FOUND)
        SET (VSMC_STDTBB_FOUND TRUE CACHE BOOL "Found vSMC STDTBB")
        SET (VSMC_THREAD_LINK_LIBRARIES ${CMAKE_THREAD_LIBS_INIT}
            CACHE STRING "Link to thread")
    ELSE (VSMC_THREAD_FOUND OR VSMC_FUTURE_FOUND)
        SET (VSMC_STDTBB_FOUND FALSE CACHE BOOL "NOT Found vSMC STDTBB")
    ENDIF (VSMC_THREAD_FOUND OR VSMC_FUTURE_FOUND)
    SET (CMAKE_REQUIRED_DEFINITIONS ${SAFE_CMAKE_REQUIRED_DEFINITIONS})
    SET (CMAKE_REQUIRED_LIBRARIES ${SAFE_CMAKE_REQUIRED_LIBRAREIS})
ENDIF (NOT DEFINED VSMC_STDTBB_FOUND AND VSMC_ENABLE_CXX11LIB_THREAD)
