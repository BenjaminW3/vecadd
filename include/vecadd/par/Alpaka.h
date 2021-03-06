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

#if defined(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_SEQ) || defined(VECADD_BUILD_PAR_ALPAKA_ACC_GPU_CUDA) || defined(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_OMP2_T_SEQ) || defined(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_OMP2) || defined(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_BT_OMP4) || defined(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_THREADS) || defined(VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_FIBERS)

    #include <vecadd/common/Config.h>   // TElem, TSize

    #ifdef __cplusplus
        extern "C"
        {
    #endif
    #ifdef VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_SEQ
        //-----------------------------------------------------------------------------
        //! (S/D) vector addition Y = A + X using alpaka`s serial accelerator back-end.
        //!
        //! \param n Specifies the number of elements of the vectors X and Y.
        //! \param alpha Scalar the X vector is multiplied with.
        //! \param X Vector of at least n elements.
        //! \param Y Vector of at least n elements.
        //-----------------------------------------------------------------------------
        TReturn vecadd_axpy_seq_alpaka_cpu_b_seq_t_seq(
            TSize const n,
            TElem const alpha,
            TElem const * const X,
            TElem * const Y);
        //-----------------------------------------------------------------------------
        //! (S/D) vector addition Y = A + X using alpaka`s serial vectorized accelerator back-end.
        //!
        //! \param n Specifies the number of elements of the vectors X and Y.
        //! \param alpha Scalar the X vector is multiplied with.
        //! \param X Vector of at least n elements.
        //! \param Y Vector of at least n elements.
        //-----------------------------------------------------------------------------
        TReturn vecadd_axpy_seq_vec_alpaka_cpu_b_seq_t_seq(
            TSize const n,
            TElem const alpha,
            TElem const * const X,
            TElem * const Y);
    #endif
    #ifdef VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_OMP2_T_SEQ
        //-----------------------------------------------------------------------------
        //! (S/D) vector addition Y = A + X using alpaka`s OpenMP 2.0 block accelerator back-end.
        //!
        //! \param n Specifies the number of elements of the vectors X and Y.
        //! \param alpha Scalar the X vector is multiplied with.
        //! \param X Vector of at least n elements.
        //! \param Y Vector of at least n elements.
        //-----------------------------------------------------------------------------
        TReturn vecadd_axpy_par_alpaka_cpu_b_omp2_t_seq(
            TSize const n,
            TElem const alpha,
            TElem const * const X,
            TElem * const Y);
        //-----------------------------------------------------------------------------
        //! (S/D) vector addition Y = A + X using alpaka`s OpenMP 2.0 block accelerator back-end.
        //!
        //! \param n Specifies the number of elements of the vectors X and Y.
        //! \param alpha Scalar the X vector is multiplied with.
        //! \param X Vector of at least n elements.
        //! \param Y Vector of at least n elements.
        //-----------------------------------------------------------------------------
        TReturn vecadd_axpy_par_vec_alpaka_cpu_b_omp2_t_seq(
            TSize const n,
            TElem const alpha,
            TElem const * const X,
            TElem * const Y);
    #endif
    #ifdef VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_OMP2
        //-----------------------------------------------------------------------------
        //! (S/D) vector addition Y = A + X using alpaka`s OpenMP 2.0 thread accelerator back-end.
        //!
        //! \param n Specifies the number of elements of the vectors X and Y.
        //! \param alpha Scalar the X vector is multiplied with.
        //! \param X Vector of at least n elements.
        //! \param Y Vector of at least n elements.
        //-----------------------------------------------------------------------------
        TReturn vecadd_axpy_par_alpaka_cpu_b_seq_t_omp2(
            TSize const n,
            TElem const alpha,
            TElem const * const X,
            TElem * const Y);
    #endif
    #ifdef VECADD_BUILD_PAR_ALPAKA_ACC_CPU_BT_OMP4
        //-----------------------------------------------------------------------------
        //! (S/D) vector addition Y = A + X using alpaka`s OpenMP 4.0 accelerator back-end.
        //!
        //! \param n Specifies the number of elements of the vectors X and Y.
        //! \param alpha Scalar the X vector is multiplied with.
        //! \param X Vector of at least n elements.
        //! \param Y Vector of at least n elements.
        //-----------------------------------------------------------------------------
        TReturn vecadd_axpy_par_alpaka_cpu_bt_omp4(
            TSize const n,
            TElem const alpha,
            TElem const * const X,
            TElem * const Y);
    #endif
    #ifdef VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_THREADS
        //-----------------------------------------------------------------------------
        //! (S/D) vector addition Y = A + X using alpaka`s std::thread back-end.
        //!
        //! \param n Specifies the number of elements of the vectors X and Y.
        //! \param alpha Scalar the X vector is multiplied with.
        //! \param X Vector of at least n elements.
        //! \param Y Vector of at least n elements.
        //-----------------------------------------------------------------------------
        TReturn vecadd_axpy_par_alpaka_cpu_b_seq_t_threads(
            TSize const n,
            TElem const alpha,
            TElem const * const X,
            TElem * const Y);
    #endif
    #ifdef VECADD_BUILD_PAR_ALPAKA_ACC_CPU_B_SEQ_T_FIBERS
        //-----------------------------------------------------------------------------
        //! (S/D) vector addition Y = A + X using alpaka`s boost::fiber back-end.
        //!
        //! \param n Specifies the number of elements of the vectors X and Y.
        //! \param alpha Scalar the X vector is multiplied with.
        //! \param X Vector of at least n elements.
        //! \param Y Vector of at least n elements.
        //-----------------------------------------------------------------------------
        TReturn vecadd_axpy_seq_alpaka_cpu_b_seq_t_fibers(
            TSize const n,
            TElem const alpha,
            TElem const * const X,
            TElem * const Y);
    #endif
    #ifdef VECADD_BUILD_PAR_ALPAKA_ACC_GPU_CUDA
        //-----------------------------------------------------------------------------
        //! (S/D) vector addition Y = A + X using alpaka`s CUDA back-end.
        //!
        //! \param n Specifies the number of elements of the vectors X and Y.
        //! \param alpha Scalar the X vector is multiplied with.
        //! \param X Vector of at least n elements.
        //! \param Y Vector of at least n elements.
        //-----------------------------------------------------------------------------
        TReturn vecadd_axpy_par_alpaka_gpu_cuda(
            TSize const n,
            TElem const alpha,
            TElem const * const X,
            TElem * const Y);
        //-----------------------------------------------------------------------------
        //! (S/D) vector addition Y = A + X using alpaka`s CUDA back-end.
        //!
        //! \param n Specifies the number of elements of the vectors X and Y.
        //! \param alpha Scalar the X vector is multiplied with.
        //! \param X Vector of at least n elements.
        //! \param Y Vector of at least n elements.
        //-----------------------------------------------------------------------------
        TReturn vecadd_axpy_par_vec2_alpaka_gpu_cuda(
            TSize const n,
            TElem const alpha,
            TElem const * const X,
            TElem * const Y);
        //-----------------------------------------------------------------------------
        //! (S/D) vector addition Y = A + X using alpaka`s CUDA back-end.
        //!
        //! \param n Specifies the number of elements of the vectors X and Y.
        //! \param alpha Scalar the X vector is multiplied with.
        //! \param X Vector of at least n elements.
        //! \param Y Vector of at least n elements.
        //-----------------------------------------------------------------------------
        TReturn vecadd_axpy_par_vec4_alpaka_gpu_cuda(
            TSize const n,
            TElem const alpha,
            TElem const * const X,
            TElem * const Y);
        //-----------------------------------------------------------------------------
        //! (S/D) vector addition Y = A + X using alpaka`s CUDA back-end.
        //!
        //! \param n Specifies the number of elements of the vectors X and Y.
        //! \param alpha Scalar the X vector is multiplied with.
        //! \param X Vector of at least n elements.
        //! \param Y Vector of at least n elements.
        //-----------------------------------------------------------------------------
        TReturn vecadd_axpy_par_vec8_alpaka_gpu_cuda(
            TSize const n,
            TElem const alpha,
            TElem const * const X,
            TElem * const Y);
    #endif
    #ifdef __cplusplus
        }
    #endif
#endif
