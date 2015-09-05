#ifndef cronometer_H
#define cronometer_H

#include <sys/time.h>

class TCronometer {
public:
  TCronometer();
  void Start();
  double GetTime();  //returns elapsed time in milliseconds
  
private:
  struct timeval start;
};

#endif 


