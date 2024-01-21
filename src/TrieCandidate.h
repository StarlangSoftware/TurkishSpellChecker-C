//
// Created by Olcay Taner YILDIZ on 12.01.2024.
//

#ifndef SPELLCHECKER_TRIECANDIDATE_H
#define SPELLCHECKER_TRIECANDIDATE_H

struct trie_candidate{
    char* name;
    int current_index;
    double current_penalty;
};

typedef struct trie_candidate Trie_candidate;

typedef Trie_candidate *Trie_candidate_ptr;

Trie_candidate_ptr create_trie_candidate(char* word, int current_index, double current_penalty);

void free_trie_candidate(Trie_candidate_ptr trie_candidate);

void next_index(Trie_candidate_ptr trie_candidate);

#endif //SPELLCHECKER_TRIECANDIDATE_H
