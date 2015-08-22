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

#ifdef VECADD_BUILD_SEQ_BASIC

    #include <vecadd/seq/Basic.h>

    #include <vecadd/common/Time.h> // getTimeSec

    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    TReturn vecadd_axpy_seq_basic(
        TSize const n,
        TElem const alpha,
        TElem const * const VECADD_RESTRICT X,
        TElem * const VECADD_RESTRICT Y)
    {
        VECADD_TIME_START;

        for(TSize i = 0; i < n; ++i)
        {
            Y[i] = alpha * X[i] + Y[i];
        }

        VECADD_TIME_END;
        VECADD_TIME_RETURN;
    }
#endif
