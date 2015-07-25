#ifndef _NNGDS33_
#define _NNGDS33_

#include<stdlib.h>
#include<setjmp.h>
#include<curses.h>

jmp_buf ___Kosmos;

enum{CZERN,BIEL,ZOLC,KARMAZYN,ZIELEN,CYJAN,CYKLAMEN,INDYGO};
typedef struct{char znak;int kolor;}Sym;

enum{NIE,TAK};
typedef unsigned char TakNie;

enum{MAX_NAZWA=48,MAX_ZBADAJ=68};
typedef struct{char ktoco[MAX_NAZWA],kogoco[MAX_NAZWA],kogoczego[MAX_NAZWA];
               TakNie zenska,mnoga;}Nazwa;

typedef struct{int x,y;}Poz;

enum{FZ,FZ_MAX,SI,KB,KP,UDZWIG,R_WIDZENIA,SZYBKOSC,REGENERACJA,MAX_STAT};
typedef int Stat[MAX_STAT];

enum{PRZYSPIESZONY,SPOWOLNIONY,POPAZONY,OSLEPIONY,DUZE_ZRENICE,SILNIEJSZY,
SLABSZY,SZYBKOZDROWIEJE,ZATRUTY,ZDEZORIENTOWANY,MAX_STAN};
struct op_stan
{
  Stat mod;
  char oznaczenie[10]; // powaznie sie streszczac w paru znakach!
  char poczatek[79]; // magiczne sryczne.
  char koniec[79]; // liczby sryczby.
}opis_stanu[MAX_STAN];

typedef struct Stan Stan;
struct Stan
{
  int typ;
  int ile;
  Stan *nast,*poprz;
};

typedef struct Stwor Stwor;

enum{BRON,PANCERZ,HELM,REKAWICE,RAJTUZY,TARCZA,SWIATLO,TORBA,INNY};
typedef struct Przedmiot Przedmiot;
struct Przedmiot
{
  Sym sym;
  Nazwa nazwa;
  int waga;
  int typ;
  union
  {
    struct{int zuzycie;
           Stat mod;}ns;
    struct{int ilosc;
           void (*dzialanie)(Stwor *kto);
           TakNie odkryty;}in;
  };
  void (*trafiony)(Stwor *kto);
  Przedmiot *nast,*poprz;
};

#define MAX_SLOT INNY
struct Stwor
{
  Sym sym;
  Nazwa nazwa;
  Poz poz;
  Stat stat;
  Stan *stany;
  Przedmiot *ekwipunek,*sloty[MAX_SLOT];
  int pa,zdrowie,obciazenie;
  int dsw,p_wtaj;
  int nideryt;
  Stwor *nast;
}gracz,*lista_potworow;
int g_poziom;

typedef struct Pocisk Pocisk;
struct Pocisk
{
  Poz poz,d;
  int ile;
  Przedmiot *przedmiot;
  TakNie wybucha,tluczesie;
  Pocisk *nast,*poprz;
} *lista_pociskow;

typedef struct
{
  Sym sym;
  Poz poz;
  TakNie wybucha;
}Pojemnik;

typedef struct
        {
	  Sym sym;
	  Nazwa nazwa;
  	  char zbadaj[MAX_ZBADAJ];
	  TakNie przepuszcza;
	  void (*akcja)(Stwor *kto,int x,int y);
	  void (*wlezienie)(Stwor *kto);
        }Teren;
enum{NIC,SCIANA_H,SCIANA_W,UWLAZ_H,UWLAZ_W,WLAZ_Z,WLAZ_O,PODLOGA,KORYTARZ,
       NIDERYT,ZEJSCIE,FONTANNA,
       PIACH,SKALA,
       MASZYNA_PRZENIESIEN,PYROWID,
       _ZAPADNIA,ZAPADNIA,_KOLCE,KOLCE,
     _OSTRZA,OSTRZA,_PASTUCH,PASTUCH,_PALNIKI,PALNIKI,MAX_PODST};
Teren podstawka[MAX_PODST];

enum{SZE=78,WYS=21};
struct
{
  Sym sym;
  int t;
  Pocisk *pocisk;
  Pojemnik *pojemnik;
  Przedmiot *przedmiot;
  Stwor *stwor;
}mapa[SZE][WYS];

void inicjuj_mape();
void nowa_plansza();
void widz();
void zobacz(int,int);
void rysuj();
int wejscie();
Poz wez_kierunek(int);
TakNie mozna_wejsc(int,int);
int a_czekaj(Stwor *);
int a_idz(Stwor *,int,int);
int a_atak(Stwor *,int,int);
void zbadaj(Stwor*,int,int);
int akcja(Stwor *,int,int);
int a_zejdz(Stwor *);
int a_podnies(Stwor *);
int a_wyrzuc(Stwor *);
int a_uzyj(Stwor *);
int a_rzuc(Stwor *);
void inicjuj_komunikaty();
void dodaj_komunikat(char *);
void wypisz_komunikaty();
void kom_otworzyles_wlaz();
void kom_zamknales_wlaz();
void kom_nizszy_poziom();
void kom_przeniesienie();
void kom_wtajemn(int);
void kom_cos_lezy(Nazwa);
void kom_podniosles(Nazwa);
void kom_nie_ma_co_podniesc();
void kom_wyrzuciles(Nazwa);
void kom_nie_mozesz_wyrzucic(Nazwa);
void kom_zaczales_uzywac(Nazwa);
void kom_przestales_uzywac(Nazwa);
void kom_juz_uzywasz(Nazwa,Nazwa);
void kom_zuzylo_sie(Przedmiot *);
void kom_zabiles(Nazwa);
void kom_zabil_cie(Nazwa);
void kom_zadales_cios(Nazwa,int);
void kom_otrzymales_cios(Nazwa,int);
void kom_chybiles(Nazwa);
void kom_stwor_chybil(Nazwa);
int losuj(int,int);
void inicjuj_losuj();
void sprawdz(Stwor *);
void sprawdz_doswiadczenie();
void sprawdz_stany(Stwor *);
void dodaj_stan(Stwor *,int,int);
void dodaj_przedmiot(int,int,Przedmiot *);
void usun_z_ekwipunku(Stwor *,Przedmiot *);
Przedmiot *wymysl_przedmiot();
Przedmiot *klonuj_przedmiot(Przedmiot *);
void podnies(Stwor *);
void wyrzuc(Stwor *,Przedmiot *);
void uzyj(Stwor *,Przedmiot *);
void pisz_ekw();
void ekwipunek();
void cykl_stwora(Stwor *);
void cykl_pocisku(Pocisk *);
void stworz_potwora(int,int);
void usun_stwora(Stwor *);
void zabij_stwora(Stwor *);
Pocisk *nowy_pocisk(Przedmiot *,int,int,int,int,int,TakNie,TakNie);
void zakoncz_pocisk(Pocisk *);
void eksplozja(int,int);
void dodaj_pojemnik(int,int,TakNie);
void usun_pojemnik(Pojemnik *);
void rzuc(Stwor *,Przedmiot *,int,int,int);
void decydent(Stwor *);
void tasuj_kolory_pigulek();
void smierc();
void tytulowa();
void pomoc();
void mand();
void koniec(int);
#endif
