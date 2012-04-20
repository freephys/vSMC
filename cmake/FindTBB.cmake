SET (TBB_FOUND TRUE)

SET (TBB_LIB_PATH ${TBB_LIB_PATH} $ENV{TBBROOT}/lib)
SET (TBB_LIB_PATH ${TBB_LIB_PATH} $ENV{TBBROOT}/lib/intel64)
SET (TBB_LIB_PATH ${TBB_LIB_PATH} $ENV{TBBROOT}/lib/ia32)
SET (TBB_LIB_PATH ${TBB_LIB_PATH} /opt/intel/tbb/lib)
SET (TBB_LIB_PATH ${TBB_LIB_PATH} /opt/intel/tbb/lib/intel64)
SET (TBB_LIB_PATH ${TBB_LIB_PATH} /opt/intel/tbb/lib/ia32)

IF (NOT TBB_LINK_LIBRARIES)
    FIND_LIBRARY (TBB_LINK_LIBRARIES tbb
        PATHS ${TBB_LIB_PATH} ENV LIBRARY_PATH)
    IF (${TBB_LINK_LIBRARIES} STREQUAL TBB_LINK_LIBRARIES-NOTFOUND)
        MESSAGE (STATUS "NOT Found TBB")
        SET (TBB_FOUND FALSE)
    ELSE (${TBB_LINK_LIBRARIES} STREQUAL TBB_LINK_LIBRARIES-NOTFOUND)
        MESSAGE (STATUS "Found TBB: ${TBB_LINK_LIBRARIES}")
    ENDIF (${TBB_LINK_LIBRARIES} STREQUAL TBB_LINK_LIBRARIES-NOTFOUND)
ELSE (NOT TBB_LINK_LIBRARIES)
    MESSAGE (STATUS "Found TBB: ${TBB_LINK_LIBRARIES}")
ENDIF (NOT TBB_LINK_LIBRARIES)

IF (NOT TBB_INCLUDE_DIR)
    SET (TBB_INC_PATH ${TBB_INC_PATH} $ENV{TBBROOT}/include)
    FIND_PATH (TBB_INCLUDE_DIR tbb/tbb.h PATHS ${TBB_INC_PATH} ENV CPATH)
    IF (${TBB_INCLUDE_DIR} STREQUAL TBB_INCLUDE_DIR-NOTFOUND)
        MESSAGE (STATUS "NOT Found TBB headers")
        SET (TBB_FOUND FALSE)
    ELSE (${TBB_INCLUDE_DIR} STREQUAL TBB_INCLUDE_DIR-NOTFOUND)
        MESSAGE (STATUS "Found TBB headers: ${TBB_INCLUDE_DIR}")
    ENDIF (${TBB_INCLUDE_DIR} STREQUAL TBB_INCLUDE_DIR-NOTFOUND)
ELSE (NOT TBB_INCLUDE_DIR)
    MESSAGE (STATUS "Found TBB headers: ${TBB_INCLUDE_DIR}")
ENDIF (NOT TBB_INCLUDE_DIR)

IF (WIN32)
    IF (NOT TBB_DLL)
        SET (TBB_DLL_PATH ${TBB_DLL_PATH} $ENV{TBBROOT}/bin)
        FIND_FILE (TBB_DLL tbb.dll PATHS ${TBB_DLL_PATH})
        IF (${TBB_DLL} STREQUAL TBB_DLL-NOTFOUND)
            MESSAGE (STATUS "NOT Found TBB DLL")
            SET (TBB_FOUND FALSE)
        ELSE (${TBB_DLL} STREQUAL TBB_DLL-NOTFOUND)
            MESSAGE (STATUS "Found TBB DLL: ${TBB_DLL}")
        ENDIF (${TBB_DLL} STREQUAL TBB_DLL-NOTFOUND)
    ELSE (NOT TBB_DLL)
        MESSAGE (STATUS "Found TBB DLL: ${TBB_DLL}")
    ENDIF (NOT TBB_DLL)
ENDIF (WIN32)
