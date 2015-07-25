#include "drogue.h"

void inicjuj_mape()
{
  int i,j;
  for(i=0;i<SZE;i++)
    for(j=0;j<WYS;j++)
      {
	mapa[i][j].stwor=NULL;
	mapa[i][j].przedmiot=NULL;
	mapa[i][j].pocisk=NULL;
	mapa[i][j].pojemnik=NULL;
      }
}

void mapa_zmien(int x,int y,int t)
{
  mapa[x][y].t=t;
  widz();
}

TakNie mozna_wejsc(int x,int y)
{
  return podstawka[mapa[x][y].t].przepuszcza;
}

enum{KOSZT_OTWORZ=17,KOSZT_ZAMKNIJ=18,KOSZT_ZBADAJ=14,KOSZT_ZEJDZ=16};

void fnic(Stwor *,int,int);
void fotworz(Stwor *,int,int);
void fzamknij(Stwor *,int,int);
void fzejdz(Stwor *,int,int);
void fprzenies(Stwor *,int,int);
void frozbroj(Stwor *,int,int);
void ffontanna(Stwor *,int,int);
void fpyrowid(Stwor *,int,int);

void gnic(Stwor *);
void gtujestzejscie(Stwor *);
void gnideryt(Stwor *);

void gzsunsie(Stwor *);
void gkolce(Stwor *);
void gostrza(Stwor *);
void gpastuch(Stwor *);
void gpalniki(Stwor *);

Teren podstawka[MAX_PODST]={ /* sym,nazwa,zbadaj,przepuszcza,akcja,wlezienie */
 {{' ',CZERN},{"nic","nicosc","nicosci",TAK,NIE},
  "tu nic, ale to nic nie ma.",NIE,fnic,gnic},
 {{'-',CYKLAMEN},{"sciana","sciane","sciany",TAK,NIE},
  "zimna gladka sciana.",NIE,fnic,gnic},
 {{'|',CYKLAMEN},{"sciana","sciane","sciany",TAK,NIE},
  "zimna gladka sciana.",NIE,fnic,gnic},
 {{'-',CYKLAMEN},{"sciana","sciane","sciany",TAK,NIE},
  "lekko wklesla sciana.",NIE,fotworz,gnic},
 {{'|',CYKLAMEN},{"sciana","sciane","sciany",TAK,NIE},
  "lekko wklesla sciana.",NIE,fotworz,gnic},
 {{'+',ZOLC},{"wlaz","wlaz","wlazu",NIE,NIE},
  "potezny stalowy wlaz.",NIE,fotworz,gnic},
 {{':',ZOLC},{"wlaz","wlaz","wlazu",NIE,NIE},
  "potezny stalowy wlaz.",TAK,fzamknij,gnic},
 {{'.',CYKLAMEN},{"posadzka","posadzke","posadzki",TAK,NIE},
  "lsniaca posadzka.",TAK,fnic,gnic},
 {{'#',CYJAN},{"korytarz","korytarz","korytarza",NIE,NIE},
  "zakurzony korytarz.",TAK,fnic,gnic},
 {{'*',ZIELEN},{"nideryt","nideryt","niderytu",NIE,NIE},
  "na podlodze lezy solidna brylka niderytu.",TAK,fnic,gnideryt},
 {{'>',ZOLC},{"zejscie","zejscie","zejscia",NIE,NIE},
  "ciemny szyb wiodacy w dol.",TAK,fzejdz,gtujestzejscie},
 {{'$',CYJAN},{"fontanna","fontanne","fontanny",TAK,NIE},
  "fontanna glupcow, woda chlupie.",TAK,ffontanna,gnic},
 {{'.',CYJAN},{"piach","piach","piachu",NIE,NIE},
  "piaszczyste podloze.",TAK,fnic,gnic},
 {{'#',CYKLAMEN},{"skala","skale","skaly",TAK,NIE},
  "szorstka skala.",NIE,fnic,gnic},
 {{'m',CYJAN},
  {"maszyna przeniesien","maszyne przeniesien","maszyny przeniesien",TAK,NIE},
  "tu stoi maszyna przeniesien.",NIE,fprzenies,gnic},
 {{'m',ZOLC},
  {"pyrowid","pyrowida","pyrowida",TAK,NIE},
  "tu stoi pyrowid.",NIE,fpyrowid,gnic},
 {{'.',CYKLAMEN},{"zapadnia","zapadnie","zapadni",TAK,NIE},
  "posadzka.",TAK,fnic,gzsunsie},
 {{'^',KARMAZYN},{"zapadnia","zapadnie","zapadni",TAK,NIE},
  "tu jest zapadnia.",TAK,frozbroj,gzsunsie},
 {{'.',CYKLAMEN},{"kolce","kolce","kolcow",NIE,TAK},
  "posadzka.",TAK,fnic,gkolce},
 {{'^',KARMAZYN},{"kolce","kolce","kolcow",NIE,TAK},
  "tu sa wysuwane kolce.",TAK,frozbroj,gkolce},
 {{'.',CYKLAMEN},{"ostrza","ostrza","ostrzy",NIE,TAK},
  "posadzka.",TAK,fnic,gostrza},
 {{'^',KARMAZYN},{"ostrza","ostrza","ostrzy",NIE,TAK},
  "tu sa wirujace ostrza.",TAK,frozbroj,gostrza},
 {{'.',CYKLAMEN},{"pastuch","pastucha","pastucha",NIE,NIE},
  "posadzka.",TAK,fnic,gpastuch},
 {{'^',KARMAZYN},{"pastuch","pastucha","pastucha",NIE,NIE},
  "tu jest elektryczny pastuch.",TAK,frozbroj,gpastuch},
 {{'.',CYKLAMEN},{"palniki","palniki","palnikow",NIE,TAK},
  "posadzka.",TAK,fnic,gpalniki},
 {{'^',KARMAZYN},{"palniki","palniki","palnikow",NIE,TAK},
  "tu sa palniki.",TAK,frozbroj,gpalniki}};

void fnic(Stwor *kto,int x,int y){}

void fotworz(Stwor *kto,int x,int y)
{
  mapa_zmien(x,y,WLAZ_O);
  if(kto==&gracz) kom_otworzyles_wlaz();
  kto->pa+=KOSZT_OTWORZ;
}

void fzamknij(Stwor *kto,int x,int y)
{
  mapa_zmien(x,y,WLAZ_Z);
  if(kto==&gracz) kom_zamknales_wlaz();
  kto->pa+=KOSZT_ZAMKNIJ;
}

void fzejdz(Stwor *kto,int x,int y)
{
  kto->pa+=KOSZT_ZEJDZ;
  if(kto==&gracz)
    {
      nowa_plansza();
      kom_nizszy_poziom();
    }
  //    else usun_stwora(kto);
}

void fprzenies(Stwor *kto,int x,int y)
{
  do
    {
      x=losuj(1,SZE-1);
      y=losuj(1,WYS-1);
    }
  while(mozna_wejsc(x,y)!=TAK || mapa[x][y].stwor);
  mapa[kto->poz.x][kto->poz.y].stwor=NULL;
  kto->poz.x=x; kto->poz.y=y;
  mapa[kto->poz.x][kto->poz.y].stwor=kto;
  widz();
  if(kto==&gracz) kom_przeniesienie();
}

void frozbroj(Stwor *kto,int x,int y)
{
  if(losuj(0,1))
    {
      if(kto==&gracz)
	{
	  char kk[60];
	  sprintf(kk,"rozbroiles %s!",podstawka[mapa[x][y].t].nazwa.kogoco);
	  dodaj_komunikat(kk);
	  kto->dsw+=2;
	  sprawdz_doswiadczenie();
	}
      mapa_zmien(x,y,PODLOGA);
    }
  else kto->pa=a_idz(kto,x-kto->poz.x,y-kto->poz.y);
}

void ffontanna(Stwor *kto,int x,int y)
{
  if(kto->stat[FZ]<kto->stat[FZ_MAX]) kto->stat[FZ]++;
  if(kto==&gracz) dodaj_komunikat("woda z fontanny jest wysoce orzezwiajaca!");
}

void fpyrowid(Stwor *kto,int x,int y)
{
  if(kto==&gracz)
    {
      for(x=0;x<SZE;x++)
	for(y=0;y<WYS;y++) zobacz(x,y);
      widz();
      dodaj_komunikat("pyrowid zeslal ci widzenie...");
    }
}

void gnic(Stwor *kto){}

void gzsunsie(Stwor *kto)
{
  mapa_zmien(kto->poz.x,kto->poz.y,ZAPADNIA);
  if(kto==&gracz)
    {
      dodaj_komunikat("zapadnia! spadasz na nizszy poziom.");
      dodaj_komunikat(" ");
      nowa_plansza();
      widz();
      rysuj();
    }
  //    else usun_stwora(kto);
}

void gkolce(Stwor *kto)
{
  mapa_zmien(kto->poz.x,kto->poz.y,KOLCE);
  kto->stat[FZ]-=losuj(1,3);
  if(kto==&gracz)
    {
      dodaj_komunikat("z podlogi wysuwaja sie kolce!");
      dodaj_komunikat("");
    }
  sprawdz(kto);
}

void gostrza(Stwor *kto)
{
  mapa_zmien(kto->poz.x,kto->poz.y,OSTRZA);
  kto->stat[FZ]-=losuj(2,3);
  if(kto==&gracz)
    {
      dodaj_komunikat("z podlogi wysuwaja sie wirujace ostrza!");
      dodaj_komunikat("");
    }
  sprawdz(kto);  
}

void gpastuch(Stwor *kto)
{
  mapa_zmien(kto->poz.x,kto->poz.y,PASTUCH);
  // moze sie zepsul?
  if(losuj(1,6)==1)
    {
      mapa_zmien(kto->poz.x,kto->poz.y,PODLOGA);
      if(kto==&gracz) dodaj_komunikat("elektryczny pastuch sie zepsul.");
    }
  kto->stat[FZ]-=losuj(0,1);
  if(kto==&gracz)
    {
      dodaj_komunikat("porazil cie prad!");
      dodaj_komunikat("");
    }
  sprawdz(kto);  
}

void gpalniki(Stwor *kto)
{
  mapa_zmien(kto->poz.x,kto->poz.y,PALNIKI);
  kto->stat[FZ]-=losuj(1,2);
  if(kto==&gracz)
    {
      dodaj_komunikat("nagle otoczyla cie chmura ognia!");
    }
  dodaj_stan(kto,POPAZONY,losuj(3,23));
  sprawdz(kto);  
}


void gtujestzejscie(Stwor *kto)
{
  if(kto==&gracz)dodaj_komunikat("(tu jest szyb wiodacy w dol)");
}

void gnideryt(Stwor *kto)
{
  mapa_zmien(kto->poz.x,kto->poz.y,PODLOGA);
  kto->nideryt+=losuj(1,5);
  if(kto==&gracz) dodaj_komunikat("podniosles kawalek niderytu.");
}


void zbadaj(Stwor*kto,int x,int y)
{
  if(kto==&gracz)
    {
      dodaj_komunikat(podstawka[mapa[x][y].t].zbadaj);
      if(mapa[x][y].przedmiot)
	dodaj_komunikat(mapa[x][y].przedmiot->nazwa.ktoco);
      if(mapa[x][y].stwor)
	dodaj_komunikat(mapa[x][y].stwor->nazwa.ktoco);
      if(mapa[x][y].pojemnik)
	dodaj_komunikat("blaszany pojemnik");
    }
  kto->pa+=KOSZT_ZBADAJ;
}
