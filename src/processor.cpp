#include "processor.h"


void Processor::update(const LinuxParser::StatData & stat_data) {

    // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux

    const unsigned int idle = stat_data.cpu_times[LinuxParser::CPU_STATE_IDLE] + stat_data.cpu_times[LinuxParser::CPU_STATE_IOWAIT];
    const unsigned int total = idle + 
       stat_data.cpu_times[LinuxParser::CPU_STATE_USER] +
       stat_data.cpu_times[LinuxParser::CPU_STATE_NICE] +
       stat_data.cpu_times[LinuxParser::CPU_STATE_SYSTEM] +
       stat_data.cpu_times[LinuxParser::CPU_STATE_IRQ] +
       stat_data.cpu_times[LinuxParser::CPU_STATE_SOFTIRQ] +
       stat_data.cpu_times[LinuxParser::CPU_STATE_STEAL];

    const unsigned int d_total = total - prev_total;
    const unsigned int d_idle = idle - prev_idle;

    if (d_total==0) utilization = 0.0f;
    else
        utilization = float(d_total - d_idle) / float(d_total);

    prev_total = total;
    prev_idle = idle;
}