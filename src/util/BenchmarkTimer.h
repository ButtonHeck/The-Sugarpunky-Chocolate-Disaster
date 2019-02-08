#ifndef BENCHMARKTIMER_H
#define BENCHMARKTIMER_H
#include <fstream>
#include <map>
#include <string.h>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include "util/DirectoriesSettings.h"
#include "util/Logger.h"

//benchmarking
#ifdef _DEBUG
#define BENCHMARK(benchmarkName, perFrame) BenchmarkTimer b(benchmarkName, perFrame);
#else
#define BENCHMARK(dont, care) //noop
#endif

class BenchmarkTimer
{
public:
  BenchmarkTimer() = default;
  BenchmarkTimer(const std::string& text, bool perFrameBenchmark);
  void operator()(const std::string& text, bool perFrameBenchmark);
  virtual ~BenchmarkTimer();
  static void finish(unsigned int updateCount);
  static void printBenchmarksPerApp(unsigned int updateCount);
  static void printBenchmarksPerFrame(unsigned int updateCount, unsigned int ups);
  static void clearBenchmarksPerFrameValues();

private:
  using chronoClock = std::chrono::high_resolution_clock;

  static std::ofstream perFrameLog, perAppLog;
  static bool outputCreated;
  static std::map<std::string, unsigned long> benchmarksTimers;
  static std::map<std::string, int> benchmarksInvocations;
  static std::map<std::string, float> appBenchmarks;
  static constexpr int FORMAT_VALUE_ASCII = 45, BENCH_NAME_MAX_LENGTH = 40;
  std::string benchmark;
  bool perFrame;
  decltype(chronoClock::now()) startTime = chronoClock::now();
  decltype(startTime) endTime = startTime;
};

#endif // BENCHMARKTIMER_H
