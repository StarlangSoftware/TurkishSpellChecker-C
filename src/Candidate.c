//
// Created by Olcay Taner YILDIZ on 12.01.2024.
//

#include <StringUtils.h>
#include "Candidate.h"
#include "Memory/Memory.h"

/**
 * Constructs a new Candidate object with the specified candidate and operator.
 *
 * @param name The word candidate to be checked for spelling.
 * @param operator The operator to be applied to the candidate in the spell checking process.
 */
Candidate_ptr create_candidate(const char *name, Operator operator) {
    Candidate_ptr result = malloc_(sizeof(Candidate), "create_candidate");
    result->name = str_copy(result->name, name);
    result->operator = operator;
    return result;
}

void free_candidate(Candidate_ptr candidate) {
    free_(candidate->name);
    free_(candidate);
}
