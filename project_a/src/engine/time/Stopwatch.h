//
// Created by Gabriel Lugo on 3/27/2021.
//

#ifndef PROJECT_A_STOPWATCH_H
#define PROJECT_A_STOPWATCH_H

#include <chrono>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>


namespace Hallow {

  class Stopwatch {
  public:
    using Clock = std::chrono::steady_clock;
    // duration TYPES
    using Nanoseconds = std::chrono::duration<double, std::nano>;
    using Microseconds = std::chrono::duration<double, std::micro>;
    using Milliseconds = std::chrono::duration<double, std::milli>;
    using Seconds = std::chrono::duration<double>;
    using Minutes = std::chrono::duration<double, std::ratio<60>>;
    using Hours = std::chrono::duration<double, std::ratio<3600>>;

    Stopwatch()
            : m_start(Clock::now()) {}

    void reset() {
      m_start = Clock::now();
    }

    template<typename T>
    double timeElapsed() const {
      return std::chrono::duration_cast<T>(
              Clock::now() - m_start).count();
    }

    double hours() const { return timeElapsed<Hours>(); }

    double minutes() const { return timeElapsed<Minutes>(); }

    double seconds() const { return timeElapsed<Seconds>(); }

    double milliseconds() const { return timeElapsed<Milliseconds>(); }

    double microseconds() const { return timeElapsed<Microseconds>(); }

    double nanoseconds() const { return timeElapsed<Nanoseconds>(); }

    // Friends
    friend std::ostream&
    operator<<(std::ostream& output, const Stopwatch& interval) {
      interval.display(output);
      return output;
    }

  private:
    std::chrono::time_point<Clock> m_start;

    void display(std::ostream& output) const {
      output << seconds();
    }
  };
}


#endif //PROJECT_A_STOPWATCH_H
