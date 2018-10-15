#include "BenchmarkTimer.h"

std::ofstream BenchmarkTimer::perFrameLog, BenchmarkTimer::perAppLog;
bool BenchmarkTimer::outputCreated = false;
std::map<std::string, unsigned long> BenchmarkTimer::benchmarksTimers;
std::map<std::string, int> BenchmarkTimer::benchmarksInvocations;
std::map<std::string, float> BenchmarkTimer::appBenchmarks;

BenchmarkTimer::BenchmarkTimer(const std::string &text, bool isPerFrame)
  :
    benchmark(text),
    perFrame(isPerFrame)
{
  if (!outputCreated)
    {
      perFrameLog.open(RES_DIR + "perFrameLog.txt", std::ios_base::ate);
      if (!perFrameLog)
        std::cerr << "perFrameLog file not found!\n";
      perFrameLog << "SOME INFORMATION MIGHT BE IRRELEVANT OR EVEN INCORRECT, GREAT CARE SHOULD BE TAKEN\n";
      perFrameLog << "SI == Single Invocation (per second), ST == separate thread, m.p.u/a/i. == mean per update/app/invocation\n";
      perAppLog.open(RES_DIR + "perAppLog.txt", std::ios_base::app);
      if (!perAppLog)
        std::cerr << "perAppLog file not found!\n";
      perAppLog << "SOME INFORMATION MIGHT BE IRRELEVANT OR EVEN INCORRECT, GREAT CARE SHOULD BE TAKEN\n";
      perAppLog << "SI == Single Invocation (per second), ST == separate thread, m.p.u/a/i. == mean per update/app/invocation\n";
      outputCreated = true;
    }
  if (appBenchmarks.find(benchmark) == appBenchmarks.end())
    {
      appBenchmarks[benchmark] = 0;
      if (perFrame)
        {
          benchmarksTimers[benchmark] = 0;
          benchmarksInvocations[benchmark] = 0;
        }
    }
}

BenchmarkTimer::~BenchmarkTimer()
{
  endTime = std::chrono::high_resolution_clock::now();
  unsigned long benchmarkResult = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
  float appBenchmarkResult = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() / 1000.0f;
  if (perFrame)
    {
      benchmarksTimers[benchmark] += benchmarkResult;
      benchmarksInvocations[benchmark]++;
    }
  appBenchmarks[benchmark] += appBenchmarkResult;
}

void BenchmarkTimer::finish(unsigned int updateCount)
{
  perFrameLog.flush();
  perFrameLog.close();
  printBenchmarksPerApp(updateCount);
  perAppLog << '\n' << std::flush;
  perAppLog.close();
}

void BenchmarkTimer::printBenchmarksPerApp(unsigned int updateCount)
{
  time_t nowTime = time(0);
  struct tm  tstruct;
  char       dateCharBuffer[80];
  tstruct = *localtime(&nowTime);
  strftime(dateCharBuffer, sizeof(dateCharBuffer), "%Y.%m.%d %X", &tstruct);
  perAppLog << "Benchmark: " << dateCharBuffer << ", summary ticks: " << updateCount << '\n';
  std::multimap<float, std::string> appBenchmarksSorted;
  std::transform(appBenchmarks.begin(),
                 appBenchmarks.end(),
                 std::inserter(appBenchmarksSorted, appBenchmarksSorted.begin()),
                 [](std::pair<std::string, float> p)->std::pair<float, std::string>
  {
    return std::pair<float, std::string>(p.second, p.first);
  });
  //print in descending value order without external sorting
  for (auto bench = appBenchmarksSorted.end(); bench != appBenchmarksSorted.begin();)
    {
      --bench;
      char benchmarkName[BENCH_NAME_MAX_LENGTH];
      memset(benchmarkName, FORMAT_VALUE_ASCII, BENCH_NAME_MAX_LENGTH);
      memcpy(benchmarkName, (bench->second.substr(0, BENCH_NAME_MAX_LENGTH)).c_str(), (bench->second.substr(0, BENCH_NAME_MAX_LENGTH)).size());
      benchmarkName[BENCH_NAME_MAX_LENGTH-1] = '\0';
      perAppLog << benchmarkName << ": " << std::setw(10) << std::setprecision(6) << bench->first << "ms";
      if (benchmarksTimers.find(bench->second) != benchmarksTimers.end()) //check whether a benchmark is a "per frame" and if it is - print mean value
        perAppLog << ", " << std::setw(10) << std::setprecision(5) << (bench->first / updateCount * 1000) << "us (m.p.a.)\n";
      else
        perAppLog << '\n';
    }
}

void BenchmarkTimer::printBenchmarksPerFrame(unsigned int updateCount, unsigned int ups)
{
  perFrameLog << "--Update: "
              << updateCount
              << ", UPS: "
              << ups
              << ", mean CPU frametime: "
              << (1000.0f / ups)
              << "ms ("
              << (1000000.0f / ups)
              << "us)\n";
  for (auto bench = benchmarksTimers.begin(); bench != benchmarksTimers.end(); ++bench)
    {
      char benchmarkName[BENCH_NAME_MAX_LENGTH];
      memset(benchmarkName, FORMAT_VALUE_ASCII, BENCH_NAME_MAX_LENGTH);
      memcpy(benchmarkName, (bench->first.substr(0, BENCH_NAME_MAX_LENGTH)).c_str(), (bench->first.substr(0, BENCH_NAME_MAX_LENGTH)).size());
      benchmarkName[BENCH_NAME_MAX_LENGTH-1] = '\0';
      perFrameLog << benchmarkName << ": "
                  << std::setw(8) << bench->second
                  << ",\t"
                  << std::setw(8) << ((float)(bench->second) / ups)
                  << "us (m.p.u.), invocations: "
                  << benchmarksInvocations[bench->first]
                  << ",\t"
                  << std::setw(8) << ((float)(bench->second) / benchmarksInvocations[bench->first])
                  << "us (m.p.i.)\n";
    }
  perFrameLog << '\n';
}

void BenchmarkTimer::clearBenchmarksPerFrameValues()
{
  for (auto bench = benchmarksTimers.begin(); bench != benchmarksTimers.end(); ++bench)
    bench->second = 0;
  for (auto bench = benchmarksInvocations.begin(); bench != benchmarksInvocations.end(); ++bench)
    bench->second = 0;
}
