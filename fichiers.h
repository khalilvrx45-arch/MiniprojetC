#ifndef FICHIERS_H_INCLUDED
#define FICHIERS_H_INCLUDED

#include "structures.h"

// Noms des fichiers
#define FICHIER_POKEMONS "Pokemon.txt"
#define FICHIER_MACHINES "machines.txt"
#define FICHIER_CLIENTS "clients.txt"
#define FICHIER_COMMANDES "commandes.txt"

// Fonctions de chargement
void charger_pokemons_depuis_fichier(liste_pokemon** liste);
void charger_clients_depuis_fichier(liste_client** liste);
void charger_machines_depuis_fichier(liste_machine** liste, liste_pokemon* liste_pokemons);
void charger_commandes_depuis_fichier(liste_commande** liste, liste_client* liste_clients,
                                      liste_pokemon* liste_pokemons);

// Fonctions de sauvegarde
void sauvegarder_pokemons_dans_fichier(liste_pokemon* liste);
void sauvegarder_clients_dans_fichier(liste_client* liste);
void sauvegarder_machines_dans_fichier(liste_machine* liste);
void sauvegarder_commandes_dans_fichier(liste_commande* liste);

#endif // FICHIERS_H_INCLUDED
