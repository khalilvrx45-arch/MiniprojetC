#include "stats.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Trouve le Pokémon ayant la plus grande quantité totale vendue (commandes réalisées)
 */
Pokemon* trouver_pokemon_le_plus_vendu(liste_commande* commandes, int* qte_totale) {
    if (commandes == NULL) return NULL;

    typedef struct {
        Pokemon* ptr;
        int total;
    } PokeStats;

    PokeStats stats[100]; // Limite arbitraire pour simplifier
    int nb_types = 0;

    liste_commande* lc = commandes;
    while (lc) {
        if (lc->commande.etat == REALISEE && lc->commande.pokemon_commande != NULL) {
            int trouve = 0;
            for (int i = 0; i < nb_types; i++) {
                if (stats[i].ptr->identifiant == lc->commande.pokemon_commande->identifiant) {
                    stats[i].total += lc->commande.quantite;
                    trouve = 1;
                    break;
                }
            }
            if (!trouve && nb_types < 100) {
                stats[nb_types].ptr = lc->commande.pokemon_commande;
                stats[nb_types].total = lc->commande.quantite;
                nb_types++;
            }
        }
        lc = lc->suivant;
    }

    if (nb_types == 0) return NULL;

    int max_idx = 0;
    for (int i = 1; i < nb_types; i++) {
        if (stats[i].total > stats[max_idx].total) max_idx = i;
    }

    if (stats[max_idx].total == 0) return NULL;

    if (qte_totale) *qte_totale = stats[max_idx].total;
    return stats[max_idx].ptr;
}

/**
 * Trouve le client ayant généré le plus gros chiffre d'affaires
 */
Client* trouver_client_le_plus_rentable(liste_commande* commandes, float* ca_total) {
    if (commandes == NULL) return NULL;

    typedef struct {
        Client* ptr;
        float total_argent;
    } ClientStats;

    ClientStats stats[100];
    int nb_clients = 0;

    liste_commande* lc = commandes;
    while (lc) {
        if (lc->commande.etat == REALISEE && lc->commande.client != NULL && lc->commande.pokemon_commande != NULL) {
            int trouve = 0;
            float montant = (float)lc->commande.quantite * lc->commande.pokemon_commande->cout_unitaire;
            
            for (int i = 0; i < nb_clients; i++) {
                if (strcmp(stats[i].ptr->matricule, lc->commande.client->matricule) == 0) {
                    stats[i].total_argent += montant;
                    trouve = 1;
                    break;
                }
            }
            if (!trouve && nb_clients < 100) {
                stats[nb_clients].ptr = lc->commande.client;
                stats[nb_clients].total_argent = montant;
                nb_clients++;
            }
        }
        lc = lc->suivant;
    }

    if (nb_clients == 0) return NULL;

    int max_idx = 0;
    for (int i = 1; i < nb_clients; i++) {
        if (stats[i].total_argent > stats[max_idx].total_argent) max_idx = i;
    }

    if (stats[max_idx].total_argent <= 0.01f) return NULL;

    if (ca_total) *ca_total = stats[max_idx].total_argent;
    return stats[max_idx].ptr;
}

/**
 * Trouve la machine ayant produit le plus grand nombre de figurines au total (historique)
 */
Machine* trouver_machine_la_plus_efficace(liste_machine* machines) {
    if (machines == NULL) return NULL;

    liste_machine* lm = machines;
    Machine* top = &(lm->machine);
    
    lm = lm->suivant;
    while (lm) {
        if (lm->machine.total_produit > top->total_produit) {
            top = &(lm->machine);
        }
        lm = lm->suivant;
    }

    return top;
}
