//
// Created by Olcay Taner YILDIZ on 13.01.2024.
//

#include <Memory/Memory.h>
#include "SpellCheckerParameter.h"

/**
 * Constructs a SpellCheckerParameter object with default values.
 *
 * The default threshold is 0.0, the De-Mi check is enabled, the root ngram is enabled and
 * the minimum word length is 4.
 */
Spell_checker_parameter_ptr create_spell_checker_parameter() {
    Spell_checker_parameter_ptr result = malloc_(sizeof(Spell_checker_parameter), "create_spell_checker_parameter");
    result->threshold = 0.0;
    result->suffix_check = true;
    result->root_n_gram = true;
    result->min_word_length = 4;
    result->domain = NULL;
    return result;
}

/**
 * Frees memory allocated for SpellCheckerParameter object.
 * @param spell_check_parameter SpellCheckerParameter to be deallocated.
 */
void free_spell_check_parameter(Spell_checker_parameter_ptr spell_check_parameter) {
    free_(spell_check_parameter->domain);
    free_(spell_check_parameter);
}
