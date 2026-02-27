#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pokemon.h"
#include "machine.h"

Pokemon* trouver_pokemon_par_id(liste_pokemon* liste, int id) {
    liste_pokemon* courant = liste;
    while (courant != NULL) {
        if (courant->pokemon.identifiant == id) {
            return &(courant->pokemon);
        }
        courant = courant->suivant;
    }
    return NULL;
}

int compter_pokemons(liste_pokemon* liste) {
    int c = 0;
    while(liste) {
        c++;
        liste = liste->suivant;
    }
    return c;
}

void liberer_liste_pokemons(liste_pokemon** liste) {
    if (liste == NULL || *liste == NULL) return;
    liste_pokemon* courant = *liste;
    while(courant) {
        liste_pokemon* temp = courant;
        courant = courant->suivant;
        free(temp);
    }
    *liste = NULL;
}
