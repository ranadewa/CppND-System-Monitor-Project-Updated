#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization() { 
    float utilisation = 0;

    auto utilisations = LinuxParser::CpuUtilization();

    if(utilisations.size() == 10)
    {
        double user = std::stod(utilisations[LinuxParser::CPUStates::kUser_]);
        double nice = std::stod(utilisations[LinuxParser::CPUStates::kNice_]);
        double system = std::stod(utilisations[LinuxParser::CPUStates::kSystem_]);
        double idle = std::stod(utilisations[LinuxParser::CPUStates::kIdle_]);
        double iowait = std::stod(utilisations[LinuxParser::CPUStates::kIOwait_]);
        double irq= std::stod(utilisations[LinuxParser::CPUStates::kIRQ_]);
        double softirq= std::stod(utilisations[LinuxParser::CPUStates::kSoftIRQ_]);
        double steal= std::stod(utilisations[LinuxParser::CPUStates::kSteal_]);

        double totalIdle = idle + iowait;
        double totalNonIdle = user + nice + system + irq + softirq + steal;
        double total = totalIdle + totalNonIdle;

        utilisation = (totalNonIdle -  prev_non_idle_) / (total - previous_total_);
        previous_total_ = total;
        prev_non_idle_ = totalNonIdle;
    }

    return utilisation;
 }