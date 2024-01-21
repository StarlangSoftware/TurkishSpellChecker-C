//
// Created by Olcay Taner YILDIZ on 21.01.2024.
//

#include <Memory/Memory.h>
#include <FileUtils.h>
#include <string.h>
#include <math.h>
#include "TrieBasedSpellChecker.h"
#include "SimpleSpellChecker.h"
#include "Candidate.h"

Trie_based_spell_checker_ptr create_trie_based_spell_checker(Fsm_morphological_analyzer_ptr fsm,
                                                             Spell_checker_parameter_ptr parameter,
                                                             N_gram_ptr n_gram) {
    Trie_based_spell_checker_ptr result = malloc_(sizeof(Trie_based_spell_checker), "create_trie_based_spell_checker");
    result->fsm = fsm;
    result->parameter = parameter;
    result->n_gram = n_gram;
    result->merged_words = load_merged_dictionary();
    result->split_words = load_split_dictionary();
    result->generated_words = read_lines("generated_words.txt");
    result->trie = prepare_spell_checker_trie(result);
    return result;
}

void free_trie_based_spell_checker(Trie_based_spell_checker_ptr spell_checker) {
    free_spell_check_parameter(spell_checker->parameter);
    free_n_gram(spell_checker->n_gram);
    free_hash_map2(spell_checker->merged_words, free_, free_);
    free_hash_map2(spell_checker->split_words, free_, free_);
    free_array_list(spell_checker->generated_words, free_);
    free_spell_checker_trie(spell_checker->trie);
    free_(spell_checker);
}

Spell_checker_trie_ptr prepare_spell_checker_trie(Trie_based_spell_checker_ptr spell_checker) {
    Spell_checker_trie_ptr result = create_spell_checker_trie();
    for (int i = 0; i < spell_checker->generated_words->size; i++){
        insert_to_spell_checker_trie(result, array_list_get(spell_checker->generated_words, i));
    }
    return result;
}

int search_trie_candidates(Array_list_ptr candidates, Trie_candidate_ptr candidate) {
     for (int i = 0; i < candidates->size; i++){
         Candidate_ptr candidate1 = array_list_get(candidates, i);
         if (strcmp(candidate1->name, candidate->name) == 0){
             return i;
         }
     }
     return -1;
}

/**
 * The candidateList method takes a Word as an input and creates a candidates ArrayList by calling generateCandidateList
 * method with given word. Then, it loops i times where i ranges from 0 to size of candidates ArrayList and creates a
 * FsmParseList by calling morphologicalAnalysis with each item of candidates ArrayList. If the size of
 * FsmParseList is 0, it then removes the ith item.
 *
 * @param word Word input.
 * @return candidates ArrayList.
 */
Array_list_ptr candidate_list_trie(Spell_checker_trie_ptr trie, char *word) {
    Trie_candidate_ptr candidate;
    Queue_ptr candidates = create_queue();
    Array_list_ptr results = create_array_list();
    enqueue_front(candidates, create_trie_candidate(word, -1, 0));
    char* candidate_name;
    double penaltyLimit = fmin(word_size(word) / 2.0, 3.0);
    while (!is_queue_empty(candidates)) {
        candidate = dequeue(candidates);
        candidate_name = candidate->name;
        if (search_trie(trie, candidate_name)) {
            int itemIndex = search_trie_candidates(results, candidate);
            Trie_candidate_ptr trie_candidate = array_list_get(results, itemIndex);
            if (itemIndex != -1 && trie_candidate->current_penalty <= candidate->current_penalty) {
            } else {
                array_list_add(results, candidate);
            }
        } else {
            if (candidate->current_penalty > penaltyLimit - 1 ||
                candidate->current_index >= word_size(candidate_name) - 1) {
            } else {
                next_index(candidate);
                enqueue(candidates, create_trie_candidate(candidate->name, candidate->current_index,
                                                       candidate->current_penalty));
                Array_list_ptr new_candidates = generate_trie_candidates(trie, candidate);
                for (int i = 0; i < new_candidates->size; i++){
                    enqueue(candidates, array_list_get(new_candidates, i));
                }
                free_array_list(new_candidates, NULL);
            }
        }
    }
    return results;
}

Array_list_ptr generate_trie_candidates(Spell_checker_trie_ptr trie, Trie_candidate_ptr candidate){
    Array_list_ptr candidates = create_array_list();
    char* currentName = candidate->name;
    String_ptr deasciifiedWord;
    int currentIndex = candidate->current_index;
    double currentPenalty = candidate->current_penalty;
    char* letters;
    String_ptr st = substring(currentName, 0, currentIndex);
    Spell_checker_trie_node_ptr currentNode = get_trie_node(trie, st->s);
    free_string_ptr(st);
    if (currentNode == NULL) {
        return candidates;
    }
    String_ptr ch = char_at(currentName, currentIndex);
    if (string_in_list(ch->s, (char*[]) {"c", "g", "i", "s", "o", "u"}, 6)){
        char* ch2 = NULL;
        if (string_equals2(ch, "c")) {
            ch2 = "ç";
        } else {
            if (string_equals2(ch, "g")) {
                ch2 = "ğ";
            } else {
                if (string_equals2(ch, "i")) {
                    ch2 = "ı";
                } else {
                    if (string_equals2(ch, "s")) {
                        ch2 = "ş";
                    } else {
                        if (string_equals2(ch, "o")) {
                            ch2 = "ö";
                        } else {
                            if (string_equals2(ch, "u")) {
                                ch2 = "ü";
                            }
                        }
                    }
                }
            }
        }
        String_ptr s1 = substring(currentName, 0, currentIndex - 1);
        String_ptr s3 = substring2(currentName, currentIndex + 1);
        deasciifiedWord = create_string4(s1->s, ch2, s3->s);
        String_ptr s4 = substring(deasciifiedWord->s, 0, currentIndex + 1);
        if (trie_starts_with(trie, s4->s)){
            array_list_add(candidates, create_trie_candidate(deasciifiedWord->s, currentIndex, currentPenalty + 0.2));
        }
        free_string_ptr(s4);
        free_string_ptr(deasciifiedWord);
        free_string_ptr(s1);
        free_string_ptr(s3);
    }
    free_string_ptr(ch);
    letters = children_to_string(currentNode);
    String_ptr s1 = substring(currentName, 0, currentIndex);
    String_ptr s2 = substring2(currentName, currentIndex + 1);
    String_ptr s3 = substring2(currentName, currentIndex);
    for (int j = 0; j < word_size(letters); j++) {
        ch = char_at(letters, j);
        String_ptr replaced = create_string4(s1->s, ch->s, s2->s);
        array_list_add(candidates, create_trie_candidate(replaced->s, currentIndex, currentPenalty + 1));
        free_string_ptr(replaced);
        String_ptr added = create_string4(s1->s, ch->s, s3->s);
        array_list_add(candidates, create_trie_candidate(added->s, currentIndex, currentPenalty + 1));
        free_string_ptr(added);
        String_ptr addedLast = create_string3(currentName, ch->s);
        if (trie_starts_with(trie, addedLast->s)) {
            array_list_add(candidates, create_trie_candidate(addedLast->s, currentIndex, currentPenalty + 1));
        }
        free_string_ptr(addedLast);
        free_string_ptr(ch);
    }
    String_ptr deleted = create_string3(s1->s, s2->s);
    if (word_size(deleted->s) > 1) {
        array_list_add(candidates, create_trie_candidate(deleted->s, currentIndex - 1, currentPenalty + 1));
    }
    free_string_ptr(deleted);
    String_ptr s4 = substring2(currentName, currentIndex + 2);
    if (currentIndex < word_size(currentName) - 1 && trie_starts_with(trie, s4->s)) {
        String_ptr ch1 = char_at(currentName, currentIndex);
        String_ptr ch2 = char_at(currentName, currentIndex + 1);
        String_ptr swapped = create_string4(s1->s, ch2->s, ch1->s);
        string_append(swapped, s4->s);
        array_list_add(candidates, create_trie_candidate(swapped->s, currentIndex, currentPenalty + 1));
        free_string_ptr(ch1);
        free_string_ptr(ch2);
        free_string_ptr(swapped);
    }
    free_string_ptr(s4);
    free_string_ptr(s1);
    free_string_ptr(s2);
    free_string_ptr(s3);
    return candidates;
}