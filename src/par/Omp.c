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

#if defined(VECADD_BUILD_PAR_OMP2_GUIDED)

    #include <vecadd/par/Omp.h>

    #include <vecadd/common/Time.h> // getTimeSec

    #include <omp.h>

    #include <stdio.h>              // printf

    #if _OPENMP >= 200203   // OpenMP 2.0
        #ifdef VECADD_BUILD_PAR_OMP2_GUIDED
            //-----------------------------------------------------------------------------
            //
            //-----------------------------------------------------------------------------
            TReturn vecadd_axpy_par_omp2_guided_schedule(
                TSize const n,
                TElem const alpha,
                TElem const * const VECADD_RESTRICT X,
                TElem * const VECADD_RESTRICT Y)
            {
                VECADD_TIME_START;

                #pragma omp parallel
                {
            #ifdef VECADD_OMP_PRINT_NUM_CORES
                    #pragma omp single
                    {
                        printf(" p=%d ", omp_get_num_threads());
                    }
            #endif

            #if _OPENMP < 200805    // For OpenMP < 3.0 you have to declare the loop index outside of the loop header.
                    int iN = (int)n;
                    int i;
                    #pragma omp for schedule(guided)
                    for(i = 0; i < iN; ++i)
            #else
                    #pragma omp for schedule(guided)
                    for(TSize i = 0; i < n; ++i)
            #endif
                    {
                        Y[i] = alpha * X[i] + Y[i];
                    }
                }

                VECADD_TIME_END;
                VECADD_TIME_RETURN;
            }
        #endif
    #endif
#endif
