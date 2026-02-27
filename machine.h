#ifndef MACHINE_H
#define MACHINE_H

#include "structures.h"

// Recherche et statistiques
Machine* trouver_machine_par_id(liste_machine* liste_machines, int id);
Machine* trouver_machine_libre_pour_pokemon(liste_machine* liste_machines, Pokemon* pokemon);
int compter_machines(liste_machine* liste_machines);

// Production et maintenance
void lancer_production(Machine* machine, Commande* commande, int quantite_a_produire);
void terminer_production(Machine* machine, Caisse* caisse);
void effectuer_maintenance(Machine* machine, Caisse* caisse);
int production_terminee(Machine* machine);
int maintenance_terminee(Machine* machine);

// File d'attente et simulation
void ajouter_a_file(Machine* machine, Commande* commande);
Commande* retirer_de_file(Machine* machine);
int simuler_temps_et_compteur(Machine* machine, int quantite, int compteur_depart, int* compteur_final);

// Mise a jour et nettoyage
void mettre_a_jour_machines(liste_machine* liste_machines, Caisse* caisse, liste_commande* liste_commandes);
void liberer_liste_machines(liste_machine** liste_machines);
void supprimer_commande_de_toutes_files(liste_machine* liste, int id_commande);
void liberer_commandes_machine_supprimee(Machine* m);
void supprimer_machines_par_pokemon(liste_machine** liste_machines, int id_pokemon);

#endif
