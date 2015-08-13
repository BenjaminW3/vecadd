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

#if defined(VECADD_BUILD_PAR_CUDA)

    #include <vecadd/common/Config.h>   // TElem, TIdx

    #ifdef __cplusplus
        extern "C"
        {
    #endif
        //-----------------------------------------------------------------------------
        //! (S/D) vector addition Y = A + X using CUDA with a dynamic block size.
        //!
        //! \param n Specifies the number of elements of the vectors X and Y.
        //! \param alpha Scalar the X vector is multiplied with.
        //! \param X Vector of at least n elements.
        //! \param Y Vector of at least n elements.
        //-----------------------------------------------------------------------------
        double vecadd_axpy_par_cuda(
            TIdx const n,
            TElem const alpha,
            TElem const * const X,
            TElem * const Y);
    #ifdef __cplusplus
        }
    #endif
#endif
