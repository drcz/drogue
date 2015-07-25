#include"drogue.h"

struct op_stan opis_stanu[MAX_STAN]={
  {{0,0,1,0,0,0,0,8,0},"przysp.",
   "czujesz ze zaczyna cie roznosic!",
   "wewnetrzny scisk znika, czujesz sie zmeczony."},
  {{0,0,0,0,0,0,0,-8,0},"spow.",
   "czujesz sie strasznie ciezki i wolny...",
   "ciezkosc znika, troche boli cie glowa."},
  {{0,0,-2,0,0,0,0,0,0},"popaz.",
   "jestes popazony!",
   "popazenia sie zagoily."},
  {{0,0,0,0,0,0,-18,0,0},"oslep.",
   "nic nie widzisz!",
   "powoli odzyskujesz wzrok..."},
  {{0,0,0,0,0,0,8,0,0},"duzeoczy",
   "wszystko jest takie jasne!",
   "powoli natezenie swiatla wraca do normy..."},
  {{0,0,3,0,0,0,0,0,0},"szal",
   "zaczyna cie roznosic!",
   "wewnetrzny scisk znika, czujesz sie zmeczony..."},
  {{0,0,-4,0,0,0,0,0,0},"sennosc",
   "robisz sie zmeczony i spiacy",
   "sennosc mija, pozostaje tylko bol glowy..."},
  {{0,0,0,0,0,0,0,55,0},"regen.",
   "czujesz niezwykle mrowienie w calym ciele.",
   "mrowienie w calym ciele przechodzi."},
  {{0,0,-1,0,0,0,-2,-13,0},"zatrucie",
   "zaczal cie strasznie bolec brzuch.",
   "...zwymiotowales i bol brzucha minal."},
  {{0,0,0,0,0,0,0,0,0},"dezornt.",
   "ojej, wszystko sie tak bardzo rusza!",
   "wracasz do rownowagi."}};

void dodaj_stan(Stwor *komu,int typ,int ile)
{
  int i;
  Stan *s;

  for(s=komu->stany;s!=NULL;s=s->nast)
    if(s->typ==typ)
      {
	s->ile+=ile;
	break;
      }
  if(s==NULL)
    {
      s=(Stan *)malloc(sizeof(Stan));
      if(s)
	{
	  s->typ=typ;
	  s->ile=ile;
	  s->poprz=NULL;
	  s->nast=komu->stany;
	  if(komu->stany) (komu->stany)->poprz=s;
	  komu->stany=s;
	  for(i=0;i<MAX_STAT;i++)komu->stat[i]+=opis_stanu[typ].mod[i];
	}
    }
  if(komu==&gracz) dodaj_komunikat(opis_stanu[typ].poczatek);
}

void sprawdz_stany(Stwor *komu)
{
  Stan *v,*q,*p=komu->stany;
  int i;
  for(;p!=NULL;p=q)
    {
      q=p->nast;
      v=p->poprz;
      if((p->ile--)<1)
	{
	  for(i=0;i<MAX_STAT;i++)komu->stat[i]-=opis_stanu[p->typ].mod[i];
	  if(komu==&gracz)
	    {
	      dodaj_komunikat(opis_stanu[p->typ].koniec);
	      dodaj_komunikat(" ");
	      // nieelegancja francja wrrr:
	      move(WYS+2,1);
	      printw("                                                                            ");
	      move(WYS+3,1);
	      printw("                                                                            ");
	    }
	  if(v) v->nast=q;
	   else  komu->stany=q;
	  if(q) q->poprz=v;
	  free(p);
  	}      
    }
}
