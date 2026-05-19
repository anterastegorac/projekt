#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#define MAX_NAME 50
#define BOARD_SIZE 3
#define FILENAME "igra_podaci.bin"


//koncept 9
#define IS_POINTER_NULL(p) ((p)==NULL)

//4 11
typedef enum {
	IZLAZ=0,
	IGRAJ,
	PRIKAZ_SVIH,
	PRETRAGA,
	SORTIRANJE,
	BRISANJE_DATOTEKE
}Izbornik;

typedef enum{PRAZNO=0,X,0};
Stanje;

//3 4
typedef struct {
	char ime[MAX_NAME];
	int pobjede;
	int porazi;
	union {
		float postotakPobjeda;
		int ukupnoOdigrano;
	}dodatnaStatistika;
}Igrac;

//13
typedef struct {
	Stanje ploca[BOARD_SIZE][BOARD_SIZE];
	//15 12
	Igrac* trenutniIgrac;
}krizicKruzic;

//8

extern int globalniBrojZapisa;
int globalniBrojZapisa = 0;

void CRUD_create(const Igrac* igrac);
void CRUD_read();
void CRUD_delete();
void oslobodiMemoriju(void** ptr);
void usporediIgrace(const void* a, const void* b);
void sortirajIgrace();
static inline void pozdrav() {
	printf("Dobrodosli u Krizic kruzic crud\n");
}

//6
static int sessionCounter = 0;

//18
void oslobodiMemoriju(void** ptr) {
	if (ptr != NULL && *ptr != NULL) {
		free(*ptr);
		*ptr = NULL;
	}
}

//1
void CRUD_create(const Igrac* igrac) {
	//14
	if (IS_POINTER_NULL(igrac)) return;

	FILE* fp = fopen(FILENAME, "ab");
	if (!fp) {
		perror("Greska pri otvaranju datoteke");
		return;
	}
	fwrite(igrac, sizeof(Igrac), 1, fp);
	fclose(fp);
	globalniBrojZapisa++;
}

//1 19 20
void CRUD_read() {
	FILE* fp = fopen(FILENAME, "rb");
	if (!fp) {
		if (errno == ENOENT)
			printf("Datoteka ne postoji\n");
		return;
	}
}
