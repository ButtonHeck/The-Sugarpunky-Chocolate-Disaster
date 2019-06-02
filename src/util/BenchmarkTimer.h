#ifndef BENCHMARKTIMER_H
#define BENCHMARKTIMER_H
#include <fstream>
#include <map>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include "../../src/util/DirectoriesSettings.h"
#include "../../src/util/Logger.h"

/* if in debug build - BENCHMARK macro creates an instance of BenchmarkTimer
 * which takes timestamps in both ctor and dtor
 * and automatically update timing results in one of the maps
 * if in release build - just bypass and do nothing
 */
#ifdef _DEBUG
#define BENCHMARK(benchmarkName, perFrame) BenchmarkTimer b(benchmarkName, perFrame);
#else
#define BENCHMARK(dont, care) //noop
#endif
#define FORCE_BENCHMARK(benchmarkName, perFrame) BenchmarkTimer b(benchmarkName, perFrame);

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
  static std::map<std::string, float> applicationBenchmarks;
  static std::map<std::string, unsigned long> frameBenchmarksTimings;
  static std::map<std::string, int> frameBenchmarksInvocations;

  std::string benchmarkTitle;
  bool isPerFrame;
  decltype(chronoClock::now()) startTimestamp = chronoClock::now();
  decltype(startTimestamp) endTimestamp = startTimestamp;
};

#endif // BENCHMARKTIMER_H
