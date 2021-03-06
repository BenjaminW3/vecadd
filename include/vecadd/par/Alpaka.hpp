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

    #include <alpaka/alpaka.hpp>

    #include <vecadd/common/Time.h> // getTimeSec

    #include <stdio.h>              // printf
    #include <math.h>               // ceil
    #include <type_traits>          // std::is_same

    //#############################################################################
    //
    //#############################################################################
    class AxpyAlpakaKernel
    {
    public:
        //-----------------------------------------------------------------------------
        //
        //-----------------------------------------------------------------------------
        ALPAKA_NO_HOST_ACC_WARNING
        template<
            typename TAcc,
            typename TElem>
        ALPAKA_FN_ACC auto operator()(
            TAcc const & acc,
            TSize const & n,
            TElem const & alpha,
            TElem const * const VECADD_RESTRICT X,
            TElem * const VECADD_RESTRICT Y) const
        -> void
        {
            static_assert(alpaka::dim::Dim<TAcc>::value == 1u,
                "The accelerator used for the AxpyAlpakaKernel has to be 1 dimensional!");

            auto const i(alpaka::idx::getIdx<alpaka::Grid, alpaka::Threads>(acc)[0u]);

            if(i < n)
            {
                Y[i] = alpha * X[i] + Y[i];
            }
        }
    };

    //#############################################################################
    //
    //#############################################################################
    class AxpyVectorizedAlpakaKernel
    {
    public:
        //-----------------------------------------------------------------------------
        //
        //-----------------------------------------------------------------------------
        ALPAKA_NO_HOST_ACC_WARNING
        template<
            typename TAcc,
            typename TElem>
        ALPAKA_FN_ACC auto operator()(
            TAcc const & acc,
            TSize const & n,
            TElem const & alpha,
            TElem const * const VECADD_RESTRICT X,
            TElem * const VECADD_RESTRICT Y) const
        -> void
        {
            static_assert(alpaka::dim::Dim<TAcc>::value == 1u,
                "The accelerator used for the AxpyAlpakaKernel has to be 1 dimensional!");

            auto const gridThreadIdx(alpaka::idx::getIdx<alpaka::Grid, alpaka::Threads>(acc)[0u]);
            auto const threadElemExtent(alpaka::workdiv::getWorkDiv<alpaka::Thread, alpaka::Elems>(acc)[0u]);
            auto const threadFirstElemIdx(gridThreadIdx * threadElemExtent);

            if(threadFirstElemIdx < n)
            {
                // Calculate the number of elements to compute in this thread.
                // The result is uniform for all but the last thread.
                auto const elems(threadElemExtent + alpaka::math::min(acc, 0, n-(threadFirstElemIdx+threadElemExtent)));

                for(TSize i(threadFirstElemIdx); i<(threadFirstElemIdx+elems); ++i)
                {
                    Y[i] = alpha * X[i] + Y[i];
                }
            }
        }
    };

    namespace detail
    {
        //#############################################################################
        //! The stream type trait for the stream that should be used for the given device.
        //#############################################################################
        template<
            typename TDev,
            typename TSfinae = void>
        struct StreamType;

        //#############################################################################
        //! The stream type trait specialization for the CPU device.
        //#############################################################################
        template<>
        struct StreamType<
            alpaka::dev::DevCpu>
        {
//#if (VECADD_DEBUG >= VECADD_DEBUG_FULL)
            using type = alpaka::stream::StreamCpuSync;
/*#else
            using type = alpaka::stream::StreamCpuAsync;
#endif*/
        };

#if defined(ALPAKA_ACC_GPU_CUDA_ENABLED) && defined(__CUDACC__)
        //#############################################################################
        //! The stream type trait specialization for the CUDA device.
        //#############################################################################
        template<>
        struct StreamType<
            alpaka::dev::DevCudaRt>
        {
#if (VECADD_DEBUG >= VECADD_DEBUG_FULL)
            using type = alpaka::stream::StreamCudaRtSync;
#else
            using type = alpaka::stream::StreamCudaRtAsync;
#endif
        };
#endif
    }
    //#############################################################################
    //! The stream type that should be used for the given device.
    //#############################################################################
    template<
        typename TDev>
    using Stream = typename detail::StreamType<TDev>::type;

    //-----------------------------------------------------------------------------
    //
    //-----------------------------------------------------------------------------
    template<
        typename TAcc,
        typename TKernelFnObj>
    TReturn vecadd_axpy_par_alpaka(
        TSize const n,
        TSize const threadElemExtent,
        TElem const alpha,
        TElem const * const VECADD_RESTRICT X,
        TElem * const VECADD_RESTRICT Y)
    {
        // Select a device to execute on.
        alpaka::dev::Dev<TAcc> devAcc(
            alpaka::dev::DevMan<TAcc>::getDevByIdx(0));

        // Get a stream on this device.
        Stream<alpaka::dev::Dev<TAcc>> stream(devAcc);

        // Let alpaka calculate good block and grid sizes given our full problem extent.
        alpaka::workdiv::WorkDivMembers<alpaka::dim::DimInt<1u>, TSize> const workDiv(
            alpaka::workdiv::getValidWorkDiv<
                TAcc>(
                    devAcc,
                    n,
                    threadElemExtent,
                    false,
                    alpaka::workdiv::GridBlockExtentSubDivRestrictions::EqualExtent));

        // Create an instance of the kernel functor.
        TKernelFnObj kernel;

        // Create the executor.
        // NOTE: We remove the __restrict__ because alpaka calls std::ref on the arguments and std::ref errors.
        // This is most probably undefined. MSVC compiles it without any warning.
        auto const exec(
            alpaka::exec::create<TAcc>(
                workDiv,
                kernel,
                n,
                alpha,
                reinterpret_cast<TElem const *>(X),
                reinterpret_cast<TElem *>(Y)));

        VECADD_TIME_START;

        // Execute the kernel.
        alpaka::stream::enqueue(stream, exec);

        // Wait for the stream to finish the operations.
        alpaka::wait::wait(stream);

        VECADD_TIME_END;
        VECADD_TIME_RETURN;
    }
#endif
