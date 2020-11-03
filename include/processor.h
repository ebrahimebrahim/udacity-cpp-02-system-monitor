#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
 public:
  float Utilization() const {return utilization;} 

  void update(const LinuxParser::StatData &);

  // TODO: Declare any necessary private members
 private:
    float utilization{};
};

#endif