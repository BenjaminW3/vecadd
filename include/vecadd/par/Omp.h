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

#if defined(VECADD_BUILD_PAR_OMP2_GUIDED)

    #include <vecadd/common/Config.h>   // TElem, TIdx

    #ifdef __cplusplus
        extern "C"
        {
    #endif
    #if _OPENMP >= 200203   // OpenMP 2.0
        #ifdef VECADD_BUILD_PAR_OMP2_GUIDED
            //-----------------------------------------------------------------------------
            //! (S/D) vector addition Y = A + X using OpenMP 2.0 parallel for guided schedule.
            //!
            //! \param n Specifies the number of elements of the vectors X and Y.
            //! \param alpha Scalar the X vector is multiplied with.
            //! \param X Vector of at least n elements.
            //! \param Y Vector of at least n elements.
            //-----------------------------------------------------------------------------
            TReturn vecadd_axpy_par_omp2_guided_schedule(
                TIdx const n,
                TElem const alpha,
                TElem const * const VECADD_RESTRICT X,
                TElem * const VECADD_RESTRICT Y);
        #endif
    #endif
    #ifdef __cplusplus
        }
    #endif
#endif
