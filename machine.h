#ifndef MACHINE_H_INCLUDED
#define MACHINE_H_INCLUDED

#include "structures.h"

// Fonctions d'affichage
void afficher_Machines(liste_machine* liste_machines);
void afficher_temps_restant(Machine* machine);
void afficher_temps_attente_file(Machine* machine);

// Fonctions CRUD
void ajouter_Machine(liste_machine** liste_machines, liste_pokemon* liste_pokemons);
void modifier_Machine(liste_machine* liste_machines, liste_pokemon* liste_pokemons);
void supprimer_Machine(liste_machine** liste_machines);
Machine* trouver_machine_par_id(liste_machine* liste_machines, int id);
Machine* trouver_machine_libre_pour_pokemon(liste_machine* liste_machines, Pokemon* pokemon);

// Gestion de la production
void lancer_production(Machine* machine, Commande* commande, int quantite_a_produire);
void terminer_production(Machine* machine, Caisse* caisse);
void effectuer_maintenance(Machine* machine, Caisse* caisse);
int production_terminee(Machine* machine);
int maintenance_terminee(Machine* machine);

// Gestion de la file d'attente
void ajouter_a_file(Machine* machine, Commande* commande);
Commande* retirer_de_file(Machine* machine);
int simuler_temps_et_compteur(Machine* machine, int quantite, int compteur_depart, int* compteur_final);

// Automates
void mettre_a_jour_machines(liste_machine* liste_machines, Caisse* caisse, liste_commande* liste_commandes);

// Utilitaires
int compter_machines(liste_machine* liste_machines);
int compter_machines_par_etat(liste_machine* liste_machines, EtatMachine etat);
void liberer_liste_machines(liste_machine** liste_machines);

// Nettoyage lors des suppressions
void supprimer_commande_de_toutes_files(liste_machine* liste, int id_commande);
void liberer_commandes_machine_supprimee(Machine* m);
void supprimer_machines_par_pokemon(liste_machine** liste_machines, int id_pokemon);

#endif // MACHINE_H_INCLUDED
