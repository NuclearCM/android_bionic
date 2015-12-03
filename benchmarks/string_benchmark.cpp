/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include <stdint.h>
#include <string.h>

#include <benchmark/Benchmark.h>

#define KB 1024
#define MB 1024*KB

#define AT_COMMON_SIZES \
    Arg(8)->Arg(64)->Arg(512)->Arg(1*KB)->Arg(8*KB)->Arg(16*KB)->Arg(32*KB)->Arg(64*KB)

// TODO: test unaligned operation too? (currently everything will be 8-byte aligned by malloc.)

BENCHMARK_WITH_ARG(BM_string_memcmp, int)->AT_COMMON_SIZES;
void BM_string_memcmp::Run(int iters, int nbytes) {
  StopBenchmarkTiming();
  char* src = new char[nbytes]; char* dst = new char[nbytes];
  memset(src, 'x', nbytes);
  memset(dst, 'x', nbytes);

  volatile int c __attribute__((unused)) = 0;
  for (int i = 0; i < iters; ++i) {
  	StartBenchmarkTiming();

    c += memcmp(dst, src, nbytes);

  	StopBenchmarkTimingWithStd();
  }

  SetBenchmarkBytesProcessed(uint64_t(iters) * uint64_t(nbytes));
  delete[] src;
  delete[] dst;
}

BENCHMARK_WITH_ARG(BM_string_memcpy, int)->AT_COMMON_SIZES;
void BM_string_memcpy::Run(int iters, int nbytes) {
  StopBenchmarkTiming();
  char* src = new char[nbytes]; char* dst = new char[nbytes];
  memset(src, 'x', nbytes);

  for (int i = 0; i < iters; ++i) {
  	StartBenchmarkTiming();

    memcpy(dst, src, nbytes);

  	StopBenchmarkTimingWithStd();
  }

  SetBenchmarkBytesProcessed(uint64_t(iters) * uint64_t(nbytes));
  delete[] src;
  delete[] dst;
}

BENCHMARK_WITH_ARG(BM_string_memmove, int)->AT_COMMON_SIZES;
void BM_string_memmove::Run(int iters, int nbytes) {
  StopBenchmarkTiming();
  char* buf = new char[nbytes + 64];
  memset(buf, 'x', nbytes + 64);

  for (int i = 0; i < iters; ++i) {
  	StartBenchmarkTiming();

    memmove(buf, buf + 1, nbytes); // Worst-case overlap.

  	StopBenchmarkTimingWithStd();
  }

  SetBenchmarkBytesProcessed(uint64_t(iters) * uint64_t(nbytes));
  delete[] buf;
}

BENCHMARK_WITH_ARG(BM_string_memset, int)->AT_COMMON_SIZES;
void BM_string_memset::Run(int iters, int nbytes) {
  StopBenchmarkTiming();
  char* dst = new char[nbytes];

  for (int i = 0; i < iters; ++i) {
  	StartBenchmarkTiming();

    memset(dst, 0, nbytes);

  	StopBenchmarkTimingWithStd();
  }

  SetBenchmarkBytesProcessed(uint64_t(iters) * uint64_t(nbytes));
  delete[] dst;
}

BENCHMARK_WITH_ARG(BM_string_strlen, int)->AT_COMMON_SIZES;
void BM_string_strlen::Run(int iters, int nbytes) {
  StopBenchmarkTiming();
  char* s = new char[nbytes];
  memset(s, 'x', nbytes);
  s[nbytes - 1] = 0;

  volatile int c __attribute__((unused)) = 0;
  for (int i = 0; i < iters; ++i) {
  	StartBenchmarkTiming();

    c += strlen(s);

  	StopBenchmarkTimingWithStd();
  }

  SetBenchmarkBytesProcessed(uint64_t(iters) * uint64_t(nbytes));
  delete[] s;
}


BENCHMARK_WITH_ARG(BM_string_strrchr)->AT_COMMON_SIZES;
static void BM_string_strrchr(int iters, int nbytes) {
  StopBenchmarkTiming();
  char* s = new char[nbytes];
  memset(s, 'x', nbytes);
  s[nbytes - 1] = 0;
  StartBenchmarkTiming();

  volatile char* c = new char[1];
  volatile int d __attribute__((unused)) = 0;
  for (int i = 0; i < iters; ++i) {
    c = strrchr(s, 'y');
    if (c != NULL)
        d += (int)c[0];
  }

  StopBenchmarkTiming();
  SetBenchmarkBytesProcessed(int64_t(iters) * int64_t(nbytes));
  delete[] s;
}

BENCHMARK_WITH_ARG(BM_string_strcmp)->AT_COMMON_SIZES;
static void BM_string_strcmp(int iters, int nbytes) {
  StopBenchmarkTiming();
  char* src = new char[nbytes]; char* dst = new char[nbytes];
  memset(src, 'x', nbytes);
  memset(dst, 'x', nbytes);
  src[nbytes - 1] = 0;
  dst[nbytes - 1] = 0;
  StartBenchmarkTiming();

  volatile int c __attribute__((unused)) = 0;
  for (int i = 0; i < iters; ++i) {
    c += strcmp(dst, src);
  }

  StopBenchmarkTiming();
  SetBenchmarkBytesProcessed(int64_t(iters) * int64_t(nbytes));
  delete[] src;
  delete[] dst;
}

BENCHMARK_WITH_ARG(BM_string_strncmp)->AT_COMMON_SIZES;
static void BM_string_strncmp(int iters, int nbytes) {
  StopBenchmarkTiming();
  char* src = new char[nbytes]; char* dst = new char[nbytes];
  memset(src, 'x', nbytes);
  memset(dst, 'x', nbytes);
  src[nbytes - 1] = 0;
  dst[nbytes - 1] = 0;
  StartBenchmarkTiming();

  volatile int c __attribute__((unused)) = 0;
  for (int i = 0; i < iters; ++i) {
    c += strncmp(dst, src, nbytes - 1);
  }

  StopBenchmarkTiming();
  SetBenchmarkBytesProcessed(int64_t(iters) * int64_t(nbytes));
  delete[] src;
  delete[] dst;
}

BENCHMARK_WITH_ARG(BM_string_memchr)->AT_COMMON_SIZES;
static void BM_string_memchr(int iters, int nbytes) {
  StopBenchmarkTiming();
  char* s = new char[nbytes];
  memset(s, 'x', nbytes);
  s[nbytes - 1] = 0;
  StartBenchmarkTiming();

  volatile void* c __attribute__((unused)) = NULL;
  volatile int d __attribute__((unused)) = 0;
  for (int i = 0; i < iters; ++i) {
    c = memchr(s, 'y', nbytes - 1);
    if (c != NULL)
        d += (int)c;
  }

  StopBenchmarkTiming();
  SetBenchmarkBytesProcessed(int64_t(iters) * int64_t(nbytes));
  delete[] s;
}

BENCHMARK_WITH_ARG(BM_string_memrchr)->AT_COMMON_SIZES;
static void BM_string_memrchr(int iters, int nbytes) {
  StopBenchmarkTiming();
  char* s = new char[nbytes];
  memset(s, 'x', nbytes);
  s[nbytes - 1] = 0;
  StartBenchmarkTiming();

  volatile void* c __attribute__((unused)) = NULL;
  volatile int d __attribute__((unused)) = 0;
  for (int i = 0; i < iters; ++i) {
    c = memrchr(s, 'y', nbytes - 1);
    if (c != NULL)
        d += (int)c;
  }

  StopBenchmarkTiming();
  SetBenchmarkBytesProcessed(int64_t(iters) * int64_t(nbytes));
  delete[] s;
}

BENCHMARK_WITH_ARG(BM_string_strchr)->AT_COMMON_SIZES;
static void BM_string_strchr(int iters, int nbytes) {
  StopBenchmarkTiming();
  char* s = new char[nbytes];
  memset(s, 'x', nbytes);
  s[nbytes - 1] = 0;
  StartBenchmarkTiming();

  volatile char* c = new char[1];
  volatile int d __attribute__((unused)) = 0;
  for (int i = 0; i < iters; ++i) {
    c = strchr(s, 'y');
    if (c != NULL)
        d += (int)c[0];
  }

  StopBenchmarkTiming();
  SetBenchmarkBytesProcessed(int64_t(iters) * int64_t(nbytes));
  delete[] s;
}
