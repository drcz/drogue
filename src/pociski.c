#include "drogue.h"

/* wszystko czym mozna rzucic, trafic, uderzyc, zdetonowac ratatata */

Pocisk *nowy_pocisk(Przedmiot *p,int x,int y,int dx,int dy,
		    int ile,TakNie w,TakNie t)
{
  Pocisk *n=(Pocisk *)malloc(sizeof(Pocisk));
  if(n)
    {
      n->poz.x=x; n->poz.y=y;
      n->d.x=dx; n->d.y=dy;
      n->wybucha=w; n->tluczesie=t;
      n->ile=ile;
      n->przedmiot=p;
      n->poprz=NULL;
      n->nast=lista_pociskow;
      if(lista_pociskow)lista_pociskow->poprz=n;
      lista_pociskow=n;
    }
  return n;
}

void zakoncz_pocisk(Pocisk *p)
{
  Pocisk *na=p->nast,*po=p->poprz;
  mapa[p->poz.x][p->poz.y].pocisk=NULL;
  if(p->tluczesie==NIE)
    {
      dodaj_przedmiot(p->poz.x,p->poz.y,p->przedmiot);
      if(mapa[p->poz.x][p->poz.y].stwor)
	p->przedmiot->trafiony(mapa[p->poz.x][p->poz.y].stwor);
    }
  if(mapa[p->poz.x][p->poz.y].pojemnik
     && mapa[p->poz.x][p->poz.y].pojemnik->wybucha==TAK)
    usun_pojemnik(mapa[p->poz.x][p->poz.y].pojemnik); // tez eksplozja!
  else if(p->wybucha==TAK) eksplozja(p->poz.x,p->poz.y);
  if(na)na->poprz=po;
  if(po)po->nast=na;
   else lista_pociskow=na;
  free(p);
}

void cykl_pocisku(Pocisk *p)
{
  if(p->ile--<1
  || mapa[p->poz.x+p->d.x][p->poz.y+p->d.y].pocisk
  || mozna_wejsc(p->poz.x+p->d.x,p->poz.y+p->d.y)==NIE) zakoncz_pocisk(p);
  else
    {
      mapa[p->poz.x][p->poz.y].pocisk=NULL;
      p->poz.x+=p->d.x;
      p->poz.y+=p->d.y;
      mapa[p->poz.x][p->poz.y].pocisk=p;
      if(mapa[p->poz.x][p->poz.y].stwor
      || mapa[p->poz.x][p->poz.y].pojemnik)zakoncz_pocisk(p);
    }
}

void rzuc(Stwor *kto,Przedmiot *co,int ile,int dx,int dy)
{

  if(co->typ==INNY && co->in.ilosc>1)
    {
      co->in.ilosc--;
      nowy_pocisk(klonuj_przedmiot(co),kto->poz.x,kto->poz.y,dx,dy,
		  ile,NIE,NIE);
    }
  else
    {
      nowy_pocisk(co,kto->poz.x,kto->poz.y,dx,dy,ile,NIE,NIE);
      usun_z_ekwipunku(kto,co);
    }
  widz();
  rysuj();
}

void eksplozja(int x,int y)
{
  int i,j;
  dodaj_komunikat("BUM!");
  for(i=x-1;i<=x+1;i++)
    for(j=y-1;j<=y+1;j++)
      if(i>=0 && j>=0 && i<SZE && j<WYS)
	{
	  mapa[i][j].sym.kolor=KARMAZYN;
	  mapa[i][j].sym.znak='*';
	  if(mapa[i][j].stwor)
	    {	      
	      dodaj_stan(mapa[i][j].stwor,POPAZONY,losuj(6,16));
	      sprawdz(mapa[i][j].stwor);
	    }
	  //	  if(mapa[i][j].pojemnik) usun_pojemnik(mapa[i][j].pojemnik);
	}
  dodaj_komunikat(" ");
}
