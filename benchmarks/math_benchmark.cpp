/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fenv.h>
#include <math.h>

#include <benchmark/Benchmark.h>

#define AT_COMMON_VALS \
    Arg(1234.0)->Arg(nan(""))->Arg(HUGE_VAL)->Arg(0.0)

// Avoid optimization.
volatile double d;
volatile double v;

BENCHMARK_NO_ARG(BM_math_sqrt);
void BM_math_sqrt::Run(int iters) {

  d = 0.0;
  v = 2.0;
  for (int i = 0; i < iters; ++i) {
	StartBenchmarkTiming();

    d += sqrt(v);

  	StopBenchmarkTimingWithStd();
  }

}

BENCHMARK_NO_ARG(BM_math_log10);
void BM_math_log10::Run(int iters) {

  d = 0.0;
  v = 1234.0;
  for (int i = 0; i < iters; ++i) {
  	StartBenchmarkTiming();

    d += log10(v);

  	StopBenchmarkTimingWithStd();
  }

}

BENCHMARK_NO_ARG(BM_math_logb);
void BM_math_logb::Run(int iters) {

  d = 0.0;
  v = 1234.0;
  for (int i = 0; i < iters; ++i) {
  	StartBenchmarkTiming();

    d += logb(v);

  	StopBenchmarkTimingWithStd();
  }

}

BENCHMARK_WITH_ARG(BM_math_isfinite_macro, double)->AT_COMMON_VALS;
void BM_math_isfinite_macro::Run(int iters, double value) {

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
  	StartBenchmarkTiming();

    d += isfinite(v);

  	StopBenchmarkTimingWithStd();
  }

}

#if defined(__BIONIC__)
#define test_isfinite __isfinite
#else
#define test_isfinite __finite
#endif
BENCHMARK_WITH_ARG(BM_math_isfinite, double)->AT_COMMON_VALS;
void BM_math_isfinite::Run(int iters, double value) {

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
  	StartBenchmarkTiming();

    d += test_isfinite(v);

  	StopBenchmarkTimingWithStd();
  }

}

BENCHMARK_WITH_ARG(BM_math_isinf_macro, double)->AT_COMMON_VALS;
void BM_math_isinf_macro::Run(int iters, double value) {

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
  	StartBenchmarkTiming();

    d += isinf(v);

  	StopBenchmarkTimingWithStd();
  }

}

BENCHMARK_WITH_ARG(BM_math_isinf, double)->AT_COMMON_VALS;
void BM_math_isinf::Run(int iters, double value) {

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
  	StartBenchmarkTiming();

    d += (isinf)(v);

  	StopBenchmarkTimingWithStd();
  }

}

BENCHMARK_WITH_ARG(BM_math_isnan_macro, double)->AT_COMMON_VALS;
void BM_math_isnan_macro::Run(int iters, double value) {

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
  	StartBenchmarkTiming();

    d += isnan(v);

  	StopBenchmarkTimingWithStd();
  }

}

BENCHMARK_WITH_ARG(BM_math_isnan, double)->AT_COMMON_VALS;
void BM_math_isnan::Run(int iters, double value) {

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
  	StartBenchmarkTiming();

    d += (isnan)(v);

  	StopBenchmarkTimingWithStd();
  }

}

BENCHMARK_WITH_ARG(BM_math_isnormal_macro, double)->AT_COMMON_VALS;
void BM_math_isnormal_macro::Run(int iters, double value) {

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
  	StartBenchmarkTiming();

    d += isnormal(v);

  	StopBenchmarkTimingWithStd();
  }

}

#if defined(__BIONIC__)
BENCHMARK_WITH_ARG(BM_math_isnormal, double)->AT_COMMON_VALS;
void BM_math_isnormal::Run(int iters, double value) {

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
  	StartBenchmarkTiming();

    d += (__isnormal)(v);

  	StopBenchmarkTimingWithStd();
  }

}
#endif

BENCHMARK_NO_ARG(BM_math_sin_fast);
void BM_math_sin_fast::Run(int iters) {

  d = 1.0;
  for (int i = 0; i < iters; ++i) {
  	StartBenchmarkTiming();

    d += sin(d);

  	StopBenchmarkTimingWithStd();
  }

}

BENCHMARK_NO_ARG(BM_math_sin_feupdateenv);
void BM_math_sin_feupdateenv::Run(int iters) {

  d = 1.0;
  for (int i = 0; i < iters; ++i) {
  	StartBenchmarkTiming();

    fenv_t __libc_save_rm;
    feholdexcept(&__libc_save_rm);
    fesetround(FE_TONEAREST);
    d += sin(d);
    feupdateenv(&__libc_save_rm);

  	StopBenchmarkTimingWithStd();
  }

}

BENCHMARK_NO_ARG(BM_math_sin_fesetenv);
void BM_math_sin_fesetenv::Run(int iters) {

  d = 1.0;
  for (int i = 0; i < iters; ++i) {
  	StartBenchmarkTiming();

    fenv_t __libc_save_rm;
    feholdexcept(&__libc_save_rm);
    fesetround(FE_TONEAREST);
    d += sin(d);
    fesetenv(&__libc_save_rm);

  	StopBenchmarkTimingWithStd();
  }

}

BENCHMARK_WITH_ARG(BM_math_fpclassify, double)->AT_COMMON_VALS;
void BM_math_fpclassify::Run(int iters, double value) {

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
  	StartBenchmarkTiming();

    d += fpclassify(v);

  	StopBenchmarkTimingWithStd();
  }

}

BENCHMARK_WITH_ARG(BM_math_signbit_macro, double)->AT_COMMON_VALS;
void BM_math_signbit_macro::Run(int iters, double value) {

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
  	StartBenchmarkTiming();

    d += signbit(v);

  	StopBenchmarkTimingWithStd();
  }

}

BENCHMARK_WITH_ARG(BM_math_signbit, double)->AT_COMMON_VALS;
void BM_math_signbit::Run(int iters, double value) {

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
  	StartBenchmarkTiming();

    d += (__signbit)(v);

  	StopBenchmarkTimingWithStd();
  }

}

BENCHMARK_WITH_ARG(BM_math_fabs_macro, double)->AT_COMMON_VALS;
void BM_math_fabs_macro::Run(int iters, double value) {

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
 	StartBenchmarkTiming();

	d += fabs(v);

  	StopBenchmarkTimingWithStd();
  }

}

BENCHMARK_WITH_ARG(BM_math_fabs, double)->AT_COMMON_VALS;
void BM_math_fabs::Run(int iters, double value) {

  d = 0.0;
  v = value;
  for (int i = 0; i < iters; ++i) {
  	StartBenchmarkTiming();

    d += (fabs)(v);

	StopBenchmarkTimingWithStd();
  }

}
