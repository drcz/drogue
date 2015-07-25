#include"drogue.h"

void rysuj()
{
  Stan *st;
  int i,j;

  for(j=0;j<WYS;j++)
    {
      move(j+1,0);
      for(i=0;i<SZE;i++)
	{
	  attron(COLOR_PAIR(mapa[i][j].sym.kolor));
	  printw("%c",mapa[i][j].sym.znak);
	}
    }
  attron(COLOR_PAIR(BIEL));
  wypisz_komunikaty();
  move(WYS+1,1);
  attron(COLOR_PAIR(BIEL));
  printw("poziom:%d   fz:%d/%d si:%d kb:%d kp:%d p.wtajemn.:%d nideryt:%d    ",
	 g_poziom,gracz.stat[FZ],gracz.stat[FZ_MAX],gracz.stat[SI],
	 gracz.stat[KB],gracz.stat[KP],gracz.p_wtaj,gracz.nideryt);
  move(WYS+2,1);
  attron(COLOR_PAIR(KARMAZYN));
  for(st=gracz.stany;st!=NULL;st=st->nast)
    {
      //      printw("{%s [%d]} ",opis_stanu[st->typ].oznaczenie,st->ile);
      printw("{%s} ",opis_stanu[st->typ].oznaczenie);
    }
  move(gracz.poz.y+1,gracz.poz.x);
  refresh();
}

