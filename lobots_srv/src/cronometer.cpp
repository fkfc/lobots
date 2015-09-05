#include "cronometer.h"

#include <stdlib.h>


TCronometer::TCronometer() {
}


void TCronometer::Start() {
  gettimeofday(&(this->start), NULL);
}

double TCronometer::GetTime() {
  struct timeval t2;
  gettimeofday(&t2, NULL);
  double milliSeconds = (t2.tv_sec - start.tv_sec) * 1000 + (t2.tv_usec - start.tv_usec)/1000;
  return milliSeconds;
}
