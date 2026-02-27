#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "commande.h"
#include "client.h"
#include "machine.h"

float calculer_montant_commande(Commande* commande) {
    if (commande == NULL || commande->pokemon_commande == NULL) return 0.0;
    return commande->quantite * commande->pokemon_commande->cout_unitaire;
}

void changer_etat_commande(Commande* cmd, EtatCommande nouvel_etat) {
    if (cmd != NULL) cmd->etat = nouvel_etat;
}

Commande* trouver_commande_par_id(liste_commande* liste_commandes, int id) {
    liste_commande* courant = liste_commandes;
    while (courant != NULL) {
        if (courant->commande.identifiant == id) return &(courant->commande);
        courant = courant->suivant;
    }
    return NULL;
}

int compter_commandes(liste_commande* liste_commandes) {
    int count = 0;
    liste_commande* courant = liste_commandes;
    while (courant != NULL) {
        count++;
        courant = courant->suivant;
    }
    return count;
}

int compter_commandes_par_etat(liste_commande* liste_commandes, EtatCommande etat) {
    int count = 0;
    liste_commande* courant = liste_commandes;
    while (courant != NULL) {
        if (courant->commande.etat == etat) count++;
        courant = courant->suivant;
    }
    return count;
}

void liberer_liste_commandes(liste_commande** liste_commandes) {
    if (liste_commandes == NULL || *liste_commandes == NULL) return;
    liste_commande* courant = *liste_commandes;
    while (courant != NULL) {
        liste_commande* suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }
    *liste_commandes = NULL;
}

void supprimer_commandes_par_client(liste_commande** liste_commandes, liste_machine* liste_machines, const char* matricule) {
    if (liste_commandes == NULL || *liste_commandes == NULL || matricule == NULL) return;

    liste_commande* courant = *liste_commandes;
    liste_commande* precedent = NULL;

    while (courant != NULL) {
        if (courant->commande.client != NULL && strcmp(courant->commande.client->matricule, matricule) == 0) {
            supprimer_commande_de_toutes_files(liste_machines, courant->commande.identifiant);
            liste_commande* a_supprimer = courant;
            if (precedent == NULL) {
                *liste_commandes = courant->suivant;
                courant = *liste_commandes;
            } else {
                precedent->suivant = courant->suivant;
                courant = courant->suivant;
            }
            free(a_supprimer);
        } else {
            precedent = courant;
            courant = courant->suivant;
        }
    }
}
