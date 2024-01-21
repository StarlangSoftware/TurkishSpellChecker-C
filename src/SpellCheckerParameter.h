//
// Created by Olcay Taner YILDIZ on 13.01.2024.
//

#ifndef SPELLCHECKER_SPELLCHECKERPARAMETER_H
#define SPELLCHECKER_SPELLCHECKERPARAMETER_H

#include <stdbool.h>

struct spell_checker_parameter{
    double threshold;
    bool suffix_check;
    bool root_n_gram;
    int min_word_length;
    char* domain;
};

typedef struct spell_checker_parameter Spell_checker_parameter;

typedef Spell_checker_parameter *Spell_checker_parameter_ptr;

Spell_checker_parameter_ptr create_spell_checker_parameter();

void free_spell_check_parameter(Spell_checker_parameter_ptr spell_check_parameter);

#endif //SPELLCHECKER_SPELLCHECKERPARAMETER_H
