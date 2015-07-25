#include"drogue.h"

double sinus[]={0.000000,0.099833,0.198669,0.295520,0.389418,0.479426,0.564642,0.644218,0.717356,0.783327};

double kosinus[]={1.000000,0.995004,0.980067,0.955336,0.921061,0.877583,0.825336,0.764842,0.696707,0.621610};

void zobacz(int x,int y)
{
  if(mapa[x][y].pocisk)
    mapa[x][y].sym=mapa[x][y].pocisk->przedmiot->sym;    
  else if(mapa[x][y].stwor)
    mapa[x][y].sym=mapa[x][y].stwor->sym;
  else if(mapa[x][y].pojemnik)
    mapa[x][y].sym=mapa[x][y].pojemnik->sym;
  else if(mapa[x][y].przedmiot)
    mapa[x][y].sym=mapa[x][y].przedmiot->sym;
  else
    mapa[x][y].sym=podstawka[mapa[x][y].t].sym;
}

TakNie okt(int dx,int dy)
{
  Poz p;

  p.x=gracz.poz.x+dx;
  p.y=gracz.poz.y+dy;
  zobacz(p.x,p.y);
  return mozna_wejsc(p.x,p.y);
}

void _widz()
{
  int i,j;
  for(i=0;i<SZE;i++)
    for(j=0;j<WYS;j++) zobacz(i,j);
}

void widz()
{
  register int x,y,r,d;
  TakNie o[8];

  //  _widz();
  //  return;

 for(x=0;x<SZE;x++)
  for(y=0;y<WYS;y++)
   mapa[x][y].sym.kolor=INDYGO;
  for(d=0;d<10;d++)
    {
      for(r=0;r<8;r++)o[r]=1;
      for(r=1;r<gracz.stat[R_WIDZENIA];r++)
	{
	  x=r*sinus[d]; y=r*kosinus[d];
	  if(o[0])o[0]=okt(x,y);
	  if(o[1])o[1]=okt(y,x);
	  if(o[2])o[2]=okt(-x,-y);
	  if(o[3])o[3]=okt(-y,-x);
	  if(o[4])o[4]=okt(-x,y);
	  if(o[5])o[5]=okt(-y,x);
	  if(o[6])o[6]=okt(x,-y);
	  if(o[7])o[7]=okt(y,-x);
	}
    }
}

