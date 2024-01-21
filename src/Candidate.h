//
// Created by Olcay Taner YILDIZ on 12.01.2024.
//

#ifndef SPELLCHECKER_CANDIDATE_H
#define SPELLCHECKER_CANDIDATE_H

#include "Operator.h"

struct candidate{
    char* name;
    Operator operator;
};

typedef struct candidate Candidate;

typedef Candidate *Candidate_ptr;

Candidate_ptr create_candidate(const char* name, Operator operator);

void free_candidate(Candidate_ptr candidate);

#endif //SPELLCHECKER_CANDIDATE_H
