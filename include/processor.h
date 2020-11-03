#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
 public:
  float Utilization() const {return utilization;} 

  void update(const LinuxParser::StatData &);

 private:
    float utilization{};
    unsigned int prev_total{};
    unsigned int prev_idle{};
};

#endif