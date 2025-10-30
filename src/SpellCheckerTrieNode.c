//
// Created by Olcay Taner YILDIZ on 13.01.2024.
//

#include <Memory/Memory.h>
#include <FileUtils.h>
#include <Dictionary/Word.h>
#include "SpellCheckerTrieNode.h"

/**
 * A constructor of SpellCheckerTrieNode class which constructs a new SpellCheckerTrieNode with an empty children HashMap
 */
Spell_checker_trie_node_ptr create_spell_checker_trie_node() {
    Spell_checker_trie_node_ptr result = malloc_(sizeof(Spell_checker_trie_node));
    result->children = create_string_hash_map();
    result->is_end_of_word = false;
    return result;
}

/**
 * Returns the child SpellCheckerTrieNode with the given character as its value.
 * @param ch The character value of the child SpellCheckerTrieNode.
 * @return SpellCheckerTrieNode with the given character value.
 */
Spell_checker_trie_node_ptr get_trie_node_child(Spell_checker_trie_node_ptr trie_node, const char *ch) {
    if (hash_map_contains(trie_node->children, ch)){
        return hash_map_get(trie_node->children, ch);
    } else {
        return NULL;
    }
}

/**
 * Adds a child TrieNode to the current TrieNode instance.
 *
 * @param ch the character key of the child node to be added.
 * @param child the TrieNode object to be added as a child.
 */
void add_trie_node_child(Spell_checker_trie_node_ptr parent,
                         char *ch,
                         Spell_checker_trie_node_ptr child) {
    hash_map_insert(parent->children, ch, child);
}

/**
 * Returns a string representation of the keys of all child TrieNodes of the current TrieNode instance.
 *
 * @return a string of characters representing the keys of all child TrieNodes.
 */
char *children_to_string(Spell_checker_trie_node_ptr trie_node) {
    char result[MAX_WORD_LENGTH] = "";
    Array_list_ptr list = key_list(trie_node->children);
    for (int i = 0; i < list->size; i++){
        sprintf(result, "%s%s", result, (char*) array_list_get(list, i));
    }
    free_array_list(list, NULL);
    return clone_string(result);
}

/**
 * Recursive method to deallocate trie nodes. It calls itself with its children.
 * @param trie_node Trie node to be deallocated.
 */
void free_spell_checker_trie_node(Spell_checker_trie_node_ptr trie_node) {
    free_hash_map(trie_node->children, (void (*)(void *)) free_spell_checker_trie_node);
    free_(trie_node);
}
