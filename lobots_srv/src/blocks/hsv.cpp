#include "hsv.h"



void HSV(tcor* cor, float H, float S, float V) {
     if (S < 0) S = 0;
     if (S > 1) S = 1;
     if (V < 0) V = 0;
     if (V > 1) V = 1;
     
float f, p, q, t, hi;
        hi = ((int) (H / 60)) % 6;
        f = (H/60) - hi;
        p = ( V*(1 - S) )*127;
        q = ( V*(1 - f*S) )*127;
        t = ( V*(1 - (1 - f)*S) )*127;
        if (hi == 0) {
               cor->r = (int) (V*127);
               cor->g = (int) t;
               cor->b = (int) p;
               return;
               }
        if (hi == 1)  {
               cor->r = (int) q;
               cor->g = (int) (V*127);
               cor->b = (int) p;
               return;
               }
        if (hi == 2) {
               cor->r = (int) p;
               cor->g = (int) (V*127);
               cor->b = (int) t;
               return;
               }
        if (hi == 3) {
               cor->r = (int) p;
               cor->g = (int) q;
               cor->b = (int) (V*127);
               return;
               }
        if (hi == 4) {
               cor->r = (int) t;
               cor->g = (int) p;
               cor->b = (int) (V*127);
               return;
               }
        if (hi == 5) {
               cor->r = (int) (V*127);
               cor->g = (int) p;
               cor->b = (int) q;
               return;
               }
        cor->r = 0;
        cor->g = 0;
        cor->b = 0;
}
//---------------------------------------------------------------------------
float GetHValue(tcor* cor){
        float r, g, b, max, min;
        r = (float) (cor->r)/126;
        g = (float) (cor->g)/126;
        b = (float) (cor->b)/126;
        max = r;
        min = r;
        if (g > max) max = g;
        if (b > max) max = b;
        if (g < min) min = g;
        if (b < min) min = b;

        if (max == min) return 0;
        if ((max == r) && (g >= b)) return 60*((g-b)/(max-min));
        if ((max == r) && (g < b)) return 60*((g-b)/(max-min)) + 360;
        if (max == g) return 60*((b-r)/(max-min)) + 120;
        if (max == b) return 60*((r-g)/(max-min)) + 240;
        return 0;
}

//---------------------------------------------------------------------------
float GetSValue(tcor* cor){
        float r, g, b, max, min;
        r = (float) (cor->r)/126;
        g = (float) (cor->g)/126;
        b = (float) (cor->b)/126;
        max = r;
        min = r;
        if (g > max) max = g;
        if (b > max) max = b;
        if (g < min) min = g;
        if (b < min) min = b;

        if (max == 0) return 0;
        return 1 - (min/max);

}

//---------------------------------------------------------------------------
float GetVValue(tcor* cor){
        float r, g, b, max, min;
        r = (float) (cor->r)/127;
        g = (float) (cor->g)/127;
        b = (float) (cor->b)/127;
        max = r;
        if (g > max) max = g;
        if (b > max) max = b;

        return max;
}


