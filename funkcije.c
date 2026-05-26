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

int usporediStatistiku(const void* a, const void* b) {
	const Statistika* sa = (const Statistika*)a;
	const Statistika* sb = (const Statistika*)b;
	return strcmp(sa->ime, sb->ime);
}

int binaraPretragaRekurzivna(Statistika* niz, int lijevo, int desno, const char* ime) {
	if (desno >= lijevo) {
		int sredina = lijevo + (desno - lijevo) / 2;
		int res = strcmp(niz[sredina].ime, ime);
		if (res == 0)return sredina;
		if (res > 0)return binaraPretragaRekurzivna(niz, lijevo, sredina - 1, ime);
		return binaraPretragaRekurzivna(niz, sredina + 1, desno, ime);
	}
	return -1;
}

void sortirajIgrace(void) {
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
	fclose(fp);

	qsort(niz, n, sizeof(Statistika), usporediStatistiku);

	fp = fopen(FILENAME, "wb");
	if (fp != NULL) {
		fwrite(niz, sizeof(Statistika), n, fp);
		fclose(fp);
	}

	oslobodiMemoriju((void**)&niz);
	printf("Podaci sortirani.\n");
}

void pretraziRezultat(const char* ime) {
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
	fclose(fp);

	qsort(niz, n, sizeof(Statistika), usporediStatistiku);

	int idx = binaraPretragaRekurzivna(niz, 0, n - 1, ime);
	if (idx != -1) {
		printf("Igrac:%-15s|P:%d|I:%d\n", niz[idx].ime, niz[idx].pobjede, niz[idx].porazi);
	}
	else {
		printf("Igrac '%s' nije pronaden.\n", ime);
	}

	oslobodiMemoriju((void**)&niz);
}

void CRUD_update(const char* ime, int pobjeda) {
	FILE* fp = fopen(FILENAME, "r+b");
	if (!fp) return;

	Statistika temp;
	while (fread(&temp, sizeof(Statistika), 1, fp) == 1) {
		if (strcmp(temp.ime, ime) == 0) {
			if (pobjeda) temp.pobjede++;
			else temp.porazi++;
			fseek(fp, -(long)sizeof(Statistika), SEEK_CUR);
			fwrite(&temp, sizeof(Statistika), 1, fp);
			fclose(fp);
			return;
		}
	}
	fclose(fp);
}

/* --- LOGIKA IGRE --- */

void inicijalizirajPlocu(Igra* igra) {
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			igra->ploca[i][j] = PRAZNO;
}

void prikaziPlocu(const Igra* igra) {
	printf("\n");
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			char c = '.';
			if (igra->ploca[i][j] == IGRAC_X) c = 'X';
			else if (igra->ploca[i][j] == IGRAC_O) c = 'O';
			printf(" %c ", c);
			if (j < BOARD_SIZE - 1) printf("|");
		}
		printf("\n");
		if (i < BOARD_SIZE - 1) printf("---|---|---\n");
	}
	printf("\n");
}

int provjeriPobjednika(const Igra* igra, Polje igrac) {
	/* Redovi i stupci */
	for (int i = 0; i < BOARD_SIZE; i++) {
		int red = 1, stupac = 1;
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (igra->ploca[i][j] != igrac) red = 0;
			if (igra->ploca[j][i] != igrac) stupac = 0;
		}
		if (red || stupac) return 1;
	}
	/* Dijagonale */
	int dijag1 = 1, dijag2 = 1;
	for (int i = 0; i < BOARD_SIZE; i++) {
		if (igra->ploca[i][i] != igrac) dijag1 = 0;
		if (igra->ploca[i][BOARD_SIZE - 1 - i] != igrac) dijag2 = 0;
	}
	return dijag1 || dijag2;
}

int provjeriNerjeseno(const Igra* igra) {
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			if (igra->ploca[i][j] == PRAZNO) return 0;
	return 1;
}

void spremiRezultat(Statistika* pobjednik, Statistika* gubitnik) {
	/* Pokusaj azurirati postojeci zapis, inace spremi novi */
	FILE* fp = fopen(FILENAME, "r+b");
	int nadjenPobjednik = 0, nadjenGubitnik = 0;
	Statistika temp;

	if (fp) {
		while (fread(&temp, sizeof(Statistika), 1, fp) == 1) {
			if (strcmp(temp.ime, pobjednik->ime) == 0) {
				temp.pobjede++;
				fseek(fp, -(long)sizeof(Statistika), SEEK_CUR);
				fwrite(&temp, sizeof(Statistika), 1, fp);
				nadjenPobjednik = 1;
			}
			else if (strcmp(temp.ime, gubitnik->ime) == 0) {
				temp.porazi++;
				fseek(fp, -(long)sizeof(Statistika), SEEK_CUR);
				fwrite(&temp, sizeof(Statistika), 1, fp);
				nadjenGubitnik = 1;
			}
		}
		fclose(fp);
	}

	if (!nadjenPobjednik) {
		pobjednik->pobjede = 1;
		pobjednik->porazi = 0;
		CRUD_create(pobjednik);
	}
	if (!nadjenGubitnik) {
		gubitnik->pobjede = 0;
		gubitnik->porazi = 1;
		CRUD_create(gubitnik);
	}
}

void pokreniIgru() {
	Igra igra;
	Statistika* s1 = (Statistika*)calloc(1, sizeof(Statistika));
	Statistika* s2 = (Statistika*)calloc(1, sizeof(Statistika));

	if (s1 == NULL || s2 == NULL) {
		oslobodiMemoriju((void**)&s1);
		oslobodiMemoriju((void**)&s2);
		return;
	}

	printf("Unesite ime igraca X: ");
	scanf("%49s", s1->ime);
	printf("Unesite ime igraca O: ");
	scanf("%49s", s2->ime);

	igra.igrac1 = s1;
	igra.igrac2 = s2;
	inicijalizirajPlocu(&igra);

	int potez = 0;
	int ukupnoPoteza = BOARD_SIZE * BOARD_SIZE;

	while (potez < ukupnoPoteza) {
		Polje trenutniIgrac = (potez % 2 == 0) ? IGRAC_X : IGRAC_O;
		Statistika* trenutnaStatistika = (potez % 2 == 0) ? s1 : s2;
		char simbol = (trenutniIgrac == IGRAC_X) ? 'X' : 'O';

		prikaziPlocu(&igra);
		printf("Potez igraca %s (%c)\n", trenutnaStatistika->ime, simbol);

		int red, stupac;
		printf("Unesite redak (1-3) i stupac (1-3): ");
		if (scanf("%d %d", &red, &stupac) != 2) {
			while (getchar() != '\n');
			printf("Neispravan unos!\n");
			continue;
		}
		red--; stupac--;

		if (red < 0 || red >= BOARD_SIZE || stupac < 0 || stupac >= BOARD_SIZE) {
			printf("Koordinate izvan ploce!\n");
			continue;
		}
		if (igra.ploca[red][stupac] != PRAZNO) {
			printf("Polje je vec zauzeto!\n");
			continue;
		}

		igra.ploca[red][stupac] = trenutniIgrac;
		potez++;

		if (provjeriPobjednika(&igra, trenutniIgrac)) {
			prikaziPlocu(&igra);
			printf("Igrac %s (%c) je pobijedio!\n", trenutnaStatistika->ime, simbol);
			Statistika* gubitnik = (trenutniIgrac == IGRAC_X) ? s2 : s1;
			spremiRezultat(trenutnaStatistika, gubitnik);
			oslobodiMemoriju((void**)&s1);
			oslobodiMemoriju((void**)&s2);
			return;
		}

		if (provjeriNerjeseno(&igra)) {
			prikaziPlocu(&igra);
			printf("Nerijesen rezultat!\n");
			oslobodiMemoriju((void**)&s1);
			oslobodiMemoriju((void**)&s2);
			return;
		}
	}

	oslobodiMemoriju((void**)&s1);
	oslobodiMemoriju((void**)&s2);
}

void CRUD_delete() {
	if (remove(FILENAME) == 0) {
		printf("Datoteka obrisana.\n");
	}
	else {
		perror("Pogreska pri brisanju");
	}
}
