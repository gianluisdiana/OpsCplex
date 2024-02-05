#include <timer_t.hpp>

namespace emir {

timer_t::timer_t() : acumulated_(0.0), start_() {}

timer_t::~timer_t() {}

void timer_t::start() {
  getrusage(RUSAGE_THREAD, &start_);
}

void timer_t::reset() {
  acumulated_ = 0;
}

void timer_t::stop() {
  rusage stop;
  getrusage(RUSAGE_THREAD, &stop);

  acumulated_ +=
    (double)(stop.ru_utime.tv_sec) - (double)(start_.ru_utime.tv_sec);
  acumulated_ +=
    ((double)(stop.ru_utime.tv_usec) - (double)(start_.ru_utime.tv_usec)) /
    1000000.0;
}

double timer_t::seconds() const {
  return acumulated_;
}

double timer_t::current_time() const {
  rusage stop;
  getrusage(RUSAGE_THREAD, &stop);

  double acumulated = 0.0;

  acumulated = acumulated_ + (double)(stop.ru_utime.tv_sec) -
               (double)(start_.ru_utime.tv_sec);
  acumulated += acumulated_ + ((double)(stop.ru_utime.tv_usec) -
                               (double)(start_.ru_utime.tv_usec)) /
                                1000000.0;

  return acumulated;
}
}  // namespace emir
