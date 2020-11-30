#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization() { 
    float utilisation = 0;

    auto utilisations = LinuxParser::CpuUtilization();

    if(utilisations.size() == 10)
    {
        double user = std::stod(utilisations[0]);
        double nice = std::stod(utilisations[1]);
        double system = std::stod(utilisations[2]);
        double idle = std::stod(utilisations[3]);
        double iowait = std::stod(utilisations[4]);
        double irq= std::stod(utilisations[5]);
        double softirq= std::stod(utilisations[6]);
        double steal= std::stod(utilisations[7]);

        double totalIdle = idle + iowait;
        double totalNonIdle = user + nice + system + irq + softirq + steal;
        double total = totalIdle + totalNonIdle;

        utilisation = (totalNonIdle -  prev_non_idle_) / (total - previous_total_);
        previous_total_ = total;
        prev_non_idle_ = totalNonIdle;
    }

    return utilisation;
 }