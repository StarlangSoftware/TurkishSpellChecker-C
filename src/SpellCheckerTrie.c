//
// Created by Olcay Taner YILDIZ on 13.01.2024.
//

#include <Memory/Memory.h>
#include <Dictionary/Word.h>
#include "SpellCheckerTrie.h"

/**
 * A constructor of SpellCheckerTrie class which constructs a new SpellCheckerTrie with an empty root node
 */
Spell_checker_trie_ptr create_spell_checker_trie() {
    Spell_checker_trie_ptr result = malloc_(sizeof(Spell_checker_trie));
    result->root_node = create_spell_checker_trie_node();
    return result;
}

/**
 * Inserts a new word into the SpellCheckerTrie
 *
 * @param word The word to be inserted
 */
void insert_to_spell_checker_trie(Spell_checker_trie_ptr trie, char *word) {
    Spell_checker_trie_node_ptr current_node = trie->root_node;
    for (int i = 0; i < word_size(word); i++){
        String_ptr ch = char_at(word, i);
        if (get_trie_node_child(current_node, ch->s) == NULL){
            add_trie_node_child(current_node, ch->s, create_spell_checker_trie_node());
        }
        current_node = get_trie_node_child(current_node, ch->s);
        free_string_ptr(ch);
    }
    current_node->is_end_of_word = true;
}

/**
 * Checks if a word is in the SpellCheckerTrie
 *
 * @param word The word to be searched for
 * @return true if the word is in the SpellCheckerTrie, false otherwise
 */
bool search_trie(Spell_checker_trie_ptr trie, char *word) {
    char* lower_cased = to_lowercase(word);
    Spell_checker_trie_node_ptr node = get_trie_node(trie, lower_cased);
    free_(lower_cased);
    if (node == NULL){
        return false;
    } else {
        return node->is_end_of_word;
    }
}

/**
 * Returns the SpellCheckerTrieNode corresponding to the last character of a given word
 *
 * @param word The word to be searched for
 * @return The SpellCheckerTrieNode corresponding to the last character of the word
 */
Spell_checker_trie_node_ptr get_trie_node(Spell_checker_trie_ptr trie, char *word) {
    Spell_checker_trie_node_ptr current_node = trie->root_node;
    for (int i = 0; i < word_size(word); i++){
        String_ptr ch = char_at(word, i);
        if (get_trie_node_child(current_node, ch->s) == NULL){
            return NULL;
        }
        current_node = get_trie_node_child(current_node, ch->s);
        free_string_ptr(ch);
    }
    return current_node;
}

/**
 * Checks if a given prefix exists in the SpellCheckerTrie
 *
 * @param prefix The prefix to be searched for
 * @return true if the prefix exists, false otherwise
 */
bool trie_starts_with(Spell_checker_trie_ptr trie, char *prefix) {
    if (get_trie_node(trie, prefix) == NULL){
        return false;
    } else {
        return true;
    }
}

/**
 * Frees memory allocated for spellChecker trie. Calls recursive free trie node method.
 * @param trie Trei to be deallocated.
 */
void free_spell_checker_trie(Spell_checker_trie_ptr trie) {
    free_spell_checker_trie_node(trie->root_node);
    free_(trie);
}
