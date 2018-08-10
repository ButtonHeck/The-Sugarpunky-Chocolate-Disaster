#ifndef BENCHMARKTIMER_H
#define BENCHMARKTIMER_H
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <algorithm>
#include <iomanip>
#include "game/Settings.h"

class BenchmarkTimer
{
public:
  BenchmarkTimer(const std::string& text, bool perFrameBenchmark, bool isPassThrough = false);
  ~BenchmarkTimer();
  static void finish(unsigned int updateCount);
  static void printBenchmarksPerApp(unsigned int updateCount);
  static void printBenchmarksPerFrame(unsigned int updateCount, unsigned int ups);
  static void clearBenchmarksPerFrameValues();
private:
  static std::ofstream perFrameLog, perAppLog;
  static bool outputCreated;
  static std::map<std::string, unsigned long> benchmarks;
  static std::map<std::string, float> appBenchmarks;
  static constexpr int FORMAT_VALUE_ASCII = 45, BENCH_NAME_MAX_LENGTH = 40;
  std::string benchmark;
  bool perFrame;
  /*
    passThrough - We check this one in case we only need one invocation for benchmarking some part of program
    For example - for every benchmarks map update we should gather info about water animation frame only once till next update
  */
  bool passThrough;
  decltype(std::chrono::high_resolution_clock::now()) startTime = std::chrono::high_resolution_clock::now();
  decltype(startTime) endTime = startTime;
};

#endif // BENCHMARKTIMER_H
