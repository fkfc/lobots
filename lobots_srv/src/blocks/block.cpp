#include "block.h"

#include <math.h>



float TBlock::Distance(TCamera *Camera) {
      float dx, dy, dz;
      dx = Camera->transx + this->x;
      dy = Camera->transy + this->y;
      dz = Camera->transz + this->z;
      
      float dist = sqrt(dx*dx + dy*dy + dz*dz);
      return dist;
}
