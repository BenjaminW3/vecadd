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

#include <vecadd/common/Config.h>   // TElem, TSize

#ifdef __cplusplus
    extern "C"
    {
#endif
//-----------------------------------------------------------------------------
//! \return A random value in the range [fMin, fMax].
//-----------------------------------------------------------------------------
TElem vecadd_gen_rand_val(
    TElem const min,
    TElem const max);

//-----------------------------------------------------------------------------
//! Fills the array with the given value.
//! \param pArray The array.
//! \param elemCount The number of elements in the array.
//-----------------------------------------------------------------------------
void vecadd_arr_fill_val(
    TElem * const pArray,
    TSize const elemCount,
    TElem const val);
//-----------------------------------------------------------------------------
//! Fills the array with zeros.
//! \param pArray The array.
//! \param elemCount The number of elements in the array.
//-----------------------------------------------------------------------------
void vecadd_arr_fill_zero(
    TElem * const pArray,
    TSize const elemCount);
//-----------------------------------------------------------------------------
//! Fills the array with the indices as values.
//! \param pArray The array.
//! \param elemCount The number of elements in the array.
//-----------------------------------------------------------------------------
void vecadd_arr_fill_idx(
    TElem * const pArray,
    TSize const elemCount);
//-----------------------------------------------------------------------------
//! Fills the array with random numbers.
//! \param pArray The array.
//! \param elemCount The number of elements in the array.
//-----------------------------------------------------------------------------
void vecadd_arr_fill_rand(
    TElem * const pArray,
    TSize const elemCount,
    TElem const min,
    TElem const max);

//-----------------------------------------------------------------------------
//! \return A array of the given type initialized with the given value.
//! \param elemCount The number of elements in the array.
//-----------------------------------------------------------------------------
TElem * vecadd_arr_alloc_fill_val(
    TSize const elemCount,
    TElem const val);
//-----------------------------------------------------------------------------
//! \return A array of the given type initialized with zero.
//! \param elemCount The number of elements in the array.
//-----------------------------------------------------------------------------
TElem * vecadd_arr_alloc_fill_zero(
    TSize const elemCount);
//-----------------------------------------------------------------------------
//! \return A array of the given type initialized with the indices as values.
//! \param elemCount The number of elements in the array.
//-----------------------------------------------------------------------------
TElem * vecadd_arr_alloc_fill_idx(
    TSize const elemCount);
//-----------------------------------------------------------------------------
//! \return A array of random values of the given type.
//! \param elemCount The number of elements in the array.
//-----------------------------------------------------------------------------
TElem * vecadd_arr_alloc_fill_rand(
    TSize const elemCount,
    TElem const min,
    TElem const max);
#ifdef __cplusplus
    }
#endif
