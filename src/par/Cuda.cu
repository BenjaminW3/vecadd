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

#if defined(VECADD_BUILD_PAR_CUDA)

    #include <vecadd/par/Cuda.h>

    #include <vecadd/common/Time.h> // getTimeSec

    #include <cuda_runtime.h>

    #include <stdio.h>              // printf
    #include <math.h>               // ceil
    #include <algorithm>            // std::min

    #define VECADD_CUDA_RT_CHECK(cmd) {cudaError_t error = cmd; if(error!=cudaSuccess){printf("<%s>:%i ",__FILE__,__LINE__); printf("[CUDA] Error: %s\n", cudaGetErrorString(error));}}

        //-----------------------------------------------------------------------------
        // This function only works for square blocks.
        //-----------------------------------------------------------------------------
        __global__ void vecadd_axpy_par_cuda_kernel(
            TSize const n,
            TElem const alpha,
            TElem const * const VECADD_RESTRICT X,
            TElem * const VECADD_RESTRICT Y)
        {
            TSize const i(blockIdx.x*blockDim.x + threadIdx.x);

            if(i < n)
            {
                Y[i] = alpha * X[i] + Y[i];
            }
        }
        //-----------------------------------------------------------------------------
        //
        //-----------------------------------------------------------------------------
        TReturn vecadd_axpy_par_cuda(
            TSize const n,
            TElem const alpha,
            TElem const * const VECADD_RESTRICT X,
            TElem * const VECADD_RESTRICT Y)
        {
            // VECADD_CUDA_RT_CHECK(cudaSetDevice(0));
            cudaStream_t stream;
            VECADD_CUDA_RT_CHECK(cudaStreamCreate(&stream));

            // Get its properties.
            cudaDeviceProp cudaDevProp;
            VECADD_CUDA_RT_CHECK(cudaGetDeviceProperties(
                &cudaDevProp,
                0));

            TSize gridThreadExtent[] = {n};
            TSize blockThreadExtent[] = {cudaDevProp.maxThreadsDim[0]};

            // Restrict the max block thread extent with the grid thread extent.
            // This removes dimensions not required in the given grid thread extent.
            // This has to be done before the maxThreadsPerBlock clipping to get the maximum correctly.
            for(TSize i(0); i<1; ++i)
            {
                blockThreadExtent[i] = std::min(blockThreadExtent[i], gridThreadExtent[i]);
            }

            // Restrict it to its minimum component.
            // For example (512, 256) will get (256, 256).
            auto minBlockThreadExtent(blockThreadExtent[0]);
            for(TSize i(1); i<1; ++i)
            {
                minBlockThreadExtent = std::min(minBlockThreadExtent, blockThreadExtent[i]);
            }
            for(TSize i(0); i<1; ++i)
            {
                blockThreadExtent[i] = minBlockThreadExtent;
            }

            // Adjust blockThreadExtent if its product is too large.
            if ((blockThreadExtent[0]) > cudaDevProp.maxThreadsPerBlock)
            {
                // Satisfy the following equation:
                // udaDevProp.maxThreadsPerBlock >= blockThreadExtent[0]*blockThreadExtent[1]
                // For example 1024 >= 512 * 512

                // For equal block thread extent this is easily the nth root of cudaDevProp.maxThreadsPerBlock.
                double const fNthRoot(std::pow(cudaDevProp.maxThreadsPerBlock, 1.0 / 1.0));
                auto const nthRoot(static_cast<TSize>(fNthRoot));
                for(TSize i(0); i<1; ++i)
                {
                    blockThreadExtent[i] = nthRoot;
                }
            }

            // Set the grid block extent (rounded to the next integer not less then the quotient.
            TSize gridBlockExtent[] = {1};
            for(TSize i(0); i<1; ++i)
            {
                gridBlockExtent[i] =
                    static_cast<TSize>(
                        std::ceil(static_cast<double>(gridThreadExtent[i])
                            / static_cast<double>(blockThreadExtent[i])));
            }

            dim3 const dimBlock(blockThreadExtent[0]);
            dim3 const dimGrid(gridBlockExtent[0]);

            VECADD_TIME_START;

            vecadd_axpy_par_cuda_kernel<<<
                dimGrid,
                dimBlock,
                0,
                stream>>>(
                    n,
                    alpha,
                    X,
                    Y);

            VECADD_CUDA_RT_CHECK(cudaStreamSynchronize(stream));

            VECADD_TIME_END;

            VECADD_CUDA_RT_CHECK(cudaStreamDestroy(stream));

            VECADD_TIME_RETURN;
        }
#endif
