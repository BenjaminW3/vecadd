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

#if defined(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_SEQ) || defined(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_OMP2_T_SEQ) || defined(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_OMP2) || defined(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_BT_OMP4) || defined(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_THREADS) || defined(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_FIBERS)

    #include <vecadd/par/Alpaka.h>

    #include <vecadd/par/Alpaka.hpp>

    #ifdef VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_SEQ
        //-----------------------------------------------------------------------------
        //
        //-----------------------------------------------------------------------------
        TReturn vecadd_axpy_seq_alpaka_cpu_b_seq_t_seq(
            TSize const n,
            TElem const alpha,
            TElem const * const X,
            TElem * const Y)
        {
            return
                vecadd_axpy_par_alpaka<alpaka::acc::AccCpuSerial<alpaka::dim::DimInt<1u>, TSize>, AxpyAlpakaKernel>(
                    n,
                    1u,
                    alpha,
                    X,
                    Y);
        }
        //-----------------------------------------------------------------------------
        //
        //-----------------------------------------------------------------------------
        TReturn vecadd_axpy_seq_vec_alpaka_cpu_b_seq_t_seq(
            TSize const n,
            TElem const alpha,
            TElem const * const X,
            TElem * const Y)
        {
            return
                vecadd_axpy_par_alpaka<alpaka::acc::AccCpuSerial<alpaka::dim::DimInt<1u>, TSize>, AxpyVectorizedAlpakaKernel>(
                    n,
                    n,
                    alpha,
                    X,
                    Y);
        }
    #endif
    #ifdef VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_OMP2_T_SEQ
        //-----------------------------------------------------------------------------
        //
        //-----------------------------------------------------------------------------
        TReturn vecadd_axpy_par_alpaka_cpu_b_omp2_t_seq(
            TSize const n,
            TElem const alpha,
            TElem const * const X,
            TElem * const Y)
        {
            return
                vecadd_axpy_par_alpaka<alpaka::acc::AccCpuOmp2Blocks<alpaka::dim::DimInt<1u>, TSize>, AxpyAlpakaKernel>(
                    n,
                    1u,
                    alpha,
                    X,
                    Y);
        }
        //-----------------------------------------------------------------------------
        //
        //-----------------------------------------------------------------------------
        TReturn vecadd_axpy_par_vec_alpaka_cpu_b_omp2_t_seq(
            TSize const n,
            TElem const alpha,
            TElem const * const X,
            TElem * const Y)
        {
            return
                vecadd_axpy_par_alpaka<alpaka::acc::AccCpuOmp2Blocks<alpaka::dim::DimInt<1u>, TSize>, AxpyVectorizedAlpakaKernel>(
                    n,
                    n / alpaka::omp::getMaxOmpThreads(),
                    alpha,
                    X,
                    Y);
        }
    #endif
    #ifdef VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_OMP2
        //-----------------------------------------------------------------------------
        //
        //-----------------------------------------------------------------------------
        TReturn vecadd_axpy_par_alpaka_cpu_b_seq_t_omp2(
            TSize const n,
            TElem const alpha,
            TElem const * const X,
            TElem * const Y)
        {
            return
                vecadd_axpy_par_alpaka<alpaka::acc::AccCpuOmp2Threads<alpaka::dim::DimInt<1u>, TSize>, AxpyAlpakaKernel>(
                    n,
                    1u,
                    alpha,
                    X,
                    Y);
        }
    #endif
    #ifdef VECADD_BUILD_PAR_ALPAKA_ACC_CPU_BT_OMP4
        //-----------------------------------------------------------------------------
        //
        //-----------------------------------------------------------------------------
        TReturn vecadd_axpy_par_alpaka_cpu_bt_omp4(
            TSize const n,
            TElem const alpha,
            TElem const * const X,
            TElem * const Y)
        {
            return
                vecadd_axpy_par_alpaka<alpaka::acc::AccCpuOmp4<alpaka::dim::DimInt<1u>, TSize>, AxpyAlpakaKernel>(
                    n,
                    1u,
                    alpha,
                    X,
                    Y);
        }
    #endif
    #ifdef VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_THREADS
        //-----------------------------------------------------------------------------
        //
        //-----------------------------------------------------------------------------
        TReturn vecadd_axpy_par_alpaka_cpu_b_seq_t_threads(
            TSize const n,
            TElem const alpha,
            TElem const * const X,
            TElem * const Y)
        {
            return
                vecadd_axpy_par_alpaka<alpaka::acc::AccCpuThreads<alpaka::dim::DimInt<1u>, TSize>, AxpyAlpakaKernel>(
                    n,
                    1u,
                    alpha,
                    X,
                    Y);
        }
    #endif
    #ifdef VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_FIBERS
        //-----------------------------------------------------------------------------
        //
        //-----------------------------------------------------------------------------
        TReturn vecadd_axpy_seq_alpaka_cpu_b_seq_t_fibers(
            TSize const n,
            TElem const alpha,
            TElem const * const X,
            TElem * const Y)
        {
            return
                vecadd_axpy_par_alpaka<alpaka::acc::AccCpuFibers<alpaka::dim::DimInt<1u>, TSize>, AxpyAlpakaKernel>(
                    n,
                    1u,
                    alpha,
                    X,
                    Y);
        }
    #endif
#endif
