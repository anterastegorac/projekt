#define _CRT_SECURE_NO_WARNINGS
#include "header.h"
#include <stdio.h>

int main() {
    int izbor;
    char ime[50];

    do {
        printf("\n--- KRIZIC KRUZIC ---\n");
        printf("1. Igraj\n");
        printf("2. Prikazi rezultate\n");
        printf("3. Pretrazi rezultat\n");
        printf("4. Obrisi povijest\n");
        printf("5. Sortiraj rezultate (A-Z)\n");
        printf("0. Izlaz\n");
        printf("Vas izbor: ");

        if (scanf("%d", &izbor) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch ((Izbornik)izbor) {
        case IGRAJ:
            pokreniIgru();
            break;
        case PRIKAZ_REZULTATA:
            CRUD_read();
            break;
        case PRETRAGA:
            printf("Unesite ime igraca: ");
            scanf("%49s", ime);
            pretraziRezultat(ime);
            break;
        case BRISANJE:
            CRUD_delete();
            break;
        case SORTIRANJE:
            sortirajIgrace();
            break;
        case IZLAZ:
            printf("Pozdrav!\n");
            break;
        default:
            printf("Nevazeca opcija.\n");
        }
    } while (izbor != IZLAZ);

    return 0;
}
