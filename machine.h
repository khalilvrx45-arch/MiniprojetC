#ifndef MACHINE_H_INCLUDED
#define MACHINE_H_INCLUDED
#include "structures.h"

void afficher_Machines(liste_machine* liste_machines);
void ajouter_Machine(liste_machine** liste_machines, liste_pokemon* liste_pokemons);
void modifier_Machine(liste_machine* liste_machines, liste_pokemon* liste_pokemons);
void supprimer_Machine(liste_machine** liste_machines, liste_commande* liste_commandes);
Machine* trouver_machine_par_id(liste_machine* liste_machines, int id);
int compter_machines(liste_machine* liste_machines);
int compter_machines_par_etat(liste_machine* liste_machines, EtatMachine etat);
void liberer_liste_machines(liste_machine** liste_machines);

#endif // MACHINE_H_INCLUDED
