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

#include <vecadd/common/Vec.h>

#include <assert.h>             // assert
#include <math.h>               // fabs
#include <string.h>             // memcpy
#include <stdio.h>              // printf

#include <float.h>

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool vecadd_vec_cmp(
    TIdx const n,
    TElem const * const VECADD_RESTRICT A,
    TElem const * const VECADD_RESTRICT B,
    TElem const errorThreshold)
{
    // The maximum number of error values being printed.
    static TIdx const maxErrorsPrint = 100;

    TIdx errorCount = 0;

    // Loop through all values, print out errors and get the maximum error.
    TElem maxError = (TElem)0.0;
    for(TIdx i = 0; i < n; ++i)
    {
        TElem const error = (TElem)fabs(A[i] - B[i]);
        if(error > errorThreshold)
        {
            if(errorCount < maxErrorsPrint)
            {
                printf("\nError in Cell [%"VECADD_PRINTF_SIZE_T"] of %16.16lf A: %f B: %f", (size_t)i, error, A[i], B[i]);
            }
            ++errorCount;
        }

        maxError = (maxError<error) ? error : maxError;
    }
    // Print the number of errors not printed.
    if(errorCount > maxErrorsPrint)
    {
        printf("\n... %"VECADD_PRINTF_SIZE_T" more errors in the vector.", (size_t)errorCount-maxErrorsPrint);
    }
    // Print the maximum error.
    if(maxError > errorThreshold)
    {
        printf("\nfMaxDiff=%32.28lf", maxError);
    }
    // If something has been printed, add a newline.
    if(errorCount > 0)
    {
        printf("\n");
    }

    return (errorCount==0);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void vecadd_vec_print(
    TIdx const n,
    TElem const * const VECADD_RESTRICT A,
    char * const elemSeperator,
    char * const dimBegin, char * const dimEnd)
{
    printf("%s", dimBegin);
    for(TIdx i = 0; i < n; ++i)
    {
        if(i>0)
        {
            printf("%s", elemSeperator);
        }
        printf("%f", A[i]);
    }
    printf("%s", dimEnd);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void vecadd_vec_print_simple(
    TIdx const n,
    TElem const * const VECADD_RESTRICT A)
{
    vecadd_vec_print(
        n,
        A,
        ",",
        "", "");
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void vecadd_vec_print_mathematica(
    TIdx const n,
    TElem const * const VECADD_RESTRICT A)
{
    vecadd_vec_print(
        n,
        A,
        ",",
        "{", "}");
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void matmul_vec_copy(
    TIdx const n,
    TElem const * const VECADD_RESTRICT pSrcVec,
    TElem * const VECADD_RESTRICT pDstVec)
{
    memcpy(pDstVec, pSrcVec, sizeof(TElem)*n);
}
