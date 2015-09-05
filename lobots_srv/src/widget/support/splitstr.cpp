#include "splitstr.h"
#include <string.h>

void splitstr(const char *orig, const char sep, char *str1, char *str2) {
  
  struct support {
    static int findnext(const char *str, const char sep) {
      for (int i = 0; i < strlen(str); i++) {
	if (str[i] == sep) return i;
      }
      return -1;
    }

    static void trim(char *str) {
      while (str[0] == ' ') {
	for (int i = 0; i < strlen(str); i++) str[i] = str[i + 1];
      }
      while (strlen(str) > 0 && str[strlen(str) - 1] == ' ') {
	str[strlen(str) - 1] = 0;
      }
    }
  };

  int p = support::findnext(orig, sep);
  if (p != -1) {
    int len = strlen(orig);
    for (int i = 0; i < p; i++) {
      str1[i] = orig[i];
    }
    str1[p] = 0;
    
    for (int i = p + 1; i < len; i++) {
      str2[i - p - 1] = orig[i];
    }
    str2[len - p - 1] = 0;
  } else {
    strcpy(str1, orig);
    str2[0] = 0;
  }
  
  support::trim(str1);
  support::trim(str2);
}

