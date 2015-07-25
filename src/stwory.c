#include "drogue.h"

/* sym nazwa poz stat{fz,fz_max,si,kb,kp,udz,r,sz,reg}
   *stan ekwip. sloty pa zdr obc dsw wtaj nideryt */
Stwor gracz={{'@',BIEL},{"ty","ciebie","siebie",NIE,NIE},{-1,-1},
             {10,10,6,1,1,9,4,6,5},NULL,NULL,
	     {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},0,0,0,0,0,0};

int dsw_skok(int p) /* kompletnie bez sensu */
{
  int i,w=1;
  for(i=1;i<p+4;i++)w*=2;
  return w;
}

void sprawdz_doswiadczenie()
{
  if(gracz.dsw>dsw_skok(gracz.p_wtaj))
    {
      gracz.p_wtaj++;
      gracz.stat[SI]+=5;
      gracz.stat[FZ_MAX]+=10;
      gracz.stat[FZ]=gracz.stat[FZ_MAX];
      rysuj();
      kom_wtajemn(gracz.p_wtaj);
    }
}

void sprawdz(Stwor *kto)
{
  if(kto->stat[FZ]==kto->stat[FZ_MAX]) return;
  if(kto->zdrowie<0)
    {
      kto->zdrowie=99;
      kto->stat[FZ]++;
    }
  if(kto->stat[FZ]<1)
    {
      if(kto==&gracz) smierc();
      else zabij_stwora(kto);
    }
}

void cykl_stwora(Stwor *s)
{
  int i;

  if(s->stat[SZYBKOSC]>0) s->pa-=s->stat[SZYBKOSC];
  else s->pa--;
  if(s->pa<=0)
    {
      if(s==&gracz)
	{
	  widz();
	  rysuj();
	  gracz.pa=0;
	  gracz.pa+=wejscie(); /* uwaga! niektore akcje same zmieniaja
				  pa podmiotu wtedy wejscie() zwraca 0. */
	}
      else decydent(s);
    }
  if(s->zdrowie>0)s->zdrowie-=s->stat[REGENERACJA];
  sprawdz_stany(s);
  sprawdz(s);
  if(s->sloty[SWIATLO]) s->sloty[SWIATLO]->ns.zuzycie--;
  if(s->sloty[TORBA]) s->sloty[TORBA]->ns.zuzycie--;
  for(i=0;i<MAX_SLOT;i++)
    if(s->sloty[i] && s->sloty[i]->ns.zuzycie<1)
      {
	if(s==&gracz)kom_zuzylo_sie(s->sloty[i]);
	usun_z_ekwipunku(s,s->sloty[i]);
	free(s->sloty[i]);
	s->sloty[i]=NULL;
      }
}

void dowymysl_potwora(Stwor *p)
{
  int i,j;
  i=losuj(1,g_poziom*4);
  if(i>40)i=40;
  j=i/5;
  p->stat[FZ_MAX]=p->stat[FZ]=7+losuj(0,3)+j;
  p->stat[KB]=p->stat[KP]=j+1;
  p->stat[UDZWIG]=p->stat[SI]=j+1;
  p->stat[SZYBKOSC]=p->stat[REGENERACJA]=3;
  if(losuj(0,1))p->stat[SZYBKOSC]+=3;
  if(g_poziom>3)
    for(j=0;j<MAX_STAT;j++)if(losuj(0,1))p->stat[j]++;
  if(g_poziom>6)
    for(j=0;j<MAX_STAT;j++)if(losuj(0,1))p->stat[j]++;
  if(g_poziom>9)
    for(j=0;j<MAX_STAT;j++)if(losuj(0,1))p->stat[j]++;
  if(g_poziom>12)
    for(j=0;j<MAX_STAT;j++)if(losuj(0,1))p->stat[j]++;
  if(g_poziom>16)
    for(j=0;j<MAX_STAT;j++)if(losuj(0,1))p->stat[j]+=2;
  switch(i)
    {
    case 1:
      p->sym.znak=';';
      sprintf(p->nazwa.ktoco,"pluskwa");
      sprintf(p->nazwa.kogoco,"pluskwe");
      sprintf(p->nazwa.kogoczego,"pluskwy");
      p->nazwa.zenska=TAK; p->nazwa.mnoga=NIE;
      break;
    case 2:
      p->sym.znak='r';
      sprintf(p->nazwa.ktoco,"straszny szczur");
      sprintf(p->nazwa.kogoco,"strasznego szczura");
      sprintf(p->nazwa.kogoczego,"strasznego szczura");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 3:
      p->sym.znak='n';
      sprintf(p->nazwa.ktoco,"netoper");
      sprintf(p->nazwa.kogoco,"netopera");
      sprintf(p->nazwa.kogoczego,"netopera");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 4:
      p->sym.znak='p';
      sprintf(p->nazwa.ktoco,"plastikowy pajak");
      sprintf(p->nazwa.kogoco,"plastikowego pajaka");
      sprintf(p->nazwa.kogoczego,"plastikowego pajaka");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 5:
      p->sym.znak='S';
      sprintf(p->nazwa.ktoco,"dwuglowy waz");
      sprintf(p->nazwa.kogoco,"dwuglowego weza");
      sprintf(p->nazwa.kogoczego,"dwuglowego weza");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 6:
      p->sym.znak='o';
      sprintf(p->nazwa.ktoco,"kula szpiegula");
      sprintf(p->nazwa.kogoco,"kule szpiegule");
      sprintf(p->nazwa.kogoczego,"kuli szpieguli");
      p->nazwa.zenska=TAK; p->nazwa.mnoga=NIE;
      break;
    case 7:
      p->sym.znak='r';
      sprintf(p->nazwa.ktoco,"wielki szczur");
      sprintf(p->nazwa.kogoco,"wielkiego szczura");
      sprintf(p->nazwa.kogoczego,"wielkiego szczura");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 8:
      p->sym.znak='a';
      sprintf(p->nazwa.ktoco,"stalowa mrowka");
      sprintf(p->nazwa.kogoco,"stalowa mrowke");
      sprintf(p->nazwa.kogoczego,"stalowej mrowki");
      p->nazwa.zenska=TAK; p->nazwa.mnoga=NIE;
      break;
    case 9:
      p->sym.znak='f';
      sprintf(p->nazwa.ktoco,"pierdzacy szynszyl");
      sprintf(p->nazwa.kogoco,"pierdzacego szynszyla");
      sprintf(p->nazwa.kogoczego,"pierdzacego szynszyla");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 10:
      p->sym.znak='p';
      sprintf(p->nazwa.ktoco,"pukan");
      sprintf(p->nazwa.kogoco,"pukana");
      sprintf(p->nazwa.kogoczego,"pukana");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 11:
      p->sym.znak='j';
      sprintf(p->nazwa.ktoco,"elektryczny jezozwierz");
      sprintf(p->nazwa.kogoco,"elektrycznego jezozwierza");
      sprintf(p->nazwa.kogoczego,"elektrycznego jezozwierza");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 12:
      p->sym.znak='k';
      sprintf(p->nazwa.ktoco,"kalowy ludek");
      sprintf(p->nazwa.kogoco,"kalowego ludka");
      sprintf(p->nazwa.kogoczego,"kalowego ludka");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 13:
      p->sym.znak='s';
      sprintf(p->nazwa.ktoco,"chodzacy szkielet");
      sprintf(p->nazwa.kogoco,"chodzacy szkielet");
      sprintf(p->nazwa.kogoczego,"chodzacego szkieletu");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 14:
      p->sym.znak='z';
      sprintf(p->nazwa.ktoco,"ozywiony zygacz");
      sprintf(p->nazwa.kogoco,"ozywionego zygacza");
      sprintf(p->nazwa.kogoczego,"ozywionego zygacza");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 15:
      p->sym.znak='P';
      sprintf(p->nazwa.ktoco,"piesolak");
      sprintf(p->nazwa.kogoco,"piesolaka");
      sprintf(p->nazwa.kogoczego,"piesolaka");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 16:
      p->sym.znak='K';
      sprintf(p->nazwa.ktoco,"bluzgajacy karaluch");
      sprintf(p->nazwa.kogoco,"bluzgajacego karalucha");
      sprintf(p->nazwa.kogoczego,"bluzgajacego karalucha");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 17:
      p->sym.znak='K';
      sprintf(p->nazwa.ktoco,"kukla leborska");
      sprintf(p->nazwa.kogoco,"kukle leborska");
      sprintf(p->nazwa.kogoczego,"kukly leborskiej");
      p->nazwa.zenska=TAK; p->nazwa.mnoga=NIE;
      break;
    case 18:
      p->sym.znak='N';
      sprintf(p->nazwa.ktoco,"wielka niedzwiedzica");
      sprintf(p->nazwa.kogoco,"wielka niedzwiedzice");
      sprintf(p->nazwa.kogoczego,"wielkiej niedzwiedzicy");
      p->nazwa.zenska=TAK; p->nazwa.mnoga=NIE;
      break;
    case 19:
      p->sym.znak='Z';
      sprintf(p->nazwa.ktoco,"elektryczna zjawa");
      sprintf(p->nazwa.kogoco,"elektryczna zjawe");
      sprintf(p->nazwa.kogoczego,"elektrycznej zjawy");
      p->nazwa.zenska=TAK; p->nazwa.mnoga=NIE;
      break;
    case 20:
      p->sym.znak='b';
      sprintf(p->nazwa.ktoco,"bobrolak");
      sprintf(p->nazwa.kogoco,"bobrolaka");
      sprintf(p->nazwa.kogoczego,"bobrolaka");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 21:
      p->sym.znak='w';
      sprintf(p->nazwa.ktoco,"swietlisty wilk");
      sprintf(p->nazwa.kogoco,"swietlistego wilka");
      sprintf(p->nazwa.kogoczego,"swietlistego wilka");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 22:
      p->sym.znak='d';
      sprintf(p->nazwa.ktoco,"pies dingo");
      sprintf(p->nazwa.kogoco,"psa dingo");
      sprintf(p->nazwa.kogoczego,"psa dingo");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 23:
      p->sym.znak='L';
      sprintf(p->nazwa.ktoco,"lew");
      sprintf(p->nazwa.kogoco,"lwa");
      sprintf(p->nazwa.kogoczego,"lwa");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 24:
      p->sym.znak='c';
      sprintf(p->nazwa.ktoco,"chowaniec");
      sprintf(p->nazwa.kogoco,"chowanca");
      sprintf(p->nazwa.kogoczego,"chowanca");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 25:
      p->sym.znak='A';
      sprintf(p->nazwa.ktoco,"diahel analny");
      sprintf(p->nazwa.kogoco,"diahla analnego");
      sprintf(p->nazwa.kogoczego,"diahla analnego");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 26:
      p->sym.znak='p';
      sprintf(p->nazwa.ktoco,"pszczol krolowa");
      sprintf(p->nazwa.kogoco,"pszczol krolowa");
      sprintf(p->nazwa.kogoczego,"pszczol krolowej");
      p->nazwa.zenska=TAK; p->nazwa.mnoga=NIE;
      break;
    case 27:
      p->sym.znak='m';
      sprintf(p->nazwa.ktoco,"atomowy motyl");
      sprintf(p->nazwa.kogoco,"atomowego motyla");
      sprintf(p->nazwa.kogoczego,"atomowego motyla");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 28:
      p->sym.znak='O';
      sprintf(p->nazwa.ktoco,"mechaniczny owad");
      sprintf(p->nazwa.kogoco,"mechanicznego owada");
      sprintf(p->nazwa.kogoczego,"mechanicznego owada");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 29:
      p->sym.znak='E';
      sprintf(p->nazwa.ktoco,"eteryczny szescian");
      sprintf(p->nazwa.kogoco,"eterycznego szczescianu");
      sprintf(p->nazwa.kogoczego,"eterycznego szescianu");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 30:
      p->sym.znak='H';
      sprintf(p->nazwa.ktoco,"hydra");
      sprintf(p->nazwa.kogoco,"hydre");
      sprintf(p->nazwa.kogoczego,"hydry");
      p->nazwa.zenska=TAK; p->nazwa.mnoga=NIE;
      break;
    case 31:
      p->sym.znak='w';
      sprintf(p->nazwa.ktoco,"wiroptyr");
      sprintf(p->nazwa.kogoco,"wiroptyra");
      sprintf(p->nazwa.kogoczego,"wiroptyra");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 32:
      p->sym.znak='w';
      sprintf(p->nazwa.ktoco,"wilkopirt");
      sprintf(p->nazwa.kogoco,"wilkopira");
      sprintf(p->nazwa.kogoczego,"wilkopira");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 33:
      p->sym.znak='Q';
      sprintf(p->nazwa.ktoco,"pyrolak");
      sprintf(p->nazwa.kogoco,"pyrolaka");
      sprintf(p->nazwa.kogoczego,"pyrolaka");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 34:
      p->sym.znak='P';
      sprintf(p->nazwa.ktoco,"piesolak matriarchalny");
      sprintf(p->nazwa.kogoco,"piesolaka matriarchalnego");
      sprintf(p->nazwa.kogoczego,"piesolaka matriarchalnego");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 35:
      p->sym.znak='G';
      sprintf(p->nazwa.ktoco,"golem");
      sprintf(p->nazwa.kogoco,"golema");
      sprintf(p->nazwa.kogoczego,"golema");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 36:
      p->sym.znak='C';
      sprintf(p->nazwa.ktoco,"wielki czerw");
      sprintf(p->nazwa.kogoco,"wielkiego czerwia");
      sprintf(p->nazwa.kogoczego,"wielkiego czerwia");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 37:
      p->sym.znak='B';
      sprintf(p->nazwa.ktoco,"baczal bagienny");
      sprintf(p->nazwa.kogoco,"baczala bagiennego");
      sprintf(p->nazwa.kogoczego,"baczala bagiennego");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 38:
      p->sym.znak='u';
      sprintf(p->nazwa.ktoco,"umarlak");
      sprintf(p->nazwa.kogoco,"umarlaka");
      sprintf(p->nazwa.kogoczego,"umarlaka");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 39:
      p->sym.znak='R';
      sprintf(p->nazwa.ktoco,"nakrecany kret");
      sprintf(p->nazwa.kogoco,"nakrecanego kreta");
      sprintf(p->nazwa.kogoczego,"nakrecanego kreta");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    case 40:
      p->sym.znak='t';
      sprintf(p->nazwa.ktoco,"tygrys szablozeby");
      sprintf(p->nazwa.kogoco,"tygrysa szablozebego");
      sprintf(p->nazwa.kogoczego,"tygrysa szablozebego");
      p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;
      break;
    }
}

void stworz_potwora(int x,int y)
{
  Stwor *p=(Stwor *)malloc(sizeof(Stwor));
  int i;
  if(mapa[x][y].stwor) usun_stwora(mapa[x][y].stwor); // zawor
  if(p)
    {
      p->sym.znak='S'; p->sym.kolor=BIEL;
      sprintf(p->nazwa.ktoco,"straszny potwor");
      sprintf(p->nazwa.kogoco,"strasznego potwora");
      sprintf(p->nazwa.kogoczego,"strasznego potwora");
      p->nazwa.zenska=p->nazwa.mnoga=NIE;
      p->stany=NULL;
      p->ekwipunek=NULL;
      p->obciazenie=0;
      for(i=0;i<MAX_SLOT;i++) p->sloty[i]=NULL;
      for(i=0;i<MAX_STAT;i++) p->stat[i]=0;
      p->stat[FZ]=p->stat[FZ_MAX]=5;
      p->pa=p->zdrowie=0;
      if(losuj(0,2))p->nideryt=losuj(2,9);
      p->dsw=1; p->p_wtaj=997;  /* cii, to tajemnica. */
      dowymysl_potwora(p);
      p->poz.x=x; p->poz.y=y;
      p->nast=lista_potworow;
      lista_potworow=p;
      mapa[x][y].stwor=p;
    }
}

void usun_stwora(Stwor *s)
{
  Stan *st;
  Przedmiot *p;
  Stwor *stw=NULL;
  if(!s)return; // zawor bezpieczenstwa tez
  mapa[s->poz.x][s->poz.y].stwor=NULL;
  for(;s->ekwipunek;)
    {
      p=s->ekwipunek->nast;
      free(s->ekwipunek);
      s->ekwipunek=p;
    }
  for(;s->stany;)
    {
      st=s->stany->nast;
      free(s->stany);
      s->stany=st;
    }
  if(s==&gracz) return;
  if(s==lista_potworow) lista_potworow=s->nast;
  else for(stw=lista_potworow;stw && stw->nast!=s;stw=stw->nast);
  if(stw) stw->nast=s->nast;
  free(s);
}

void zabij_stwora(Stwor *kto)
{
  Przedmiot *p;

  if(kto->nideryt>0 && mapa[kto->poz.x][kto->poz.y].t==PODLOGA)
    mapa[kto->poz.x][kto->poz.y].t=NIDERYT; 
  // no powaznie! nideryt tylko na posadzce sie pojawia.
  // bo ona jest sliska, kumasz?
  if((p=kto->ekwipunek))
    {
      dodaj_przedmiot(kto->poz.x,kto->poz.y,p);
      usun_z_ekwipunku(kto,p);  
    }
  usun_stwora(kto);
}
