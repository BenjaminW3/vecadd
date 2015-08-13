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

#include <vecadd/common/Config.h>   // TElem, TIdx

#include <stdbool.h>                // bool

#ifdef __cplusplus
    extern "C"
    {
#endif
    //-----------------------------------------------------------------------------
    //! Vector comparison.
    //!
    //! \param n The number of columns.
    //! \param A The left input vector.
    //! \param X The right input vector.
    //! \return If the matrices compare equal (under the given threshold).
    //-----------------------------------------------------------------------------
    bool vecadd_vec_cmp(
        TIdx const n,
        TElem const * const VECADD_RESTRICT A,
        TElem const * const VECADD_RESTRICT B,
        TElem const errorThreshold);

    //-----------------------------------------------------------------------------
    //! Prints the vector to the console.
    //!
    //! \param n The number of columns.
    //! \param A The vector to print.
    //-----------------------------------------------------------------------------
    void vecadd_vec_print(
        TIdx const n,
        TElem const * const VECADD_RESTRICT A,
        char * const elemSeperator,
        char * const dimBegin, char * const dimEnd);
    //-----------------------------------------------------------------------------
    //! Prints the vector to the console.
    //!
    //! \param n The number of columns.
    //! \param A The vector to print.
    //-----------------------------------------------------------------------------
    void vecadd_vec_print_simple(
        TIdx const n,
        TElem const * const VECADD_RESTRICT A);
    //-----------------------------------------------------------------------------
    //! Prints the vector to the console.
    //!
    //! \param n The number of columns.
    //! \param A The vector to print.
    //-----------------------------------------------------------------------------
    void vecadd_vec_print_mathematica(
        TIdx const n,
        TElem const * const VECADD_RESTRICT A);
    //-----------------------------------------------------------------------------
    //! Copy the vector pSrcVec to the pDstVec.
    //!
    //! \param n The number of columns.
    //! \param pSrcVec Row major source vector.
    //! \param pDstVec Row major destination vector.
    //-----------------------------------------------------------------------------
    void matmul_vec_copy(
        TIdx const n,
        TElem const * const VECADD_RESTRICT pSrcVec,
        TElem * const VECADD_RESTRICT pDstVec);
#ifdef __cplusplus
    }
#endif
