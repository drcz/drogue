#include "drogue.h"

/*
int odl(Poz a,Poz b)
{
  return (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y*b.y);
}
*/

void decydent(Stwor *s)
{
 Poz p;
 int dx,dy,i;
 
  dx=0;dy=0;
  if(losuj(1,4)!=2) // jak nie wiadomo co robic, mozna zawsze
                    // zwiekszyc chaotycznosc zachowan.
    {
      if(gracz.poz.x>s->poz.x) dx=1;
      else if(gracz.poz.x<s->poz.x) dx=-1;
      if(gracz.poz.y>s->poz.y) dy=1;
      else if(gracz.poz.y<s->poz.y) dy=-1;
      p.x=s->poz.x+dx;
      p.y=s->poz.y+dy; 
      if(mozna_wejsc(p.x,p.y)==TAK)
	{
	  s->pa+=a_idz(s,dx,dy);
	  return;
	}
    }
  // tu zeby jak co szedl za najblizszym potworem? czy nie?
    i=0;
    do
     {
      if(++i>6)
	{
	  s->pa+=a_czekaj(s);
	  return;
	}
      dx=losuj(-1,1);
      dy=losuj(-1,1);
      p.x=s->poz.x+dx;
      p.y=s->poz.y+dy;  
     }while(!mozna_wejsc(p.x,p.y) || (mapa[p.x][p.y].stwor
				      && mapa[p.x][p.y].stwor!=&gracz));
     s->pa+=a_idz(s,dx,dy);
}

