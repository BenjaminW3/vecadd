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

#include <vecadd/vecadd.h>

#include <stdio.h>                  // printf
#include <assert.h>                 // assert
#include <stdlib.h>                 // malloc, free
#include <stdbool.h>                // bool, true, false
#include <time.h>                   // time()
#include <float.h>                  // DBL_MAX
#include <math.h>                   // pow

//-----------------------------------------------------------------------------
//! A struct holding the algorithms to benchmark.
//-----------------------------------------------------------------------------
typedef struct AxpyAlgo
{
    double(*pAxpy)(TIdx const, TElem const, TElem const * const, TElem * const);
    char const * pszName;
} AxpyAlgo;

#ifdef VECADD_BENCHMARK_CUDA_NO_COPY
    #include <cuda_runtime.h>

    #define VECADD_CUDA_RT_CHECK(cmd) {cudaError_t error = cmd; if(error!=cudaSuccess){printf("<%s>:%i ",__FILE__,__LINE__); printf("[CUDA] Error: %s\n", cudaGetErrorString(error));}}
#endif

//-----------------------------------------------------------------------------
//! \return The time in milliseconds required to multiply 2 random matrices of the given type and size
//! \param n The vector extent.
//-----------------------------------------------------------------------------
double measureRandomVecAdd(
    AxpyAlgo const * const algo,
    TIdx const n,
    TIdx const repeatCount,
    bool const bRepeatTakeMinimum
 #ifdef VECADD_BENCHMARK_VERIFY_RESULT
    ,bool * pResultsCorrect
#endif
    )
{

    TElem const minVal = VECADD_EPSILON;
    TElem const maxVal = (TElem)10;

    TElem const alpha = vecadd_gen_rand_val(minVal, maxVal);

    // Allocate and initialize the matrices of the given size.
    TIdx const elemCount = n;

    TElem const * const X = vecadd_arr_alloc_fill_rand(elemCount, minVal, maxVal);
    TElem * const Y = vecadd_arr_alloc(elemCount);
    #ifdef VECADD_BENCHMARK_VERIFY_RESULT
        TElem * const D = vecadd_arr_alloc(elemCount);
    #endif

#ifdef VECADD_BENCHMARK_CUDA_NO_COPY
    VECADD_CUDA_RT_CHECK(cudaSetDevice(0));

    size_t const bytes = n*sizeof(TElem);
    TElem * pXDev = 0;
    TElem * pYDev = 0;
    VECADD_CUDA_RT_CHECK(cudaMalloc((void **)&pXDev, bytes));
    VECADD_CUDA_RT_CHECK(cudaMemcpy(pXDev, X, bytes, cudaMemcpyHostToDevice));
    VECADD_CUDA_RT_CHECK(cudaMalloc((void **)&pYDev, bytes));
    VECADD_CUDA_RT_CHECK(cudaMemcpy(pYDev, Y, bytes, cudaMemcpyHostToDevice));
#endif

    // Initialize the measurement result.
    double timeMeasuredSec = 0.0;
    if(bRepeatTakeMinimum)
    {
        timeMeasuredSec = DBL_MAX;
    }

    // Iterate.
    for(TIdx i = 0; i < repeatCount; ++i)
    {
        // We have to initialize the Y array with data before using it because else we would measure page table time on first write.
        vecadd_arr_fill_rand(Y, elemCount, minVal, maxVal);
#ifdef VECADD_BENCHMARK_VERIFY_RESULT
        matmul_vec_copy(elemCount, Y, D);
#endif

#ifdef VECADD_BENCHMARK_CUDA_NO_COPY
        VECADD_CUDA_RT_CHECK(cudaMemcpy(pYDev, Y, bytes, cudaMemcpyHostToDevice));
#endif

#ifdef VECADD_BENCHMARK_PRINT_ITERATIONS
        // If there are multiple repetitions, print the iteration we are at now.
        if(repeatCount!=1)
        {
            if(i>0)
            {
                printf("; ");
            }
            printf("\ti=%"VECADD_PRINTF_SIZE_T, (size_t)i);
        }
#endif

#ifdef VECADD_BENCHMARK_PRINT_MATRICES
        printf("\n");
        printf("%f\n*\n", alpha);
        vecadd_vec_print_simple(n, X);
        printf("\n+\n");
        vecadd_vec_print_simple(n, Y);
#endif

        //double const timeStart = getTimeSec();

        // Matrix multiplication.
        double const timeElapsed =
#ifdef VECADD_BENCHMARK_CUDA_NO_COPY
        (algo->pAxpy)(n, alpha, pXDev, pYDev);
#else
        (algo->pAxpy)(n, alpha, X, Y);
#endif

        //double const timeEnd = getTimeSec();
        //double const timeElapsed = timeEnd - timeStart;

#ifdef VECADD_BENCHMARK_PRINT_MATRICES
        printf("\n=\n");
        vecadd_vec_print_simple(n, Y);
#endif

#ifdef VECADD_BENCHMARK_VERIFY_RESULT
    #ifdef VECADD_BENCHMARK_CUDA_NO_COPY
            MATMUL_CUDA_RT_CHECK(cudaMemcpy(C, pCDev, bytes, cudaMemcpyDeviceToHost));
    #endif

        vecadd_axpy_seq_basic(n, alpha, X, D);

    #ifdef VECADD_BENCHMARK_PRINT_MATRICES
        printf("\n=\n");
        vecadd_vec_print_simple(n, D);
    #endif

        // The threshold difference from where the value is considered to be a real error.
        TElem const errorThreshold = (TElem)(VECADD_EPSILON * ((TElem)n) * maxVal);
        bool const resultCorrect = vecadd_vec_cmp(n, Y, D, errorThreshold);
        if(!resultCorrect)
        {
            printf("%s iteration %"VECADD_PRINTF_SIZE_T" result incorrect!", algo->pszName, (size_t)i);
        }
        *pResultsCorrect = (*pResultsCorrect) && resultCorrect;
#endif

#ifdef VECADD_BENCHMARK_PRINT_MATRICES
        printf("\n");
#endif

        if(bRepeatTakeMinimum)
        {
            timeMeasuredSec = (timeElapsed<timeMeasuredSec) ? timeElapsed : timeMeasuredSec;
        }
        else
        {
            timeMeasuredSec += timeElapsed * (1.0/(double)repeatCount);
        }

    }

#ifdef VECADD_BENCHMARK_CUDA_NO_COPY
    cudaFree(pXDev);
    cudaFree(pYDev);
#endif

#ifndef VECADD_BENCHMARK_PRINT_GFLOPS
    // Print the time needed for the calculation.
    printf("\t%12.8lf", timeMeasuredSec);
#else
    // Print the GFLOPS.
    double const operationCount = 2.0*(double)n;
    double const flops = (timeMeasuredSec!=0) ? (operationCount/timeMeasuredSec) : 0.0;
    printf("\t%12.8lf", flops*1.0e-9);
#endif

    vecadd_arr_free((TElem * const)X);
    vecadd_arr_free(Y);
#ifdef VECADD_BENCHMARK_VERIFY_RESULT
    vecadd_arr_free(D);
#endif

    return timeMeasuredSec;
}

//-----------------------------------------------------------------------------
//! A struct containing an array of all vector sizes to test.
//-----------------------------------------------------------------------------
typedef struct AxpySizes
{
    TIdx sizeCount;
    TIdx * pSizes;
} AxpySizes;

//-----------------------------------------------------------------------------
//! Fills the vector sizes struct.
//! \param minN The start vector dimension.
//! \param stepN The step width for each iteration. If set to 0 the size is doubled on each iteration.
//! \param maxN The maximum vector dimension.
//-----------------------------------------------------------------------------
AxpySizes buildSizes(
    TIdx const minN,
    TIdx const maxN,
    TIdx const stepN)
{
    AxpySizes sizes;
    sizes.sizeCount = 0;
    sizes.pSizes = 0;

    TIdx n;
    for(n = minN; n <= maxN; n += (stepN == 0) ? n : stepN)
    {
        ++sizes.sizeCount;
    }

    sizes.pSizes = (TIdx *)malloc(sizes.sizeCount * sizeof(TIdx));

    TIdx idx = 0;
    for(n = minN; n <= maxN; n += (stepN == 0) ? n : stepN)
    {
        sizes.pSizes[idx] = n;
        ++idx;
    }

    return sizes;
}

//-----------------------------------------------------------------------------
//! Class template with static member templates because function templates do not allow partial specialization.
//! \param minN The start vector dimension.
//! \param stepN The step width for each iteration. If set to 0 the size is doubled on each iteration.
//! \param maxN The maximum vector dimension.
//! \return True, if all results are correct.
//-----------------------------------------------------------------------------
#ifdef VECADD_BENCHMARK_VERIFY_RESULT
    bool
#else
    void
#endif
measureRandomVecAdds(
    AxpyAlgo const * const pAlgos,
    TIdx const algoCount,
    AxpySizes const * const pSizes,
    TIdx const repeatCount)
{
#ifndef VECADD_BENCHMARK_PRINT_GFLOPS
    printf("\n#time in s");
#else
    printf("\n#GFLOPS");
#endif
    printf("\nn");
    // Table heading
    for(TIdx algoIdx = 0; algoIdx < algoCount; ++algoIdx)
    {
            printf(" \t%s", pAlgos[algoIdx].pszName);
    }


#ifdef VECADD_BENCHMARK_VERIFY_RESULT
    bool allResultsCorrect = true;
#endif
    if(pSizes)
    {
        for(TIdx sizeIdx = 0; sizeIdx < pSizes->sizeCount; ++sizeIdx)
        {
            TIdx const n = pSizes->pSizes[sizeIdx];

            // Print the operation
            printf("\n%"VECADD_PRINTF_SIZE_T, (size_t)n);

            for(TIdx algoIdx = 0; algoIdx < algoCount; ++algoIdx)
            {
#ifdef VECADD_BENCHMARK_VERIFY_RESULT
                bool resultsCorrectAlgo = true;
#endif
                // Execute the operation and measure the time taken.
                measureRandomVecAdd(
                    &pAlgos[algoIdx],
                    n,
                    repeatCount,
#ifdef VECADD_BENCHMARK_REPEAT_TAKE_MINIMUM
                    true
#else
                    false
#endif
#ifdef VECADD_BENCHMARK_VERIFY_RESULT
                    , &resultsCorrectAlgo
#endif
                    );

#ifdef VECADD_BENCHMARK_VERIFY_RESULT
                    allResultsCorrect &= resultsCorrectAlgo;
#endif
            }
        }
    }
    else
    {
        printf("Pointer to structure of test sizes 'pSizes' is not allowed to be nullptr!\n");
    }

#ifdef VECADD_BENCHMARK_VERIFY_RESULT
    return allResultsCorrect;
#endif
}

#define VECADD_STRINGIFY(s) VECADD_STRINGIFY_INTERNAL(s)
#define VECADD_STRINGIFY_INTERNAL(s) #s

//-----------------------------------------------------------------------------
//! Prints some startup informations.
//-----------------------------------------------------------------------------
void main_print_startup(
    TIdx minN,
    TIdx maxN,
    TIdx stepN,
    TIdx repeatCount)
{
    printf("# vecadd benchmark copyright (c) 2015, Benjamin Worpitz");
    printf(" | Config:");
#ifdef NDEBUG
    printf("Release");
#else
    printf("Debug");
#endif
    printf("; element type:");
#ifdef VECADD_ELEMENT_TYPE_DOUBLE
    printf("double");
#else
    printf("float");
#endif
    printf("; index type:%s", VECADD_STRINGIFY(VECADD_INDEX_TYPE));
    printf("; min n:%"VECADD_PRINTF_SIZE_T, (size_t)minN);
    printf("; max n:%"VECADD_PRINTF_SIZE_T, (size_t)maxN);
    printf("; step n:%"VECADD_PRINTF_SIZE_T, (size_t)stepN);
    printf("; repeat count:%"VECADD_PRINTF_SIZE_T, (size_t)repeatCount);
#ifdef VECADD_BENCHMARK_PRINT_GFLOPS
    printf("; VECADD_BENCHMARK_PRINT_GFLOPS=ON");
#else
    printf("; VECADD_BENCHMARK_PRINT_GFLOPS=OFF");
#endif
#ifdef VECADD_BENCHMARK_REPEAT_TAKE_MINIMUM
    printf("; VECADD_BENCHMARK_REPEAT_TAKE_MINIMUM=ON");
#else
    printf("; VECADD_BENCHMARK_REPEAT_TAKE_MINIMUM=OFF");
#endif
#ifdef VECADD_BENCHMARK_VERIFY_RESULT
    printf("; VECADD_BENCHMARK_VERIFY_RESULT");
#endif
#ifdef VECADD_BENCHMARK_PRINT_MATRICES
    printf("; VECADD_BENCHMARK_PRINT_MATRICES");
#endif
#ifdef VECADD_BENCHMARK_PRINT_ITERATIONS
    printf("; VECADD_BENCHMARK_PRINT_ITERATIONS");
#endif
}

//-----------------------------------------------------------------------------
//! Main method initiating the measurements of all algorithms selected in config.h.
//-----------------------------------------------------------------------------
int main(
    int argc,
    char ** argv
    )
{
    // Disable buffering of printf. Always print it immediately.
    setvbuf(stdout, 0, _IONBF, 0);

    // Set the initial seed to make the measurements repeatable.
    srand(42u);

    TIdx minN = 1;
    TIdx maxN = 1;
    TIdx stepN = 1;
    TIdx repeatCount = 1;

    // Read all arguments.
    if(argc != (4+1))
    {
        printf("\nExactly four arguments are required!");
        printf("\nOptions: min max step repeat!\n");
        return EXIT_FAILURE;
    }
    else
    {
        minN = atoi(argv[1]);
        maxN = atoi(argv[2]);
        stepN = atoi(argv[3]);
        repeatCount = atoi(argv[4]);
    }

    main_print_startup(minN, maxN, stepN, repeatCount);

    double const timeStart = getTimeSec();

    srand((unsigned int)time(0));

    static AxpyAlgo const algos[] = {
    #ifdef VECADD_BENCHMARK_SEQ_BASIC
        {vecadd_axpy_seq_basic, "axpy_seq_basic"},
    #endif
    #if _OPENMP >= 200203   // OpenMP 2.0
        #ifdef VECADD_BENCHMARK_PAR_OMP2_GUIDED
            {vecadd_axpy_par_omp2_guided_schedule, "axpy_par_omp2_guided_schedule"},
        #endif
    #endif
    #ifdef VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_OMP2_T_SEQ
        {vecadd_axpy_par_alpaka_cpu_b_omp2_t_seq, "axpy_par_alpaka_cpu_b_omp2_t_seq"},
    #endif
    #ifdef VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_SEQ_T_OMP2
        {vecadd_axpy_par_alpaka_cpu_b_seq_t_omp2, "axpy_par_alpaka_cpu_b_seq_t_omp2"},
    #endif
    #ifdef VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_BT_OMP4
        {vecadd_axpy_par_alpaka_cpu_bt_omp4, "axpy_par_alpaka_cpu_bt_omp4"},
    #endif
    #ifdef VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_SEQ_T_THREADS
        {vecadd_axpy_par_alpaka_cpu_b_seq_t_threads, "axpy_par_alpaka_cpu_b_seq_t_threads"},
    #endif
    #ifdef VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_SEQ_T_FIBERS
        {vecadd_axpy_seq_alpaka_cpu_b_seq_t_fibers, "axpy_seq_alpaka_cpu_b_seq_t_fibers"},
    #endif
    #ifdef VECADD_BENCHMARK_PAR_ALPAKA_ACC_CPU_B_SEQ_T_SEQ
        {vecadd_axpy_seq_alpaka_cpu_b_seq_t_seq, "axpy_seq_alpaka_cpu_b_seq_t_seq"},
    #endif
    #ifdef VECADD_BENCHMARK_PAR_ALPAKA_ACC_GPU_CUDA_MEMCPY
        {vecadd_axpy_par_alpaka_gpu_cuda_memcpy, "axpy_par_alpaka_gpu_cuda_memcpy"},
    #endif

    #ifdef VECADD_BENCHMARK_PAR_ALPAKA_ACC_GPU_CUDA
        {vecadd_axpy_par_alpaka_gpu_cuda, "axpy_par_alpaka_gpu_cuda"},
    #endif
    #ifdef VECADD_BENCHMARK_PAR_CUDA
        {vecadd_axpy_par_cuda, "axpy_par_cuda"},
    #endif
    };

    AxpySizes const sizes = buildSizes(
        minN,
        maxN,
        stepN);

#ifdef VECADD_BENCHMARK_VERIFY_RESULT
    bool const allResultsCorrect =
#endif
    measureRandomVecAdds(
        algos,
        sizeof(algos)/sizeof(algos[0]),
        &sizes,
        repeatCount);

    free(sizes.pSizes);

    double const timeEnd = getTimeSec();
    double const timeElapsed = timeEnd - timeStart;
    printf("\nTotal runtime: %12.6lf s\n", timeElapsed);

#ifdef VECADD_BENCHMARK_VERIFY_RESULT
    return !allResultsCorrect;
#else
    return EXIT_SUCCESS;
#endif
}
