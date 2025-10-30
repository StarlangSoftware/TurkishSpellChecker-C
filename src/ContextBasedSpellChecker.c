//
// Created by Olcay Taner YILDIZ on 21.01.2024.
//

#include <Memory/Memory.h>
#include <FileUtils.h>
#include <string.h>
#include <math.h>
#include "ContextBasedSpellChecker.h"
#include "SimpleSpellChecker.h"
#include "Candidate.h"

/**
 * A constructor of {@link ContextBasedSpellChecker} class which takes an {@link FsmMorphologicalAnalyzer}, an {@link NGram}
 * and a {@link SpellCheckerParameter} as inputs. Then, calls its super class {@link NGramSpellChecker} with given inputs.
 *
 * @param fsm       {@link FsmMorphologicalAnalyzer} type input.
 * @param parameter {@link SpellCheckerParameter} type input.
 * @param n_gram     {@link NGram} type input.
 */
Context_based_spell_checker_ptr create_context_based_spell_checker(Fsm_morphological_analyzer_ptr fsm,
                                                                   Spell_checker_parameter_ptr parameter,
                                                                   N_gram_ptr n_gram) {
    Context_based_spell_checker_ptr result = malloc_(sizeof(Context_based_spell_checker));
    result->fsm = fsm;
    result->parameter = parameter;
    result->n_gram = n_gram;
    result->merged_words = load_merged_dictionary();
    result->split_words = load_split_dictionary();
    result->context_list = load_context_dictionaries();
    return result;
}

/**
 * Frees memory allocated for a SpellChecker. Frees spell checker parameter, N-Gram, merged words and split word hash
 * maps, the context list hash map.
 * @param spell_checker SpellChecker object to be deallocated.
 */
void free_context_based_spell_checker(Context_based_spell_checker_ptr spell_checker) {
    free_spell_check_parameter(spell_checker->parameter);
    free_n_gram(spell_checker->n_gram);
    free_hash_map2(spell_checker->merged_words, free_, free_);
    free_hash_map2(spell_checker->split_words, free_, free_);
    Array_list_ptr list = value_list(spell_checker->context_list);
    for (int i = 0; i < list->size; i++){
        Array_list_ptr items = array_list_get(list, i);
        free_array_list(items, free_);
    }
    free_array_list(list, NULL);
    free_hash_map2(spell_checker->context_list, free_, NULL);
    free_(spell_checker);
}

/**
 * {@inheritDoc}
 * This method also loads context information from a file.
 */
Hash_map_ptr load_context_dictionaries() {
    Hash_map_ptr result = create_string_hash_map();
    Array_list_ptr lines = read_lines("context_list.txt");
    for (int i = 0; i < lines->size; i++){
        char* line = array_list_get(lines, i);
        Array_list_ptr items = str_split(line, '\t');
        char* word = array_list_get(items, 0);
        Array_list_ptr items2 = str_split(array_list_get(items, 1), ' ');
        hash_map_insert(result, clone_string(word), items2);
        free_array_list(items, free_);
    }
    free_array_list(lines, free_);
    return result;
}

/**
 * Calculates the Damerau-Levenshtein distance between two strings.
 * This method also allows for the transposition of adjacent characters,
 * which is not possible in a standard Levenshtein distance calculation.
 *
 * @param first  the first string
 * @param second the second string
 * @return the Damerau-Levenshtein distance between the two strings
 */
int damerau_levenshtein_distance(char *first, char *second) {
    if (first == NULL) {
        return second == NULL ? 0 : word_size(second);
    }
    if (second == NULL) {
        return word_size(first);
    }
    int firstLength = word_size(first);
    int secondLength = word_size(second);
    int** distanceMatrix = malloc_((firstLength + 1) * sizeof(int*));
    for (int i = 0; i < firstLength + 1; i++){
        distanceMatrix[i] = malloc_((secondLength + 1) * sizeof(int));
    }
    for (int firstIndex = 0; firstIndex <= firstLength; firstIndex++) {
        distanceMatrix[firstIndex][0] = firstIndex;
    }
    for (int secondIndex = 0; secondIndex <= secondLength; secondIndex++) {
        distanceMatrix[0][secondIndex] = secondIndex;
    }
    for (int firstIndex = 1; firstIndex <= firstLength; firstIndex++) {
        for (int secondIndex = 1; secondIndex <= secondLength; secondIndex++) {
            String_ptr ch1 = char_at(first, firstIndex - 1);
            String_ptr ch2 = char_at(second, secondIndex - 1);
            int cost = strcmp(ch1->s, ch2->s) ? 0 : 1;
            distanceMatrix[firstIndex][secondIndex] = fmin(fmin(distanceMatrix[firstIndex - 1][secondIndex] + 1,
                                                              distanceMatrix[firstIndex][secondIndex - 1] + 1), distanceMatrix[firstIndex - 1][secondIndex - 1] + cost);

            if (firstIndex == 1 || secondIndex == 1) {
                free_string_ptr(ch1);
                free_string_ptr(ch2);
                continue;
            }
            String_ptr ch3 = char_at(first, firstIndex - 2);
            String_ptr ch4 = char_at(second, secondIndex - 2);
            if (strcmp(ch1->s, ch4->s) == 0 && strcmp(ch2->s, ch3->s) == 0) {
                distanceMatrix[firstIndex][secondIndex] = fmin(distanceMatrix[firstIndex][secondIndex], distanceMatrix[firstIndex - 2][secondIndex - 2] + cost);
            }
            free_string_ptr(ch1);
            free_string_ptr(ch2);
            free_string_ptr(ch3);
            free_string_ptr(ch4);
        }
    }
    int result = distanceMatrix[firstLength][secondLength];
    for (int i = 0; i < firstLength + 1; i++){
        free_(distanceMatrix[i]);
    }
    free_(distanceMatrix);
    return result;
}

/**
 * Uses context information to generate candidates for a misspelled word.
 * The candidates are the words that are in the context of the neighbouring words of the misspelled word.
 * Uses the {@Link damerauLevenshteinDistance(String, String) method to calculate the distance between the misspelled word and
 * the candidates and to determine whether the candidates are valid.
 *
 * @param word     the misspelled word
 * @param sentence the sentence containing the misspelled word
 * @return an ArrayList of valid candidates for the misspelled word
 */
Array_list_ptr candidate_list_context(Hash_map_ptr context_list, Fsm_morphological_analyzer_ptr fsm, char *word, Sentence_ptr sentence) {
    Array_list_ptr candidates = create_array_list();
    Array_list_ptr validCandidates = create_array_list();
    for (int i = 0; i < sentence->words->size; i++){
        if (strcmp(word, array_list_get(sentence->words, i)) == 0){
            array_list_remove(sentence->words, i, free_);
            break;
        }
    }
    for (int i = 0; i < sentence->words->size; i++){
        char* w = array_list_get(sentence->words, i);
        Fsm_parse_list_ptr parse_list = morphological_analysis(fsm, w);
        if (parse_list->fsm_parses->size > 0){
            char* root = get_parse_with_longest_root_word(parse_list)->root->name;
            if (hash_map_contains(context_list, root)){
                Array_list_ptr context = hash_map_get(context_list, root);
                for (int j = 0; j < context->size; j++){
                    char* s = array_list_get(context, j);
                    array_list_add(candidates, create_candidate(clone_string(s), CONTEXT_BASED));
                }
            }
        }
        free_fsm_parse_list(parse_list);
    }
    for (int i = 0; i < candidates->size; i++){
        Candidate_ptr candidate = array_list_get(candidates, i);
        int distance;
        if (word_size(candidate->name) < 5){
            distance = 1;
        } else {
            if (word_size(candidate->name) < 7){
                distance = 2;
            } else {
                distance = 3;
            }
        }
        if (damerau_levenshtein_distance(word, candidate->name) <= distance){
            array_list_add(validCandidates, candidate);
        } else {
            free_candidate(candidate);
        }
    }
    free_array_list(candidates, NULL);
    return validCandidates;
}
