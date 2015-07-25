#include "drogue.h"

void niedzialanie(Stwor *taa){}

#define MAX_PIGULKI 8
void dp_sily(Stwor *);
void dp_oslabienia(Stwor *);
void dp_slepoty(Stwor *);
void dp_widzenia(Stwor *);
void dp_regeneracji(Stwor *);
void dp_szybkosci(Stwor *);
void dp_nasennej(Stwor *);
void dp_dezorientacji(Stwor *);

char kolory_pigulek[MAX_PIGULKI][23]=
  {
    "zielon",
    "czerwon",
    "pomaranczow",
    "granatow",
    "fioletow",
    "bial",
    "zolt",
    "bezow"};

void tasuj_kolory_pigulek()
{
  char t[23];
  int i,j,k;
  for(k=0;k<6;k++) // tasuj 6 razy
    {
      i=losuj(1,MAX_PIGULKI)-1;
      j=losuj(1,MAX_PIGULKI)-1;
      // tak, moze sie stac ze i==j.
      sprintf(t,"%s",kolory_pigulek[i]);
      sprintf(kolory_pigulek[i],"%s",kolory_pigulek[j]);
      sprintf(kolory_pigulek[j],"%s",t);
    }
}

void (*dzialania_pigulek[MAX_PIGULKI])(Stwor *kto) =
{
 dp_sily,dp_oslabienia,dp_slepoty,dp_widzenia,
 dp_regeneracji,dp_szybkosci,dp_nasennej,dp_dezorientacji
};

void trnic(Stwor *sciema){}

void dodaj_przedmiot(int,int,Przedmiot *);
Przedmiot *wymysl_przedmiot();
void podnies(Stwor *);
void wyrzuc(Stwor *,Przedmiot *);

void dodaj_przedmiot(int x,int y,Przedmiot *p)
{
  // najpierw chyba nie calkiem elegancki zawor bezpieczenstwa.
  if(mapa[x][y].przedmiot) free(mapa[x][y].przedmiot); // nono.
  mapa[x][y].przedmiot=p;
  widz();
}

Przedmiot *klonuj_przedmiot(Przedmiot *p)
{
  Przedmiot *q=(Przedmiot *)malloc(sizeof(Przedmiot));
  int i;
  if(!q) return NULL;
  q->sym=p->sym;
  q->nazwa=p->nazwa;
  q->typ=p->typ;
  q->waga=p->waga;
  if(q->typ<INNY)
    {
      q->ns.zuzycie=p->ns.zuzycie;
      for(i=0;i<MAX_STAT;i++)
	q->ns.mod[i]=p->ns.mod[i];
    }
  else
    {
      q->in.ilosc=1; // uwaga! klonuje tylko 1 sztuke.
      q->in.dzialanie=p->in.dzialanie;
      q->in.odkryty=p->in.odkryty;
    }
  q->trafiony=p->trafiony;
  q->nast=p->nast;
  q->poprz=p->poprz;
  return q;
}

void podnies(Stwor *kto)
{
  Przedmiot *p=mapa[kto->poz.x][kto->poz.y].przedmiot;
  mapa[kto->poz.x][kto->poz.y].przedmiot=NULL;
  if(p->typ==INNY)
    {
      Przedmiot *q;
      for(q=kto->ekwipunek;q;q=q->nast)
	if(q->typ==INNY && q->in.dzialanie==p->in.dzialanie)
	  {
	    /* "jesli dziala tak samo to jest tym samym"
	       moze byc zgubne, trzeba uwazac na to! */
	    q->in.ilosc+=p->in.ilosc;
	    free(p); // usuwasiego!
	    return;
	  }
    }
  p->nast=kto->ekwipunek;
  if(kto->ekwipunek) kto->ekwipunek->poprz=p;
  kto->ekwipunek=p;
}

void usun_z_ekwipunku(Stwor *czyjego,Przedmiot *co)
{
  Przedmiot *p,*q;
  if(!co) return;
  p=co->poprz; q=co->nast;
  if(p) p->nast=q;
   else czyjego->ekwipunek=q;
  if(q) q->poprz=p;
  co->poprz=NULL;
  co->nast=NULL;  
}

void wyrzuc(Stwor *kto,Przedmiot *co)
{
  if(mapa[kto->poz.x][kto->poz.y].przedmiot) return;
  if(kto->sloty[co->typ]==co)
    {
      if(kto==&gracz) kom_nie_mozesz_wyrzucic(co->nazwa);
      return;
    }
  if(co->typ==INNY && co->in.ilosc>1)
    {
      co->in.ilosc--;
      dodaj_przedmiot(kto->poz.x,kto->poz.y,klonuj_przedmiot(co));      
    }
  else
    {
      usun_z_ekwipunku(kto,co);
      dodaj_przedmiot(kto->poz.x,kto->poz.y,co);
    }

  if(kto==&gracz)kom_wyrzuciles(co->nazwa);
}

void zaloz(Stwor *kto,Przedmiot *co)
{
  int i;
  kto->sloty[co->typ]=co;
  for(i=0;i<MAX_STAT;i++)kto->stat[i]+=co->ns.mod[i];
  if(kto==&gracz) kom_zaczales_uzywac(co->nazwa);
  // else jesli w polu widzenia: [kto] zalozyl [co] .
}
 
void zdejmij(Stwor *kto,Przedmiot *co)
{
  int i;
  kto->sloty[co->typ]=NULL;
  for(i=0;i<MAX_STAT;i++)kto->stat[i]-=co->ns.mod[i];
  if(kto==&gracz) kom_przestales_uzywac(co->nazwa);
  // else jesli w polu widzenia: [kto] zdjal [co] .  
}

void uzyj(Stwor *kto,Przedmiot *czego)
{
  if(czego->typ<INNY)
    {
      if(kto->sloty[czego->typ]==czego) zdejmij(kto,czego);
      else if(kto->sloty[czego->typ]==NULL) zaloz(kto,czego);
      else kom_juz_uzywasz(czego->nazwa,kto->sloty[czego->typ]->nazwa);
    }
  else
    {
      kto->obciazenie-=czego->waga;
      czego->in.dzialanie(kto);
      if(czego->in.ilosc>1) czego->in.ilosc--;
      else usun_z_ekwipunku(kto,czego);
    }
  
}

/* tu sie zaczyna zmyslacz rzeczy: **************************/

Przedmiot *wymysl_pigulke()
{
  int a;
  Przedmiot *p=(Przedmiot *)malloc(sizeof(Przedmiot));
  if(p)
    {
      p->sym.znak='!';
      p->sym.kolor=ZIELEN;
      p->nast=p->poprz=NULL;
      p->waga=1;
      p->typ=INNY;
      p->in.ilosc=1;
      a=losuj(1,MAX_PIGULKI)-1;
      p->in.dzialanie=dzialania_pigulek[a];
      sprintf(p->nazwa.ktoco,"%sa pigulka",kolory_pigulek[a]);
      sprintf(p->nazwa.kogoco,"%sa pigulke",kolory_pigulek[a]);
      sprintf(p->nazwa.kogoczego,"%sej pigulki",kolory_pigulek[a]);
      p->nazwa.zenska=TAK; p->nazwa.mnoga=NIE;
    }
  return p;
}

Przedmiot *wymysl_totem()
{
  Przedmiot *p=(Przedmiot *)malloc(sizeof(Przedmiot));
  if(p)
    {
      p->sym.znak='?';
      p->sym.kolor=ZIELEN;
      p->nast=p->poprz=NULL;
      p->waga=1;
      p->typ=INNY;
      p->in.ilosc=1;
      p->in.dzialanie=niedzialanie;
      p->nast=p->poprz=NULL;
      switch(losuj(1,5))
	{
	case 1:
	  sprintf(p->nazwa.ktoco,"moneta z dziurka");
	  sprintf(p->nazwa.kogoco,"monete z dziurka");
	  sprintf(p->nazwa.kogoczego,"monety z dziurka");
	  p->nazwa.zenska=TAK;p->nazwa.mnoga=NIE;
	  break;
	case 2:
	  sprintf(p->nazwa.ktoco,"szmaciana kukielka");
	  sprintf(p->nazwa.kogoco,"szmaciana kukielke");
	  sprintf(p->nazwa.kogoczego,"szmacianej kukielki");
	  p->nazwa.zenska=TAK;p->nazwa.mnoga=NIE;
	  break;
	case 3:
	  sprintf(p->nazwa.ktoco,"kamien-wiezienie z Bieszczad");
	  sprintf(p->nazwa.kogoco,"kamien-wiezienie z Bieszczad");
	  sprintf(p->nazwa.kogoczego,"kamienia-wiezienia z Bieszczad");
	  p->nazwa.zenska=NIE;p->nazwa.mnoga=NIE;
	  break;

	default:
	  sprintf(p->nazwa.ktoco,"srubka");
	  sprintf(p->nazwa.kogoco,"srubke");
	  sprintf(p->nazwa.kogoczego,"srubki");
	  p->nazwa.zenska=TAK;p->nazwa.mnoga=NIE;
	}
    }
  return p;
}

void _zrob_bron(Przedmiot *p)
{
  int i;
  for(i=0;i<MAX_STAT;i++) p->ns.mod[i]=0;
  if(g_poziom>18) i=losuj(1,24);
   else i=losuj(g_poziom/2,6+g_poziom);
  if(i<1)i=1;
  if(i>24)i=24;
  p->waga=1;
  if(i>5)p->waga++;
  if(i>10)p->waga++;
  if(i>16)p->waga++;
  if(i==23)p->waga=1;
  switch(i)
    {
    case 1:
      sprintf(p->nazwa.ktoco,"pret metalowy");
      sprintf(p->nazwa.kogoco,"pret metalowy");
      sprintf(p->nazwa.kogoczego,"preta metalowego");
      p->nazwa.zenska=NIE;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=1;
      break;
    case 2:
      sprintf(p->nazwa.ktoco,"sztylet");
      sprintf(p->nazwa.kogoco,"sztylet");
      sprintf(p->nazwa.kogoczego,"sztyletu");
      p->nazwa.zenska=NIE;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=3;
      break;
    case 3:
      sprintf(p->nazwa.ktoco,"gaz pieprzowy");
      sprintf(p->nazwa.kogoco,"gaz pieprzowy");
      sprintf(p->nazwa.kogoczego,"gazu pieprzowego");
      p->nazwa.zenska=NIE;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=2;
      break;
    case 4:
      sprintf(p->nazwa.ktoco,"ciupaga");
      sprintf(p->nazwa.kogoco,"ciupage");
      sprintf(p->nazwa.kogoczego,"ciupagi");
      p->nazwa.zenska=TAK;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=4;
      break;
    case 5:
      sprintf(p->nazwa.ktoco,"cep");
      sprintf(p->nazwa.kogoco,"cep");
      sprintf(p->nazwa.kogoczego,"cepa");
      p->nazwa.zenska=NIE;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=4;
      break;
    case 6:
      sprintf(p->nazwa.ktoco,"wlocznia");
      sprintf(p->nazwa.kogoco,"wlocznie");
      sprintf(p->nazwa.kogoczego,"wloczni");
      p->nazwa.zenska=TAK;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=5;
      break;
    case 7:
      sprintf(p->nazwa.ktoco,"maczeta");
      sprintf(p->nazwa.kogoco,"maczete");
      sprintf(p->nazwa.kogoczego,"maczety");
      p->nazwa.zenska=TAK;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=5;
      break;
    case 8:
      sprintf(p->nazwa.ktoco,"miecz");
      sprintf(p->nazwa.kogoco,"miecz");
      sprintf(p->nazwa.kogoczego,"miecza");
      p->nazwa.zenska=NIE;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=5;
      break;
    case 9:
      sprintf(p->nazwa.ktoco,"obuh");
      sprintf(p->nazwa.kogoco,"obuh");
      sprintf(p->nazwa.kogoczego,"obuha");
      p->nazwa.zenska=NIE;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=6;
      break;
    case 10:
      sprintf(p->nazwa.ktoco,"szabla");
      sprintf(p->nazwa.kogoco,"szable");
      sprintf(p->nazwa.kogoczego,"szabli");
      p->nazwa.zenska=TAK;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=5;
      break;
    case 11:
      sprintf(p->nazwa.ktoco,"halabarda");
      sprintf(p->nazwa.kogoco,"halabarde");
      sprintf(p->nazwa.kogoczego,"halabardy");
      p->nazwa.zenska=TAK;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=6;
      break;
    case 12:
      sprintf(p->nazwa.ktoco,"sekator");
      sprintf(p->nazwa.kogoco,"sekator");
      sprintf(p->nazwa.kogoczego,"sekatora");
      p->nazwa.zenska=NIE;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=4;
      break;
    case 13:
      sprintf(p->nazwa.ktoco,"maly topor");
      sprintf(p->nazwa.kogoco,"maly topor");
      sprintf(p->nazwa.kogoczego,"malego topora");
      p->nazwa.zenska=NIE;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=7;
      break;
    case 14:
      sprintf(p->nazwa.ktoco,"sejmitar");
      sprintf(p->nazwa.kogoco,"sejmitar");
      sprintf(p->nazwa.kogoczego,"sejmitara");
      p->nazwa.zenska=NIE;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=6;
      break;
    case 15:
      sprintf(p->nazwa.ktoco,"rapier");
      sprintf(p->nazwa.kogoco,"rapier");
      sprintf(p->nazwa.kogoczego,"rapiera");
      p->nazwa.zenska=NIE;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=6;
      break;
    case 16:
      sprintf(p->nazwa.ktoco,"paralizator");
      sprintf(p->nazwa.kogoco,"paralizator");
      sprintf(p->nazwa.kogoczego,"paralizatora");
      p->nazwa.zenska=NIE;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=7;
      break;
    case 17:
      sprintf(p->nazwa.ktoco,"gwiazda zaranna");
      sprintf(p->nazwa.kogoco,"gwiazde zaranna");
      sprintf(p->nazwa.kogoczego,"gwiazdy zarannej");
      p->nazwa.zenska=TAK;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=8;
      break;
    case 18:
      sprintf(p->nazwa.ktoco,"miecz obusieczny");
      sprintf(p->nazwa.kogoco,"miecz obusieczny");
      sprintf(p->nazwa.kogoczego,"miecza obusiecznego");
      p->nazwa.zenska=NIE;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=8;
      break;
    case 19:
      sprintf(p->nazwa.ktoco,"topor");
      sprintf(p->nazwa.kogoco,"topor");
      sprintf(p->nazwa.kogoczego,"topora");
      p->nazwa.zenska=NIE;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=9;
      break;
    case 20:
      sprintf(p->nazwa.ktoco,"trojzab");
      sprintf(p->nazwa.kogoco,"trojzab");
      sprintf(p->nazwa.kogoczego,"trojzeba");
      p->nazwa.zenska=NIE;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=9;
      break;
    case 21:
      sprintf(p->nazwa.ktoco,"miotacz ognia");
      sprintf(p->nazwa.kogoco,"miotacz ognia");
      sprintf(p->nazwa.kogoczego,"miotacza ognia");
      p->nazwa.zenska=NIE;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=11;
      break;
    case 22:
      sprintf(p->nazwa.ktoco,"pila lancuchowa");
      sprintf(p->nazwa.kogoco,"pile lancuchowa");
      sprintf(p->nazwa.kogoczego,"pily lancuchowej");
      p->nazwa.zenska=TAK;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=10;
      break;
    case 23:
      sprintf(p->nazwa.ktoco,"dezintegrator astralny");
      sprintf(p->nazwa.kogoco,"dezintegrator astralny");
      sprintf(p->nazwa.kogoczego,"dezintegratora astralnego");
      p->nazwa.zenska=NIE;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=23;
      break;
    case 24:
      sprintf(p->nazwa.ktoco,"spalinowa pila lancuchowa");
      sprintf(p->nazwa.kogoco,"spalinowa pile lancuchowa");
      sprintf(p->nazwa.kogoczego,"spalinowej pily lancuchowej");
      p->nazwa.zenska=TAK;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=12;
      break;
    }
}

void _zrob_pancerz(Przedmiot *p)
{
  int i;
  for(i=0;i<MAX_STAT;i++) p->ns.mod[i]=0;
  i=2;
  if(g_poziom>3)i=3;
  if(g_poziom>6)i=4;
  if(g_poziom>8)i=6;
  switch(losuj(1,i))
    {
    case 1:
      sprintf(p->nazwa.ktoco,"skorzana kurtka");
      sprintf(p->nazwa.kogoco,"skorzana kurtke");
      sprintf(p->nazwa.kogoczego,"skorzanej kurtki");
      p->nazwa.zenska=TAK;p->nazwa.mnoga=NIE;      
      p->ns.mod[KP]=2;
      break;
    case 2:
      sprintf(p->nazwa.ktoco,"kolczuga");
      sprintf(p->nazwa.kogoco,"kolczuge");
      sprintf(p->nazwa.kogoczego,"kolczugi");
      p->nazwa.zenska=TAK;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=6;
      break;
    case 3:
      sprintf(p->nazwa.ktoco,"kombinezon");
      sprintf(p->nazwa.kogoco,"kombinaezon");
      sprintf(p->nazwa.kogoczego,"kombinezonu");
      p->nazwa.zenska=NIE;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=4;
      break;
    case 4:
      sprintf(p->nazwa.ktoco,"skorzana zbroja");
      sprintf(p->nazwa.kogoco,"skorzana zbroje");
      sprintf(p->nazwa.kogoczego,"skorzanej zbroji");
      p->nazwa.zenska=TAK;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=7;
      break;
    case 5:
      sprintf(p->nazwa.ktoco,"stalowa zbroja");
      sprintf(p->nazwa.kogoco,"stalowa zbroje");
      sprintf(p->nazwa.kogoczego,"stalowej zbroji");
      p->nazwa.zenska=TAK;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=9;
      break;
    case 6:
      sprintf(p->nazwa.ktoco,"zbroja plytowa");
      sprintf(p->nazwa.kogoco,"zbroje plytowa");
      sprintf(p->nazwa.kogoczego,"zbroji plytowej");
      p->nazwa.zenska=TAK;p->nazwa.mnoga=NIE;      
      p->ns.mod[KB]=13;
      break;
    }
}

void _zrob_rekawice(Przedmiot *p)
{
  char jakie[15];
  int i;
  for(i=0;i<MAX_STAT;i++) p->ns.mod[i]=0;
  switch(losuj(1,3))
    {
    case 1: sprintf(jakie,"skorzan"); p->ns.mod[KP]=1; break;
    case 2: sprintf(jakie,"azbestow"); p->ns.mod[KP]=2; break;
    case 3: sprintf(jakie,"stalow"); p->ns.mod[KP]=3; break;
    }
  sprintf(p->nazwa.ktoco,"%se rekawice",jakie);
  sprintf(p->nazwa.kogoco,"%se rekawice",jakie);
  sprintf(p->nazwa.kogoczego,"%sych rekawic",jakie);
  p->nazwa.zenska=TAK; p->nazwa.mnoga=TAK;
}

void _zrob_rajtuzy(Przedmiot *p)
{
  char jakie[15];
  int i;
  for(i=0;i<MAX_STAT;i++) p->ns.mod[i]=0;
  switch(losuj(1,3))
    {
    case 1: sprintf(jakie,"skorzan"); p->ns.mod[KP]=1; break;
    case 2: sprintf(jakie,"azbestow"); p->ns.mod[KP]=2; break;
    case 3: sprintf(jakie,"stalow"); p->ns.mod[KP]=3; break;
    }
  sprintf(p->nazwa.ktoco,"%se rajtuzy",jakie);
  sprintf(p->nazwa.kogoco,"%se rajtuzy",jakie);
  sprintf(p->nazwa.kogoczego,"%sych rajtuz",jakie);
  p->nazwa.zenska=TAK; p->nazwa.mnoga=TAK;  
}

void _zrob_tarcze(Przedmiot *p)
{
  char jaka[11],jaka2[15];
  int i;
  for(i=0;i<MAX_STAT;i++) p->ns.mod[i]=0;
  i=2; if(g_poziom>6) i=3;
  switch(losuj(1,i))
    {
    case 1: sprintf(jaka,"mal"); break;
    case 2: sprintf(jaka,"sredni"); break;
    case 3: sprintf(jaka,"duz"); break;
    }
  i=2;
  if(g_poziom>6) i=3;
  if(g_poziom>9) i=5;
  switch(losuj(1,i))
    {
    case 1: sprintf(jaka2,"drewnian"); p->ns.mod[KP]+=2; break;
    case 2: sprintf(jaka2,"zelazn"); p->ns.mod[KP]+=3; break;
    case 3: sprintf(jaka2,"stalow"); p->ns.mod[KP]+=4; break;
    case 4: sprintf(jaka2,"tunteksow"); p->ns.mod[KP]+=6; break;
    case 5: sprintf(jaka2,"kosmiczn"); p->ns.mod[KP]+=9; break;
    }
  sprintf(p->nazwa.ktoco,"%sa %sa tarcza",jaka,jaka2);
  sprintf(p->nazwa.kogoco,"%sa %sa tarcze",jaka,jaka2);
  sprintf(p->nazwa.kogoczego,"%sej %sej tarczy",jaka,jaka2);
  p->nazwa.zenska=TAK; p->nazwa.mnoga=NIE;  
}

void _zrob_helm(Przedmiot *p)
{
  char jaki[10];
  int i;
  for(i=0;i<MAX_STAT;i++) p->ns.mod[i]=0;
  switch(losuj(1,3))
    {
    case 1: sprintf(jaki,"zelazn");p->ns.mod[KP]=2;break;
    case 2: sprintf(jaki,"stalow");p->ns.mod[KP]=3;break;
    case 3: sprintf(jaki,"tytanow");p->ns.mod[KP]=5;break;
    } 
  sprintf(p->nazwa.ktoco,"%sy helm",jaki);
  sprintf(p->nazwa.kogoco,"%sy helm",jaki);
  sprintf(p->nazwa.kogoczego,"%sgo helmu",jaki);
  p->nazwa.zenska=NIE; p->nazwa.mnoga=NIE;  
}

void _zrob_swiatlo(Przedmiot *p)
{
  int i;
  for(i=0;i<MAX_STAT;i++) p->ns.mod[i]=0;
  switch(losuj(1,3))
    {
    case 1:
      sprintf(p->nazwa.ktoco,"kaganek");
      sprintf(p->nazwa.kogoco,"kaganek");
      sprintf(p->nazwa.kogoczego,"kaganka");
      p->nazwa.zenska=NIE;p->nazwa.mnoga=NIE;      
      p->ns.mod[R_WIDZENIA]=2;
      break;
    case 2:
      sprintf(p->nazwa.ktoco,"pochodnia");
      sprintf(p->nazwa.kogoco,"pochodnie");
      sprintf(p->nazwa.kogoczego,"pochodni");
      p->nazwa.zenska=TAK;p->nazwa.mnoga=NIE;      
      p->ns.mod[R_WIDZENIA]=4;
      break;
    case 3:
      sprintf(p->nazwa.ktoco,"latarka");
      sprintf(p->nazwa.kogoco,"latarke");
      sprintf(p->nazwa.kogoczego,"latarki");
      p->nazwa.zenska=TAK;p->nazwa.mnoga=NIE;      
      p->ns.mod[R_WIDZENIA]=6;
      break;
    }
}

void _zrob_torbe(Przedmiot *p)
{
  char jaka[10];
  int i;
  for(i=0;i<MAX_STAT;i++) p->ns.mod[i]=0;
  switch(losuj(1,3))
    {
    case 1: sprintf(jaka,"mal");p->ns.mod[UDZWIG]=5;break;
    case 2: sprintf(jaka,"sredni");p->ns.mod[UDZWIG]=10;break;
    case 3: sprintf(jaka,"duz");p->ns.mod[UDZWIG]=15;break;
    } 
  sprintf(p->nazwa.ktoco,"%sa torba",jaka);
  sprintf(p->nazwa.kogoco,"%sa torbe",jaka);
  sprintf(p->nazwa.kogoczego,"%sej torby",jaka);
  p->nazwa.zenska=TAK; p->nazwa.mnoga=NIE;  
}


Przedmiot *wymysl_cos_pozytecznego()
{
  Przedmiot *p=(Przedmiot *)malloc(sizeof(Przedmiot));
  if(p)
    {
      p->sym.kolor=ZIELEN;
      p->nast=p->poprz=NULL;
      p->waga=1;
      p->typ=losuj(0,INNY-1);
      p->ns.zuzycie=losuj(33,333);
      if(p->typ==SWIATLO) p->ns.zuzycie+=50;
      switch(p->typ)
	{
	case BRON: p->sym.znak=']'; _zrob_bron(p); break;
	case PANCERZ: p->sym.znak='['; _zrob_pancerz(p); break;
	case HELM: p->sym.znak='('; _zrob_helm(p); break;
	case REKAWICE: p->sym.znak='('; _zrob_rekawice(p); break;
	case RAJTUZY: p->sym.znak='('; _zrob_rajtuzy(p); break;
	case TARCZA: p->sym.znak=')'; _zrob_tarcze(p); break;
	case SWIATLO: p->sym.znak='$'; _zrob_swiatlo(p); break;
	case TORBA: p->sym.znak='#'; _zrob_torbe(p); break;
	}
      p->nast=p->poprz=NULL;
    }
  return p;
}

Przedmiot *wymysl_przedmiot()
{
  switch(losuj(1,6))
    {
    case 1: return wymysl_pigulke();
    case 2:
    case 3: return wymysl_totem();
    default: return wymysl_cos_pozytecznego();
    }
}

void dp_sily(Stwor *kto)
{
  if(kto==&gracz) dodaj_komunikat("zjadasz pigulke...");
  dodaj_stan(kto,SILNIEJSZY,losuj(130,200));
}

void dp_oslabienia(Stwor *kto)
{
  if(kto==&gracz) dodaj_komunikat("zjadasz pigulke...");
  dodaj_stan(kto,SLABSZY,losuj(130,200));
}

void dp_slepoty(Stwor *kto)
{
  if(kto==&gracz) dodaj_komunikat("zjadasz pigulke...");
  dodaj_stan(kto,OSLEPIONY,losuj(50,100));
}

void dp_widzenia(Stwor *kto)
{
  if(kto==&gracz) dodaj_komunikat("zjadasz pigulke...");
  dodaj_stan(kto,DUZE_ZRENICE,losuj(130,200));
}

void dp_regeneracji(Stwor *kto)
{
  if(kto==&gracz) dodaj_komunikat("zjadasz pigulke...");
  dodaj_stan(kto,SILNIEJSZY,losuj(130,200));
}

void dp_szybkosci(Stwor *kto)
{
  if(kto==&gracz) dodaj_komunikat("zjadasz pigulke...");
  dodaj_stan(kto,SZYBKOZDROWIEJE,losuj(80,160));
}

void dp_nasennej(Stwor *kto)
{
  if(kto==&gracz) dodaj_komunikat("zjadasz pigulke...");
  dodaj_stan(kto,SILNIEJSZY,losuj(130,200));
}

void dp_dezorientacji(Stwor *kto)
{
  if(kto==&gracz) dodaj_komunikat("zjadasz pigulke...");
  dodaj_stan(kto,ZDEZORIENTOWANY,losuj(40,180));
}
