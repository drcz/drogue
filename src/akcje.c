#include"drogue.h"

enum{KOSZT_IDZ=15,KOSZT_ATAK=13,
     KOSZT_PODNIES=17,KOSZT_WYRZUC=14,KOSZT_UZYJ=19,KOSZT_RZUC=12};

int akcja(Stwor *s,int dx,int dy)
{
  int x=s->poz.x+dx,y=s->poz.y+dy;
  if(mapa[x][y].pojemnik)
    {
      dodaj_komunikat("rozbiles pojemnik...");      
      usun_pojemnik(mapa[x][y].pojemnik);
      rysuj(); // zeby sie narysowali eksplozye...
    }
  if(s==&gracz && (mapa[x][y].t==UWLAZ_W || mapa[x][y].t==UWLAZ_H))
     dodaj_komunikat("odkryles ukryte przejscie!");
  podstawka[mapa[x][y].t].akcja(s,x,y);
  return 0;
}

int a_czekaj(Stwor *s)
{
  return KOSZT_IDZ;
}

int a_pchnij(Stwor *s,int dx,int dy)
{
  int x=s->poz.x+2*dx,y=s->poz.y+2*dy;
  Pojemnik *p;
  if(mozna_wejsc(x,y)==TAK && !mapa[x][y].stwor && !mapa[x][y].pojemnik)
    {
      p=mapa[x-dx][y-dy].pojemnik;
      mapa[x-dx][y-dy].pojemnik=NULL;
      mapa[x][y].pojemnik=p;
      p->poz.x=x; p->poz.x=x;
      if(s==&gracz) dodaj_komunikat("pchnales pojemnik.");
      // jesli odpowiedni bliski potwor to %s pchnal/ela/eli/ely pojemnik
      return a_idz(s,dx,dy);     
    }
  return 0;
}

int a_idz(Stwor *s,int dx,int dy)
{
  if(s)
    {
      int x=s->poz.x+dx, y=s->poz.y+dy;
      if(mapa[x][y].stwor) return a_atak(s,dx,dy);
      if(mapa[x][y].pojemnik) return a_pchnij(s,dx,dy);
      if(mozna_wejsc(x,y)==TAK)
	{
	  mapa[s->poz.x][s->poz.y].stwor=NULL;
	  s->poz.x=x; s->poz.y=y;
	  mapa[s->poz.x][s->poz.y].stwor=s;
	  podstawka[mapa[s->poz.x][s->poz.y].t].wlezienie(s);
	  if(s==&gracz && mapa[s->poz.x][s->poz.y].przedmiot)
	    kom_cos_lezy(mapa[s->poz.x][s->poz.y].przedmiot->nazwa);
	  return KOSZT_IDZ;
	}
      else if(mapa[x][y].t==WLAZ_Z
           || mapa[x][y].t==UWLAZ_H || mapa[x][y].t==UWLAZ_W)
	return akcja(s,dx,dy);
    }
  return 0;
}

int a_zejdz(Stwor *s) // !!!!!!!!!!!!!
{
  if(s==&gracz)
    {
      if(mapa[gracz.poz.x][gracz.poz.y].t==ZEJSCIE)
	{      
	  nowa_plansza();
	  kom_nizszy_poziom();
	  widz();
	  return 23;
	}
      else dodaj_komunikat("tu nie ma zadnego zejscia...");
    }
  return 0;
}

int a_podnies(Stwor *kto)
{
  Przedmiot *p;

  if((p=mapa[kto->poz.x][kto->poz.y].przedmiot))
    {
      if(kto->obciazenie<kto->stat[UDZWIG])
	{
	  podnies(kto);
	  widz();
	  kto->obciazenie+=p->waga;
	  if(kto==&gracz) kom_podniosles(p->nazwa);
	  return KOSZT_PODNIES;
	}
      else
	{
	  if(kto==&gracz) dodaj_komunikat("za duzo juz ze soba niesiesz!");
	  return 0;
	}
    }
  if(kto==&gracz) kom_nie_ma_co_podniesc();
  return 0;
}

int a_wyrzuc(Stwor *kto)
{
  Przedmiot *p;
  int c;

  if(mapa[kto->poz.x][kto->poz.y].przedmiot)
    {
      if(kto==&gracz) dodaj_komunikat("tu juz cos lezy.");
      return 0;
    }
  if((p=kto->ekwipunek))
    {
      pisz_ekw();
      printw("ktory przedmiot wyrzucic?");
      refresh();
      c=getchar();
      rysuj();
      c-='a';
      if(c<0) return 0;
      for(p=kto->ekwipunek;p&&(c>0);p=p->nast) c--;
      if(p)
	{      
	  wyrzuc(kto,p);
	  kto->obciazenie-=p->waga;
	  return KOSZT_WYRZUC;
	}
    }
  else if(kto==&gracz)dodaj_komunikat("nic ze soba nie masz!");
  return 0;
}

int a_uzyj(Stwor *kto)
{
  Przedmiot *p;
  int c;
  if(kto!=&gracz) return 0; // phi
  pisz_ekw();
  printw("ktorego przedmiotu uzyc?");
  refresh();
  c=getchar();
  rysuj();
  c-='a';
  if(c<0) return 0;
  for(p=kto->ekwipunek;p && c>0;p=p->nast) c--;
  if(p)
    {
      uzyj(kto,p);
      return KOSZT_UZYJ;
    }    
  return 0;
}

int a_atak(Stwor *s,int x,int y)
{
  Stwor *of=mapa[s->poz.x+x][s->poz.y+y].stwor;
  double a,b,th;
  int d,i;
  a=s->stat[SI]*s->stat[KB];
  b=of->stat[SI]*of->stat[KP];
  th=a/(a+b)*10;
  if(th>(double)losuj(0,9))
    {
      d=a/losuj(1,3); // 3slabo 2styka 1mocno
      if(s->sloty[BRON]) s->sloty[BRON]->ns.zuzycie--;
      i=losuj(PANCERZ,TARCZA);
      if(of->sloty[i]) of->sloty[i]->ns.zuzycie--;
      if(d<1)d=1;
      of->stat[FZ]-=d;
      if(s==&gracz) kom_zadales_cios(of->nazwa,d);
      else if(of==&gracz) kom_otrzymales_cios(s->nazwa,d);
      //      else dodaj_komunikat("uslyszales straszliwy ryk.");
      if(of->stat[FZ]<0)
	{
	  if(s==&gracz)
	    {
	      kom_zabiles(of->nazwa);
	      gracz.dsw+=of->dsw;
	      zabij_stwora(of);
	      sprawdz_doswiadczenie();
	    }
	  else if(of==&gracz)
	    {
	      kom_zabil_cie(s->nazwa);
	      smierc();
	    }
	}
    }
  else
    {
      if(s==&gracz) kom_chybiles(of->nazwa);
      else if(of==&gracz) kom_stwor_chybil(s->nazwa);
    }
  return KOSZT_ATAK;
}

int a_rzuc(Stwor *kto)
{
  int c;
  Przedmiot *co;
  if(!kto->ekwipunek)
    {
      if(kto==&gracz) dodaj_komunikat("nie masz czym rzucac.");
      return 0;
    }
  if(kto==&gracz)
    {
      pisz_ekw();
      printw("ktorym przedmiotem rzucic?");
      refresh();
      c=getchar();
      rysuj();
      c-='a';
      if(c<0) return 0;
      for(co=kto->ekwipunek;co && c>0;co=co->nast) c--;
    }
  else co=kto->ekwipunek;
  if(co)
    {
      Poz d;
      for(c=0;c<MAX_SLOT;c++) if(kto->sloty[c]==co)
	{
	  if(kto==&gracz) dodaj_komunikat("uzywasz tego!");
	  return 0;
	}
      if(kto==&gracz)
	{
	  dodaj_komunikat("rzuc: wybierz kierunek...");
	  rysuj();
	  d=wez_kierunek(getchar());
	}
      else
	{
	  d.x=losuj(-1,1);
	  d.y=losuj(-1,1);
	  if(d.x==0 && d.y==0)d.x=-1; // na lewo!	  
	}
      rzuc(kto,co,kto->stat[SI]+losuj(-1,1),d.x,d.y);
      return KOSZT_RZUC;
    }
  return 0;
}
