################################################################################
# Copyright 2015 Benjamin Worpitz
#
# This file is part of vecadd.
#
# vecadd is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# vecadd is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with vecadd.
# If not, see <http://www.gnu.org/licenses/>.
################################################################################

################################################################################
# Required CMake version.
################################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

################################################################################
# vecadd.
################################################################################

# Return values.
UNSET(vecadd_FOUND)
UNSET(vecadd_VERSION)
UNSET(vecadd_DEFINITIONS)
UNSET(vecadd_INCLUDE_DIR)
UNSET(vecadd_INCLUDE_DIRS)
UNSET(vecadd_LIBRARY)
UNSET(vecadd_LIBRARIES)

# Internal usage.
UNSET(_VECADD_FOUND)
UNSET(_VECADD_COMPILE_OPTIONS_C_PRIVATE)
UNSET(_VECADD_COMPILE_OPTIONS_CXX_PRIVATE)
UNSET(_VECADD_COMPILE_DEFINITIONS_PRIVATE)
UNSET(_VECADD_COMPILE_DEFINITIONS_PUBLIC)
UNSET(_VECADD_INCLUDE_DIRECTORY)
UNSET(_VECADD_INCLUDE_DIRECTORIES_PRIVATE)
UNSET(_VECADD_INCLUDE_DIRECTORIES_PUBLIC)
UNSET(_VECADD_LINK_LIBRARY)
UNSET(_VECADD_LINK_LIBRARIES_PRIVATE)
UNSET(_VECADD_LINK_LIBRARIES_INTERFACE)
UNSET(_VECADD_FILES_HEADER)
UNSET(_VECADD_FILES_SOURCE_CXX)
UNSET(_VECADD_FILES_SOURCE_CU)
UNSET(_VECADD_FILES_OTHER)

#-------------------------------------------------------------------------------
# Directory of this file.
#-------------------------------------------------------------------------------
SET(_VECADD_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

# Normalize the path (e.g. remove ../)
GET_FILENAME_COMPONENT(_VECADD_ROOT_DIR "${_VECADD_ROOT_DIR}" ABSOLUTE)

#-------------------------------------------------------------------------------
# Set found to true initially and set it on false if a required dependency is missing.
#-------------------------------------------------------------------------------
SET(_VECADD_FOUND TRUE)

#-------------------------------------------------------------------------------
# Common.
#-------------------------------------------------------------------------------
# Add common functions.
SET(_VECADD_COMMON_FILE "${_VECADD_ROOT_DIR}/cmake/common.cmake")
INCLUDE("${_VECADD_COMMON_FILE}")

#-------------------------------------------------------------------------------
# Options.
#-------------------------------------------------------------------------------
# Drop-down combo box in cmake-gui.
SET(VECADD_DEBUG "0" CACHE STRING "Debug level")
SET_PROPERTY(CACHE VECADD_DEBUG PROPERTY STRINGS "0;1;2")

OPTION(VECADD_ELEMENT_TYPE_DOUBLE "If this is defined, double precision data elements are used, else single precision." ON)
IF(VECADD_ELEMENT_TYPE_DOUBLE)
    LIST(APPEND _VECADD_COMPILE_DEFINITIONS_PUBLIC "VECADD_ELEMENT_TYPE_DOUBLE")
ENDIF()

SET(VECADD_INDEX_TYPE "int" CACHE STRING "The index type.")
LIST(APPEND _VECADD_COMPILE_DEFINITIONS_PUBLIC "VECADD_INDEX_TYPE=${VECADD_INDEX_TYPE}")

OPTION(VECADD_ALIGNED_MALLOC "The matrices will be allocated in aligned storage if this is defined." ON)
IF(VECADD_ALIGNED_MALLOC)
    LIST(APPEND _VECADD_COMPILE_DEFINITIONS_PUBLIC "VECADD_ALIGNED_MALLOC")
ENDIF()

OPTION(VECADD_RETURN_COMPUTATION_TIME "If this is defined, the functions return the time needed for the computation itself omitting initialization and shutdown (if possible)." ON)
IF(VECADD_RETURN_COMPUTATION_TIME)
    LIST(APPEND _VECADD_COMPILE_DEFINITIONS_PUBLIC "VECADD_RETURN_COMPUTATION_TIME")
ENDIF()

#-------------------------------------------------------------------------------
# Add definitions and dependencies.
#-------------------------------------------------------------------------------

OPTION(VECADD_BUILD_SEQ_BASIC "Enable the basic sequential GEMM" OFF)
IF(VECADD_BUILD_SEQ_BASIC)
    LIST(APPEND _VECADD_COMPILE_DEFINITIONS_PUBLIC "VECADD_BUILD_SEQ_BASIC")
ENDIF()
OPTION(VECADD_BUILD_PAR_OMP2_GUIDED "The optimized but not blocked algorithm with OpenMP 2 annotations" OFF)
IF(VECADD_BUILD_PAR_OMP2_GUIDED)
    SET(_VECADD_BUILD_OMP ON)
    LIST(APPEND _VECADD_COMPILE_DEFINITIONS_PUBLIC "VECADD_BUILD_PAR_OMP2_GUIDED")
ENDIF()
OPTION(VECADD_BUILD_PAR_CUDA "Enable the GEMM algorithm from the CUDA developers guide with fixed block size" OFF)
IF(VECADD_BUILD_PAR_CUDA)
    SET(_VECADD_BUILD_CUDA ON)
    LIST(APPEND _VECADD_COMPILE_DEFINITIONS_PUBLIC "VECADD_BUILD_PAR_CUDA")
ENDIF()

OPTION(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_SEQ "Enable the GEMM using the alpaka serial accelerator back-end on the CPU" OFF)
IF(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_SEQ)
    SET(_VECADD_BUILD_ALPAKA ON)
    SET(ALPAKA_ACC_CPU_B_SEQ_T_SEQ_ENABLE ON)
    LIST(APPEND _VECADD_COMPILE_DEFINITIONS_PUBLIC "VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_SEQ")
ELSE()
    SET(ALPAKA_ACC_CPU_B_SEQ_T_SEQ_ENABLE OFF)
ENDIF()
OPTION(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_OMP2_T_SEQ "Enable the GEMM using the alpaka OpenMP 2.0 grid block accelerator back-end on the CPU" OFF)
IF(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_OMP2_T_SEQ)
    SET(_VECADD_BUILD_ALPAKA ON)
    SET(ALPAKA_ACC_CPU_B_OMP2_T_SEQ_ENABLE ON CACHE BOOL "" FORCE)
    SET(_VECADD_BUILD_OMP ON)
    LIST(APPEND _VECADD_COMPILE_DEFINITIONS_PUBLIC "VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_OMP2_T_SEQ")
ELSE()
    SET(ALPAKA_ACC_CPU_B_OMP2_T_SEQ_ENABLE OFF CACHE BOOL "" FORCE)
ENDIF()
OPTION(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_OMP2 "Enable the GEMM using the alpaka OpenMP 2.0 block thread accelerator back-end on the CPU" OFF)
IF(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_OMP2)
    SET(_VECADD_BUILD_ALPAKA ON)
    SET(ALPAKA_ACC_CPU_B_SEQ_T_OMP2_ENABLE ON CACHE BOOL "" FORCE)
    SET(_VECADD_BUILD_OMP ON)
    LIST(APPEND _VECADD_COMPILE_DEFINITIONS_PUBLIC "VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_OMP2")
ELSE()
    SET(ALPAKA_ACC_CPU_B_SEQ_T_OMP2_ENABLE OFF CACHE BOOL "" FORCE)
ENDIF()
OPTION(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_BT_OMP4 "Enable the GEMM using the alpaka OpenMP 4.0 accelerator back-end on the CPU" OFF)
IF(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_BT_OMP4)
    SET(_VECADD_BUILD_ALPAKA ON)
    SET(ALPAKA_ACC_CPU_BT_OMP4_ENABLE ON CACHE BOOL "" FORCE)
    SET(_VECADD_BUILD_OMP ON)
    LIST(APPEND _VECADD_COMPILE_DEFINITIONS_PUBLIC "VECADD_BUILD_PAR_ALPAKA_ACC_CPU_BT_OMP4")
ELSE()
    SET(ALPAKA_ACC_CPU_BT_OMP4_ENABLE OFF CACHE BOOL "" FORCE)
ENDIF()
OPTION(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_FIBERS "Enable the GEMM using the alpaka Boost.Fiber accelerator back-end on the CPU" OFF)
IF(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_FIBERS)
    SET(_VECADD_BUILD_ALPAKA ON)
    SET(ALPAKA_ACC_CPU_B_SEQ_T_FIBERS_ENABLE ON CACHE BOOL "" FORCE)
    LIST(APPEND _VECADD_COMPILE_DEFINITIONS_PUBLIC "VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_FIBERS")
ELSE()
    SET(ALPAKA_ACC_CPU_B_SEQ_T_FIBERS_ENABLE OFF CACHE BOOL "" FORCE)
ENDIF()
OPTION(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_THREADS "Enable the GEMM using the alpaka std::thread accelerator back-end on the CPU" OFF)
IF(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_THREADS)
    SET(_VECADD_BUILD_ALPAKA ON)
    SET(ALPAKA_ACC_CPU_B_SEQ_T_THREADS_ENABLE ON CACHE BOOL "" FORCE)
    LIST(APPEND _VECADD_COMPILE_DEFINITIONS_PUBLIC "VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_THREADS")
ELSE()
    SET(ALPAKA_ACC_CPU_B_SEQ_T_THREADS_ENABLE OFF CACHE BOOL "" FORCE)
ENDIF()
OPTION(VECADD_BUILD_PAR_ALPAKA_ACC_GPU_CUDA "Enable the GEMM using the alpaka CUDA accelerator" OFF)
IF(VECADD_BUILD_PAR_ALPAKA_ACC_GPU_CUDA)
    SET(_VECADD_BUILD_ALPAKA ON)
    SET(ALPAKA_ACC_GPU_CUDA_ENABLE ON CACHE BOOL "" FORCE)
    SET(_VECADD_BUILD_CUDA ON)
    LIST(APPEND _VECADD_COMPILE_DEFINITIONS_PUBLIC "VECADD_BUILD_PAR_ALPAKA_ACC_GPU_CUDA")
ENDIF()

#-------------------------------------------------------------------------------
# OpenMP Settings.
#-------------------------------------------------------------------------------
IF(_VECADD_BUILD_OMP)
    OPTION(VECADD_OMP_PRINT_NUM_CORES "If this is defined, each call to a vecadd function will print out the number of cores used currently. This can have a huge performance impact especially for the recursive Strassen Method." OFF)
    IF(VECADD_OMP_PRINT_NUM_CORES)
        LIST(APPEND _VECADD_COMPILE_DEFINITIONS_PUBLIC "VECADD_OMP_PRINT_NUM_CORES")
    ENDIF()
ENDIF()

#-------------------------------------------------------------------------------
# Find OpenMP.
#-------------------------------------------------------------------------------
IF(_VECADD_BUILD_OMP)
    FIND_PACKAGE(OpenMP)
    IF(NOT OPENMP_FOUND)
        MESSAGE(WARNING "Required vecadd dependency OpenMP could not be found!")
        SET(_VECADD_FOUND FALSE)

    ELSE()
        LIST(APPEND _VECADD_COMPILE_OPTIONS_C_PRIVATE ${OpenMP_C_FLAGS})
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_C_FLAGS}")
        IF((NOT ALPAKA_ACC_GPU_CUDA_ENABLE) AND _VECADD_BUILD_CUDA)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        ENDIF()
    ENDIF()
ENDIF()

#-------------------------------------------------------------------------------
# Find CUDA.
#-------------------------------------------------------------------------------
IF(_VECADD_BUILD_CUDA OR _VECADD_BUILD_CUBLAS)
    IF(_VECADD_BUILD_CUBLAS)
        # cuBLAS 2 is supported in CUDA 4.0+.
        FIND_PACKAGE(CUDA "4.0")
    ELSE()
        FIND_PACKAGE(CUDA)
    ENDIF()

    IF(NOT CUDA_FOUND)
        MESSAGE(WARNING "Required vecadd dependency CUDA could not be found!")
        SET(_VECADD_FOUND FALSE)

    ELSE()
        # If the flags have not already been set.
        IF(NOT ALPAKA_ACC_GPU_CUDA_ENABLE)
            IF(${VECADD_DEBUG} GREATER 1)
                SET(CUDA_VERBOSE_BUILD ON)
            ENDIF()
            SET(CUDA_PROPAGATE_HOST_FLAGS ON)

            SET(VECADD_CUDA_ARCH sm_20 CACHE STRING "Set GPU architecture")
            LIST(APPEND CUDA_NVCC_FLAGS "-arch=${VECADD_CUDA_ARCH}")

            IF(NOT MSVC)
                SET(CUDA_HOST_COMPILER "${CMAKE_CXX_COMPILER}")
            ENDIF()

            IF(CMAKE_BUILD_TYPE MATCHES "Debug")
                LIST(APPEND CUDA_NVCC_FLAGS "-g" "-G")
            ENDIF()

            OPTION(VECADD_CUDA_FAST_MATH "Enable fast-math" ON)
            IF(VECADD_CUDA_FAST_MATH)
                LIST(APPEND CUDA_NVCC_FLAGS "--use_fast_math")
            ENDIF()

            OPTION(VECADD_CUDA_FTZ "Set flush to zero for GPU" OFF)
            IF(VECADD_CUDA_FTZ)
                LIST(APPEND CUDA_NVCC_FLAGS "--ftz=true")
            ELSE()
                LIST(APPEND CUDA_NVCC_FLAGS "--ftz=false")
            ENDIF()

            OPTION(VECADD_CUDA_SHOW_REGISTER "Show kernel registers and create PTX" OFF)
            IF(VECADD_CUDA_SHOW_REGISTER)
                LIST(APPEND CUDA_NVCC_FLAGS "-Xptxas=-v")
            ENDIF()

            OPTION(VECADD_CUDA_KEEP_FILES "Keep all intermediate files that are generated during internal compilation steps (folder: nvcc_tmp)" OFF)
            IF(VECADD_CUDA_KEEP_FILES)
                MAKE_DIRECTORY("${PROJECT_BINARY_DIR}/nvcc_tmp")
                LIST(APPEND CUDA_NVCC_FLAGS "--keep" "--keep-dir" "${PROJECT_BINARY_DIR}/nvcc_tmp")
            ENDIF()

            OPTION(VECADD_CUDA_SHOW_CODELINES "Show kernel lines in cuda-gdb and cuda-memcheck" OFF)
            IF(VECADD_CUDA_SHOW_CODELINES)
                LIST(APPEND CUDA_NVCC_FLAGS "--source-in-ptx" "-lineinfo")
                IF(NOT MSVC)
                    LIST(APPEND CUDA_NVCC_FLAGS "-Xcompiler" "-rdynamic")
                ENDIF()
                SET(VECADD_CUDA_KEEP_FILES ON CACHE BOOL "activate keep files" FORCE)
            ENDIF()

            LIST(APPEND _VECADD_LINK_LIBRARIES_PRIVATE "general;${CUDA_CUDART_LIBRARY}")
            LIST(APPEND _VECADD_INCLUDE_DIRECTORIES_PRIVATE ${CUDA_INCLUDE_DIRS})
        ENDIF()

        IF(_VECADD_BUILD_CUBLAS)
            LIST(APPEND _VECADD_LINK_LIBRARIES_PRIVATE "general;${CUDA_CUBLAS_LIBRARIES}")
        ENDIF()
    ENDIF()
ENDIF()

#-------------------------------------------------------------------------------
# Find alpaka.
#-------------------------------------------------------------------------------
IF(_VECADD_BUILD_ALPAKA)
    LIST(APPEND CMAKE_MODULE_PATH "${ALPAKA_ROOT}")
    FIND_PACKAGE(alpaka)
    IF(NOT alpaka_FOUND)
        MESSAGE(WARNING "Required vecadd dependency alpaka could not be found!")
        SET(_VECADD_FOUND FALSE)

    ELSE()
        LIST(APPEND _VECADD_COMPILE_OPTIONS_CXX_PRIVATE ${alpaka_COMPILE_OPTIONS})
        LIST(APPEND _VECADD_COMPILE_DEFINITIONS_PRIVATE ${alpaka_COMPILE_DEFINITIONS})
        LIST(APPEND _VECADD_INCLUDE_DIRECTORIES_PRIVATE ${alpaka_INCLUDE_DIRS})
        LIST(APPEND _VECADD_LINK_LIBRARIES_PRIVATE ${alpaka_LIBRARIES})
    ENDIF()
ENDIF()

#-------------------------------------------------------------------------------
# vecadd.
#-------------------------------------------------------------------------------
SET(_VECADD_INCLUDE_DIRECTORY "${_VECADD_ROOT_DIR}/include")
LIST(APPEND _VECADD_INCLUDE_DIRECTORIES_PUBLIC ${_VECADD_INCLUDE_DIRECTORY})
SET(_VECADD_LINK_LIBRARY "$<TARGET_FILE:>")
LIST(APPEND _VECADD_LINK_LIBRARIES_INTERFACE ${_VECADD_LINK_LIBRARY})

IF(NOT MSVC)
    LIST(APPEND _VECADD_COMPILE_OPTIONS_C_PRIVATE "-std=c99")
ENDIF()

LIST(APPEND _VECADD_COMPILE_DEFINITIONS_PUBLIC "VECADD_DEBUG=${VECADD_DEBUG}")

SET(_VECADD_SUFFIXED_INCLUDE_DIR "${_VECADD_INCLUDE_DIRECTORY}/vecadd")
SET(_VECADD_SOURCE_DIR "${_VECADD_ROOT_DIR}/src")

SET(_VECADD_FILES_OTHER "${_VECADD_ROOT_DIR}/vecaddConfig.cmake" "${_VECADD_COMMON_FILE}" "${_VECADD_ROOT_DIR}/README.md")

# Add all the source and include files in all recursive subdirectories and group them accordingly.
append_recursive_files_add_to_src_group("${_VECADD_SUFFIXED_INCLUDE_DIR}" "${_VECADD_SUFFIXED_INCLUDE_DIR}" "h" _VECADD_FILES_HEADER)
append_recursive_files_add_to_src_group("${_VECADD_SUFFIXED_INCLUDE_DIR}" "${_VECADD_SUFFIXED_INCLUDE_DIR}" "hpp" _VECADD_FILES_HEADER)
append_recursive_files_add_to_src_group("${_VECADD_SOURCE_DIR}" "${_VECADD_SOURCE_DIR}" "cpp" _VECADD_FILES_SOURCE_CXX)
append_recursive_files_add_to_src_group("${_VECADD_SOURCE_DIR}" "${_VECADD_SOURCE_DIR}" "c" _VECADD_FILES_SOURCE_C)
append_recursive_files_add_to_src_group("${_VECADD_SOURCE_DIR}" "${_VECADD_SOURCE_DIR}" "cu" _VECADD_FILES_SOURCE_CU)

# Compile options (PRIVATE).
# Y
IF(${VECADD_DEBUG} GREATER 1)
    MESSAGE(STATUS "_VECADD_COMPILE_OPTIONS_C_PRIVATE: ${_VECADD_COMPILE_OPTIONS_C_PRIVATE}")
ENDIF()
# COMPILE_FLAGS is NOT a list so we have to append the options (prefixed by a space) one by one to not insert a semicolon in between.
FOREACH(_VECADD_COMPILE_OPTION_C_PRIVATE ${_VECADD_COMPILE_OPTIONS_C_PRIVATE})
    SET_PROPERTY(
        SOURCE ${_VECADD_FILES_SOURCE_C}
        APPEND_STRING
        PROPERTY COMPILE_FLAGS " ${_VECADD_COMPILE_OPTION_C_PRIVATE}")
ENDFOREACH()
# CXX
IF(${VECADD_DEBUG} GREATER 1)
    MESSAGE(STATUS "_VECADD_COMPILE_OPTIONS_CXX_PRIVATE: ${_VECADD_COMPILE_OPTIONS_CXX_PRIVATE}")
ENDIF()
FOREACH(_VECADD_COMPILE_OPTION_CXX_PRIVATE ${_VECADD_COMPILE_OPTIONS_CXX_PRIVATE})
    SET_PROPERTY(
        SOURCE ${_VECADD_FILES_SOURCE_CXX} ${_VECADD_FILES_SOURCE_CU}
        APPEND_STRING
        PROPERTY COMPILE_FLAGS " ${_VECADD_COMPILE_OPTION_CXX_PRIVATE}")
ENDFOREACH()

#-------------------------------------------------------------------------------
# Target.
#-------------------------------------------------------------------------------
IF(NOT TARGET "vecadd")
    IF(NOT _VECADD_BUILD_CUDA)
        # Always add all files to the target executable build call to add them to the build project.
        ADD_LIBRARY(
            "vecadd"
            ${_VECADD_FILES_HEADER} ${_VECADD_FILES_SOURCE_C} ${_VECADD_FILES_SOURCE_CXX} ${_VECADD_FILES_SOURCE_CU} ${_VECADD_FILES_OTHER})

        # Compile definitions.
        IF(${VECADD_DEBUG} GREATER 1)
            MESSAGE(STATUS "_VECADD_COMPILE_DEFINITIONS_PRIVATE: ${_VECADD_COMPILE_DEFINITIONS_PRIVATE}")
            MESSAGE(STATUS "_VECADD_COMPILE_DEFINITIONS_PUBLIC: ${_VECADD_COMPILE_DEFINITIONS_PUBLIC}")
        ENDIF()
        LIST(
            LENGTH
            _VECADD_COMPILE_DEFINITIONS_PRIVATE
            _VECADD_COMPILE_DEFINITIONS_PRIVATE_LENGTH)
        IF(${_VECADD_COMPILE_DEFINITIONS_PRIVATE_LENGTH} GREATER 0)
            TARGET_COMPILE_DEFINITIONS(
                "vecadd"
                PRIVATE ${_VECADD_COMPILE_DEFINITIONS_PRIVATE})
        ENDIF()
        LIST(
            LENGTH
            _VECADD_COMPILE_DEFINITIONS_PUBLIC
            _VECADD_COMPILE_DEFINITIONS_PUBLIC_LENGTH)
        IF(${_VECADD_COMPILE_DEFINITIONS_PUBLIC_LENGTH} GREATER 0)
            TARGET_COMPILE_DEFINITIONS(
                "vecadd"
                PUBLIC ${_VECADD_COMPILE_DEFINITIONS_PUBLIC})
        ENDIF()

        # Include directories.
        IF(${VECADD_DEBUG} GREATER 1)
            MESSAGE(STATUS "_VECADD_INCLUDE_DIRECTORIES_PRIVATE: ${_VECADD_INCLUDE_DIRECTORIES_PRIVATE}")
        ENDIF()
        LIST(
            LENGTH
            _VECADD_INCLUDE_DIRECTORIES_PRIVATE
            _VECADD_INCLUDE_DIRECTORIES_PRIVATE_LENGTH)
        IF(${_VECADD_INCLUDE_DIRECTORIES_PRIVATE_LENGTH} GREATER 0)
            TARGET_INCLUDE_DIRECTORIES(
                "vecadd"
                PRIVATE ${_VECADD_INCLUDE_DIRECTORIES_PRIVATE})
        ENDIF()
        LIST(
            LENGTH
            _VECADD_INCLUDE_DIRECTORIES_PUBLIC
            _VECADD_INCLUDE_DIRECTORIES_PUBLIC_LENGTH)
        IF(${_VECADD_INCLUDE_DIRECTORIES_PUBLIC_LENGTH} GREATER 0)
            TARGET_INCLUDE_DIRECTORIES(
                "vecadd"
                PUBLIC ${_VECADD_INCLUDE_DIRECTORIES_PUBLIC})
        ENDIF()
    ENDIF()
ENDIF()

SET(vecadd_DEFINITIONS ${_VECADD_COMPILE_DEFINITIONS_PUBLIC})
# Add '-D' to the definitions
list_add_prefix("-D" vecadd_DEFINITIONS)
# Add the compile options to the definitions.
SET(vecadd_INCLUDE_DIR ${_VECADD_INCLUDE_DIRECTORY})
SET(vecadd_INCLUDE_DIRS ${_VECADD_INCLUDE_DIRECTORIES_PUBLIC})
SET(vecadd_LIBRARY ${_VECADD_LINK_LIBRARY})
SET(vecadd_LIBRARIES ${_VECADD_LINK_LIBRARIES_INTERFACE})
SET(vecadd_VERSION "1.0.0")

IF(NOT TARGET "vecadd")
    IF(_VECADD_BUILD_CUDA)
        # CUDA does not work well with the much better target dependent TARGET_XXX commands but requires the settings to be available globally: https://www.cmake.org/Bug/view.php?id=14201&nbn=1
        INCLUDE_DIRECTORIES(
            ${_VECADD_INCLUDE_DIRECTORY}
            ${_VECADD_INCLUDE_DIRECTORIES_PRIVATE})
        SET(_VECADD_COMPILE_DEFINITIONS_COPY ${_VECADD_COMPILE_DEFINITIONS_PRIVATE} ${_VECADD_COMPILE_DEFINITIONS_PUBLIC})
        list_add_prefix("-D" _VECADD_COMPILE_DEFINITIONS_COPY)
        ADD_DEFINITIONS(
            ${_VECADD_COMPILE_DEFINITIONS_COPY})
        UNSET(_VECADD_COMPILE_DEFINITIONS_COPY)
        CMAKE_POLICY(SET CMP0023 OLD)   # CUDA_ADD_EXECUTABLE calls TARGET_LINK_LIBRARIES without keywords.
        CUDA_ADD_LIBRARY(
            "vecadd"
            ${_VECADD_FILES_HEADER} ${_VECADD_FILES_SOURCE_C} ${_VECADD_FILES_SOURCE_CXX} ${_VECADD_FILES_SOURCE_CU} ${_VECADD_FILES_OTHER})
    ENDIF()
ENDIF()

# Link libraries.
IF(${VECADD_DEBUG} GREATER 0)
    MESSAGE(STATUS "_VECADD_LINK_LIBRARIES_PRIVATE: ${_VECADD_LINK_LIBRARIES_PRIVATE}")
ENDIF()
LIST(
    LENGTH
    _VECADD_LINK_LIBRARIES_PRIVATE
    _VECADD_LINK_LIBRARIES_PRIVATE_LENGTH)
IF(${_VECADD_LINK_LIBRARIES_PRIVATE_LENGTH} GREATER 0)
    TARGET_LINK_LIBRARIES(
        "vecadd"
        PRIVATE ${_VECADD_LINK_LIBRARIES_PRIVATE})
ENDIF()

#-------------------------------------------------------------------------------
# Print the return values.
#-------------------------------------------------------------------------------
IF(${VECADD_DEBUG} GREATER 0)
    MESSAGE(STATUS "vecadd_FOUND: ${vecadd_FOUND}")
    MESSAGE(STATUS "vecadd_VERSION: ${vecadd_VERSION}")
    MESSAGE(STATUS "vecadd_DEFINITIONS: ${vecadd_DEFINITIONS}")
    MESSAGE(STATUS "vecadd_INCLUDE_DIR: ${vecadd_INCLUDE_DIR}")
    MESSAGE(STATUS "vecadd_INCLUDE_DIRS: ${vecadd_INCLUDE_DIRS}")
    MESSAGE(STATUS "vecadd_LIBRARY: ${vecadd_LIBRARY}")
    MESSAGE(STATUS "vecadd_LIBRARIES: ${vecadd_LIBRARIES}")
ENDIF()

# Unset already set variables if not found.
IF(NOT _VECADD_FOUND)
    UNSET(vecadd_FOUND)
    UNSET(vecadd_VERSION)
    UNSET(vecadd_DEFINITIONS)
    UNSET(vecadd_INCLUDE_DIR)
    UNSET(vecadd_INCLUDE_DIRS)
    UNSET(vecadd_LIBRARY)
    UNSET(vecadd_LIBRARIES)

    UNSET(_VECADD_FOUND)
    UNSET(_VECADD_COMPILE_OPTIONS_C_PRIVATE)
    UNSET(_VECADD_COMPILE_OPTIONS_CXX_PRIVATE)
    UNSET(_VECADD_COMPILE_DEFINITIONS_PRIVATE)
    UNSET(_VECADD_COMPILE_DEFINITIONS_PUBLIC)
    UNSET(_VECADD_INCLUDE_DIRECTORY)
    UNSET(_VECADD_INCLUDE_DIRECTORIES_PRIVATE)
    UNSET(_VECADD_INCLUDE_DIRECTORIES_PUBLIC)
    UNSET(_VECADD_LINK_LIBRARY)
    UNSET(_VECADD_LINK_LIBRARIES_PRIVATE)
    UNSET(_VECADD_LINK_LIBRARIES_INTERFACE)
    UNSET(_VECADD_FILES_HEADER)
    UNSET(_VECADD_FILES_SOURCE_CXX)
    UNSET(_VECADD_FILES_SOURCE_CU)
    UNSET(_VECADD_FILES_OTHER)
ELSE()
    # Make internal variables advanced options in the GUI.
    MARK_AS_ADVANCED(
        vecadd_INCLUDE_DIR
        vecadd_LIBRARY
        _VECADD_COMPILE_OPTIONS_C_PRIVATE
        _VECADD_COMPILE_OPTIONS_CXX_PRIVATE
        _VECADD_COMPILE_DEFINITIONS_PRIVATE
        _VECADD_COMPILE_DEFINITIONS_PUBLIC
        _VECADD_INCLUDE_DIRECTORY
        _VECADD_INCLUDE_DIRECTORIES_PRIVATE
        _VECADD_INCLUDE_DIRECTORIES_PUBLIC
        _VECADD_LINK_LIBRARY
        _VECADD_LINK_LIBRARIES_PRIVATE
        _VECADD_LINK_LIBRARIES_INTERFACE
        _VECADD_FILES_HEADER
        _VECADD_FILES_SOURCE_CXX
        _VECADD_FILES_SOURCE_CU
        _VECADD_FILES_OTHER)
ENDIF()

###############################################################################
# FindPackage options
###############################################################################

# Handles the REQUIRED, QUIET and version-related arguments for FIND_PACKAGE.
# NOTE: We do not check for vecadd_LIBRARIES and vecadd_DEFINITIONS because they can be empty.
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    "vecadd"
    FOUND_VAR vecadd_FOUND
    REQUIRED_VARS vecadd_INCLUDE_DIR vecadd_LIBRARY
    VERSION_VAR vecadd_VERSION)
