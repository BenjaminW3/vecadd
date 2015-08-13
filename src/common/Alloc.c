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

#include <vecadd/common/Alloc.h>

#include <stdlib.h>             // malloc, free

#if defined VECADD_ALIGNED_MALLOC
    #ifndef _MSC_VER
        #include <malloc.h>     // memalign or valloc are not always declared in stdlib.h.
    #endif
#endif

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void * vecadd_arr_aligned_alloc_internal(
    TIdx const numBytes)
{
#if defined VECADD_ALIGNED_MALLOC
    // If c11 were supported we could use void *aligned_alloc(size_t alignment, size_t size);
#if defined _MSC_VER
    return _aligned_malloc(numBytes, 64);
#elif defined __linux__
    return memalign(64, numBytes);
#elif defined __MACH__      // Mac OS X
    return malloc(numBytes);    // malloc is always 16 byte aligned on Mac.
#else
    return valloc(numBytes);    // other (use valloc for page-aligned memory)
#endif
#else
    return malloc(numBytes);
#endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TElem * vecadd_arr_alloc(
    TIdx const numElements)
{
    TIdx const numBytes = numElements * sizeof(TElem);

    return (TElem*) vecadd_arr_aligned_alloc_internal(numBytes);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void vecadd_arr_aligned_free_internal(
    void * const VECADD_RESTRICT ptr)
{
#if defined VECADD_ALIGNED_MALLOC
#if defined _MSC_VER
    _aligned_free(ptr);
#elif defined __linux__
    free(ptr);
#elif defined __MACH__
    free(ptr);
#else
    free(ptr);
#endif
#else
    free(ptr);
#endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void vecadd_arr_free(
    TElem * const VECADD_RESTRICT ptr)
{
    vecadd_arr_aligned_free_internal((void*)ptr);
}
