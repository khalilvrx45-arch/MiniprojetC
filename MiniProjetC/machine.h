#ifndef MACHINE_H_INCLUDED
#define MACHINE_H_INCLUDED

#include "structures.h"

#define MAX_MACHINES 50

// Machine function prototypes
void afficher_Machines(Machine machines[], int nbMachines);
void ajouter_Machine(Machine machines[], int* nbMachines, Pokemon pokemons[], int nbPokemons);
void modifier_Machine(Machine machines[], int nbMachines, Pokemon pokemons[], int nbPokemons);
void supprimer_Machine(Machine machines[], int* nbMachines);

#endif // MACHINE_H_INCLUDED
