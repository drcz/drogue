#include"drogue.h"

typedef struct Komunikat Komunikat;
struct Komunikat
{
  char tresc[79];
  Komunikat *nastepny;
} *pierwszy;


void inicjuj_komunikaty() /* koniecznie wywolac w main */
{
  pierwszy=NULL;
}

void czysc_komunikaty()
{
  Komunikat *c,*d;
  for(c=pierwszy;c!=NULL;)
    {
      d=c->nastepny;
      free(c);
      c=d;
    }
  pierwszy=NULL;
}

void wypisz_komunikaty()
{
  Komunikat *c;
  int w=0;

  move(0,0);
  printw("                                                                            ");
  for(c=pierwszy;c!=NULL;c=c->nastepny)
    {
      move(w++,0);
      attron(COLOR_PAIR(BIEL));
      printw(c->tresc);
    }
  czysc_komunikaty();
  if(w>1)
    {
      move(w,0);
      printw("-spacja-"); // spacya?
      refresh();
      while(getchar()!=' ') ;
      rysuj();
    }
}

void dodaj_komunikat(char *t)
{
  Komunikat *c,*k=(Komunikat *)malloc(sizeof(Komunikat));
  if(k)
    {
      sprintf(k->tresc,t);
      k->nastepny=NULL;
      if((c=pierwszy))
	{
	  for(;c->nastepny!=NULL;c=c->nastepny) ;
	  c->nastepny=k;
	}
      else pierwszy=k;
    }
}

void kom_otworzyles_wlaz()
{
  switch(losuj(1,3))
    {
    case 1: dodaj_komunikat("wlaz otworzyl sie z hukiem."); break;
    case 2: dodaj_komunikat("otworzyles stalowy wlaz."); break;
    case 3: dodaj_komunikat("wlaz stoi otworem."); break;
    }
}

void kom_zamknales_wlaz()
{
  switch(losuj(1,2))
    {
    case 1: dodaj_komunikat("wlaz sie zatrzasnal."); break;
    case 2: dodaj_komunikat("zatrzasnales stalowy wlaz."); break;
    }
}

void kom_nizszy_poziom()
{
  switch(losuj(1,2))
    {
    case 1: dodaj_komunikat("zsunales sie na nizszy poziom."); break;
    case 2: dodaj_komunikat("zszedles na nizszy poziom."); break;
    }
}

void kom_przeniesienie()
{
  switch(losuj(1,3))
    {
    case 1: dodaj_komunikat("orety, zmieniles kontekst!"); break;
    case 2: dodaj_komunikat("ulegles przeniesieniu."); break;
    case 3: dodaj_komunikat("wszystko wokol sie tak szybko zmienilo!"); break;
    }
}

void kom_wtajemn(int p)
{
  char kk[79];
  sprintf(kk,"osiagnales %d poziom wtajemniczenia w najbardziej niebezpieczna gre.",p);
  dodaj_komunikat(kk);
}

void kom_cos_lezy(Nazwa n)
{
  char kk[79];
  sprintf(kk,"(tu lez%c %s)",(n.mnoga?'a':'y'),n.ktoco);
  dodaj_komunikat(kk);
}

void kom_podniosles(Nazwa n)
{
  char kk[79];
  switch(losuj(1,2))
    {
    case 1: sprintf(kk,"podniosles %s.",n.kogoco); break;
    default: sprintf(kk,"wziales %s.",n.kogoco);
    }
  dodaj_komunikat(kk);
}

void kom_nie_ma_co_podniesc()
{
  switch(losuj(1,2))
    {
    case 1: dodaj_komunikat("ale ze niby co podniesc?"); break;
    default: dodaj_komunikat("tu nic nie lezy.");
    }
}

void kom_wyrzuciles(Nazwa n)
{
  char kk[69];
  switch(losuj(1,2))
    {
    case 1: sprintf(kk,"wyrzuciles %s.",n.kogoco); break;
    case 2: sprintf(kk,"pozbyles sie %s.",n.kogoczego); break;
    }
  dodaj_komunikat(kk);  
}

void kom_nie_mozesz_wyrzucic(Nazwa n)
{
  char kk[79];
  switch(losuj(1,3))
  {
  case 1: sprintf(kk,"nie mozesz wyrzucic %s.",n.kogoczego);break;
  case 2: sprintf(kk,"ale przeciez uzywasz %s!",n.kogoczego);break;
  default: sprintf(kk,"nie mozna wyrzucac przedmiotow bedacych w uzyciu!");
  }
  dodaj_komunikat(kk);
}

void kom_zaczales_uzywac(Nazwa n)
{
  char kk[79];
  sprintf(kk,"zaczales uzywac %s.",n.kogoczego);
  dodaj_komunikat(kk);
}

void kom_przestales_uzywac(Nazwa n)
{
  char kk[79];
  sprintf(kk,"przestales uzywac %s.",n.kogoczego);
  dodaj_komunikat(kk);
}

void kom_juz_uzywasz(Nazwa a,Nazwa b)
{
  char kk[79];
  sprintf(kk,"nie mozesz, bo juz uzywasz %s.",b.kogoczego);
  dodaj_komunikat(kk);
}

void kom_zuzylo_sie(Przedmiot *p)
{
  char kk[79];
  switch(p->typ)
    {
    case BRON: sprintf(kk,"twoja bron sie rozleciala!"); break;
    case PANCERZ: dodaj_komunikat("twoj pancerz sie rozlecial!"); break;
    case HELM: dodaj_komunikat("twoj pancerz sie rozlecial!"); break;
    case REKAWICE: dodaj_komunikat("twoj pancerz sie rozlecial!"); break;
    case RAJTUZY: dodaj_komunikat("twoje rajtuzy poszly!"); break;
    case TARCZA: dodaj_komunikat("twoja tarcza sie rozleciala!"); break;
    case SWIATLO: dodaj_komunikat("twoje zrodlo swiatla zgaslo!"); break;
    case TORBA: dodaj_komunikat("twoja torba sie rozleciala!"); break;

    default: sprintf(kk,"cos ci sie tam rozwalilo...");
    }
}

void kom_zadales_cios(Nazwa n,int d)
{
  char kk[79];
  switch(losuj(1,9))
    {
    case 1: sprintf(kk,"zaatakowales %s (-%dfz).",n.kogoco,d); break;
    case 2: sprintf(kk,"jebnales %s (-%dfz).",n.kogoco,d); break;
    case 3: sprintf(kk,"zadales cios (-%dfz).",d); break;
    case 4: 
      switch(n.zenska*2+n.mnoga)
	{
	case 2*NIE+NIE: sprintf(kk,"%s oberwal! (-%dfz)",n.ktoco,d);break;
	case 2*TAK+NIE: sprintf(kk,"%s oberwala! (-%dfz)",n.ktoco,d);break;
	case 2*NIE+TAK: sprintf(kk,"%s oberwali! (-%dfz)",n.ktoco,d);break;
	case 2*TAK+TAK: sprintf(kk,"%s oberwaly! (-%dfz)",n.ktoco,d);break;
	}
      break;
    case 5:
    switch(n.zenska*2+n.mnoga)
      {
      case 2*NIE+NIE:sprintf(kk,"%s dostal w miche! (-%dfz)",n.ktoco,d);break;
      case 2*TAK+NIE:sprintf(kk,"%s dostala w miche! (-%dfz)",n.ktoco,d);break;
      case 2*NIE+TAK:sprintf(kk,"%s dostali w miche! (-%dfz)",n.ktoco,d);break;
      case 2*TAK+TAK:sprintf(kk,"%s dostaly w miche! (-%dfz)",n.ktoco,d);break;
      }
    break;
    case 6:
    switch(n.zenska*2+n.mnoga)
      {
      case 2*NIE+NIE:sprintf(kk,"%s wylapal bencki! (-%dfz)",n.ktoco,d);break;
      case 2*TAK+NIE:sprintf(kk,"%s wylapala bencki! (-%dfz)",n.ktoco,d);break;
      case 2*NIE+TAK:sprintf(kk,"%s wylapali bencki! (-%dfz)",n.ktoco,d);break;
      case 2*TAK+TAK:sprintf(kk,"%s wylapaly bencki! (-%dfz)",n.ktoco,d);break;
      }
    break;
    case 7: sprintf(kk,"zadales cios (-%dfz).",d); break;
    default: sprintf(kk,"uderzyles %s (-%dfz).",n.kogoco,d); break;
    }
  dodaj_komunikat(kk);
}

void kom_otrzymales_cios(Nazwa n,int d)
{
  char kk[79];
  if(n.zenska==NIE)
    {
      switch(losuj(1,7))
	{
	case 1: sprintf(kk,"jebnal cie %s (-%dfz)",n.ktoco,d);
	case 2: sprintf(kk,"%s zadal cios (-%dfz)",n.ktoco,d);
	case 3: sprintf(kk,"zostales zaatakowany przez %s (-%dfz)",n.kogoco,d);
	case 4: sprintf(kk,"%s zaatakowal (-%dfz)",n.ktoco,d);
	case 5: sprintf(kk,"wylapales bencki od (-%dfz)",d);
	case 6: sprintf(kk,"dostales w miche (-%dfz)",d);
	case 7: sprintf(kk,"%s zaatakowal (-%dfz)",n.ktoco,d);
	}
    }
  else
    {
      switch(losuj(1,7))
	{
	case 1: sprintf(kk,"jebnela cie %s (-%dfz)",n.ktoco,d);
	case 2: sprintf(kk,"%s zadala cios (-%dfz)",n.ktoco,d);
	case 3: sprintf(kk,"zostales zaatakowany przez %s (-%dfz)",n.kogoco,d);
	case 4: sprintf(kk,"%s zaatakowala (-%dfz)",n.ktoco,d);
	case 5: sprintf(kk,"wylapales bencki od %s (-%dfz)",n.kogoczego,d);
	case 6: sprintf(kk,"dostales w miche (-%dfz)",d);
	case 7: sprintf(kk,"%s zaatakowala (-%dfz)",n.ktoco,d);
	}
    }
  dodaj_komunikat(kk);
}

void kom_zabil_cie(Nazwa n)
{
  char kk[79];
  sprintf(kk,"zostales zabity przez %s...",n.kogoco);
  dodaj_komunikat(kk);
}

void kom_zabiles(Nazwa n)
{
  char kk[79];
  switch(losuj(1,3))
    {
    case 1: sprintf(kk,"zabiles %s!",n.kogoco); break;
    case 2: if(n.zenska==NIE)
             sprintf(kk,"%s kopnal w kalendarz.",n.ktoco);
            else 
             sprintf(kk,"%s kopnela w kalendarz.",n.ktoco);
	break;
    case 3: sprintf(kk,"%s sie przekrecil%c...",n.ktoco,
		    (n.zenska==TAK)?'a':' '); break;
    }
  dodaj_komunikat(kk);
}

void kom_chybiles(Nazwa n)
{
  char kk[79];
  switch(losuj(1,5))
    {
    case 1: sprintf(kk,"chybiles!"); break;
    case 2: sprintf(kk,"%s zrobil%c unik!",n.ktoco,(n.zenska==TAK)?'a':' ');
      break;
    case 3: sprintf(kk,"%s sparowal%c twoj cios!",n.ktoco,(n.zenska==TAK)?'a':' ');
      break;
    case 4: sprintf(kk,"nie trafiles %s.",n.kogoczego);
    case 5: sprintf(kk,"chybiles.");
      break;
    }
  dodaj_komunikat(kk);
}

void kom_stwor_chybil(Nazwa n)
{
  char kk[79];
  switch(losuj(1,5))
    {
    case 1: sprintf(kk,"zrobiles unik."); break;
    case 2: sprintf(kk,"zrobiles unik!"); break;
    case 3: sprintf(kk,"sparowales cios %s!",n.kogoczego); break;
    case 4: sprintf(kk,"%s nie trafil%c.",n.ktoco,(n.zenska==TAK)?'a':' ');
     break;
    case 5: sprintf(kk,"%s chybil%c.",n.ktoco,(n.zenska==TAK)?'a':' ');
     break;
    }
  dodaj_komunikat(kk);
}


void pisz_ekw()
{
  Przedmiot *p=gracz.ekwipunek;
  int i=0;
  attron(COLOR_PAIR(BIEL));
  if(p==NULL)
    {
      move(1,0);
      printw("<nic ze soba nie niesiesz> ");
      move(2,0);
      return;
    }
  while(p)
    {
      move(i+1,0);
      printw("%c)",i+'a');
      attron(COLOR_PAIR(ZIELEN));
      printw("%s ",p->nazwa.ktoco);
      attron(COLOR_PAIR(BIEL));
      if(gracz.sloty[BRON]==p) printw("(uzywana bron)");
      if(gracz.sloty[PANCERZ]==p) printw("(zalozony pancerz)");
      if(gracz.sloty[HELM]==p) printw("(zalozony helm)");
      if(gracz.sloty[REKAWICE]==p) printw("(zalozone rekawice)");
      if(gracz.sloty[RAJTUZY]==p) printw("(zalozone rajtuzy)");
      if(gracz.sloty[TARCZA]==p) printw("(uzywana tarcza)");
      if(gracz.sloty[SWIATLO]==p) printw("(uzywane zrodlo swiatla)");
      if(gracz.sloty[TORBA]==p) printw("(uzywana torba)");
      if(p->typ==INNY && p->in.ilosc>1) printw("[%d]",p->in.ilosc);
      p=p->nast;
      i++;
    }
  move(i+1,0);
}

void ekwipunek()
{
  pisz_ekw();
  printw("-spacja-");
  refresh();
  while(getchar()!=' ') ;
  rysuj();
}
