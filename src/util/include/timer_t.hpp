#ifndef _EMIR_TIMER_HPP_
#define _EMIR_TIMER_HPP_

#ifdef _DEBUG
#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>

#define IT_PER_LINE 8
#define WIDE_OUTPUT 9
#endif

#include <sys/resource.h>

namespace emir {

class timer_t {
 protected:
  double acumulated_;
  rusage start_;

 public:
  timer_t();
  virtual ~timer_t();

  void start();
  void stop();
  void reset();

  double current_time() const;

  double seconds() const;
};

}  // namespace emir

#endif  // _EMIR_TIMER_HPP_