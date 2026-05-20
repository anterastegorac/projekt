#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>

#define MAX_NAME 50
#define BOARD_SIZE 3
#define FILENAME "rezultati.bin"

typedef enum {ILZAZ=0,IGRAJ,PRIKAZ_REZULTATA,PRETRAGA,BRISANJE,SORTIRANJE}Izbornik;
typedef enum {PRAZNO=0,IGRAC_X,IGRAC_O}Polje;
 
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

void prikaziIzbornik(void);
void pokreniIgru(void);
void CRUD_create(const Statistika* s);
void CRUD_read(void);
void CRUD_update(const char* ime, int pobjeda);
void CRUD_delete(void);
int usporediStatistiku(const void* a, const void* b);
void sortirajRezultate(void);
void pretraziRezultat(const char* ime);
void oslobodiMemoriju(void** ptr);

static inline void ocistiEkran() {
	printf("\033[H\033[J");
}

#endif
