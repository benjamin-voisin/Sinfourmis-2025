#ifndef ANT_PILE
#define ANT_PILE

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct pile_s {
    uint8_t id;
    uint8_t poid;
    uint8_t degree_entrant;
    uint8_t degree_sortant;
    uint8_t type;
} pile_t;


/*
 CETTE STRUCTURE PERMET DE CALCULER LA TAILLE PRISE PAR LES METADATAS
 Elles est morallement ABSTRAITE, ne pas l'utiliser, merci <3 
*/
typedef struct pilemetadata_s { 
    uint8_t taillepilemax;
    uint8_t taillepile;
    uint8_t pointeurpile;
} pilemetadata_t;


pile_t* head(char* memoire);

// Savoir si on est bien revenus sur la reine
bool pile_vide(char* memoire);

// Pour empiler un bout de chemin
void empiler(char* memoire, pile_t e);

// Pour dépiler un bout de chemin
pile_t* depiler(char* memoire);

// Pour que la reine puisse retrouver le trajet d'une fourmisse
pile_t* pile_dumps(char* memoire, size_t* size);
void pile_loads(char* memoire, pile_t* pile, size_t size);


uint32_t water2base(char* memoire);
uint32_t water2dest(char* memoire);

#endif
