#include "pile.h"

#include "../../sinfourmis.h"
#include "b_constants.h"

#include <assert.h>

pile_t* pile(char* memoire) {
    return (pile_t*) (memoire + 256);
}

pilemetadata_t* pilemetadata(char* memoire) {
    return (pilemetadata_t*) memoire;
}

bool pile_vide(char* memoire) {
    pilemetadata_t* met = pilemetadata(memoire);
    return met->taillepile > 0; 
}

pile_t* pile_get(char* memoire, size_t i) {
    pilemetadata_t* met = pilemetadata(memoire);
    assert(met->taillepilemax > 0);
    assert(i < met->taillepilemax);
    pile_t* p = pile(memoire);
    return p - i - 1;
}

pile_t* head(char* memoire) {
    pilemetadata_t* met = pilemetadata(memoire);
    assert(!pile_vide(memoire));
    return pile_get(memoire, met->taillepile - 1);
}

void empiler(char* memoire, pile_t e) {
    pilemetadata_t* met = pilemetadata(memoire);
    met->taillepile += 1;
    pile_t* hd = head(memoire);
    hd->direction = e.direction;
    hd->id_noeud = e.id_noeud;
    hd->poid = e.poid;
    hd->type = e.type;
    if (met->taillepilemax < met->taillepile) {
        met->taillepilemax = met->taillepile;
    }
}

pile_t* depiler(char* memoire) {
    pilemetadata_t* met = pilemetadata(memoire);
    assert(!pile_vide(memoire));
    pile_t* hd = head(memoire);
    met->taillepile -= 1;
    return hd;
}

pile_t* pile_dumps(char* memoire, size_t* size) {
    pilemetadata_t* met = pilemetadata(memoire);
    *size = met->taillepilemax;
    return pile_get(memoire, met->taillepilemax - 1);
} 



