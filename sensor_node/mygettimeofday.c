//#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "mygettimeofday.h"

double gettimeofday_sec(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}

long long gettimeofday_msec(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec * 1e3) + (tv.tv_usec * 1e-3);
}

long long gettimeofday_usec(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec * 1e6) + tv.tv_usec;
}
/*
int main(){
  printf("%lf\n",gettimeofday_sec());
  printf("%ld\n",gettimeofday_msec());
  printf("%ld\n",gettimeofday_usec());
  return 0;
  }*/
