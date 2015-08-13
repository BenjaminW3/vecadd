#
# Copyright 2014-2015 Benjamin Worpitz
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
#
# Set the following CMake variables to change the behavior:
# - ``VECADD_BENCHMARK_VERIFY_RESULT`` {ON, OFF}
# - ``VECADD_BENCHMARK_REPEAT_TAKE_MINIMUM`` {ON, OFF}
#
# Set the following CMake variables to select the versions being compiled:
# NOTE: Either MPI or CUDA device only or host timings can be activated.
# So only elements of one of the following 3 blocks can be active:
# - ``VECADD_BENCHMARK_BUILD_SEQ_BASIC`` {ON, OFF}
# - ``VECADD_BENCHMARK_BUILD_PAR_OMP2_GUIDED`` {ON, OFF}
# - ``VECADD_BENCHMARK_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_SEQ`` {ON, OFF}
# - ``VECADD_BENCHMARK_BUILD_PAR_ALPAKA_ACC_CPU_B_OMP2_T_SEQ`` {ON, OFF}
# - ``VECADD_BENCHMARK_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_OMP2`` {ON, OFF}
# - ``VECADD_BENCHMARK_BUILD_PAR_ALPAKA_ACC_CPU_BT_OMP4`` {ON, OFF}
# - ``VECADD_BENCHMARK_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_FIBERS`` {ON, OFF}
# - ``VECADD_BENCHMARK_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_THREADS`` {ON, OFF}
#
# - ``VECADD_BENCHMARK_BUILD_PAR_ALPAKA_ACC_GPU_CUDA`` {ON, OFF}
# - ``VECADD_BENCHMARK_BUILD_PAR_CUDA`` {ON, OFF}

################################################################################
# Required CMake version.
################################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Project.
################################################################################

PROJECT("vecadd_benchmark")

UNSET(_VECADD_BENCHMARK_COMPILE_OPTIONS)
UNSET(_VECADD_BENCHMARK_COMPILE_DEFINITIONS)
UNSET(_VECADD_BENCHMARK_INCLUDE_DIR)
UNSET(_VECADD_BENCHMARK_LINK_FLAGS)

#-------------------------------------------------------------------------------
# Options.
#-------------------------------------------------------------------------------

SET(VECADD_BENCHMARK_SEQ_BASIC OFF CACHE BOOL "Enable the basic sequential vector addition")
IF(VECADD_BENCHMARK_SEQ_BASIC)
    LIST(APPEND _VECADD_BENCHMARK_COMPILE_DEFINITIONS "VECADD_BENCHMARK_SEQ_BASIC")
    SET(VECADD_BUILD_SEQ_BASIC ON CACHE BOOL "" FORCE)
ENDIF()
SET(VECADD_BENCHMARK_PAR_OMP2_GUIDED OFF CACHE BOOL "The optimized but not blocked algorithm with OpenMP 2 annotations")
IF(VECADD_BENCHMARK_PAR_OMP2_GUIDED)
    LIST(APPEND _VECADD_BENCHMARK_COMPILE_DEFINITIONS "VECADD_BENCHMARK_PAR_OMP2_GUIDED")
    SET(VECADD_BUILD_PAR_OMP2_GUIDED ON CACHE BOOL "" FORCE)
ENDIF()

SET(VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_SEQ_T_SEQ OFF CACHE BOOL "Enable the vector addition using the alpaka serial accelerator back-end on the CPU")
IF(VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_SEQ_T_SEQ)
    LIST(APPEND _VECADD_BENCHMARK_COMPILE_DEFINITIONS "VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_SEQ_T_SEQ")
    SET(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_SEQ ON CACHE BOOL "" FORCE)
ENDIF()
SET(VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_OMP2_T_SEQ OFF CACHE BOOL "Enable the vector addition using the alpaka OpenMP 2.0 grid block accelerator back-end on the CPU")
IF(VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_OMP2_T_SEQ)
    LIST(APPEND _VECADD_BENCHMARK_COMPILE_DEFINITIONS "VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_OMP2_T_SEQ")
    SET(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_OMP2_T_SEQ ON CACHE BOOL "" FORCE)
ENDIF()
SET(VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_SEQ_T_OMP2 OFF CACHE BOOL "Enable the vector addition using the alpaka OpenMP 2.0 block thread accelerator back-end on the CPU")
IF(VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_SEQ_T_OMP2)
    LIST(APPEND _VECADD_BENCHMARK_COMPILE_DEFINITIONS "VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_SEQ_T_OMP2")
    SET(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_OMP2 ON CACHE BOOL "" FORCE)
ENDIF()
SET(VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_SEQ_T_THREADS OFF CACHE BOOL "Enable the vector addition using the alpaka std::thread accelerator back-end on the CPU")
IF(VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_SEQ_T_THREADS)
    LIST(APPEND _VECADD_BENCHMARK_COMPILE_DEFINITIONS "VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_SEQ_T_THREADS")
    SET(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_THREADS ON CACHE BOOL "" FORCE)
ENDIF()
SET(VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_SEQ_T_FIBERS OFF CACHE BOOL "Enable the vector addition using the alpaka Boost.Fiber accelerator back-end on the CPU")
IF(VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_SEQ_T_FIBERS)
    LIST(APPEND _VECADD_BENCHMARK_COMPILE_DEFINITIONS "VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_SEQ_T_FIBERS")
    SET(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_FIBERS ON CACHE BOOL "" FORCE)
ENDIF()
SET(VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_BT_OMP4 OFF CACHE BOOL "Enable the vector addition using the alpaka OpenMP 4.0 accelerator back-end on the CPU")
IF(VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_BT_OMP4)
    LIST(APPEND _VECADD_BENCHMARK_COMPILE_DEFINITIONS "VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_BT_OMP4")
    SET(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_BT_OMP4 ON CACHE BOOL "" FORCE)
ENDIF()

SET(VECADD_BENCHMARK_PAR_CUDA OFF CACHE BOOL "Enable the vector addition algorithm from the CUDA developers guide with fixed block size")
IF(VECADD_BENCHMARK_PAR_CUDA)
    LIST(APPEND _VECADD_BENCHMARK_COMPILE_DEFINITIONS "VECADD_BENCHMARK_PAR_CUDA")
    SET(VECADD_BUILD_PAR_CUDA ON CACHE BOOL "" FORCE)
ENDIF()
SET(VECADD_BENCHMARK_PAR_ALPAKA_ACC_GPU_CUDA OFF CACHE BOOL "Enable the vector addition using the alpaka CUDA accelerator")
IF(VECADD_BENCHMARK_PAR_ALPAKA_ACC_GPU_CUDA)
    LIST(APPEND _VECADD_BENCHMARK_COMPILE_DEFINITIONS "VECADD_BENCHMARK_PAR_ALPAKA_ACC_GPU_CUDA")
    SET(VECADD_BUILD_PAR_ALPAKA_ACC_GPU_CUDA ON CACHE BOOL "" FORCE)
ENDIF()

#-------------------------------------------------------------------------------
# MPI vs Host vs CUDA.
#-------------------------------------------------------------------------------
IF(VECADD_BENCHMARK_SEQ_BASIC
    OR VECADD_BENCHMARK_SEQ_BASIC
    OR VECADD_BENCHMARK_PAR_OMP2_GUIDED
    OR VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_OMP2_T_SEQ
    OR VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_SEQ_T_OMP2
    OR VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_BT_OMP4
    OR VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_SEQ_T_FIBERS
    OR VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_SEQ_T_THREADS)

    SET(_VECADD_BENCHMARK_SINGLE_NODE_HOST TRUE)
ENDIF()
IF(VECADD_BENCHMARK_PAR_CUDA
    OR VECADD_BENCHMARK_PAR_ALPAKA_ACC_GPU_CUDA)

    SET(_VECADD_BENCHMARK_SINGLE_NODE_CUDA TRUE)
    LIST(APPEND _VECADD_BENCHMARK_COMPILE_DEFINITIONS "VECADD_BENCHMARK_CUDA_NO_COPY")
ENDIF()

IF(_VECADD_BENCHMARK_SINGLE_NODE_CUDA AND _VECADD_BENCHMARK_SINGLE_NODE_HOST)
    MESSAGE(FATAL_ERROR "If GPU tests without memcopy are executed, no other benchmarks can be executed by the same executable!")
ENDIF()

#-------------------------------------------------------------------------------
# Measurement settings.
#-------------------------------------------------------------------------------
SET(VECADD_BENCHMARK_VERIFY_RESULT OFF CACHE BOOL "The result of a computation will be compared with the result of the standard sequential algorithm.")
IF(VECADD_BENCHMARK_VERIFY_RESULT)
    LIST(APPEND _VECADD_BENCHMARK_COMPILE_DEFINITIONS "VECADD_BENCHMARK_VERIFY_RESULT")
    SET(VECADD_BUILD_SEQ_BASIC ON CACHE BOOL "" FORCE)
ENDIF()
SET(VECADD_BENCHMARK_PRINT_GFLOPS OFF CACHE BOOL "If the GFLOPS should be printed instead if the time.")
IF(VECADD_BENCHMARK_PRINT_GFLOPS)
    LIST(APPEND _VECADD_BENCHMARK_COMPILE_DEFINITIONS "VECADD_BENCHMARK_PRINT_GFLOPS")
ENDIF()
SET(VECADD_BENCHMARK_REPEAT_TAKE_MINIMUM ON CACHE BOOL "If this is defined the minimum of all repetitions is returned instead of the average.")
IF(VECADD_BENCHMARK_REPEAT_TAKE_MINIMUM)
    LIST(APPEND _VECADD_BENCHMARK_COMPILE_DEFINITIONS "VECADD_BENCHMARK_REPEAT_TAKE_MINIMUM")
ENDIF()
SET(VECADD_BENCHMARK_PRINT_ITERATIONS OFF CACHE BOOL "If the current iteration number should be printed.")
IF(VECADD_BENCHMARK_PRINT_ITERATIONS)
    LIST(APPEND _VECADD_BENCHMARK_COMPILE_DEFINITIONS "VECADD_BENCHMARK_PRINT_ITERATIONS")
ENDIF()
SET(VECADD_BENCHMARK_PRINT_MATRICES OFF CACHE BOOL "If the matrices (in and out) should be printed.")
IF(VECADD_BENCHMARK_PRINT_MATRICES)
    LIST(APPEND _VECADD_BENCHMARK_COMPILE_DEFINITIONS "VECADD_BENCHMARK_PRINT_MATRICES")
ENDIF()

IF(NOT MSVC)
    LIST(APPEND _VECADD_BENCHMARK_COMPILE_OPTIONS "-std=c99")
ENDIF()

#-------------------------------------------------------------------------------
# Find OpenMP.
#-------------------------------------------------------------------------------
IF(VECADD_BENCHMARK_PAR_OMP2_GUIDED OR VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_OMP2_T_SEQ OR VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_SEQ_T_OMP2 OR VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_BT_OMP4)
    FIND_PACKAGE(OpenMP)
    IF(NOT OPENMP_FOUND)
        MESSAGE(ERROR "benchmark dependency OpenMP could not be found!")

    ELSE()
        LIST(APPEND _VECADD_BENCHMARK_COMPILE_OPTIONS ${OpenMP_C_FLAGS})
    ENDIF()
ENDIF()

#-------------------------------------------------------------------------------
# Find vecadd.
#-------------------------------------------------------------------------------

SET(VECADD_ROOT "${CMAKE_CURRENT_LIST_DIR}/../" CACHE STRING  "The location of the vecadd library")

LIST(APPEND CMAKE_MODULE_PATH "${VECADD_ROOT}")
FIND_PACKAGE("vecadd" REQUIRED)

#-------------------------------------------------------------------------------
# Common.
#-------------------------------------------------------------------------------

INCLUDE("${VECADD_ROOT}/cmake/common.cmake")

#-------------------------------------------------------------------------------
# Add executable.
#-------------------------------------------------------------------------------

SET(_VECADD_BENCHMARK_INCLUDE_DIR "include/")
SET(_VECADD_BENCHMARK_SUFFIXED_INCLUDE_DIR "${_VECADD_BENCHMARK_INCLUDE_DIR}benchmark/")
SET(_VECADD_BENCHMARK_SOURCE_DIR "src/")

# Add all the include files in all recursive subdirectories and group them accordingly.
append_recursive_files_add_to_src_group("${_VECADD_BENCHMARK_SUFFIXED_INCLUDE_DIR}" "" "h" _VECADD_BENCHMARK_FILES_HEADER)

# Add all the source files in all recursive subdirectories and group them accordingly.
append_recursive_files_add_to_src_group("${_VECADD_BENCHMARK_SOURCE_DIR}" "" "c" _VECADD_BENCHMARK_FILES_SOURCE)

ADD_EXECUTABLE(
    "vecadd_benchmark"
    ${_VECADD_BENCHMARK_FILES_HEADER} ${_VECADD_BENCHMARK_FILES_SOURCE})
TARGET_COMPILE_OPTIONS(
    "vecadd_benchmark"
    PRIVATE ${_VECADD_BENCHMARK_COMPILE_OPTIONS})
TARGET_COMPILE_DEFINITIONS(
    "vecadd_benchmark"
    PRIVATE ${_VECADD_BENCHMARK_COMPILE_DEFINITIONS})
TARGET_INCLUDE_DIRECTORIES(
    "vecadd_benchmark"
    PUBLIC ${_VECADD_BENCHMARK_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(
    "vecadd_benchmark"
    PRIVATE "vecadd" ${_VECADD_BENCHMARK_LINK_FLAGS})
