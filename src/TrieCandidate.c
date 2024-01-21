//
// Created by Olcay Taner YILDIZ on 12.01.2024.
//

#include <Memory/Memory.h>
#include <StringUtils.h>
#include "TrieCandidate.h"

Trie_candidate_ptr create_trie_candidate(char *word, int current_index, double current_penalty) {
    Trie_candidate_ptr result = malloc_(sizeof(Trie_candidate), "create_trie_candidate");
    result->name = str_copy(result->name, word);
    result->current_index = current_index;
    result->current_penalty = current_penalty;
    return result;
}

void free_trie_candidate(Trie_candidate_ptr trie_candidate) {
    free_(trie_candidate->name);
    free_(trie_candidate);
}

void next_index(Trie_candidate_ptr trie_candidate) {
    trie_candidate->current_index++;
}
