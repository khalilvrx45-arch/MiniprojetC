#ifndef STATS_H_INCLUDED
#define STATS_H_INCLUDED

#include "structures.h"

// Fontions statistiques
Pokemon* trouver_pokemon_le_plus_vendu(liste_commande* commandes, int* qte_totale);
Client* trouver_client_le_plus_rentable(liste_commande* commandes, float* ca_total);
Machine* trouver_machine_la_plus_efficace(liste_machine* machines);

#endif // STATS_H_INCLUDED
