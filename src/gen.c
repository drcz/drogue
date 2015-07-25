#include"drogue.h"

enum
{
 POKOJE_X = 5,
 POKOJE_Y = 3,
 MAX_POKOJE = POKOJE_X*POKOJE_Y,
 MAX_W=SZE/POKOJE_X,
 MAX_H=WYS/POKOJE_Y,
 MIN_W=4,
 MIN_H=3,

 PION=1,
 POZIOM=2
};

struct
{
 Poz p1,p2;
 signed char mozna_polaczyc[MAX_POKOJE];
 TakNie polaczono[MAX_POKOJE];
 TakNie dostepny;
 TakNie istnieje;
} pokoj[MAX_POKOJE];

void czysc();
void inicjuj_pokoje();
void rysuj_pokoj(int);
void stworz_pokoj(int);
Poz losuj_punkt(int);
void zrob_polaczenia();
void tunel(int,int);
void polacz(Poz,Poz,bool);
void dodaj_wlaz(int,int);
void dodaj_zejscie();
void dodaj_nideryt();
void ustaw_gracza();
void dodaj_pulapki(int);
void dodaj_maszyny(int);
void dodaj_przedmioty(int);
void dodaj_potwory(int);
void dodaj_pojemniki(int);
void dodaj_wodotryski();
void generuj_jaskinie_instead();
void generuj_spirale_instead();
void generuj_kratke_instead();
void generuj_jamy();

void nowa_plansza()
{
  czysc();
  if(++g_poziom>=1023)
    {
      clear();
      attron(COLOR_PAIR(ZIELEN));
      move(1,1);
      printw("Wchodzisz na poziom 1023.\n");move(2,1);
      printw("To tu wieziona jest dusza Dercza.\n");move(3,1);
      printw("Dzin Najbardziej Niebezpiecznej Gry tajemniczo sie usmiecha.\n");
      move(4,1);
      printw("Wiesz juz co sie stalo. Dusza Dercza odlatuje uwolniona.\n");
      move(5,1);
      printw("Teraz to Twoja dusza bedzie tu tkwic az ktos tu dotrze i\n");
      move(6,1);
      printw("ja uwolni.\n");move(8,1);
      attron(COLOR_PAIR(CYJAN));
      printw("JA DUCH NAJBARDZIEJ NIEBEZPIECZNEJ GRY ZABIERAM CI DUSZE.\n");
      move(10,1);
      attron(COLOR_PAIR(ZOLC));
      printw("\n\ndziekuje za granie w drogue0.33\n\n                                 -spacja-\n");
      refresh();
      while(getchar()!=' ');
      koniec(0);
    }
  if(g_poziom>2)
    {
      if(losuj(1,13)==1) {generuj_spirale_instead(); return;}
      if(losuj(1,6)==6) {generuj_jaskinie_instead(); return;}
      // labirynt
      if(losuj(1,4)==3) {generuj_jamy(); return;}
      if(losuj(1,33)==23) {generuj_kratke_instead(); return;}
    }
  // 'normalna' mapa:
  inicjuj_pokoje();
  zrob_polaczenia();
  dodaj_zejscie();
  ustaw_gracza();
  dodaj_nideryt();
  if(g_poziom>1)dodaj_pulapki(losuj(1,5));
  if(g_poziom>4)dodaj_pulapki(losuj(2,4));
  dodaj_maszyny(losuj(2,5));
  dodaj_przedmioty(losuj(5,9));
  if(g_poziom>4) dodaj_przedmioty(losuj(2,5));
  dodaj_potwory(losuj(13,19));
  if(g_poziom>2) dodaj_potwory(losuj(3,5));
  if(g_poziom>5) dodaj_potwory(losuj(4,6));
  if(g_poziom>7) dodaj_potwory(losuj(2,3));
  if(g_poziom>9) dodaj_potwory(losuj(2,3));
  if(g_poziom>13) dodaj_potwory(losuj(4,6));
  if(g_poziom>20) dodaj_potwory(losuj(0,3));
  if(g_poziom>2)dodaj_pojemniki(losuj(1,4));
  if(g_poziom>5)dodaj_pojemniki(losuj(2,5));
  dodaj_wodotryski();
}

void czysc()
{
  Stwor *s;
  int i,j;
  for(i=0;i<SZE;i++)
    for(j=0;j<WYS;j++)
      {
	mapa[i][j].t=NIC;mapa[i][j].stwor=NULL;
	mapa[i][j].pocisk=NULL;
	free(mapa[i][j].przedmiot);
	mapa[i][j].przedmiot=NULL;
	free(mapa[i][j].pojemnik);
	mapa[i][j].pojemnik=NULL;
	mapa[i][j].sym.znak=' ';
      }
  for(;lista_potworow;)
    {      
      s=lista_potworow->nast;
      free(lista_potworow);
      lista_potworow=s;
    }
}

void inicjuj_pokoje()
{
 int p,i,x=0,y=0;
 
 for(p=MAX_POKOJE;p--;)
 {
  pokoj[p].p1.x=x*MAX_W; pokoj[p].p1.y=y*MAX_H;
  pokoj[p].p2.x=pokoj[p].p1.x+MAX_W-2;
  pokoj[p].p2.y=pokoj[p].p1.y+MAX_H-2;
  for(i=MAX_POKOJE;i--;)
   {
    if(i==p+1 && p%POKOJE_X!=POKOJE_X-1) pokoj[p].mozna_polaczyc[i]=-POZIOM;
     else if(i==p-1 && p%POKOJE_X!=0) pokoj[p].mozna_polaczyc[i]=POZIOM;
      else if(i==p+POKOJE_X) pokoj[p].mozna_polaczyc[i]=-PION;
       else if(i==p-POKOJE_X) pokoj[p].mozna_polaczyc[i]=PION;
        else pokoj[p].mozna_polaczyc[i]=0;
    pokoj[p].polaczono[i]=0;    
   }
  x++; if(x>=POKOJE_X) {x=0;y++;}
  pokoj[p].dostepny=0;
  if(losuj(1,10)>7) pokoj[p].istnieje=0;
   else pokoj[p].istnieje=1;
  stworz_pokoj(p);
 }
}

void stworz_pokoj(int n)
{
 if(pokoj[n].istnieje)
  {
   int d;
  
   pokoj[n].p1.x=losuj(pokoj[n].p1.x,pokoj[n].p2.x-MIN_W-1);
   pokoj[n].p1.y=losuj(pokoj[n].p1.y,pokoj[n].p2.y-MIN_H-1);
   d=pokoj[n].p2.x-pokoj[n].p1.x;
   while(d>MIN_W && !losuj(0,2)) {d--; pokoj[n].p2.x--;}
   d=pokoj[n].p2.y-pokoj[n].p1.y;
   while(d>MIN_H && !losuj(0,2)) {d--; pokoj[n].p2.y--;}
   rysuj_pokoj(n);
  }
  else
  {
   Poz s;

   pokoj[n].istnieje=1;
   s=losuj_punkt(n);
   pokoj[n].istnieje=0;
   pokoj[n].p1.x=pokoj[n].p2.x=s.x;
   pokoj[n].p1.y=pokoj[n].p2.y=s.y;   
  }
}

void rysuj_pokoj(int n)
{
 register unsigned int i,j;

 for(i=pokoj[n].p1.x;i<=pokoj[n].p2.x;i++)
  for(j=pokoj[n].p1.y;j<=pokoj[n].p2.y;j++)
   {
    if(j==pokoj[n].p1.y || j==pokoj[n].p2.y) mapa[i][j].t=SCIANA_H;
     else if(i==pokoj[n].p1.x || i==pokoj[n].p2.x) mapa[i][j].t=SCIANA_W;
     else mapa[i][j].t=PODLOGA;
   }
}

Poz losuj_punkt(int n)
{
  Poz p;
  if(pokoj[n].istnieje)
    { 
      p.x=losuj(pokoj[n].p1.x+1,pokoj[n].p2.x-1);
      p.y=losuj(pokoj[n].p1.y+1,pokoj[n].p2.y-1);
    }
  else 
    {
      p.x=pokoj[n].p1.x;
      p.y=pokoj[n].p1.y;
    }
  return p;
}


void zrob_polaczenia()
{
  int i,p1,p2;
  int l_pokoj=1;

  p1=losuj(0,MAX_POKOJE-1);
  pokoj[p1].dostepny=1;
  while(l_pokoj<MAX_POKOJE)
    {
      p2=-1;
      for(i=MAX_POKOJE;i--;)
       if(pokoj[p1].mozna_polaczyc[i]!=0 && !pokoj[i].dostepny
          && (p2<0 || losuj(0,1))) p2=i;
      if(p2==-1)
	{
	  do{
             p1=losuj(0,MAX_POKOJE-1);
            }while(!pokoj[p1].dostepny);
	}
       else
	{
	  pokoj[p1].dostepny=pokoj[p2].dostepny=1;
	  tunel(p1,p2);
	  l_pokoj++;          
	}
    }

  for(l_pokoj=losuj(0,4);l_pokoj--;)
    {
      p1=losuj(0,MAX_POKOJE-1);
      p2=-1;
      for(i=MAX_POKOJE;i--;)
       {if(pokoj[p1].mozna_polaczyc[i]!=0 && (p2<0 || losuj(0,1))) p2=i;}
      if(p2>=0) tunel(p1,p2);
    } 

}

void tunel(int n,int m)
{
 Poz a,b;
 bool pion,wlaz_a,wlaz_b;
 int k = pokoj[n].mozna_polaczyc[m];

 if(pokoj[n].polaczono[m]) k=0;
 switch(k)
 {
  case -POZIOM:
   a=losuj_punkt(m);
   a.x=pokoj[m].p2.x;
   b=losuj_punkt(n);
   b.x=pokoj[n].p1.x;
   wlaz_a=pokoj[m].istnieje;
   wlaz_b=pokoj[n].istnieje;
   pion=0;
  break;
  case -PION:
   a=losuj_punkt(m);
   a.y=pokoj[m].p2.y;
   b=losuj_punkt(n);
   b.y=pokoj[n].p1.y;
   wlaz_a=pokoj[m].istnieje;
   wlaz_b=pokoj[n].istnieje;
   pion=1;
  break;
  case PION:
   a=losuj_punkt(n);
   a.y=pokoj[n].p2.y;
   b=losuj_punkt(m);
   b.y=pokoj[m].p1.y;
   wlaz_a=pokoj[n].istnieje;
   wlaz_b=pokoj[m].istnieje;
   pion=1;   
  break;
  case POZIOM:
   a=losuj_punkt(n);
   a.x=pokoj[n].p2.x;
   b=losuj_punkt(m);
   b.x=pokoj[m].p1.x;
   wlaz_a=pokoj[n].istnieje;
   wlaz_b=pokoj[m].istnieje;
   pion=0;
  break;
  default: return;
 }
 polacz(a,b,pion);
 if(wlaz_a) dodaj_wlaz(a.x,a.y);
 if(wlaz_b) dodaj_wlaz(b.x,b.y);
 pokoj[n].polaczono[m]=pokoj[m].polaczono[n]=1;
}

void dodaj_wlaz(int x,int y)
{
  if(g_poziom>4 && rand()%6==3) /* tajne przejscie? */
    {
      if(mozna_wejsc(x-1,y)==TAK && mozna_wejsc(x+1,y)==TAK)
	mapa[x][y].t=UWLAZ_W;
      else  mapa[x][y].t=UWLAZ_H;
    }
   else mapa[x][y].t=WLAZ_Z;
}

void polacz(Poz poczatek, Poz koniec, bool pion)
{
 Poz kursor,delta,delta_s;
 int odleglosc,zakret,odleglosc2;

 if(pion)
 {
  odleglosc=koniec.y-poczatek.y;
  odleglosc2=koniec.x-poczatek.x;
  zakret=losuj(1,odleglosc-1);
  delta.x=0; delta.y=1;
  delta_s.y=0;
  if(odleglosc2>0) delta_s.x=1;
   else if(odleglosc2<0) {delta_s.x=-1;odleglosc2*=-1;}
    else delta_s.x=0;
 }
 else
 {
  odleglosc=koniec.x-poczatek.x;
  odleglosc2=koniec.y-poczatek.y;
  zakret=losuj(1,odleglosc-1);
  delta.x=1; delta.y=0;
  delta_s.x=0;
  if(odleglosc2>0) delta_s.y=1;
   else if(odleglosc2<0) {delta_s.y=-1;odleglosc2*=-1;}
    else delta_s.y=0;
 } 
 kursor.x=poczatek.x; kursor.y=poczatek.y;
 while(odleglosc--)
 {
  kursor.x+=delta.x; kursor.y+=delta.y;  
  mapa[kursor.x][kursor.y].t=KORYTARZ;
  if(odleglosc==zakret && odleglosc2>0)
  {
   while(odleglosc2--)
   {
    kursor.x+=delta_s.x; kursor.y+=delta_s.y;    
    mapa[kursor.x][kursor.y].t=KORYTARZ;
   }
  }
 }
 mapa[poczatek.x][poczatek.y].t=mapa[koniec.x][koniec.y].t=KORYTARZ;
}

void dodaj_zejscie()
{
 Poz z;
 int n;
 
 do{n=losuj(0,MAX_POKOJE-1);} while(!pokoj[n].istnieje);
 z=losuj_punkt(n);
 mapa[z.x][z.y].t=ZEJSCIE;
}


void ustaw_gracza()
{
 Poz z;
 int n;
 
 do{n=losuj(0,MAX_POKOJE-1);} while(!pokoj[n].istnieje);
 z=losuj_punkt(n);
 gracz.poz.x=z.x; gracz.poz.y=z.y;
 mapa[z.x][z.y].stwor=&gracz;
}

void generuj_jaskinie_instead()
{
 short i,j,k,m;
 short p[3][2];

 for(i=0;i<SZE;i++)
  for(j=0;j<WYS;j++)
   mapa[i][j].t=SKALA;

 p[0][0]=30;
 p[0][1]=10;
 p[1][0]=p[0][0];
 p[1][1]=p[0][1];
 p[2][0]=p[0][0];
 p[2][1]=p[0][1];
 m=losuj(23,42);
 for(i=0;i<m;i++)
  {
   for(j=0;j<3;j++)
    {
     k=losuj(0,9);

     switch(k)
     {
      case 0:
      case 1: p[j][0]++; break;
      case 2: p[j][0]++; p[j][1]--; break;
      case 3: p[j][1]--; break;
      case 4: p[j][0]--; p[j][1]--; break;
      case 5: 
      case 6: p[j][0]--; break;
      case 7: p[j][0]--; p[j][1]++; break;
      case 8: p[j][1]++; break;
      case 9: p[j][1]++; p[j][0]++; break;
     }
     if(p[j][0]<1)p[j][0]=1;
     if(p[j][1]<1)p[j][1]=1;
     if(p[j][0]>(SZE-3))p[j][0]=SZE-3;
     if(p[j][1]>(WYS-2))p[j][1]=WYS-2;   
     mapa[p[j][0]][p[j][1]].t=PIACH;
     mapa[p[j][0]+1][p[j][1]].t=PIACH;
    }
   }
 mapa[p[1][0]][p[1][1]].t=ZEJSCIE;

 gracz.poz.x=p[2][0];
 gracz.poz.y=p[2][1];
 mapa[gracz.poz.x][gracz.poz.y].stwor=&gracz;
 dodaj_przedmioty(losuj(0,3));
}

void generuj_spirale_instead()
{
  int a,b,i,j,l,t;

  switch(losuj(0,4))
    {
    case 0:
    case 1:t=KORYTARZ; break;
    case 2:t=PIACH; break;
    case 3:t=PODLOGA; break;
    case 4:t=NIDERYT; break;
    }
  a=losuj(13,66);
  b=losuj(9,18);
  i=(SZE-a)/2-1;j=1;
  gracz.poz.x=i;
  gracz.poz.y=j;
  while(a>1 && b>1)
    {
      for(l=i;l<=i+a;l++)
	mapa[l][j].t=t;
      i=l;
      for(l=j;l<=j+b;l++)
	mapa[i][l].t=t;
      j=l;
      a-=2;b-=2;
      for(l=i;l>=i-a;l--)
	mapa[l][j].t=t;
      i=l;
      for(l=j;l>=j-b;l--)
	mapa[i][l].t=t;
      j=l;
      a-=2;b-=2;
    }
  mapa[i][j].t=ZEJSCIE;
  mapa[gracz.poz.x][gracz.poz.y].stwor=&gracz;
}

void generuj_kratke_instead()
{
  int i,j;
  int sz=losuj(10,60);
  int w=losuj(6,20);
  int x=(SZE-sz)/2;
  int y=(WYS-w)/2;
  for(i=x;i<=x+sz;i++)
    for(j=y;j<=y+w;j++)
      if(i==x || i==(x+sz) || j==y || j==(y+w) || i==(x+sz/2) || j==(y+w/2))
	mapa[i][j].t=KORYTARZ;
  mapa[x][y].t=ZEJSCIE;
  gracz.poz.x=x+sz;
  gracz.poz.y=y+w;
  mapa[gracz.poz.x][gracz.poz.y].stwor=&gracz;
}


void dodaj_pulapki(int ile)
{
  int x,y;
  for(;ile>0;ile--)
    {
      do
	{
	  x=losuj(1,SZE-1);
	  y=losuj(1,WYS-1);
	}
      while(mapa[x][y].t!=PODLOGA || mapa[x][y].stwor);
      switch(losuj(1,5))
	{
	case 1: mapa[x][y].t=_ZAPADNIA; break;
	case 2: mapa[x][y].t=_KOLCE; break;
	case 3: mapa[x][y].t=_OSTRZA; break;
	case 4: mapa[x][y].t=_PASTUCH; break;
	default: mapa[x][y].t=_PALNIKI; break;
        }
    }
}

void dodaj_nideryt()
{
  int x,y,ile;
  for(ile=losuj(3,9);ile>0;ile--)
    {
      do
	{
	  x=losuj(1,SZE-1);
	  y=losuj(1,WYS-1);
	}
      while(mapa[x][y].t!=PODLOGA || mapa[x][y].stwor);
      mapa[x][y].t=NIDERYT;
    }
}

void dodaj_maszyny(int ile)
{
  int x,y;
  for(;ile>0;ile--)
    {
      do
	{
	  x=losuj(1,SZE-1);
	  y=losuj(1,WYS-1);
	}
      while(mapa[x][y].t!=SCIANA_W && mapa[x][y].t!=SCIANA_H);
      mapa[x][y].t=MASZYNA_PRZENIESIEN;
    }
  /* i jeszcze na kazdym poziomie stoi jeden pyrowid: */
  do
    {
      x=losuj(1,SZE-1);
      y=losuj(1,WYS-1);
    } while(mapa[x][y].t!=SCIANA_W && mapa[x][y].t!=SCIANA_H);
  mapa[x][y].t=PYROWID;
}


void dodaj_przedmioty(int ile)
{
  int x,y;
  for(;ile>0;ile--)
    {
      do
	{
	  x=losuj(1,SZE-1);
	  y=losuj(1,WYS-1);
	}
      while(mozna_wejsc(x,y)==NIE || mapa[x][y].stwor);
      dodaj_przedmiot(x,y,wymysl_przedmiot());
    }
}

void dodaj_potwory(int ile)
{
  int x,y;
  for(;ile>0;ile--)
    {
      do
	{
	  x=losuj(1,SZE-1);
	  y=losuj(1,WYS-1);
	}
      while(mozna_wejsc(x,y)==NIE || mapa[x][y].stwor);
      stworz_potwora(x,y);
    }
}


void dodaj_wodotryski()
{
  int x,y,ile=losuj(0,3);
  for(;ile>0;ile--)
    {
      do
	{
	  x=losuj(1,SZE-1);
	  y=losuj(1,WYS-1);
	}
      while(mozna_wejsc(x,y)==NIE);
      mapa[x][y].t=FONTANNA;
    }
}

void dodaj_pojemniki(int ile)
{
  int x,y;
  for(;ile>0;ile--)
    {
      do
	{
	  x=losuj(1,SZE-1);
	  y=losuj(1,WYS-1);
	}
      while(mozna_wejsc(x,y)==NIE || mapa[x][y].pojemnik);
      dodaj_pojemnik(x,y,TAK);
    }
}

void draz_jame(int n)
{
  int i,j;
  for(i=pokoj[n].p1.x;i<=pokoj[n].p2.x;i++)
    for(j=pokoj[n].p1.y;j<=pokoj[n].p2.y;j++)
      mapa[i][j].t=PODLOGA;
  mapa[pokoj[n].p1.x][pokoj[n].p1.y].t=SKALA;
  mapa[pokoj[n].p2.x][pokoj[n].p1.y].t=SKALA;
  mapa[pokoj[n].p2.x][pokoj[n].p1.y].t=SKALA;
  mapa[pokoj[n].p2.x][pokoj[n].p2.y].t=SKALA;
}

void j_polacz(int x1,int y1,int x2,int y2)
{
 int c,d;
 
 if(x1<x2) {for(c=x1;c<=x2;c++) mapa[c][y1].t=PODLOGA;}
 else {for(c=x2;c<=x1;c++)mapa[c][y2].t=PODLOGA;}
 if(y1<y2) {for(d=y1;d<=y2;d++)mapa[c][d].t=PODLOGA;}
  else {for(d=y2;d<=y1;d++)mapa[c][d].t=PODLOGA;}
}

void polacz_jamy(int n,int m)
{
  int x,y,X,Y;
  x=(pokoj[n].p1.x+pokoj[n].p2.x)/2;
  y=(pokoj[n].p1.y+pokoj[n].p2.y)/2;
  X=(pokoj[m].p1.x+pokoj[m].p2.x)/2;
  Y=(pokoj[m].p1.y+pokoj[m].p2.y)/2;
  j_polacz(x,y,X,Y);
}

int j_pokrywa(int n,int m)
{
 int x1,y1,X1,Y1,x2,y2,X2,Y2;
 x1=pokoj[n].p1.x;
 y1=pokoj[n].p1.y;
 x2=pokoj[n].p2.x;
 y2=pokoj[n].p2.y;
 X1=pokoj[m].p1.x;
 Y1=pokoj[m].p1.y;
 X2=pokoj[m].p2.x;
 Y2=pokoj[m].p2.y;
 if((x2<X1)||(x1>X2)) return NIE;
  else
   if((y2<Y1)||(y1>Y2)) return NIE;
    else return TAK;
}

int dobra_jama(int n)
{
  int i;
  for(i=0;i<n;i++) if(j_pokrywa(i,n)==TAK) return NIE;
  return TAK;
}

void losuj_jame(int n)
{
 int i=0,x,y,w,h;
 do
  {
   i++;
   x=losuj(1,SZE-6);
   y=losuj(1,WYS-5);
   if(i>6){h=0;w=0;}
    else
     {
      w=losuj(4,9);
      h=losuj(3,7);
     }
   if(x+w>=(SZE-1)) w=SZE-x-2;
   if(y+h>=(WYS-1)) h=WYS-y-2;
   pokoj[n].p1.x=x;
   pokoj[n].p1.y=y;
   pokoj[n].p2.x=x+w;
   pokoj[n].p2.y=y+h;
  }while((i<33)&&(dobra_jama(n)==NIE));  
}

void generuj_jamy()
{
  int i,j;
  for(i=0;i<SZE;i++)
    for(j=0;j<WYS;j++)
      mapa[i][j].t=SKALA;
    j=losuj(4,9);
  for(i=0;i<j;i++)
    losuj_jame(i);
  for(i=1;i<j;i++)
    polacz_jamy(i-1,i);
  for(i=0;i<j;i++)
    draz_jame(i);
  gracz.poz.x=pokoj[0].p1.x+2;
  gracz.poz.y=pokoj[0].p1.y+2;
  mapa[pokoj[3].p2.x-2][pokoj[3].p2.y-2].t=ZEJSCIE;
  mapa[gracz.poz.x][gracz.poz.y].stwor=&gracz;
  dodaj_przedmioty(losuj(0,4));
  dodaj_potwory(losuj(10,19));
}
