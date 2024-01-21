//
// Created by Olcay Taner YILDIZ on 21.01.2024.
//

#ifndef SPELLCHECKER_CONTEXTBASEDSPELLCHECKER_H
#define SPELLCHECKER_CONTEXTBASEDSPELLCHECKER_H

#include <FsmMorphologicalAnalyzer.h>
#include <NGram.h>
#include "SpellCheckerParameter.h"

struct context_based_spell_checker{
    Fsm_morphological_analyzer_ptr fsm;
    Spell_checker_parameter_ptr parameter;
    Hash_map_ptr merged_words;
    Hash_map_ptr split_words;
    N_gram_ptr n_gram;
    Hash_map_ptr context_list;
};

typedef struct context_based_spell_checker Context_based_spell_checker;

typedef Context_based_spell_checker *Context_based_spell_checker_ptr;

Context_based_spell_checker_ptr create_context_based_spell_checker(Fsm_morphological_analyzer_ptr fsm,
                                                                   Spell_checker_parameter_ptr parameter,
                                                                   N_gram_ptr n_gram);

void free_context_based_spell_checker(Context_based_spell_checker_ptr spell_checker);

Hash_map_ptr load_context_dictionaries();

int damerau_levenshtein_distance(char* first, char* second);

Array_list_ptr candidate_list_context(Hash_map_ptr context_list, Fsm_morphological_analyzer_ptr fsm, char* word, Sentence_ptr sentence);

#endif //SPELLCHECKER_CONTEXTBASEDSPELLCHECKER_H
