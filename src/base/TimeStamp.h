#pragma once
#include <sys/time.h>
#include <stdint.h>
#include <string>
#include <boost/operators.hpp>
namespace CzyNetFrame{

constexpr int KPerSecondMicroSecondsCount = 1000*1000;

class TimeStamp : public boost::equality_comparable<TimeStamp>,
                  public boost::less_than_comparable<TimeStamp>
{
 public:
  ///
  /// Constucts an invalid TimeStamp.
  ///
  TimeStamp()
    : microSecondsSinceEpoch_(0)
  {
  }

  ///
  /// Constucts a TimeStamp at specific time
  ///
  /// @param microSecondsSinceEpoch
  explicit TimeStamp(int64_t microSecondsSinceEpochArg)
    : microSecondsSinceEpoch_(microSecondsSinceEpochArg)
  {
  }

  void swap(TimeStamp& that)
  {
    std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
  }

  // default copy/assignment/dtor are Okay

  std::string toString() const;
  std::string toFormattedString(bool showMicroseconds = true) const;

  bool valid() const { return microSecondsSinceEpoch_ > 0; }

  // for internal usage.
  int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }
  time_t secondsSinceEpoch() const
  { return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond); }

  ///
  /// Get time of now.
  ///
  static TimeStamp now();
  static TimeStamp invalid()
  {
    return TimeStamp();
  }

  static TimeStamp fromUnixTime(time_t t)
  {
    return fromUnixTime(t, 0);
  }

  static TimeStamp fromUnixTime(time_t t, int microseconds)
  {
    return TimeStamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond + microseconds);
  }

  static const int kMicroSecondsPerSecond = 1000 * 1000;

 private:
  int64_t microSecondsSinceEpoch_;
};

inline bool operator<(TimeStamp lhs, TimeStamp rhs)
{
  return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(TimeStamp lhs, TimeStamp rhs)
{
  return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

///
/// Gets time difference of two TimeStamps, result in seconds.
///
/// @param high, low
/// @return (high-low) in seconds
/// @c double has 52-bit precision, enough for one-microsecond
/// resolution for next 100 years.
inline double timeDifference(TimeStamp high, TimeStamp low)
{
  int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
  return static_cast<double>(diff) / TimeStamp::kMicroSecondsPerSecond;
}

///
/// Add @c seconds to given TimeStamp.
///
/// @return TimeStamp+seconds as TimeStamp
///
inline TimeStamp addTime(TimeStamp timeStamp, double seconds)
{
  int64_t delta = static_cast<int64_t>(seconds * TimeStamp::kMicroSecondsPerSecond);
  return TimeStamp(timeStamp.microSecondsSinceEpoch() + delta);
}

}


