#ifndef POKEMON_H_INCLUDED
#define POKEMON_H_INCLUDED

#include "structures.h"

// Pokemon function prototypes
void Afficher_tous_pokemons(Pokemon tab[], int nb_pokemon);
void Afficher_Pokemon(Pokemon tab[], int nb_pokemon, int id);
void Ajouter_Pokemon(Pokemon pokemons[], int *nb_pokemons, int max_pokemons);
void Modifier_Pokemon(Pokemon pokemons[], int nb_pokemons);
void Supprimer_Pokemon(int id, Pokemon tab[], int *nb_pokemons);

#endif // POKEMON_H_INCLUDED
