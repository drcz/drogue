#include"drogue.h"
#include<time.h>


void inicjuj_losuj()
{
  srand(time(0));
}

int losuj(int a,int b)
{
  if(b<=a) return a;
  return a+rand()%(b-a+1);
}
