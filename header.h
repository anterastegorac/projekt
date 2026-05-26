#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>

#define MAX_NAME 50
#define BOARD_SIZE 3
#define FILENAME "rezultati.bin"

typedef enum { IZLAZ = 0, IGRAJ, PRIKAZ_REZULTATA, PRETRAGA, BRISANJE, SORTIRANJE }Izbornik;
typedef enum { PRAZNO = 0, IGRAC_X, IGRAC_O }Polje;

typedef struct {
	char ime[MAX_NAME];
	int pobjede;
	int porazi;
}Statistika;

typedef struct {
	Polje ploca[BOARD_SIZE][BOARD_SIZE];
	Statistika* igrac1;
	Statistika* igrac2;
}Igra;

extern int brojZapisa;

void pokreniIgru(void);
void inicijalizirajPlocu(Igra* igra);
void prikaziPlocu(const Igra* igra);
int provjeriPobjednika(const Igra* igra, Polje igrac);
int provjeriNerjeseno(const Igra* igra);
void spremiRezultat(Statistika* pobjednik, Statistika* gubitnik);
void CRUD_create(const Statistika* s);
void CRUD_read(void);
void CRUD_update(const char* ime, int pobjeda);
void CRUD_delete(void);
int usporediStatistiku(const void* a, const void* b);
void sortirajIgrace(void);
void pretraziRezultat(const char* ime);
void oslobodiMemoriju(void** ptr);

static inline void ocistiEkran() {
	printf("\033[H\033[J");
}

#endif
