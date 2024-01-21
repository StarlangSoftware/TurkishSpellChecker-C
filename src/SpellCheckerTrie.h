//
// Created by Olcay Taner YILDIZ on 13.01.2024.
//

#ifndef SPELLCHECKER_SPELLCHECKERTRIE_H
#define SPELLCHECKER_SPELLCHECKERTRIE_H

#include "SpellCheckerTrieNode.h"

struct spell_checker_trie{
    Spell_checker_trie_node_ptr root_node;
};

typedef struct spell_checker_trie Spell_checker_trie;

typedef Spell_checker_trie *Spell_checker_trie_ptr;

Spell_checker_trie_ptr create_spell_checker_trie();

void free_spell_checker_trie(Spell_checker_trie_ptr trie);

void insert_to_spell_checker_trie(Spell_checker_trie_ptr trie, char* word);

bool search_trie(Spell_checker_trie_ptr trie, char* word);

Spell_checker_trie_node_ptr get_trie_node(Spell_checker_trie_ptr trie, char* word);

bool trie_starts_with(Spell_checker_trie_ptr trie, char* prefix);

#endif //SPELLCHECKER_SPELLCHECKERTRIE_H
