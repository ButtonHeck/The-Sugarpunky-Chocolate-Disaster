/*
 * Copyright 2019 Ilya Malgin
 * BenchmarkTimer.cpp
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
 * Purpose: contains definition for BenchmarkTimer class
 * @version 0.1.0
 */

#include "BenchmarkTimer"
#include "Logger"
#include "DirectoriesSettings"

#include <iomanip>
#include <algorithm>
#include <iterator>

//static variables definition
std::ofstream BenchmarkTimer::perFrameLogStream, BenchmarkTimer::perAppLogStream;
bool BenchmarkTimer::outputCreated = false;
std::map<std::string, unsigned long> BenchmarkTimer::frameBenchmarksTimings;
std::map<std::string, int> BenchmarkTimer::frameBenchmarksInvocations;
std::map<std::string, float> BenchmarkTimer::applicationBenchmarksTimings;

/**
* @brief constructor of a benchmark. Responsible for register benchmark in local storage and for creating log output files
* @param title current benchmark title. No special format requirements, but better to print class and function names
* @param isPerFrame flag indicating whether this benchmark should be treated as repetitive and included in perFrame log
*/
BenchmarkTimer::BenchmarkTimer(const std::string &title, bool isPerFrame)
  :
    benchmarkTitle(title),
    isPerFrame(isPerFrame)
{
  if (!outputCreated)
    {
	  //every time perFrameLog is created it is ok to override previous one (this log is more "tactical")
      perFrameLogStream.open(RES_DIR + "perFrameLog.txt", std::ios_base::ate);
      if (!perFrameLogStream)
        Logger::log("perFrameLog file not found!\n");
      perFrameLogStream << "SOME INFORMATION MIGHT BE IRRELEVANT OR EVEN INCORRECT, GREAT CARE SHOULD BE TAKEN\n";
      perFrameLogStream << "SI == Single Invocation (per second), ST == separate thread, m.p.u/a/i. == mean per update/app/invocation\n";

	  //for application log it might be useful to append to see the differences among older benchmarking data (strategy-like approach)
      perAppLogStream.open(RES_DIR + "perAppLog.txt", std::ios_base::app);
      if (!perAppLogStream)
        Logger::log("perAppLog file not found!\n");
      perAppLogStream << "SOME INFORMATION MIGHT BE IRRELEVANT OR EVEN INCORRECT, GREAT CARE SHOULD BE TAKEN\n";
      perAppLogStream << "SI == Single Invocation (per second), ST == separate thread, m.p.u/a/i. == mean per update/app/invocation\n";
      outputCreated = true;
    }

  //register a benchmark if one is being called for the first time
  if (applicationBenchmarksTimings.find(benchmarkTitle) == applicationBenchmarksTimings.end())
    {
      applicationBenchmarksTimings[benchmarkTitle] = 0;
      if (isPerFrame)
        {
          frameBenchmarksTimings[benchmarkTitle] = 0;
          frameBenchmarksInvocations[benchmarkTitle] = 0;
        }
    }
}

/**
* @brief operator overloading is used when a title/perFrame data depends on a comprehensive function context
* @param title current benchmark title. No special format requirements, but better to print class and function names
* @param isPerFrame flag indicating whether this benchmark should be treated as repetitive and included in perFrame log
*/
void BenchmarkTimer::operator()(const std::string &title, bool isPerFrame)
{
  benchmarkTitle = title;
  this->isPerFrame = isPerFrame;
}

/**
* @brief create a timestamp when benchmark is done and update timing values of this benchmark
*/
BenchmarkTimer::~BenchmarkTimer()
{
  endTimestamp = chronoClock::now();
  unsigned long frameTimingResultMicros = std::chrono::duration_cast<std::chrono::microseconds>(endTimestamp - startTimestamp).count();
  float applicationTimingResultMillis = frameTimingResultMicros / 1000.0f;
  if (isPerFrame)
    {
      frameBenchmarksTimings[benchmarkTitle] += frameTimingResultMicros;
      frameBenchmarksInvocations[benchmarkTitle]++;
    }
  applicationBenchmarksTimings[benchmarkTitle] += applicationTimingResultMillis;
}

/**
* @brief print all accumulated app benchmarks to output file and closes log files
* @param updateCount number of timer updates(seconds) ticked since the app beginning.
*/
void BenchmarkTimer::finish(unsigned int updateCount)
{
  perFrameLogStream.flush();
  perFrameLogStream.close();
  printApplicationBenchmarks(updateCount);
  perAppLogStream << '\n' << std::flush;
  perAppLogStream.close();
}

/**
* @brief prepare and format app log file for all the benchmarks
* @param updateCount number of timer updates(seconds) ticked since the app beginning. Used for time approximation of each benchmark call
*/
void BenchmarkTimer::printApplicationBenchmarks(unsigned int updateCount)
{
  //prepare char buffer of the current timestamp and format it
  time_t nowTime = time(0);
  struct tm  tstruct;
  char       dateCharBuffer[80];
  tstruct = *localtime(&nowTime);
  strftime(dateCharBuffer, sizeof(dateCharBuffer), "%Y.%m.%d %X", &tstruct);
  perAppLogStream << "Benchmark: " << dateCharBuffer << ", summary ticks: " << updateCount << '\n';

  //create copy of application benchmark map, but switch its key<->value in order to sort them by their values, not the titles
  std::multimap<float, std::string> applicationBenchmarksSortedByValue;
  std::transform(applicationBenchmarksTimings.begin(),
                 applicationBenchmarksTimings.end(),
                 std::inserter(applicationBenchmarksSortedByValue, applicationBenchmarksSortedByValue.begin()),
                 [](std::pair<std::string, float> p) noexcept ->std::pair<float, std::string>
  {
    return std::pair<float, std::string>(p.second, p.first);
  });

  //print in descending value order without external sorting (more "timing-heavy" benchmarks printed first)
  for (auto benchmark = applicationBenchmarksSortedByValue.end(); benchmark != applicationBenchmarksSortedByValue.begin();)
    {
      --benchmark;
      std::string benchmarkString(BENCH_TITLE_MAX_LENGTH, '-');
      std::string benchmarkName(benchmark->second.substr(0, BENCH_TITLE_MAX_LENGTH));
      benchmarkString.replace(0, benchmarkName.size(), benchmarkName);
      perAppLogStream << benchmarkString << ": " << std::setw(10) << std::setprecision(6) << benchmark->first << "ms";

      /* check whether a benchmark is a "per frame" and if it is - print mean value
       * (how much in average this benchmark took time per one frame through application's lifetime)
       */
      if (frameBenchmarksTimings.find(benchmark->second) != frameBenchmarksTimings.end())
        perAppLogStream << ", " << std::setw(10) << std::setprecision(5) << (benchmark->first / updateCount * 1000) << " us(m.p.a.)";

      perAppLogStream << '\n';
    }
}

/**
* @brief prepare and format frame log file, then fill with all perFrame benchmarks timings
* @param updateCount number of timer updates(seconds) ticked since the app beginning.
* @param ups current UPS(cpu updates per second) value. Used to represent UPS for a particular timer update
*/
void BenchmarkTimer::printFrameBenchmarks(unsigned int updateCount, unsigned int ups)
{
  perFrameLogStream << "--Update: "
              << updateCount
              << ", UPS: "
              << ups
              << ", mean CPU frametime: "
              << (1000.0f / ups)
              << "ms ("
              << (1000000.0f / ups)
              << "us)\n";
  for (auto benchmark = frameBenchmarksTimings.begin(); benchmark != frameBenchmarksTimings.end(); ++benchmark)
    {
      std::string benchmarkString(BENCH_TITLE_MAX_LENGTH, '-');
      std::string benchmarkName(benchmark->first.substr(0, BENCH_TITLE_MAX_LENGTH));
      benchmarkString.replace(0, benchmarkName.size(), benchmarkName);
      perFrameLogStream << benchmarkString << ": "
                  << std::setw(8) << benchmark->second
                  << " us,  "
                  << std::setw(12) << ((float)(benchmark->second) / ups)
                  << " us(m.p.u.), calls: "
                  << frameBenchmarksInvocations[benchmark->first]
                  << ",\t"
                  << std::setw(8) << ((float)(benchmark->second) / frameBenchmarksInvocations[benchmark->first])
                  << " us(m.p.i.)\n";
    }
  perFrameLogStream << '\n';
}

/**
* @brief for each registered benchmark set timings and invocation count to zero. Should be used at the end of a timer update 
*/
void BenchmarkTimer::resetFrameBenchmarks()
{
  for (auto timingBenchmark = frameBenchmarksTimings.begin(); timingBenchmark != frameBenchmarksTimings.end(); ++timingBenchmark)
    timingBenchmark->second = 0;
  for (auto invocationCountBenchmark = frameBenchmarksInvocations.begin(); invocationCountBenchmark != frameBenchmarksInvocations.end(); ++invocationCountBenchmark)
    invocationCountBenchmark->second = 0;
}
