#ifndef POKEMON_H_INCLUDED
#define POKEMON_H_INCLUDED

#include "structures.h"

// Prototypes des fonctions

void afficher_Pokemons(liste_pokemon* liste);
void afficher_un_Pokemon(Pokemon* p);
void ajouter_Pokemon(liste_pokemon** liste);
void modifier_Pokemon(liste_pokemon* liste);
void supprimer_Pokemon(liste_pokemon** liste, liste_machine** liste_machines);
Pokemon* trouver_pokemon_par_id(liste_pokemon* liste, int id);
int compter_pokemons(liste_pokemon* liste);
void liberer_liste_pokemons(liste_pokemon** liste);

#endif // POKEMON_H_INCLUDED
