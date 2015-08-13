//-----------------------------------------------------------------------------
//! \file
//! Copyright 2015 Benjamin Worpitz
//!
//! This file is part of vecadd.
//!
//! vecadd is free software: you can redistribute it and/or modify
//! it under the terms of the GNU Lesser General Public License as published by
//! the Free Software Foundation, either version 3 of the License, or
//! (at your option) any later version.
//!
//! vecadd is distributed in the hope that it will be useful,
//! but WITHOUT ANY WARRANTY; without even the implied warranty of
//! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//! GNU Lesser General Public License for more details.
//!
//! You should have received a copy of the GNU Lesser General Public License
//! along with vecadd.
//! If not, see <http://www.gnu.org/licenses/>.
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------
// Data type depending definitions.
//-----------------------------------------------------------------------------
#ifdef VECADD_ELEMENT_TYPE_DOUBLE
    typedef double TElem;
    #define VECADD_EPSILON DBL_EPSILON    //!< This is used to calculate whether a result value is within a vector size dependent error range.
#else
    typedef float TElem;
    #define VECADD_EPSILON FLT_EPSILON    //!< This is used to calculate whether a result value is within a vector size dependent error range.
#endif

#include <stddef.h>                 // size_t
#include <stdint.h>                 // int32_t

typedef VECADD_INDEX_TYPE TIdx;

//-----------------------------------------------------------------------------
// Compiler Settings.
//-----------------------------------------------------------------------------
#if defined __INTEL_COMPILER                    // ICC additionally defines _MSC_VER if used in VS so this has to come first
    #define VECADD_ICC
    #ifdef __cplusplus
        #define VECADD_RESTRICT __restrict
    #else
        #define VECADD_RESTRICT restrict
    #endif
    #if defined(_MSC_VER) && _MSC_VER<=1800
        #define VECADD_PRINTF_SIZE_T "Iu"
    #else
        #define VECADD_PRINTF_SIZE_T "zu"
    #endif

#elif defined __clang__
    #define VECADD_CLANG
    #ifdef __cplusplus
        #define VECADD_RESTRICT __restrict__
    #else
        #define VECADD_RESTRICT restrict
    #endif
    #if defined(_MSC_VER) && _MSC_VER<=1800
        #define VECADD_PRINTF_SIZE_T "Iu"
    #else
        #define VECADD_PRINTF_SIZE_T "zu"
    #endif

#elif defined __GNUC__
    #define VECADD_GCC
    #ifdef __cplusplus
        #define VECADD_RESTRICT __restrict__
    #else
        #define VECADD_RESTRICT restrict
    #endif
    #define VECADD_PRINTF_SIZE_T "zu"

#elif defined _MSC_VER
    #define VECADD_MSVC
    #define VECADD_RESTRICT __restrict          // Visual C++ 2013 and below do not define C99 restrict keyword under its supposed name. (And its not fully standard conformant)
    #if _MSC_VER<=1800
        #define VECADD_PRINTF_SIZE_T "Iu"       // Visual C++ 2013 and below do not support C99 printf specifiers.
    #else
        #define VECADD_PRINTF_SIZE_T "zu"
    #endif

#elif defined __CUDACC__
    #define VECADD_RESTRICT __restrict__

#elif defined __PGI
    #define VECADD_RESTRICT restrict
    #define VECADD_PRINTF_SIZE_T "zu"

#else
    #define VECADD_RESTRICT restrict
    #define VECADD_PRINTF_SIZE_T "zu"
#endif

//-----------------------------------------------------------------------------
//! The no debug level.
//-----------------------------------------------------------------------------
#define VECADD_DEBUG_DISABLED 0
//-----------------------------------------------------------------------------
//! The minimal debug level.
//-----------------------------------------------------------------------------
#define VECADD_DEBUG_MINIMAL 1
//-----------------------------------------------------------------------------
//! The full debug level.
//-----------------------------------------------------------------------------
#define VECADD_DEBUG_FULL 2
