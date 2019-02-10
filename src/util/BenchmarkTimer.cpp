#include "util/BenchmarkTimer.h"

std::ofstream BenchmarkTimer::perFrameLogStream, BenchmarkTimer::perAppLogStream;
bool BenchmarkTimer::outputCreated = false;
std::map<std::string, unsigned long> BenchmarkTimer::frameBenchmarksTimings;
std::map<std::string, int> BenchmarkTimer::frameBenchmarksInvocations;
std::map<std::string, float> BenchmarkTimer::applicationBenchmarks;

BenchmarkTimer::BenchmarkTimer(const std::string &title, bool isPerFrame)
  :
    benchmarkTitle(title),
    isPerFrame(isPerFrame)
{
  if (!outputCreated)
    {
      perFrameLogStream.open(RES_DIR + "perFrameLog.txt", std::ios_base::ate);
      if (!perFrameLogStream)
        Logger::log("perFrameLog file not found!\n");
      perFrameLogStream << "SOME INFORMATION MIGHT BE IRRELEVANT OR EVEN INCORRECT, GREAT CARE SHOULD BE TAKEN\n";
      perFrameLogStream << "SI == Single Invocation (per second), ST == separate thread, m.p.u/a/i. == mean per update/app/invocation\n";
      perAppLogStream.open(RES_DIR + "perAppLog.txt", std::ios_base::app);
      if (!perAppLogStream)
        Logger::log("perAppLog file not found!\n");
      perAppLogStream << "SOME INFORMATION MIGHT BE IRRELEVANT OR EVEN INCORRECT, GREAT CARE SHOULD BE TAKEN\n";
      perAppLogStream << "SI == Single Invocation (per second), ST == separate thread, m.p.u/a/i. == mean per update/app/invocation\n";
      outputCreated = true;
    }
  if (applicationBenchmarks.find(benchmarkTitle) == applicationBenchmarks.end())
    {
      applicationBenchmarks[benchmarkTitle] = 0;
      if (isPerFrame)
        {
          frameBenchmarksTimings[benchmarkTitle] = 0;
          frameBenchmarksInvocations[benchmarkTitle] = 0;
        }
    }
}

void BenchmarkTimer::operator()(const std::string &title, bool isPerFrame)
{
  benchmarkTitle = title;
  this->isPerFrame = isPerFrame;
}

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
  applicationBenchmarks[benchmarkTitle] += applicationTimingResultMillis;
}

void BenchmarkTimer::finish(unsigned int updateCount)
{
  perFrameLogStream.flush();
  perFrameLogStream.close();
  printApplicationBenchmarks(updateCount);
  perAppLogStream << '\n' << std::flush;
  perAppLogStream.close();
}

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
  std::transform(applicationBenchmarks.begin(),
                 applicationBenchmarks.end(),
                 std::inserter(applicationBenchmarksSortedByValue, applicationBenchmarksSortedByValue.begin()),
                 [](std::pair<std::string, float> p)->std::pair<float, std::string>
  {
    return std::pair<float, std::string>(p.second, p.first);
  });

  //print in descending value order without external sorting
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

void BenchmarkTimer::resetFrameBenchmarks()
{
  for (auto timingBenchmark = frameBenchmarksTimings.begin(); timingBenchmark != frameBenchmarksTimings.end(); ++timingBenchmark)
    timingBenchmark->second = 0;
  for (auto invocationCountBenchmark = frameBenchmarksInvocations.begin(); invocationCountBenchmark != frameBenchmarksInvocations.end(); ++invocationCountBenchmark)
    invocationCountBenchmark->second = 0;
}
