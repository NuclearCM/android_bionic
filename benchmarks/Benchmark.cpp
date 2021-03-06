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

#include <inttypes.h>
#include <regex.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <string>
#include <vector>

#include <base/stringprintf.h>

#include <benchmark/Benchmark.h>

#include "utils.h"

namespace testing {

static uint64_t NanoTime() {
  struct timespec t;
  t.tv_sec = t.tv_nsec = 0;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return static_cast<uint64_t>(t.tv_sec) * 1000000000LL + t.tv_nsec;
}

bool Benchmark::header_printed_;

std::vector<Benchmark*>& Benchmark::List() {
  static std::vector<Benchmark*> list;
  return list;
}

int Benchmark::MaxNameColumnWidth() {
  size_t max = 20;
  for (auto& benchmark : List()) {
    max = std::max(max, benchmark->NameColumnWidth());
  }
  return static_cast<int>(max);
}

size_t Benchmark::RunAll(std::vector<regex_t*>& regs) {
  size_t benchmarks_run = 0;
  header_printed_ = false;
  for (auto& benchmark : List()) {
    benchmarks_run += benchmark->RunAllArgs(regs);
  }
  return benchmarks_run;
}

void Benchmark::PrintHeader() {
  if (!header_printed_) {
    printf("%-*s %10s %10s\n", MaxNameColumnWidth(), "", "iterations", "ns/op");
    header_printed_ = true;
  }
}

template <typename T>
bool BenchmarkT<T>::ShouldRun(std::vector<regex_t*>& regs, T arg) {
  if (regs.empty()) {
    return true;
  }

  for (const auto& re : regs) {
    if (regexec(re, GetNameStr(arg).c_str(), 0, NULL, 0) != REG_NOMATCH) {
      return true;
    }
  }
  return false;
}

void Benchmark::StopBenchmarkTimingWithStd() {
  if (start_time_ns_ != 0) {
	uint64_t interval = NanoTime() - start_time_ns_;
	each_time_ns_[count] = interval;
	total_time_ns_ += interval;
	count++;
  }
  start_time_ns_ = 0;
}

void Benchmark::StopBenchmarkTimingWithStdArg(int iters) {
  if (start_time_ns_ != 0) {
    double interval = NanoTime() - start_time_ns_;
    each_time_ns_[count] = interval/(double)iters;
    total_time_ns_ += interval/(double)iters;
    count ++;
  }
  start_time_ns_ = 0;
}


void Benchmark::StopBenchmarkTiming() {
  if (start_time_ns_ != 0) {
    total_time_ns_ += NanoTime() - start_time_ns_;
  }
  start_time_ns_ = 0;
}

void Benchmark::StartBenchmarkTiming() {
  if (start_time_ns_ == 0) {
    start_time_ns_ = NanoTime();
  }
}

/* Calculate Standard Diviation */
void Benchmark::Standard() {
    int i;
    double average = 0, standard = 0, sigma = 0, value = 0;
    average = total_time_ns_/Iterations;

    for(i = 0 ; i < Iterations ; i++) {
        standard = each_time_ns_[i] - average;
        standard *= standard;
        sigma += standard;
    }
    sigma /= Iterations;
    value = sqrt(sigma);
    Confidence(average, value);
}

/* Take the value between 95% confidence level. */
void Benchmark::Confidence(double average, double value) {
    int i;
    int64_t max = average + 2*value;
    int64_t min = average - 2*value;

    total_time_ns_ = 0;
	con_iterations = 0;
    for(i = 0 ; i<Iterations ; i++) {
        if(each_time_ns_[i]<max && each_time_ns_[i]>min) {
            total_time_ns_ += (uint64_t)each_time_ns_[i];
            con_iterations++;
        }
    }
}

std::string BenchmarkWithoutArg::GetNameStr(void*) {
  return Name();
}

template <>
std::string BenchmarkWithArg<int>::GetNameStr(int arg) {
  return Name() + "/" + PrettyInt(arg, 2);
}

template <>
std::string BenchmarkWithArg<double>::GetNameStr(double arg) {
  return Name() + "/" + android::base::StringPrintf("%0.6f", arg);
}

template<typename T>
void BenchmarkT<T>::RunWithArg(T arg) {
  int new_iterations = 1;
  int iterations;
  while (new_iterations < 1e8) {
    bytes_processed_ = 0;
    total_time_ns_ = 0;
    start_time_ns_ = 0;

    iterations = new_iterations;

	count = 0;
	/* new array to store each time */
	each_time_ns_ = new double[iterations];
    RunIterations(iterations, arg);
	Iterations = iterations;

    if (total_time_ns_ >= 1e9) {
      break;
    }

    if (total_time_ns_/iterations == 0) {
      new_iterations = 1e9;
    } else {
      new_iterations = 1e9/ (total_time_ns_/iterations);
    }
    new_iterations = std::max(iterations + 1,
                          std::min(new_iterations + new_iterations/2, 100*iterations));

    new_iterations = Round(new_iterations);

	total_time_ns_ = 0;
  }
	
	/* calculate Standard deviation */
	Standard();

	iterations = con_iterations;
	delete each_time_ns_;

  printf("%-*s %10s %10" PRId64, MaxNameColumnWidth(), GetNameStr(arg).c_str(),
         PrettyInt(iterations, 10).c_str(), total_time_ns_/iterations);

  if (total_time_ns_ > 0 && bytes_processed_ > 0) {
    double gib_processed = static_cast<double>(bytes_processed_)/1e9;
    double seconds = static_cast<double>(total_time_ns_)/1e9;
    printf(" %8.3f GiB/s", gib_processed/seconds);
  }
  printf("\n");
  fflush(stdout);
}

template class BenchmarkT<int>;
template class BenchmarkT<double>;
template class BenchmarkT<void*>;

template class BenchmarkWithArg<int>;
template class BenchmarkWithArg<double>;

}  // namespace testing
