#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "structures.h"
#include "pokemon.h"
#include "machine.h"
#include "client.h"
#include "commande.h"
#include "fichiers.h"
#include "stats.h"

// États de navigation
typedef enum {
    SCREEN_DASHBOARD,
    SCREEN_POKEMONS,
    SCREEN_MACHINES,
    SCREEN_CLIENTS,
    SCREEN_COMMANDES
} GuiScreen;

// Variables formulaires UI (Raygui)
bool showPokeModal = false;
char pokeNameInput[50] = "";
bool pokeNameEditMode = false;
int pokeTypeActive = 0; // 0=Electrique, 1=Feu, 2=Eau, 3=Plante
bool pokeTypeEditMode = false;
float pokeCostInput = 10.0f;
char pokeCostText[20] = "10.0";
bool pokeCostEditMode = false;
int editingPokeId = -1;  // -1 = nouveau, sinon ID existant
bool showConfirmDelete = false;
bool showNoMachineWarning = false; // Alerte si pas de machine
int deleteType = 0; // 0=Pkmn, 1=Mach, 2=Client, 3=Cmd
int deleteId = -1;
char deleteStrId[50] = "";

// Variables Globales SIMPLIFIEES
Caisse g_caisse = {0};
liste_pokemon* g_pokemons = NULL;
liste_machine* g_machines = NULL;
liste_client* g_clients = NULL;
liste_commande* g_commandes = NULL;
GuiScreen g_currentScreen = SCREEN_DASHBOARD;
bool g_forceQuit = false;

// Nouvelles couleurs "Pokemon Factory V2"
#define COLOR_BG        (Color){ 7, 10, 24, 255 }      // Deep Dark Blue
#define COLOR_CARD      (Color){ 15, 20, 35, 255 }     // Dark Panel Blue
#define COLOR_CARD_LINE (Color){ 242, 24, 56, 255 }    // Pokeball Red (#f21838)
#define COLOR_ACCENT    (Color){ 242, 24, 56, 255 }    // Pokeball Red
#define COLOR_WARNING   (Color){ 255, 204, 0, 255 }    // Pokemon Yellow
#define COLOR_SUCCESS   (Color){ 0, 255, 102, 255 }    // Green
#define COLOR_HEADER    (Color){ 4, 6, 16, 255 }       // Almost Black Blue
#define COLOR_TEXT_DIM  (Color){ 120, 130, 150, 255 }

// Prototypes
void DrawLayout();
void DrawDashboard();
void DrawPokemons();
void DrawMachines();
void DrawClients();
void DrawCommandes();
void DrawLayout();
void UpdateSimulation();

// Helper point grid
void DrawDotGrid(Rectangle bounds, int spacing, Color color) {
    for (int x = bounds.x; x < bounds.x + bounds.width; x += spacing) {
        for (int y = bounds.y; y < bounds.y + bounds.height; y += spacing) {
            DrawPixel(x, y, color);
        }
    }
}

int main(void) {
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "POKEMON FACTORY - Control Center");

    // Stylisation globale Raygui pour correspondre au design Red/Dark Blue
    GuiLoadStyleDefault();
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0x070A18FF);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xFFFFFFFF);
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, 0xF21838FF); // Red
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, 0x0F1423FF);
    GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, 0xF21838FF);
    GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, 0x070A18FF);
    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, 0x808080FF);
    GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, 0xF21838FF);
    GuiSetStyle(DEFAULT, BORDER_COLOR_PRESSED, 0xFFFFFFFF);

    // Chargement des données (comme dans main.c)
    charger_caisse_depuis_fichier(&g_caisse);
    charger_pokemons_depuis_fichier(&g_pokemons);
    charger_clients_depuis_fichier(&g_clients);
    charger_machines_depuis_fichier(&g_machines, g_pokemons);
    charger_commandes_depuis_fichier(&g_commandes, g_clients, g_pokemons);
    restaurer_commandes_en_cours(g_machines, g_commandes);

    SetTargetFPS(60);

    while (!WindowShouldClose() && !g_forceQuit) {
        // 1. Logique de simulation (Background update)
        UpdateSimulation();

        // 2. Dessin
        BeginDrawing();
            ClearBackground(COLOR_BG); // CLEAR THE FRAME BUFFER FIRST!

            // Dessiner la grille de fond
            DrawDotGrid((Rectangle){90, 70, screenWidth - 90, screenHeight - 130}, 20, (Color){ 30, 30, 40, 255 });

            switch(g_currentScreen) {
                case SCREEN_DASHBOARD: DrawDashboard(); break;
                case SCREEN_POKEMONS:  DrawPokemons();  break;
                case SCREEN_MACHINES:  DrawMachines();  break;
                case SCREEN_CLIENTS:   DrawClients();   break;
                case SCREEN_COMMANDES: DrawCommandes(); break;
                default: break;
            }

            // Avertissement Machine non trouvée
            if (showNoMachineWarning) {
                DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 150});
                Rectangle m = { screenWidth/2.0f - 180, screenHeight/2.0f - 100, 360, 180 };
                DrawRectangleRec(m, COLOR_BG);
                DrawRectangleLinesEx(m, 2, COLOR_ACCENT);
                DrawText("ERREUR PRODUCTION", (int)m.x + 50, (int)m.y + 30, 20, WHITE);
                DrawText("Aucune machine n'est configuree", (int)m.x + 30, (int)m.y + 70, 16, LIGHTGRAY);
                DrawText("pour produire ce Pokemon !", (int)m.x + 30, (int)m.y + 90, 16, LIGHTGRAY);
                
                if (GuiButton((Rectangle){m.x + 120, m.y + 120, 120, 35}, "COMPRIS")) showNoMachineWarning = false;
            }

            // Dessiner l'enveloppe par-dessus le contenu (Header, Sidebar, Navbar)
            DrawLayout();

            // --- FENETRES MODALES GLOBALES (Doivent être dessinées en dernier) ---
            
            // 1. Modal de confirmation de suppression (Global)
            if (showConfirmDelete) {
                DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 180});
                Rectangle m = { screenWidth/2.0f - 180, screenHeight/2.0f - 90, 360, 180 };
                DrawRectangleRec(m, COLOR_BG);
                DrawRectangleLinesEx(m, 2, COLOR_ACCENT);
                DrawText("CONFIRMER SUPPRESSION ?", (int)m.x + 45, (int)m.y + 40, 18, WHITE);
                DrawText("Cette action est irreversible.", (int)m.x + 60, (int)m.y + 70, 12, LIGHTGRAY);
                
                if (GuiButton((Rectangle){m.x + 50, m.y + 110, 120, 40}, "ANNULER")) showConfirmDelete = false;
                if (GuiButton((Rectangle){m.x + 190, m.y + 110, 120, 40}, "SUPPRIMER")) {
                    if (deleteType == 0) { // Pokemon
                        supprimer_machines_par_pokemon(&g_machines, deleteId);
                        liste_pokemon* prev = NULL; liste_pokemon* curr = g_pokemons;
                        while(curr) {
                            if(curr->pokemon.identifiant == deleteId) {
                                if(prev) prev->suivant = curr->suivant; else g_pokemons = curr->suivant;
                                free(curr); break;
                            }
                            prev = curr; curr = curr->suivant;
                        }
                    } else if (deleteType == 1) { // Machine
                        liste_machine* prev = NULL; liste_machine* curr = g_machines;
                        while(curr) {
                            if(curr->machine.identifiant == deleteId) {
                                liberer_commandes_machine_supprimee(&curr->machine);
                                if(prev) prev->suivant = curr->suivant; else g_machines = curr->suivant;
                                free(curr);
                                break;
                            }
                            prev = curr; curr = curr->suivant;
                        }
                    } else if (deleteType == 2) { // Client
                        supprimer_commandes_par_client(&g_commandes, g_machines, deleteStrId);
                        liste_client* prev = NULL; liste_client* curr = g_clients;
                        while(curr) {
                            if(strcmp(curr->client.matricule, deleteStrId) == 0) {
                                if(prev) prev->suivant = curr->suivant; else g_clients = curr->suivant;
                                free(curr); break;
                            }
                            prev = curr; curr = curr->suivant;
                        }
                    } else if (deleteType == 3) { // Commande
                        supprimer_commande_de_toutes_files(g_machines, deleteId);
                        liste_commande* prev = NULL; liste_commande* curr = g_commandes;
                        while(curr) {
                            if(curr->commande.identifiant == deleteId) {
                                if(prev) prev->suivant = curr->suivant; else g_commandes = curr->suivant;
                                free(curr); break;
                            }
                            prev = curr; curr = curr->suivant;
                        }
                    }
                    showConfirmDelete = false;
                }
            }

        EndDrawing();
    }

    // Sauvegarde et nettoyage
    sauvegarder_pokemons_dans_fichier(g_pokemons);
    sauvegarder_clients_dans_fichier(g_clients);
    sauvegarder_machines_dans_fichier(g_machines);
    sauvegarder_commandes_dans_fichier(g_commandes);
    sauvegarder_caisse_dans_fichier(&g_caisse);

    CloseWindow();
    return 0;
}

void UpdateSimulation() {
    static float timer = 0;
    timer += GetFrameTime();
    if (timer >= 1.0f) { // Mise à jour toutes les secondes
        mettre_a_jour_machines(g_machines, &g_caisse, g_commandes);
        timer = 0;
    }
}

void DrawLayout() {
    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();

    // 1. Header (Top)
    DrawRectangle(0, 0, screenW, 70, COLOR_HEADER);
    DrawLine(0, 70, screenW, 70, COLOR_ACCENT); // Pokeball Red divider
    
    // Logo "Pokeball" simple
    DrawCircle(40, 35, 20, WHITE);
    DrawCircleSector((Vector2){40, 35}, 20, 180, 360, 36, COLOR_ACCENT); // Red top half
    DrawRectangle(20, 33, 40, 4, COLOR_HEADER); // Center line
    DrawCircle(40, 35, 6, WHITE);
    DrawCircleLines(40, 35, 6, COLOR_HEADER);
    
    DrawText("FACTORY", 80, 25, 24, WHITE);
    DrawText("CENTER", 200, 25, 24, COLOR_ACCENT);
    
    // Solde Caisse
    DrawText("SOLDE CAISSE", screenW - 250, 15, 14, COLOR_WARNING);
    DrawText(TextFormat("%.2f COINS", g_caisse.solde), screenW - 250, 32, 24, COLOR_WARNING);

    // 2. Sidebar (Left)
    int sidebarW = 90;
    DrawRectangle(0, 71, sidebarW, screenH - 71 - 60, COLOR_HEADER);
    DrawLine(sidebarW, 71, sidebarW, screenH - 60, (Color){30, 30, 40, 255}); // Subtle divider
    
    const char* menuLabels[] = {"BORD", "PKMN", "MACH.", "CLIENTS", "CMD"};
    for (int i = 0; i < 5; i++) {
        Rectangle btnRect = { 0, 100 + (float)i * 80, (float)sidebarW, 60 };
        bool isHover = CheckCollisionPointRec(GetMousePosition(), btnRect);
        bool isActive = (g_currentScreen == (GuiScreen)i);
        
        Color textColor = isActive ? COLOR_ACCENT : (isHover ? WHITE : COLOR_TEXT_DIM);
        
        // Custom simple icons based on index
        int startX = 35;
        int startY = btnRect.y + 10;
        if (i == 0) { // Dashboard
            DrawRectangle(startX, startY, 8, 8, textColor); DrawRectangle(startX+10, startY, 8, 8, textColor);
            DrawRectangle(startX, startY+10, 8, 8, textColor); DrawRectangle(startX+10, startY+10, 8, 8, textColor);
        } else if (i == 1) { // PKMN
            DrawRectangleLines(startX-2, startY, 20, 15, textColor);
            DrawRectangle(startX+4, startY+4, 8, 6, textColor);
        } else if (i == 2) { // MACH.
            DrawRectangle(startX, startY+5, 16, 10, textColor);
            DrawLine(startX+4, startY, startX+4, startY+5, textColor);
            DrawLine(startX+12, startY, startX+12, startY+5, textColor);
        } else if (i == 3) { // CLIENTS
            DrawCircle(startX+8, startY+5, 5, textColor);
            DrawRectangle(startX-2, startY+12, 20, 6, textColor);
        } else if (i == 4) { // CMD
            DrawLineEx((Vector2){startX-5, startY+2}, (Vector2){startX+20, startY+2}, 3, textColor);
            DrawLineEx((Vector2){startX-5, startY+8}, (Vector2){startX+20, startY+8}, 3, textColor);
            DrawLineEx((Vector2){startX-5, startY+14}, (Vector2){startX+20, startY+14}, 3, textColor);
        }
        
        DrawText(menuLabels[i], startX - MeasureText(menuLabels[i], 10)/2 + 8, startY + 22, 10, textColor);

        if (isHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            g_currentScreen = (GuiScreen)i;
            
            // Reinitialiser tous les modales pour eviter la superposition
            showPokeModal = false;
            extern bool showMachineModal;
            showMachineModal = false;
            extern bool showClientModal;
            showClientModal = false;
            extern bool showCmdModal;
            showCmdModal = false;
        }
    }

    // 3. Bottom Red Button (Full Width)
    Rectangle redBtn = {0, screenH - 60, screenW, 60};
    bool btnHover = CheckCollisionPointRec(GetMousePosition(), redBtn);
    DrawRectangleRec(redBtn, btnHover ? RED : (Color){ 230, 20, 20, 255 });
    
    // Save floppy icon approx
    DrawRectangle(screenW/2 - 180, screenH - 45, 20, 24, WHITE);
    DrawRectangle(screenW/2 - 176, screenH - 43, 12, 8, RED);
    
    DrawText("SAUVEGARDER & QUITTER", screenW/2 - 140, screenH - 42, 24, WHITE);

    if (btnHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        g_forceQuit = true;
    }
}

void DrawDashboard() {
    int startX = 140;
    int startY = 100;
    int cardWidth = 230;
    int cardHeight = 110;
    int spacing = 20;

    // Calcul des KPIs
    int totalPokemons = compter_pokemons(g_pokemons);
    
    int nbMachines = compter_machines(g_machines);
    int activeMachines = 0;
    liste_machine* m_tmp = g_machines;
    while(m_tmp){ if(m_tmp->machine.etat == OCCUPEE) activeMachines++; m_tmp = m_tmp->suivant; }
    
    int waitCmds = 0;
    liste_commande* c_tmp = g_commandes;
    while(c_tmp){ if(c_tmp->commande.etat == EN_ATTENTE) waitCmds++; c_tmp = c_tmp->suivant; }

    // --- ROW 1: 4 KPI CARDS ---
    
    // Card 1: Total Pokemons
    Rectangle kpi1 = { startX, startY, cardWidth, cardHeight };
    DrawRectangleRec(kpi1, COLOR_CARD);
    DrawRectangle(kpi1.x, kpi1.y, kpi1.width, 6, COLOR_ACCENT); // Top Red Border
    DrawText("TOTAL POKEMONS", (int)kpi1.x + 20, (int)kpi1.y + 25, 12, COLOR_TEXT_DIM);
    DrawText(TextFormat("%02d", totalPokemons), (int)kpi1.x + 20, (int)kpi1.y + 50, 40, WHITE);

    // Card 2: Active Machines
    Rectangle kpi2 = { startX + (cardWidth + spacing), startY, cardWidth, cardHeight };
    DrawRectangleRec(kpi2, COLOR_CARD);
    DrawRectangle(kpi2.x, kpi2.y, kpi2.width, 6, COLOR_ACCENT);
    DrawText("ACTIVE MACHINES", (int)kpi2.x + 20, (int)kpi2.y + 25, 12, COLOR_TEXT_DIM);
    DrawText(TextFormat("%02d / %02d", activeMachines, nbMachines), (int)kpi2.x + 20, (int)kpi2.y + 50, 40, WHITE);

    // Card 3: Pending Orders
    Rectangle kpi3 = { startX + 2*(cardWidth + spacing), startY, cardWidth, cardHeight };
    DrawRectangleRec(kpi3, COLOR_CARD);
    DrawRectangle(kpi3.x, kpi3.y, kpi3.width, 6, COLOR_ACCENT);
    DrawText("PENDING ORDERS", (int)kpi3.x + 20, (int)kpi3.y + 25, 12, COLOR_TEXT_DIM);
    DrawText(TextFormat("%02d", waitCmds), (int)kpi3.x + 20, (int)kpi3.y + 50, 40, WHITE);

    // Card 4: Factory Funds
    Rectangle kpi4 = { startX + 3*(cardWidth + spacing), startY, cardWidth, cardHeight };
    DrawRectangleRec(kpi4, COLOR_CARD);
    DrawRectangle(kpi4.x, kpi4.y, kpi4.width, 6, COLOR_ACCENT);
    DrawText("FACTORY FUNDS", (int)kpi4.x + 20, (int)kpi4.y + 25, 12, COLOR_TEXT_DIM);
    DrawText(TextFormat("%.0f", g_caisse.solde), (int)kpi4.x + 20, (int)kpi4.y + 50, 40, COLOR_WARNING);

    // --- ROW 2: REAL STATISTICS ---
    int chartY = startY + cardHeight + 40;
    int chartWidth = 4 * cardWidth + 3 * spacing;
    int chartHeight = 350;
    
    Rectangle chartArea = { (float)startX, (float)chartY, (float)chartWidth, (float)chartHeight };
    DrawRectangleRec(chartArea, COLOR_CARD);
    DrawRectangleLinesEx(chartArea, 2, COLOR_HEADER);
    
    DrawText("ANALYSE DE PERFORMANCE", startX + 20, chartY + 20, 18, WHITE);
    DrawLine(startX + 20, chartY + 50, startX + chartWidth - 20, chartY + 50, COLOR_HEADER);

    // 1. Pokemon le plus vendu
    int topPokeQte = 0;
    Pokemon* topPoke = trouver_pokemon_le_plus_vendu(g_commandes, &topPokeQte);
    
    // 2. Client le plus rentable
    float topClientCA = 0;
    Client* topClient = trouver_client_le_plus_rentable(g_commandes, &topClientCA);
    
    // 3. Machine la plus efficace
    Machine* topMach = trouver_machine_la_plus_efficace(g_machines);

    int panelW = (chartWidth - 60) / 3;
    int panelY = chartY + 80;
    
    // Panel Stats 1: Pokemon
    DrawRectangle(startX + 20, panelY, panelW, 200, COLOR_BG);
    DrawText("TOP POKEMON", startX + 40, panelY + 20, 14, COLOR_TEXT_DIM);
    if (topPoke) {
        DrawText(topPoke->nom, startX + 40, panelY + 50, 22, WHITE);
        DrawText(TextFormat("%d UNITES VENDUES", topPokeQte), startX + 40, panelY + 85, 12, COLOR_SUCCESS);
    } else {
        DrawText("AUCUNE DONNEE", startX + 40, panelY + 50, 18, GRAY);
    }

    // Panel Stats 2: Client
    DrawRectangle(startX + 40 + panelW, panelY, panelW, 200, COLOR_BG);
    DrawText("CLIENT PREMIUM", startX + 60 + panelW, panelY + 20, 14, COLOR_TEXT_DIM);
    if (topClient) {
        DrawText(topClient->nom, startX + 60 + panelW, panelY + 40, 20, WHITE);
        DrawText(topClient->matricule, startX + 60 + panelW, panelY + 65, 12, COLOR_ACCENT);
        DrawText(TextFormat("%.2f COINS GENERES", topClientCA), startX + 60 + panelW, panelY + 100, 12, COLOR_WARNING);
    } else {
        DrawText("AUCUNE DONNEE", startX + 60 + panelW, panelY + 50, 18, GRAY);
    }

    // Panel Stats 3: Machine
    DrawRectangle(startX + 60 + 2*panelW, panelY, panelW, 200, COLOR_BG);
    DrawText("MACHINE D'ELITE", startX + 80 + 2*panelW, panelY + 20, 14, COLOR_TEXT_DIM);
    if (topMach && topMach->total_produit > 0) {
        DrawText(topMach->nom, startX + 80 + 2*panelW, panelY + 50, 22, WHITE);
        DrawText(TextFormat("%d FIGURINES PRODUITES", topMach->total_produit), startX + 80 + 2*panelW, panelY + 85, 12, COLOR_SUCCESS);
    } else {
        DrawText("AUCUNE PRODUCTION", startX + 80 + 2*panelW, panelY + 50, 18, GRAY);
    }
}

void DrawPokemons() {
    int startX = 230;
    int startY = 30;
    DrawText("GESTION DES POKEMONS", startX, startY, 28, WHITE);

    if (GuiButton((Rectangle){ startX, startY + 50, 180, 35 }, "NOUVEAU POKEMON")) {
        showPokeModal = true;
        editingPokeId = -1;
        pokeNameEditMode = false;
        pokeNameInput[0] = '\0';
        pokeTypeActive = 0;
        pokeCostInput = 10.0f;
    }

    // Affichage en grille
    liste_pokemon* courant = g_pokemons;
    int i = 0;
    while (courant != NULL && i < 20) {
        int row = i / 4;
        int col = i % 4;
        Rectangle card = { (float)startX + col * 230, (float)startY + 110 + row * 130, 210, 110 };
        DrawRectangleRec(card, COLOR_CARD);
        DrawRectangleLinesEx(card, 2, COLOR_HEADER); // Thick dark border
        DrawRectangle(card.x, card.y, card.width, 4, COLOR_ACCENT); // Top Red Accent
        
        DrawText(courant->pokemon.nom, (int)card.x + 15, (int)card.y + 20, 18, WHITE);
        
        // Type badge (solid block)
        DrawRectangle((int)card.x + 15, (int)card.y + 45, 80, 20, COLOR_BG);
        DrawText(type_to_string(courant->pokemon.type), (int)card.x + 20, (int)card.y + 48, 12, LIGHTGRAY);
        
        DrawText(TextFormat("%.2f Coins", courant->pokemon.cout_unitaire), (int)card.x + 15, (int)card.y + 80, 16, COLOR_SUCCESS);
        
        // Actions
        if (GuiButton((Rectangle){card.x + 130, card.y + 15, 30, 25}, "#136#")) { // Edit icon
            showPokeModal = true;
            editingPokeId = courant->pokemon.identifiant;
            strncpy(pokeNameInput, courant->pokemon.nom, 49);
            pokeTypeActive = (int)courant->pokemon.type;
            pokeCostInput = courant->pokemon.cout_unitaire;
        }
        if (GuiButton((Rectangle){card.x + 165, card.y + 15, 30, 25}, "#143#")) { // Trash icon
            showConfirmDelete = true;
            deleteType = 0;
            deleteId = courant->pokemon.identifiant;
        }

        i++;
        courant = courant->suivant;
    }

    // Modal Ajout
    if (showPokeModal) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 200});
        Rectangle modal = { GetScreenWidth()/2.0f - 200, GetScreenHeight()/2.0f - 150, 400, 300 };
        DrawRectangleRec(modal, COLOR_BG);
        DrawRectangleLinesEx(modal, 2, COLOR_ACCENT);
        DrawText(editingPokeId == -1 ? "AJOUTER UN POKEMON" : "MODIFIER LE POKEMON", (int)modal.x + 20, (int)modal.y + 20, 20, WHITE);

        // Name input
        DrawText("Nom:", (int)modal.x + 20, (int)modal.y + 70, 16, LIGHTGRAY);
        if (GuiTextBox((Rectangle){ modal.x + 120, modal.y + 65, 200, 30 }, pokeNameInput, 50, pokeNameEditMode)) {
            pokeNameEditMode = !pokeNameEditMode;
        }

        // Type Dropdown (using ToggleGroup for simplicity)
        DrawText("Type:", (int)modal.x + 20, (int)modal.y + 120, 16, LIGHTGRAY);
        GuiToggleGroup((Rectangle){ modal.x + 120, modal.y + 115, 60, 30 }, "Elec;Feu;Eau;Plante", &pokeTypeActive);

        // Cost input
        DrawText("Cout:", (int)modal.x + 20, (int)modal.y + 170, 16, LIGHTGRAY);
        if (GuiTextBox((Rectangle){ modal.x + 120, modal.y + 165, 200, 30 }, pokeCostText, 20, pokeCostEditMode)) {
            pokeCostEditMode = !pokeCostEditMode;
            pokeCostInput = (float)atof(pokeCostText);
        }

        // Actions
        if (GuiButton((Rectangle){ modal.x + 50, modal.y + 230, 120, 40 }, "ANNULER")) {
            showPokeModal = false;
        }
        if (GuiButton((Rectangle){ modal.x + 230, modal.y + 230, 120, 40 }, "ENREGISTRER")) {
            if (pokeNameInput[0] != '\0') {
                if (editingPokeId == -1) {
                    liste_pokemon* nouveau = (liste_pokemon*)malloc(sizeof(liste_pokemon));
                    nouveau->pokemon.identifiant = prochain_id_pokemon++;
                    strncpy(nouveau->pokemon.nom, pokeNameInput, 49);
                    nouveau->pokemon.nom[49] = '\0';
                    nouveau->pokemon.type = (TypePokemon)pokeTypeActive;
                    nouveau->pokemon.cout_unitaire = pokeCostInput;
                    nouveau->suivant = g_pokemons;
                    g_pokemons = nouveau;
                } else {
                    Pokemon* p = trouver_pokemon_par_id(g_pokemons, editingPokeId);
                    if (p) {
                        strncpy(p->nom, pokeNameInput, 49);
                        p->type = (TypePokemon)pokeTypeActive;
                        p->cout_unitaire = pokeCostInput;
                    }
                }
                showPokeModal = false;
            }
        }
    }
}

// Variables pour Clients
bool showClientModal = false;
char cliMatriculeInput[20] = "";
char cliNomInput[100] = "";
char editingCliMat[20] = ""; // "" = nouveau
bool cliMatEditMode = false;
bool cliNomEditMode = false;
bool showCliMatError = false;

void DrawClients() {
    int startX = 230;
    int startY = 30;
    DrawText("GESTION DES CLIENTS", startX, startY, 28, WHITE);

    if (GuiButton((Rectangle){ startX, startY + 50, 180, 35 }, "NOUVEAU CLIENT")) {
        showClientModal = true;
        showCliMatError = false; // Reset error
        editingCliMat[0] = '\0';
        cliMatEditMode = false;
        cliNomEditMode = false;
        cliMatriculeInput[0] = '\0';
        cliNomInput[0] = '\0';
    }

    liste_client* courant = g_clients;
    int i = 0;
    while (courant != NULL && i < 15) {
        int yPos = 120 + i * 40;
        DrawRectangle((int)startX, yPos - 5, 600, 35, (i % 2 == 0) ? COLOR_CARD : COLOR_BG);
        DrawRectangle((int)startX, yPos - 5, 4, 35, COLOR_ACCENT); // Red left accent
        DrawText(courant->client.matricule, (int)startX + 20, yPos + 5, 14, COLOR_ACCENT);
        DrawText(courant->client.nom, (int)startX + 150, yPos + 5, 16, WHITE);
        
        int nbCmds = 0;
        int totalPcs = 0;
        liste_commande* lc_scan = g_commandes;
        while(lc_scan) {
            if (lc_scan->commande.client && strcmp(lc_scan->commande.client->matricule, courant->client.matricule) == 0) {
                nbCmds++;
                if (lc_scan->commande.etat == REALISEE) totalPcs += lc_scan->commande.quantite;
            }
            lc_scan = lc_scan->suivant;
        }
        DrawText(TextFormat("%d COMMANDES", nbCmds), (int)startX + 320, yPos + 5, 12, LIGHTGRAY);
        DrawText(TextFormat("%d FIGURINES ACHETEES", totalPcs), (int)startX + 420, yPos + 5, 12, COLOR_WARNING);
        
        // Actions (décalés plus à droite pour éviter superposition)
        if (GuiButton((Rectangle){startX + 620, yPos - 3, 25, 25}, "#136#")) {
            showClientModal = true;
            strcpy(editingCliMat, courant->client.matricule);
            strcpy(cliMatriculeInput, courant->client.matricule);
            strcpy(cliNomInput, courant->client.nom);
        }
        if (GuiButton((Rectangle){startX + 655, yPos - 3, 25, 25}, "#143#")) {
            showConfirmDelete = true;
            deleteType = 2;
            strcpy(deleteStrId, courant->client.matricule);
        }

        i++;
        courant = courant->suivant;
    }

    // Modal Ajout
    if (showClientModal) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 200});
        Rectangle modal = { GetScreenWidth()/2.0f - 200, GetScreenHeight()/2.0f - 150, 400, 300 };
        DrawRectangleRec(modal, COLOR_BG);
        DrawRectangleLinesEx(modal, 2, COLOR_ACCENT);
        DrawText(editingCliMat[0] == '\0' ? "AJOUTER UN CLIENT" : "MODIFIER LE CLIENT", (int)modal.x + 20, (int)modal.y + 20, 20, WHITE);

        if (editingCliMat[0] == '\0') {
            DrawText("Matricule:", (int)modal.x + 20, (int)modal.y + 70, 16, LIGHTGRAY);
            if (GuiTextBox((Rectangle){ modal.x + 120, modal.y + 65, 200, 30 }, cliMatriculeInput, 20, cliMatEditMode)) cliMatEditMode = !cliMatEditMode;
        } else {
            DrawText("Matricule (fixe):", (int)modal.x + 20, (int)modal.y + 70, 16, COLOR_TEXT_DIM);
            DrawText(editingCliMat, (int)modal.x + 125, (int)modal.y + 72, 16, WHITE);
        }

        DrawText("Nom:", (int)modal.x + 20, (int)modal.y + 120, 16, LIGHTGRAY);
        if (GuiTextBox((Rectangle){ modal.x + 120, modal.y + 115, 200, 30 }, cliNomInput, 100, cliNomEditMode)) cliNomEditMode = !cliNomEditMode;

        if (showCliMatError) {
            DrawText("Erreur: Ce matricule existe deja!", (int)modal.x + 50, (int)modal.y + 160, 14, RED);
        }

        if (GuiButton((Rectangle){ modal.x + 50, modal.y + 230, 120, 40 }, "ANNULER")) showClientModal = false;
        if (GuiButton((Rectangle){ modal.x + 230, modal.y + 230, 120, 40 }, "ENREGISTRER")) {
            if (cliMatriculeInput[0] != '\0' && cliNomInput[0] != '\0') {
                if (editingCliMat[0] == '\0') {
                    // VERIFICATION UNICITE
                    if (trouver_client_par_matricule(g_clients, cliMatriculeInput) != NULL) {
                        showCliMatError = true;
                    } else {
                        liste_client* nouveau = (liste_client*)malloc(sizeof(liste_client));
                        strncpy(nouveau->client.matricule, cliMatriculeInput, 19); nouveau->client.matricule[19] = '\0';
                        strncpy(nouveau->client.nom, cliNomInput, 99); nouveau->client.nom[99] = '\0';
                        nouveau->client.liste_commandes = NULL;
                        nouveau->suivant = g_clients;
                        g_clients = nouveau;
                        showClientModal = false;
                        showCliMatError = false;
                    }
                } else {
                    Client* c = trouver_client_par_matricule(g_clients, editingCliMat);
                    if (c) {
                        strncpy(c->nom, cliNomInput, 99);
                    }
                    showClientModal = false;
                    showCliMatError = false;
                }
            }
        }
    }
}

// Variables pour Machines
bool showMachineModal = false;
char machNameInput[50] = "";
bool machNameEditMode = false;
int machTimeInput = 5;
char machTimeText[20] = "5";
bool machTimeEditMode = false;
int machThresholdInput = 10;
char machThresholdText[20] = "10";
bool machThresholdEditMode = false;
float machMaintCostInput = 50.0f;
char machMaintCostText[20] = "50.0";
bool machMaintCostEditMode = false;
int machMaintTimeInput = 2;
char machMaintTimeText[20] = "2";
bool machMaintTimeEditMode = false;
int machPokeIndex = 0; // Dropdown index
int editingMachId = -1;
bool machPokeEditMode = false;

void DrawMachines() {
    int startX = 230;
    int startY = 30;
    DrawText("ATELIER DES MACHINES", startX, startY, 28, WHITE);

    if (GuiButton((Rectangle){ (float)startX, (float)startY + 50, 180, 35 }, "NOUVELLE MACHINE")) {
        showMachineModal = true;
        editingMachId = -1;
        machNameInput[0] = '\0';
        machTimeInput = 5;
        sprintf(machTimeText, "%d", 5);
        machTimeEditMode = false;
        machThresholdInput = 10;
        sprintf(machThresholdText, "%d", 10);
        machThresholdEditMode = false;
        machMaintCostInput = 50.0f;
        sprintf(machMaintCostText, "%.2f", 50.0f);
        machMaintCostEditMode = false;
        machMaintTimeInput = 2;
        sprintf(machMaintTimeText, "%d", 2);
        machMaintTimeEditMode = false;
        machPokeIndex = 0;
        machPokeEditMode = false;
    }

    liste_machine* m_tmp = g_machines;
    int i = 0;
    while (m_tmp != NULL && i < 12) {
        Machine* m = &(m_tmp->machine);
        int row = i / 3;
        int col = i % 3;
        Rectangle card = { (float)startX + col * 310, (float)startY + 110 + row * 160, 290, 140 };
        
        DrawRectangleRec(card, COLOR_CARD);
        DrawRectangleLinesEx(card, 2, COLOR_HEADER);
        DrawRectangle(card.x, card.y, card.width, 4, WHITE);
        
        DrawText(m->nom, (int)card.x + 15, (int)card.y + 15, 18, WHITE);
        DrawText(m->pokemon_produit ? TextFormat("%s (%s)", m->pokemon_produit->nom, type_to_string(m->pokemon_produit->type)) : "Aucun Pokemon", (int)card.x + 15, (int)card.y + 35, 12, GRAY);
        
        Color statusColor = (m->etat == LIBRE) ? COLOR_SUCCESS : (m->etat == OCCUPEE ? COLOR_ACCENT : COLOR_WARNING);
        const char* statusText = (m->etat == LIBRE) ? "LIBRE" : (m->etat == OCCUPEE ? "EN PRODUCTION" : "MAINTENANCE");
        
        DrawRectangle((int)card.x + 15, (int)card.y + 60, 120, 20, statusColor);
        DrawText(statusText, (int)card.x + 20, (int)card.y + 64, 12, COLOR_BG);

        if (m->etat == EN_ATTENTE_MAINTENANCE) {
            int reste_sec = (int)difftime(m->date_disponibilite, time(NULL));
            if (reste_sec < 0) reste_sec = 0;
            DrawText(TextFormat("Fin dans: %02d:%02d min", reste_sec/60, reste_sec%60), (int)card.x + 145, (int)card.y + 64, 10, COLOR_WARNING);
        } else {
            DrawText(TextFormat("Tps. Maintenance: %d min", m->temps_maintenance), (int)card.x + 145, (int)card.y + 64, 10, LIGHTGRAY);
        }

        float ratio = (float)m->compteur_figurines / (float)m->max_figurines_avant_maintenance;
        if(ratio > 1.0f) ratio = 1.0f;
        
        DrawRectangleLines((int)card.x + 15, (int)card.y + 100, 260, 12, WHITE);
        DrawRectangle((int)card.x + 17, (int)card.y + 102, (int)(256 * ratio), 8, COLOR_ACCENT);
        DrawText(TextFormat("Usure: %d/%d", m->compteur_figurines, m->max_figurines_avant_maintenance), (int)card.x + 15, (int)card.y + 118, 10, LIGHTGRAY);

        int nb_dans_file = 0;
        liste_commande* c_file = m->file_attente;
        while(c_file) { nb_dans_file++; c_file = c_file->suivant; }
        if (nb_dans_file > 0) {
            DrawRectangle((int)card.x + 155, (int)card.y + 118, 110, 15, (Color){COLOR_WARNING.r, COLOR_WARNING.g, COLOR_WARNING.b, 40});
            DrawText(TextFormat("Waitlist: %d cmd", nb_dans_file), (int)card.x + 160, (int)card.y + 120, 10, COLOR_WARNING);
        }

        if (GuiButton((Rectangle){card.x + 220, card.y + 10, 25, 25}, "#136#")) {
            showMachineModal = true;
            editingMachId = m->identifiant;
            strcpy(machNameInput, m->nom);
            machTimeInput = m->temps_production;
            sprintf(machTimeText, "%d", m->temps_production);
            machThresholdInput = m->max_figurines_avant_maintenance;
            sprintf(machThresholdText, "%d", m->max_figurines_avant_maintenance);
            machMaintCostInput = m->cout_maintenance;
            sprintf(machMaintCostText, "%.2f", m->cout_maintenance);
            machMaintTimeInput = m->temps_maintenance;
            sprintf(machMaintTimeText, "%d", m->temps_maintenance);
        }
        if (GuiButton((Rectangle){card.x + 250, card.y + 10, 25, 25}, "#143#")) {
            showConfirmDelete = true;
            deleteType = 1;
            deleteId = m->identifiant;
        }
        if (m->etat == EN_ATTENTE_MAINTENANCE || m->compteur_figurines > 0) {
            if (GuiButton((Rectangle){card.x + 220, card.y + 60, 55, 20}, "MAINT.")) {
                if (m->etat == EN_ATTENTE_MAINTENANCE && maintenance_terminee(m)) {
                    effectuer_maintenance(m, &g_caisse);
                } else if (m->etat == LIBRE) {
                    m->etat = EN_ATTENTE_MAINTENANCE;
                    m->date_disponibilite = time(NULL) + (m->temps_maintenance * 60);
                }
            }
        }

        i++;
        m_tmp = m_tmp->suivant;
    }

    if (showMachineModal) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 200});
        Rectangle modal = { GetScreenWidth()/2.0f - 200, GetScreenHeight()/2.0f - 200, 430, 420 };
        DrawRectangleRec(modal, COLOR_BG);
        DrawRectangleLinesEx(modal, 2, COLOR_ACCENT);
        DrawText(editingMachId == -1 ? "AJOUTER UNE MACHINE" : "MODIFIER LA MACHINE", (int)modal.x + 20, (int)modal.y + 20, 20, WHITE);

        int p_count = compter_pokemons(g_pokemons);
        if (p_count == 0) {
            DrawText("Erreur: Creez d'abord un Pokemon!", (int)modal.x + 20, (int)modal.y + 60, 16, RED);
            if (GuiButton((Rectangle){ modal.x + 150, modal.y + 350, 120, 40 }, "FERMER")) showMachineModal = false;
        } else {
            DrawText("Nom:", (int)modal.x + 20, (int)modal.y + 60, 16, LIGHTGRAY);
            if(GuiTextBox((Rectangle){ modal.x + 180, modal.y + 55, 200, 30 }, machNameInput, 50, machNameEditMode)) machNameEditMode = !machNameEditMode;
            
            DrawText("Temps/fig (min):", (int)modal.x + 20, (int)modal.y + 110, 16, LIGHTGRAY);
            if (GuiTextBox((Rectangle){ modal.x + 180, modal.y + 105, 120, 30 }, machTimeText, 20, machTimeEditMode)) {
                machTimeEditMode = !machTimeEditMode;
                machTimeInput = atoi(machTimeText);
            }

            DrawText("Seuil Maint.:", (int)modal.x + 20, (int)modal.y + 160, 16, LIGHTGRAY);
            if (GuiTextBox((Rectangle){ modal.x + 180, modal.y + 155, 120, 30 }, machThresholdText, 20, machThresholdEditMode)) {
                machThresholdEditMode = !machThresholdEditMode;
                machThresholdInput = atoi(machThresholdText);
            }

            DrawText("Cout Maint.:", (int)modal.x + 20, (int)modal.y + 210, 16, LIGHTGRAY);
            if (GuiTextBox((Rectangle){ modal.x + 180, modal.y + 205, 120, 30 }, machMaintCostText, 20, machMaintCostEditMode)) {
                machMaintCostEditMode = !machMaintCostEditMode;
                machMaintCostInput = (float)atof(machMaintCostText);
            }

            DrawText("Duree Maint.:", (int)modal.x + 20, (int)modal.y + 260, 16, LIGHTGRAY);
            if (GuiTextBox((Rectangle){ modal.x + 180, modal.y + 255, 120, 30 }, machMaintTimeText, 20, machMaintTimeEditMode)) {
                machMaintTimeEditMode = !machMaintTimeEditMode;
                machMaintTimeInput = atoi(machMaintTimeText);
            }

            if (GuiButton((Rectangle){ modal.x + 50, modal.y + 350, 120, 40 }, "ANNULER")) showMachineModal = false;
            
            if (GuiButton((Rectangle){ modal.x + 250, modal.y + 350, 120, 40 }, "ENREGISTRER")) {
                if (machNameInput[0] != '\0') {
                    if (editingMachId == -1) {
                        liste_machine* nouveau = (liste_machine*)malloc(sizeof(liste_machine));
                        nouveau->machine.identifiant = prochain_id_machine++;
                        strncpy(nouveau->machine.nom, machNameInput, 49); nouveau->machine.nom[49] = '\0';
                        
                        Pokemon* target = NULL;
                        liste_pokemon* p_tmp = g_pokemons;
                        for(int k=0; p_tmp != NULL && k < machPokeIndex; k++) p_tmp = p_tmp->suivant;
                        if(p_tmp) target = &(p_tmp->pokemon);
                        
                        nouveau->machine.pokemon_produit = target;
                        nouveau->machine.temps_production = machTimeInput;
                        nouveau->machine.max_figurines_avant_maintenance = machThresholdInput;
                        nouveau->machine.compteur_figurines = 0;
                        nouveau->machine.total_produit = 0;
                        nouveau->machine.cout_maintenance = machMaintCostInput;
                        nouveau->machine.temps_maintenance = machMaintTimeInput;
                        nouveau->machine.etat = LIBRE;
                        nouveau->machine.file_attente = NULL;
                        nouveau->machine.date_disponibilite = time(NULL);
                        
                        nouveau->suivant = g_machines;
                        g_machines = nouveau;
                    } else {
                        Machine* m = trouver_machine_par_id(g_machines, editingMachId);
                        if (m) {
                            strncpy(m->nom, machNameInput, 49);
                            m->temps_production = machTimeInput;
                            m->max_figurines_avant_maintenance = machThresholdInput;
                            m->cout_maintenance = machMaintCostInput;
                            m->temps_maintenance = machMaintTimeInput;
                            
                            Pokemon* target = NULL;
                            liste_pokemon* p_tmp = g_pokemons;
                            for(int k=0; p_tmp != NULL && k < machPokeIndex; k++) p_tmp = p_tmp->suivant;
                            if(p_tmp) target = &(p_tmp->pokemon);
                            m->pokemon_produit = target;
                        }
                    }
                    showMachineModal = false;
                }
            }

            DrawText("Pokemon:", (int)modal.x + 20, (int)modal.y + 310, 16, LIGHTGRAY);
            char pokeOptions[2048] = "";
            liste_pokemon* p_tmp = g_pokemons;
            while(p_tmp != NULL) {
                strcat(pokeOptions, TextFormat("%s (#%d)", p_tmp->pokemon.nom, p_tmp->pokemon.identifiant));
                if (p_tmp->suivant != NULL) strcat(pokeOptions, ";");
                p_tmp = p_tmp->suivant;
            }
            if (GuiDropdownBox((Rectangle){ modal.x + 180, modal.y + 305, 200, 30 }, pokeOptions, &machPokeIndex, machPokeEditMode)) machPokeEditMode = !machPokeEditMode;
        }
    }
}

// Variables pour Commandes
bool showCmdModal = false;
int cmdCliIndex = 0;
int cmdPokeIndex = 0;
int cmdQteInput = 10;
char cmdQteText[20] = "10";
bool cmdQteEditMode = false;
bool cmdCliEditMode = false;
bool cmdPokeEditMode = false;
int editingCmdId = -1; // -1 = nouveau

void DrawCommandes() {
    int startX = 230;
    int startY = 30;
    DrawText("GESTION DES COMMANDES", startX, startY, 28, WHITE);

    if (GuiButton((Rectangle){ (float)startX, (float)startY + 50, 180, 35 }, "NOUVELLE COMMANDE")) {
        showCmdModal = true;
        cmdCliIndex = 0;
        cmdPokeIndex = 0;
        cmdQteInput = 10;
        sprintf(cmdQteText, "%d", 10);
        cmdQteEditMode = false;
        cmdCliEditMode = false;
        cmdPokeEditMode = false;
        editingCmdId = -1;
    }

    Rectangle tableHeader = { (float)startX, 110, 930, 40 };
    DrawRectangleRec(tableHeader, COLOR_BG);
    DrawRectangleLinesEx(tableHeader, 1, COLOR_ACCENT);
    DrawRectangle(tableHeader.x, tableHeader.y, tableHeader.width, 2, COLOR_ACCENT); // Red top line
    DrawText("ID", (int)tableHeader.x + 10, (int)tableHeader.y + 12, 16, COLOR_ACCENT);
    DrawText("CLIENT", (int)tableHeader.x + 100, (int)tableHeader.y + 12, 16, COLOR_ACCENT);
    DrawText("POKEMON", (int)tableHeader.x + 260, (int)tableHeader.y + 12, 16, COLOR_ACCENT);
    DrawText("QTE", (int)tableHeader.x + 420, (int)tableHeader.y + 12, 16, COLOR_ACCENT);
    DrawText("ESTIMATION", (int)tableHeader.x + 500, (int)tableHeader.y + 12, 16, COLOR_ACCENT);
    DrawText("STATUT", (int)tableHeader.x + 650, (int)tableHeader.y + 12, 16, COLOR_ACCENT);
    DrawText("ACTION", (int)tableHeader.x + 800, (int)tableHeader.y + 12, 16, COLOR_ACCENT);

    liste_commande* courant = g_commandes;
    int i = 0;
    while (courant != NULL && i < 15) {

        int yPos = 155 + i * 35;
        DrawText(TextFormat("#%d", courant->commande.identifiant), (int)startX + 10, yPos, 14, WHITE);
        DrawText(courant->commande.client->nom, (int)startX + 100, yPos, 14, LIGHTGRAY);
        DrawText(courant->commande.pokemon_commande->nom, (int)startX + 260, yPos, 14, LIGHTGRAY);
        DrawText(TextFormat("%d", courant->commande.quantite), (int)startX + 420, yPos, 14, LIGHTGRAY);
        
        // Calcul du temps reel
        char estimationText[50] = "N/A";
        Color estColor = COLOR_WARNING;
        bool found = false;

        if (courant->commande.etat == REALISEE) {
            strcpy(estimationText, "Termine");
            estColor = COLOR_SUCCESS;
            found = true;
        } else {
            // Chercher quelle machine s'occupe de cette commande (ou de ce type de Pokemon)
            liste_machine* lm = g_machines;
            time_t maintenant = time(NULL);

            while (lm) {
                Machine* m = &lm->machine;
                
                // CAS 1 : Commande actuellement produite (EN COURS)
                if (m->etat == OCCUPEE && m->commande_en_cours != NULL && 
                    m->commande_en_cours->identifiant == courant->commande.identifiant) {
                    
                    int reste_sec = (int)difftime(m->date_disponibilite, maintenant);
                    if (reste_sec < 0) reste_sec = 0;
                    sprintf(estimationText, "%02d:%02d min", reste_sec/60, reste_sec%60);
                    estColor = COLOR_ACCENT; // Rouge pour dire que ça tourne
                    found = true;
                    break;
                }
                
                // CAS 2 : Commande planifiée dans une file (EN FILE)
                liste_commande* lc_file = m->file_attente;
                int temps_cumule = 0;
                int compteur_virtuel = m->compteur_figurines;

                // On doit d'abord compter le temps de la commande en cours sur cette machine
                if (m->etat == OCCUPEE) {
                    temps_cumule += (int)difftime(m->date_disponibilite, maintenant) / 60;
                    int fini;
                    if (m->commande_en_cours) simuler_temps_et_compteur(m, m->commande_en_cours->quantite, m->compteur_figurines, &fini);
                    else fini = 0;
                    compteur_virtuel = fini;
                } else if (m->etat == EN_ATTENTE_MAINTENANCE) {
                    temps_cumule += (int)difftime(m->date_disponibilite, maintenant) / 60;
                    compteur_virtuel = 0;
                }

                while (lc_file) {
                    int final_comp;
                    int duree = simuler_temps_et_compteur(m, lc_file->commande.quantite, compteur_virtuel, &final_comp);
                    
                    if (lc_file->commande.identifiant == courant->commande.identifiant) {
                        temps_cumule += duree;
                        sprintf(estimationText, "~ %d min", temps_cumule);
                        found = true;
                        break;
                    }
                    
                    temps_cumule += duree;
                    compteur_virtuel = final_comp;
                    lc_file = lc_file->suivant;
                }

                if (found) break;
                lm = lm->suivant;
            }
        }

        if (!found && (courant->commande.etat == EN_ATTENTE || courant->commande.etat == PLANIFIEE)) {
            // Si pas encore lancée ou planifiée sans machine trouvée dans le premier passage
            int t_prod = 0;
            liste_machine* tm_m = g_machines;
            while(tm_m) {
                if (tm_m->machine.pokemon_produit != NULL && 
                    courant->commande.pokemon_commande != NULL &&
                    tm_m->machine.pokemon_produit->identifiant == courant->commande.pokemon_commande->identifiant) {
                    t_prod = tm_m->machine.temps_production;
                    break;
                }
                tm_m = tm_m->suivant;
            }
            
            if (t_prod > 0) {
                if (courant->commande.etat == EN_ATTENTE) {
                    sprintf(estimationText, "%d min", (courant->commande.quantite * t_prod));
                    estColor = COLOR_TEXT_DIM;
                } else {
                    // Si c'est PLANIFIEE mais qu'on n'a pas trouvé sa machine dans le premier passage (rare)
                    strcpy(estimationText, "Calcul...");
                }
            } else {
                strcpy(estimationText, "Pas de machine");
                estColor = COLOR_WARNING;
            }
        }

        DrawText(estimationText, (int)startX + 500, yPos, 14, estColor);

        Color sCol = (courant->commande.etat == REALISEE) ? COLOR_SUCCESS : 
                     (courant->commande.etat == EN_COURS ? COLOR_ACCENT : 
                     (courant->commande.etat == PLANIFIEE ? COLOR_WARNING : COLOR_WARNING));
        
        const char* statusText = "INCONNU";
        if (courant->commande.etat == REALISEE) statusText = "TERMINEE";
        else if (courant->commande.etat == EN_COURS) statusText = "EN COURS";
        else if (courant->commande.etat == PLANIFIEE) statusText = "EN FILE";
        else statusText = "ATTENTE";

        DrawRectangle((int)startX + 650, yPos - 2, 80, 20, (Color){sCol.r, sCol.g, sCol.b, 50});
        DrawText(statusText, (int)startX + 655, yPos + 2, 12, sCol);

        // Edit button (only if not finished/realisee)
        if (courant->commande.etat == EN_ATTENTE || courant->commande.etat == PLANIFIEE) {
            if (GuiButton((Rectangle){startX + 760, yPos - 5, 30, 25}, "#136#")) {
                showCmdModal = true;
                editingCmdId = courant->commande.identifiant;
                cmdQteInput = courant->commande.quantite;
                sprintf(cmdQteText, "%d", cmdQteInput);
                
                // Trouver les index pour les dropdowns
                cmdCliIndex = 0;
                liste_client* c_find = g_clients;
                while(c_find) {
                    if (strcmp(c_find->client.matricule, courant->commande.client->matricule) == 0) break;
                    cmdCliIndex++;
                    c_find = c_find->suivant;
                }
                
                cmdPokeIndex = 0;
                liste_pokemon* p_find = g_pokemons;
                while(p_find) {
                    if (p_find->pokemon.identifiant == courant->commande.pokemon_commande->identifiant) break;
                    cmdPokeIndex++;
                    p_find = p_find->suivant;
                }
            }
        }

        if (courant->commande.etat == EN_ATTENTE) {
            if (GuiButton((Rectangle){ (float)startX + 800, (float)yPos - 5, 80, 25 }, "LANCER")) {
                Machine* mac = trouver_machine_libre_pour_pokemon(g_machines, courant->commande.pokemon_commande);
                if(mac) {
                    if (mac->etat == LIBRE) {
                        lancer_production(mac, &(courant->commande), courant->commande.quantite);
                    } else {
                        ajouter_a_file(mac, &(courant->commande));
                        courant->commande.etat = PLANIFIEE; // Mettre en file d'attente
                    }
                } else {
                    // AUCUNE MACHINE DISPONIBLE POUR CE POKEMON
                    showNoMachineWarning = true;
                }
            }
        }
        
        // Delete button for ANY command (Cancel)
        if (GuiButton((Rectangle){startX + 890, yPos - 5, 30, 25}, "#143#")) {
            showConfirmDelete = true;
            deleteType = 3;
            deleteId = courant->commande.identifiant;
        }

        i++;
        courant = courant->suivant;
    }

    // Modal Ajout Commande
    if (showCmdModal) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 200});
        Rectangle modal = { GetScreenWidth()/2.0f - 200, GetScreenHeight()/2.0f - 150, 400, 300 };
        DrawRectangleRec(modal, COLOR_BG);
        DrawRectangleLinesEx(modal, 2, COLOR_ACCENT);
        DrawText(editingCmdId == -1 ? "NOUVELLE COMMANDE" : "MODIFIER LA COMMANDE", (int)modal.x + 20, (int)modal.y + 20, 20, WHITE);

        int c_count = compter_clients(g_clients);
        int p_count = compter_pokemons(g_pokemons);
        
        if (c_count == 0 || p_count == 0) {
            DrawText("Erreur: Il faut 1 Client et 1 Pokemon!", (int)modal.x + 20, (int)modal.y + 60, 16, RED);
            if (GuiButton((Rectangle){ modal.x + 150, modal.y + 230, 120, 40 }, "FERMER")) showCmdModal = false;
        } else {
            DrawText("Quantite:", (int)modal.x + 20, (int)modal.y + 70, 16, LIGHTGRAY);
            if (GuiTextBox((Rectangle){ modal.x + 180, modal.y + 65, 120, 30 }, cmdQteText, 20, cmdQteEditMode)) {
                cmdQteEditMode = !cmdQteEditMode;
                cmdQteInput = atoi(cmdQteText);
            }

            if (GuiButton((Rectangle){ modal.x + 50, modal.y + 230, 120, 40 }, "ANNULER")) showCmdModal = false;
            if (GuiButton((Rectangle){ modal.x + 230, modal.y + 230, 120, 40 }, "ENREGISTRER")) {
                Client* targetCli = NULL;
                liste_client* c_tmp = g_clients;
                for(int k=0; c_tmp != NULL && k < cmdCliIndex; k++) c_tmp = c_tmp->suivant;
                if(c_tmp) targetCli = &(c_tmp->client);

                Pokemon* targetPoke = NULL;
                liste_pokemon* p_tmp = g_pokemons;
                for(int k=0; p_tmp != NULL && k < cmdPokeIndex; k++) p_tmp = p_tmp->suivant;
                if(p_tmp) targetPoke = &(p_tmp->pokemon);

                if (editingCmdId == -1) {
                    liste_commande* nouveau = (liste_commande*)malloc(sizeof(liste_commande));
                    nouveau->commande.identifiant = prochain_id_commande++;
                    nouveau->commande.client = targetCli;
                    nouveau->commande.pokemon_commande = targetPoke;
                    nouveau->commande.quantite = cmdQteInput;
                    nouveau->commande.etat = EN_ATTENTE;
                    nouveau->commande.date_emission = time(NULL);

                    nouveau->suivant = g_commandes;
                    g_commandes = nouveau;

                    // Ajouter la commande a la liste du client
                    ajouter_commande_au_client(targetCli, &(nouveau->commande));
                } else {
                    Commande* cmd = trouver_commande_par_id(g_commandes, editingCmdId);
                    if (cmd) {
                        cmd->client = targetCli;
                        cmd->pokemon_commande = targetPoke;
                        cmd->quantite = cmdQteInput;
                    }
                }
                showCmdModal = false;
            }

            // --- GESTION DU Z-ORDER (Dessiner les dropdowns de bas en haut) ---

            // 1. Dropdown Client (Dessiné en premier pour être "en dessous" de Pokemon si besoin)
            DrawText("Client:", (int)modal.x + 20, (int)modal.y + 170, 16, LIGHTGRAY);
            char cliOptions[2048] = "";
            liste_client* c_tmp2 = g_clients;
            while(c_tmp2 != NULL) {
                strcat(cliOptions, TextFormat("%s (%s)", c_tmp2->client.nom, c_tmp2->client.matricule));
                if (c_tmp2->suivant != NULL) strcat(cliOptions, ";");
                c_tmp2 = c_tmp2->suivant;
            }
            if (GuiDropdownBox((Rectangle){ modal.x + 180, modal.y + 165, 200, 30 }, cliOptions, &cmdCliIndex, cmdCliEditMode)) {
                cmdCliEditMode = !cmdCliEditMode;
                if (cmdCliEditMode) cmdPokeEditMode = false; // Fermer l'autre
            }

            // 2. Dropdown Pokemon (Dessiné en dernier pour être au-dessus de TOUT)
            DrawText("Pokemon:", (int)modal.x + 20, (int)modal.y + 120, 16, LIGHTGRAY);
            char pokeOptions[2048] = "";
            liste_pokemon* p_tmp2 = g_pokemons;
            while(p_tmp2 != NULL) {
                strcat(pokeOptions, TextFormat("%s (#%d)", p_tmp2->pokemon.nom, p_tmp2->pokemon.identifiant));
                if (p_tmp2->suivant != NULL) strcat(pokeOptions, ";");
                p_tmp2 = p_tmp2->suivant;
            }
            if (GuiDropdownBox((Rectangle){ modal.x + 180, modal.y + 115, 200, 30 }, pokeOptions, &cmdPokeIndex, cmdPokeEditMode)) {
                cmdPokeEditMode = !cmdPokeEditMode;
                if (cmdPokeEditMode) cmdCliEditMode = false; // Fermer l'autre
            }
        }
    }
}

