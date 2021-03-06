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

#if defined(VECADD_BUILD_PAR_ALPAKA_ACC_GPU_CUDA)

    #include <vecadd/par/Alpaka.h>

    #include <vecadd/par/Alpaka.hpp>

    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    TReturn vecadd_axpy_par_alpaka_gpu_cuda(
        TSize const n,
        TElem const alpha,
        TElem const * const X,
        TElem * const Y)
    {
        return
            vecadd_axpy_par_alpaka<alpaka::acc::AccGpuCudaRt<alpaka::dim::DimInt<1u>, TSize>, AxpyAlpakaKernel>(
                n,
                1u,
                alpha,
                X,
                Y);
    }
    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    TReturn vecadd_axpy_par_vec2_alpaka_gpu_cuda(
        TSize const n,
        TElem const alpha,
        TElem const * const X,
        TElem * const Y)
    {
        return
            vecadd_axpy_par_alpaka<alpaka::acc::AccGpuCudaRt<alpaka::dim::DimInt<1u>, TSize>, AxpyVectorizedAlpakaKernel>(
                n,
                2u,
                alpha,
                X,
                Y);
    }
    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    TReturn vecadd_axpy_par_vec4_alpaka_gpu_cuda(
        TSize const n,
        TElem const alpha,
        TElem const * const X,
        TElem * const Y)
    {
        return
            vecadd_axpy_par_alpaka<alpaka::acc::AccGpuCudaRt<alpaka::dim::DimInt<1u>, TSize>, AxpyVectorizedAlpakaKernel>(
                n,
                4u,
                alpha,
                X,
                Y);
    }
    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    TReturn vecadd_axpy_par_vec8_alpaka_gpu_cuda(
        TSize const n,
        TElem const alpha,
        TElem const * const X,
        TElem * const Y)
    {
        return
            vecadd_axpy_par_alpaka<alpaka::acc::AccGpuCudaRt<alpaka::dim::DimInt<1u>, TSize>, AxpyVectorizedAlpakaKernel>(
                n,
                8u,
                alpha,
                X,
                Y);
    }
#endif
