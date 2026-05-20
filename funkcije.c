#define _CRT_SECURE_NO_WARNINGS
#include "header.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static int trenutniBrojZapisa = 0;
int brojZapisa = 0;

void oslobodiMemoriju(void** ptr) {
	if (ptr != NULL && *ptr != NULL) {
		free(*ptr);
		*ptr = NULL;
	}
}

void CRUD_create(const Statistika* s) {
	if (s == NULL)return;
	FILE* fp = fopen(FILENAME, "ab");
	if (fp == NULL) {
		perror("Pogreska pri otvaranju datoteke");
		return;
	}
	fwrite(s, sizeof(Statistika), 1, fp);
	fclose(fp);
}

void CRUD_read() {
	FILE* fp = fopen(FILENAME, "rb");
	if (!fp) return;

	Statistika temp;
	printf("\n---POVJEST REZULTATA---\n");
	while (fread(&temp, sizeof(Statistika), 1, fp) == 1) {
		printf("Igrac:%-15s|P:%d|I:%d\n", temp.ime, temp.pobjede, temp.porazi);
	}
	fclose(fp);
}
int binaraPretragaRekurzivna(Statistika* niz, int lijevo, int desno, const char* ime) {
	if (desno >= lijevo) {
		int sredina = lijevo + (desno - lijevo) / 2;
		int res = strcmp(niz[sredina].ime, ime);
		if (res == 0)return sredina;
		if (res > 0)return binaraPretragaRekurzivna(niz, lijevo, sredina-1, ime);
		return binaraPretragaRekurzivna(niz, sredina + 1, desno, ime);
	}
	return -1;
}
int sortirajRezultate() {
	FILE* fp = fopen(FILENAME, "rb");
	if (!fp) return;

	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	rewind(fp);

	int n = size / sizeof(Statistika);
	Statistika* niz = (Statistika*)malloc(n * sizeof(Statistika));

	if (niz == NULL) {
		fclose(fp);
		return;
	}
	fread(niz, sizeof(Statistika), n, fp);
	qsort(niz, n, sizeof(Statistika), usporediStatistiku);
	fclose(fp);

	oslobodiMemoriju((void**)&niz);
	printf("Podaci sortirani.\n");
}

void pokreniIgru() {
	Statistika* s1 = (Statistika*)calloc(1, sizeof(Statistika));
	printf("Unesite ime igraca X:");
	scanf("%49s", s1->ime);

	printf("\nIgra pocinje(simulacija pobjede %s)...\n", s1->ime);
	s1->pobjede = 1;

	CRUD_create(s1);
	oslobodiMemoriju((void**)&s1);
}

void CRUD_delete() {
	if (remove(FILENAME) == 0) {
		printf("Datoteka obrisana.\n");
	}
	else {
		perror("Pogreska pri brisanju");
	}
}
