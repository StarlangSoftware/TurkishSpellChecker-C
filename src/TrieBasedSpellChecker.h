//
// Created by Olcay Taner YILDIZ on 21.01.2024.
//

#ifndef SPELLCHECKER_TRIEBASEDSPELLCHECKER_H
#define SPELLCHECKER_TRIEBASEDSPELLCHECKER_H

#include <FsmMorphologicalAnalyzer.h>
#include <NGram.h>
#include "SpellCheckerParameter.h"
#include "SpellCheckerTrie.h"
#include "TrieCandidate.h"

struct trie_based_spell_checker{
    Fsm_morphological_analyzer_ptr fsm;
    Spell_checker_parameter_ptr parameter;
    Hash_map_ptr merged_words;
    Hash_map_ptr split_words;
    N_gram_ptr n_gram;
    Array_list_ptr generated_words;
    Spell_checker_trie_ptr trie;
};

typedef struct trie_based_spell_checker Trie_based_spell_checker;

typedef Trie_based_spell_checker *Trie_based_spell_checker_ptr;

Trie_based_spell_checker_ptr create_trie_based_spell_checker(Fsm_morphological_analyzer_ptr fsm,
                                                             Spell_checker_parameter_ptr parameter,
                                                             N_gram_ptr n_gram);

void free_trie_based_spell_checker(Trie_based_spell_checker_ptr spell_checker);

Spell_checker_trie_ptr prepare_spell_checker_trie(Trie_based_spell_checker_ptr spell_checker);

int search_trie_candidates(Array_list_ptr candidates, Trie_candidate_ptr candidate);

Array_list_ptr candidate_list_trie(Spell_checker_trie_ptr trie, char* word);

Array_list_ptr generate_trie_candidates(Spell_checker_trie_ptr trie, Trie_candidate_ptr candidate);

#endif //SPELLCHECKER_TRIEBASEDSPELLCHECKER_H
