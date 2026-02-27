#ifndef FICHIERS_H_INCLUDED
#define FICHIERS_H_INCLUDED
#include "structures.h"

// Noms des fichiers
#define FICHIER_POKEMONS "Pokemon.txt"
#define FICHIER_MACHINES "machines.txt"
#define FICHIER_CLIENTS "clients.txt"
#define FICHIER_COMMANDES "commandes.txt"
#define FICHIER_CAISSE "caisse.txt"  // ← NOUVEAU : Fichier pour la caisse

// Fonctions de chargement
void charger_pokemons_depuis_fichier(liste_pokemon** liste);
void charger_clients_depuis_fichier(liste_client** liste);
void charger_machines_depuis_fichier(liste_machine** liste, liste_pokemon* liste_pokemons);
void charger_commandes_depuis_fichier(liste_commande** liste, liste_client* liste_clients,
                                      liste_pokemon* liste_pokemons);
void charger_caisse_depuis_fichier(Caisse* caisse);  // ← NOUVEAU
void restaurer_commandes_en_cours(liste_machine* liste_machines, liste_commande* liste_commandes);
// Fonctions de sauvegarde
void sauvegarder_pokemons_dans_fichier(liste_pokemon* liste);
void sauvegarder_clients_dans_fichier(liste_client* liste);
void sauvegarder_machines_dans_fichier(liste_machine* liste);
void sauvegarder_commandes_dans_fichier(liste_commande* liste);
void sauvegarder_caisse_dans_fichier(Caisse* caisse);  // ← NOUVEAU

#endif // FICHIERS_H_INCLUDED
