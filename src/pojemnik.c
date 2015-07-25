#include"drogue.h"

void dodaj_pojemnik(int x,int y,TakNie w)
{
  Pojemnik *p;
  if(mapa[x][y].pojemnik) return;
  if((p=(Pojemnik *)malloc(sizeof(Pojemnik))))
    {
      p->poz.x=x; p->poz.y=y;
      p->wybucha=w;
      p->sym.znak='0';
      p->sym.kolor=ZOLC;
      mapa[x][y].pojemnik=p;
    }
}

void usun_pojemnik(Pojemnik *p)
{
  mapa[p->poz.x][p->poz.y].pojemnik=NULL;
  if(p->wybucha==TAK)eksplozja(p->poz.x,p->poz.y);
  free(p);
}

