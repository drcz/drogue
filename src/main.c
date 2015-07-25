#include"drogue.h"
#include<signal.h>

void inicjuj_curses();
void zakoncz_curses();

void przeskok(int s)
{
  // czasem to nie jest potrzebne te signalowania TUTEJ:
  signal(SIGINT,przeskok);
  signal(SIGSEGV,przeskok);
  signal(SIGILL,przeskok);
  signal(SIGQUIT,przeskok);
  signal(SIGABRT,przeskok);
  clear();
  mand();
  refresh();
  dodaj_komunikat("wuuuaaaaooo!!! co to bylo?");
  dodaj_komunikat("kosmos sie rozprul na chwilke...");
  longjmp(___Kosmos,1);
}

int main(int ac,char **av)
{
  Stwor *s,*s2;
  Pocisk *p,*p2;

  // nie powinno miec miejsca ale i tak
  signal(SIGINT,przeskok);
  signal(SIGSEGV,przeskok);
  signal(SIGILL,przeskok);
  signal(SIGQUIT,przeskok);
  signal(SIGABRT,przeskok);

  inicjuj_curses();
  inicjuj_losuj();
  tasuj_kolory_pigulek();
  inicjuj_komunikaty();
  g_poziom=0;
  lista_potworow=NULL;
  lista_pociskow=NULL;
  inicjuj_mape();
  tytulowa();
  setjmp(___Kosmos);
  nowa_plansza();
  widz();
  rysuj();
  for(;;)
    {
      cykl_stwora(&gracz);
      for(p=lista_pociskow;p;p=p2) {p2=p->nast;cykl_pocisku(p);}
      for(s=lista_potworow;s;s=s2) {s2=s->nast;cykl_stwora(s);}
    }
  koniec(0);
  return 0; /* notreached */
}

void inicjuj_curses()
{
  initscr();
  nonl();
  cbreak();
  noecho();
  start_color();
  init_pair(CZERN,    COLOR_BLACK,COLOR_BLACK);
  init_pair(BIEL,     COLOR_WHITE,COLOR_BLACK);
  init_pair(ZOLC,     COLOR_YELLOW,COLOR_BLACK);
  init_pair(KARMAZYN, COLOR_RED,COLOR_BLACK);
  init_pair(ZIELEN,   COLOR_GREEN,COLOR_BLACK);
  init_pair(CYJAN,    COLOR_CYAN,COLOR_BLACK);
  init_pair(CYKLAMEN, COLOR_MAGENTA,COLOR_BLACK);
  init_pair(INDYGO,   COLOR_BLUE,COLOR_BLACK);
  clear();
}

void zakoncz_curses(){endwin();}


void koniec(int s)
{
  zakoncz_curses();
  printf("\n\n ***********************************\n");
  printf(" *          DROGUE 0.33            *\n");
  printf(" *          -----------            *\n");
  printf(" *  Najbardziej Niebezpieczna Gra  *\n");
  printf(" *       Dercza Stanislawa         *\n");
  printf(" *             * * *               *\n");
  printf(" * Gdansk Gdynia Stezyca 2006-2008 *\n");
  printf(" *                                 *\n");
  printf(" ***********************************\n\n");
  printf("   pietro:%d\n",g_poziom);
  printf("   poziom wtajemniczenia:%d\n",gracz.p_wtaj);
  printf("   nideryt:%d\n\n",gracz.nideryt);
  printf("   - miej oczy z tylu glowy! -\n\n");
  exit(s);
}
