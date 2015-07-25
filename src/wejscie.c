#include"drogue.h"


Poz wez_kierunek(int z)
{
  Poz p;
  p.x=p.y=0;
  switch(z)
    {
    case '4': case 'h': p.x--; break;
    case '6': case 'l': p.x++; break;
    case '8': case 'k': p.y--; break;
    case '2': case 'j': p.y++; break;
    case '7': case 'y': p.x--;p.y--; break;
    case '9': case 'u': p.x++;p.y--; break;
    case '1': case 'b': p.x--;p.y++; break;
    case '3': case 'n': p.x++;p.y++; break;  
    case '5': p.x=p.y=0; break;
    default: p.x=p.y=-6; /* tak elegancko zglaszam wjomtek */
    }
  return p;
}

int wejscie()
{
 int z;
 Poz p;
 
  switch(z=getchar())
    {
     case '4': case 'h': case '6': case 'l':
     case '8': case 'k': case '2': case 'j':
     case '7': case 'y': case '9': case 'u':
     case '1': case 'b': case '3': case 'n':
      p=wez_kierunek(z);
      return a_idz(&gracz,p.x,p.y);
    case 'z':
      dodaj_komunikat("zbadaj (wybierz kierunek...)");
      wypisz_komunikaty();
      refresh();
      p=wez_kierunek(getchar());
      if(p.x>-6) zbadaj(&gracz,gracz.poz.x+p.x,gracz.poz.y+p.y);
      return 0;
    case 'o':
      dodaj_komunikat("akcja (wybierz kierunek...)");
      wypisz_komunikaty();
      refresh();
      p=wez_kierunek(getchar());
      if(p.x>-6) return akcja(&gracz,p.x,p.y);
      else return 0;
      //    case 'a': uzyj!
    case ',': return a_podnies(&gracz);
    case 'w': return a_wyrzuc(&gracz);
    case 'e': ekwipunek(); return 0;
    case 'a': return a_uzyj(&gracz);
    case 't': return a_rzuc(&gracz);
    case '>': return a_zejdz(&gracz);
    case 'q': koniec(0);
    case '?': pomoc(); return 0;
    }
  return 0;
}
