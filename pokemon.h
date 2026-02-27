#ifndef POKEMON_H
#define POKEMON_H

#include "structures.h"

// Recherche et statistiques
Pokemon* trouver_pokemon_par_id(liste_pokemon* liste, int id);
int compter_pokemons(liste_pokemon* liste);

// Nettoyage memoire
void liberer_liste_pokemons(liste_pokemon** liste);

#endif
