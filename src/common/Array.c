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

#include <vecadd/common/Array.h>

#include <vecadd/common/Alloc.h>

#include <assert.h>
#include <stdlib.h>        // RAND_MAX, srand

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TElem vecadd_gen_rand_val(
    TElem const min,
    TElem const max)
{
    assert(min <= max); // bad input

    return ((TElem)rand()/(TElem)(RAND_MAX)) * (max-min) + min;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void vecadd_arr_fill_val(
    TElem * const pArray,
    TSize const numElements,
    TElem const val)
{
    assert(pArray);

    for(TSize i = 0; i<numElements; ++i)
    {
        pArray[i] = val;
    }
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void vecadd_arr_fill_zero(
    TElem * const pArray,
    TSize const numElements)
{
    vecadd_arr_fill_val(
        pArray,
        numElements,
        (TElem)0);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void vecadd_arr_fill_idx(
    TElem * const pArray,
    TSize const numElements)
{
    assert(pArray);

    for(TSize i = 0; i<numElements; ++i)
    {
        pArray[i] = (TElem)i;
    }
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void vecadd_arr_fill_rand(
    TElem * const pArray,
    TSize const numElements,
    TElem const min,
    TElem const max)
{
    assert(pArray);

    for(TSize i = 0; i<numElements; ++i)
    {
        pArray[i] = vecadd_gen_rand_val(min, max);
    }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TElem * vecadd_arr_alloc_fill_val(
    TSize const numElements,
    TElem const val)
{
    TElem * arr = vecadd_arr_alloc(numElements);

    vecadd_arr_fill_val(arr, numElements, val);

    return arr;
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TElem * vecadd_arr_alloc_fill_zero(
    TSize const numElements)
{
    TElem * arr = vecadd_arr_alloc(numElements);

    vecadd_arr_fill_zero(arr, numElements);

    return arr;
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TElem * vecadd_arr_alloc_fill_idx(
    TSize const numElements)
{
    TElem * arr = vecadd_arr_alloc(numElements);

    vecadd_arr_fill_idx(arr, numElements);

    return arr;
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TElem * vecadd_arr_alloc_fill_rand(
    TSize const numElements,
    TElem const min,
    TElem const max)
{
    TElem * arr = vecadd_arr_alloc(numElements);

    vecadd_arr_fill_rand(arr, numElements, min, max);

    return arr;
}
