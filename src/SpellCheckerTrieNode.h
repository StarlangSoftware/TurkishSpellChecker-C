//
// Created by Olcay Taner YILDIZ on 13.01.2024.
//

#ifndef SPELLCHECKER_SPELLCHECKERTRIENODE_H
#define SPELLCHECKER_SPELLCHECKERTRIENODE_H

#include <HashMap/HashMap.h>

struct spell_checker_trie_node{
    Hash_map_ptr children;
    bool is_end_of_word;
};

typedef struct spell_checker_trie_node Spell_checker_trie_node;

typedef Spell_checker_trie_node *Spell_checker_trie_node_ptr;

Spell_checker_trie_node_ptr create_spell_checker_trie_node();

void free_spell_checker_trie_node(Spell_checker_trie_node_ptr trie_node);

Spell_checker_trie_node_ptr get_trie_node_child(Spell_checker_trie_node_ptr trie_node, const char* ch);

void add_trie_node_child(Spell_checker_trie_node_ptr parent,
                         char* ch,
                         Spell_checker_trie_node_ptr child);

char* children_to_string(Spell_checker_trie_node_ptr trie_node);

#endif //SPELLCHECKER_SPELLCHECKERTRIENODE_H
