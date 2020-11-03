#include "processor.h"


// TODO : update member variables as needed
void Processor::update(const LinuxParser::StatData & stat_data) {

    unsigned int idle = stat_data.cpu_times[LinuxParser::CPU_STATE_IDLE] + stat_data.cpu_times[LinuxParser::CPU_STATE_IOWAIT];
    unsigned int total = idle + 
       stat_data.cpu_times[LinuxParser::CPU_STATE_USER] +
       stat_data.cpu_times[LinuxParser::CPU_STATE_NICE] +
       stat_data.cpu_times[LinuxParser::CPU_STATE_SYSTEM] +
       stat_data.cpu_times[LinuxParser::CPU_STATE_IRQ] +
       stat_data.cpu_times[LinuxParser::CPU_STATE_SOFTIRQ] +
       stat_data.cpu_times[LinuxParser::CPU_STATE_STEAL];

    unsigned int d_total = total - prev_total;
    unsigned int d_idle = idle - prev_idle;

    if (d_total==0) utilization = 0.0f;
    else
        utilization = float(d_total - d_idle) / float(d_total);

    prev_total = total;
    prev_idle = idle;
}