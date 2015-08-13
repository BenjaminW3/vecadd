#.rst:
# Findvecadd
# ----------
#
# Vector addition library.
# https://github.com/BenjaminW3/vecadd
#
# Finding and Using vecadd
# ^^^^^^^^^^^^^^^^^^^^^
#
# .. code-block:: cmake
#
#   FIND_PACKAGE(vecadd
#     [version] [EXACT]     # Minimum or EXACT version, e.g. 1.0.0
#     [REQUIRED]            # Fail with an error if vecadd or a required
#                           # component is not found
#     [QUIET]               # Do not warn if this module was not found
#     [COMPONENTS <...>]    # Compiled in components: ignored
#   )
#   TARGET_LINK_LIBRARIES(<target> PUBLIC vecadd)
#
# To provide a hint to this module where to find the vecadd installation,
# set the VECADD_ROOT variable.
#
# Set the following CMake variables BEFORE calling FIND_PACKAGE to
# change the behavior of this module:
# - ``VECADD_ELEMENT_TYPE_DOUBLE`` {ON, OFF}
# - ``VECADD_INDEX_TYPE`` {int, size_t, ...}
# - ``VECADD_ALIGNED_MALLOC`` {ON, OFF}
# - ``VECADD_OMP_PRINT_NUM_CORES`` {ON, OFF}
#
# Set the following CMake variables BEFORE calling FIND_PACKAGE to
# select the versions being compiled:
# NOTE: Either MPI or CUDA device only or host timings can be activated.
# So only elements of one of the following 3 blocks can be active:
# - ``VECADD_BUILD_SEQ_BASIC`` {ON, OFF}
# - ``VECADD_BUILD_PAR_OMP2_GUIDED`` {ON, OFF}
# - ``VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_SEQ`` {ON, OFF}
# - ``VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_OMP2_T_SEQ`` {ON, OFF}
# - ``VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_OMP2`` {ON, OFF}
# - ``VECADD_BUILD_PAR_ALPAKA_ACC_CPU_BT_OMP4`` {ON, OFF}
# - ``VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_FIBERS`` {ON, OFF}
# - ``VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_THREADS`` {ON, OFF}
#
# - ``VECADD_BUILD_PAR_ALPAKA_ACC_GPU_CUDA`` {ON, OFF}
# - ``VECADD_BUILD_PAR_CUDA`` {ON, OFF}
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# - ``vecadd_DEFINITIONS``
#   Compiler definitions.
# - ``vecadd_FOUND``
#   TRUE if vecadd found a working install.
# - ``vecadd_INCLUDE_DIRS``
#   Include directories for the vecadd headers.
# - ``vecadd_LIBRARIES``
#   vecadd libraries.
# - ``vecadd_VERSION``
#   Version in format Major.Minor.Patch
#
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
# This module defines the :prop_tgt:`IMPORTED` target ``vecadd``, if vecadd has
# been found.
#


################################################################################
# Copyright 2015 Benjamin Worpitz
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
# SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
# RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
# NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE
# USE OR PERFORMANCE OF THIS SOFTWARE.
################################################################################

FIND_PATH(_VECADD_ROOT_DIR
  NAMES "include/vecadd/vecadd.h"
  HINTS "${VECADD_ROOT}" ENV VECADD_ROOT
  DOC "vecadd ROOT location")

IF(_VECADD_ROOT_DIR)
    INCLUDE("${_VECADD_ROOT_DIR}/vecaddConfig.cmake")
ELSE()
    MESSAGE(FATAL_ERROR "vecadd could not be found!")
ENDIF()
