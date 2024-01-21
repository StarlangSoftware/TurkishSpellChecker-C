//
// Created by Olcay Taner YILDIZ on 20.01.2024.
//

#ifndef SPELLCHECKER_NGRAMSPELLCHECKER_H
#define SPELLCHECKER_NGRAMSPELLCHECKER_H

#include <FsmMorphologicalAnalyzer.h>
#include <NGram.h>
#include "SpellCheckerParameter.h"

struct n_gram_spell_checker{
    Fsm_morphological_analyzer_ptr fsm;
    Spell_checker_parameter_ptr parameter;
    Hash_map_ptr merged_words;
    Hash_map_ptr split_words;
    N_gram_ptr n_gram;
};

typedef struct n_gram_spell_checker N_gram_spell_checker;

typedef N_gram_spell_checker *N_gram_spell_checker_ptr;

N_gram_spell_checker_ptr create_n_gram_spell_checker(Fsm_morphological_analyzer_ptr fsm,
                                                     Spell_checker_parameter_ptr parameter,
                                                     N_gram_ptr n_gram);

void free_n_gram_spell_checker(N_gram_spell_checker_ptr spell_checker);

char* check_analysis_and_set_root_for_word_at_index(N_gram_spell_checker spell_checker,
                                                    Sentence_ptr sentence,
                                                    int index);

char* check_analysis_and_set_root(N_gram_spell_checker spell_checker, char* word);

Sentence_ptr spell_check_n_gram(N_gram_spell_checker spell_checker, Sentence_ptr sentence);

#endif //SPELLCHECKER_NGRAMSPELLCHECKER_H
