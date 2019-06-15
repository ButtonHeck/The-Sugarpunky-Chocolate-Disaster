/*
 * Copyright 2019 Ilya Malgin
 * BenchmarkTimer.h
 * This file is part of The Sugarpunky Chocolate Disaster project
 *
 * The Sugarpunky Chocolate Disaster project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The Sugarpunky Chocolate Disaster project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * See <http://www.gnu.org/licenses/>
 *
 * Purpose: contains declaration for BenchmarkTimer class
 * @version 0.1.0
 */

#pragma once

#include <fstream>
#include <map>
#include <chrono>

/** if in debug build - BENCHMARK macro creates an instance of BenchmarkTimer
 * which takes timestamps in both ctor and dtor
 * and automatically update timing results in one of the maps
 * if in release build - just bypass and do nothing
 */
#if 0
	#ifdef _DEBUG
	#define BENCHMARK(benchmarkName, perFrame) BenchmarkTimer b(benchmarkName, perFrame);
	#else
	#define BENCHMARK(dont, care) //noop
	#endif
#else 
	#define BENCHMARK(dont, care) //noop
#endif
#define FORCE_BENCHMARK(benchmarkName, perFrame) BenchmarkTimer b(benchmarkName, perFrame);

/**
* @brief class for creating CPU time benchmarks through BENCHMARK macro
* Responsible for creating benchmarks on demand in dedicated parts of code. Used only in debug mode
* @todo remove from release version of the project
*/
class BenchmarkTimer
{
public:
  BenchmarkTimer() = default;
  BenchmarkTimer(const std::string& title, bool isPerFrame);
  void operator()(const std::string& title, bool isPerFrame);
  virtual ~BenchmarkTimer();
  static void finish(unsigned int updateCount);
  static void printApplicationBenchmarks(unsigned int updateCount);
  static void printFrameBenchmarks(unsigned int updateCount, unsigned int ups);
  static void resetFrameBenchmarks();

private:
  using chronoClock = std::chrono::high_resolution_clock;
  static constexpr int BENCH_TITLE_MAX_LENGTH = 40;

  static std::ofstream perFrameLogStream, perAppLogStream;
  static bool outputCreated;
  static std::map<std::string, float> applicationBenchmarksTimings;
  static std::map<std::string, unsigned long> frameBenchmarksTimings;
  static std::map<std::string, int> frameBenchmarksInvocations;

  std::string benchmarkTitle;
  bool isPerFrame;
  decltype(chronoClock::now()) startTimestamp = chronoClock::now();
  decltype(startTimestamp) endTimestamp = startTimestamp;
};
