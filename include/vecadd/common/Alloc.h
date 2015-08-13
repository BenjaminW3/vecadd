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

#include <vecadd/common/Config.h>   // TElem

#ifdef __cplusplus
    extern "C"
    {
#endif
//-----------------------------------------------------------------------------
//! Tries to allocate the memory on 64 Byte boundary if the operating system allows this.
//! \return A array of random values of the given type.
//! \param elemCount The number of elements in the vector.
//-----------------------------------------------------------------------------
TElem * vecadd_arr_alloc(
    TIdx const numBytes);

//-----------------------------------------------------------------------------
//! \return A array of random values of the given type.
//! \param elemCount The number of elements in the vector.
//-----------------------------------------------------------------------------
void vecadd_arr_free(
    TElem * const VECADD_RESTRICT ptr);
#ifdef __cplusplus
    }
#endif
