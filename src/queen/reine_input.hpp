#pragma once

#include "../sinfourmis.h"

#include <vector>

typedef struct {
    std::vector<fourmi_etat*> forumis_miam_miam;
    const reine_etat* state;
    const salle* node;
} reine_input_t;

