#include"drogue.h"

void tytulowa()
{
  clear(); attron(COLOR_PAIR(CYKLAMEN)); move(5,9);
  printw("       #"); move(6,9);
  printw("     ## # #  "); attron(COLOR_PAIR(CYJAN));
  printw("          - d e r c z   r o g u e 0.33 - "); move(7,9);
 attron(COLOR_PAIR(CYKLAMEN));  printw("   #   #"); move(8,9);
  printw("  ############        Najbardziej"); move(9,9);
  printw(" ## ###  ### ##       Niebezpieczna"); move(10,9);
  printw("#     #  #     #      Gra"); move(11,9);
  printw("# #####  ##### #      Dercza"); move(12,9);
  printw("# #   #  #   # #      Stanislawa"); move(13,9);
  printw("# ### #  #### ##"); move(14,9);
  printw(" ###  #  #  ###"); move(15,9);
  printw("  ############"); move(16,9); printw("         #");
  move(17,9);  printw("     #   ##"); move(18,9);
  printw("       ##         "); attron(COLOR_PAIR(ZIELEN));
  printw("    Gdansk, Gdynia, Stezyca, 2006-2008");
  attron(COLOR_PAIR(CYKLAMEN)); move(19,9); printw("       #"); move(24,1);
 attron(COLOR_PAIR(BIEL));
  printw("-wcisnij spacje aby rozpoczac-");
  refresh();
  while(getchar()!=' ');
  clear();
}

void pomoc()
{
  attron(COLOR_PAIR(BIEL));
move(1,1);printw("poruszanie sie:            czynnosci:");move(2,1);
printw("===============            ==========");move(3,1);
printw(" 7  8  9       y  k  u     o akcja (otworz,zamknij,uzyj,");move(4,1);
printw("  \\ | /         \\ | /               wlacz,rozbroj,itp.)");move(5,1);
printw(" 4- @ -6  lub  h- @ -l	   z zbadaj");move(6,1);
printw("  / | \\         / | \\	  ");move(7,1);
printw(" 1  2  3       u  j  b     , podnies");move(8,1);
printw("                           w wyrzuc");move(9,1);
printw(" 5 czekaj                  t rzuc");move(10,1);
printw(" > zejdz                   a uzyj (zaloz,zdejmij,zjedz,itp)");
move(11,1);printw("inne:                      dobra rada:");move(12,1);
printw("=====                      ===========");move(13,1);
printw(" e ekwipunek               MIEJ OCZY Z TYLU GLOWY!");move(14,1);
printw(" ? pomoc (ten ekran)");move(15,1);
printw(" q zakoncz gre                                     -spacja-");
 refresh();
 while(getchar()!=' ') ;
 rysuj();
}


#define MaxIters 200
#define S        26
#define BLACK    -1
#define LEFT     -2.0
#define RIGHT    1.0
#define TOP      1.0
#define BOTTOM   -1.0

void mand()
{
    short   x, y, count;
    long double zr, zi, cr, ci;
    long double rsquared, isquared;

    attron(COLOR_PAIR(CYKLAMEN));
    for(y=2;y<S;y++)
    {
      move(y-1,5);
        for(x=0;x<S;x++)
        {
	  zr=0.0;
          zi=0.0;
          cr=LEFT+x*(RIGHT-LEFT)/S;
          ci=TOP+y*(BOTTOM-TOP)/S;
          rsquared=zr*zr;
          isquared=zi*zi;
          for(count=0;rsquared+isquared<=4.0&&count<MaxIters;count++)
           {
	     zi=zr*zi*2;
             zi+=ci;
             zr=rsquared-isquared;
             zr+=cr;
             rsquared=zr*zr;
             isquared=zi*zi;
            }
            if(rsquared+isquared<=4.0)
                printw("##");    
            else if(rsquared+isquared <=4.4)
                printw("::");    
		else if(rsquared+isquared <=6.0)
		 printw("..");
		   else printw("  ");
        }
    }
    refresh();
}

