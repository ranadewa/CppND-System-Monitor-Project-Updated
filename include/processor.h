#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();
  
 private:

    float previous_total_ = 0;
    float prev_non_idle_ = 0;
};

#endif