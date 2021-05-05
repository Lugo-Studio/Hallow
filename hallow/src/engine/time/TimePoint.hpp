//
// Created by Gabriel Lugo on 3/29/2021.
//

#ifndef PROJECT_A_TIMEPOINT_H
#define PROJECT_A_TIMEPOINT_H

#include <chrono>


namespace Hallow {
  using Seconds = std::chrono::duration<double>;

  class TimePoint {
  public:
    TimePoint(double time = 0.0) : m_time(time) {
    }

    // override operators to increment time
    double seconds() const {
      return m_time.count();
    }

    double milliseconds() const {
      return m_time.count() * 1000.0;
    }

    bool operator==(const TimePoint& rhs) const {
      return m_time == rhs.m_time;
    }

    bool operator!=(const TimePoint& rhs) const {
      return !(m_time == rhs.m_time);
    }

    bool operator<(const TimePoint& rhs) const {
      return m_time < rhs.m_time;
    }

    bool operator>(const TimePoint& rhs) const {
      return rhs.m_time < m_time;
    }

    bool operator<=(const TimePoint& rhs) const {
      return !(m_time > rhs.m_time);
    }

    bool operator>=(const TimePoint& rhs) const {
      return !(m_time < rhs.m_time);
    }

    TimePoint& operator+=(const TimePoint& rhs) {
      m_time += rhs.m_time;
      return *this;
    }

    TimePoint& operator-=(const TimePoint& rhs) {
      m_time -= rhs.m_time;
      return *this;
    }

    TimePoint& operator*=(const TimePoint& rhs) {
      m_time = m_time * rhs.m_time.count();
      return *this;
    }

    TimePoint& operator/=(const TimePoint& rhs) {
      m_time = m_time / rhs.m_time.count();
      return *this;
    }

    //friend TimePoint operator+(TimePoint lhs, const TimePoint& rhs) { return lhs += rhs; }
    //friend TimePoint operator-(TimePoint lhs, const TimePoint& rhs) { return lhs -= rhs; }
    //friend TimePoint operator*(TimePoint lhs, const TimePoint& rhs) { return lhs *= rhs; }
    //friend TimePoint operator/(TimePoint lhs, const TimePoint& rhs) { return lhs /= rhs; }

  private:
    Seconds m_time;
  };
}

#endif //PROJECT_A_TIMEPOINT_H
