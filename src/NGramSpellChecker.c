//
// Created by Olcay Taner YILDIZ on 20.01.2024.
//

#include <Memory/Memory.h>
#include <RegularExpression.h>
#include <math.h>
#include "NGramSpellChecker.h"
#include "SimpleSpellChecker.h"
#include "Candidate.h"

/**
 * A constructor of NGramSpellChecker class which takes a FsmMorphologicalAnalyzer and an NGram
 * as inputs. Then, calls its super class SimpleSpellChecker with given FsmMorphologicalAnalyzer and
 * assigns given NGram to the nGram variable.
 *
 * @param fsm   FsmMorphologicalAnalyzer type input.
 * @param nGram NGram type input.
 */
N_gram_spell_checker_ptr create_n_gram_spell_checker(Fsm_morphological_analyzer_ptr fsm,
                                                     Spell_checker_parameter_ptr parameter,
                                                     N_gram_ptr n_gram) {
    N_gram_spell_checker_ptr result = malloc_(sizeof(N_gram_spell_checker));
    result->fsm = fsm;
    result->parameter = parameter;
    result->n_gram = n_gram;
    result->merged_words = load_merged_dictionary();
    result->split_words = load_split_dictionary();
    return result;
}

/**
 * Frees memory allocated for an NGramSpellChecker. Deallocates memory for NGram, merged words and split words hash
 * maps.
 * @param spell_checker SpellChecker object to be deallocated.
 */
void free_n_gram_spell_checker(N_gram_spell_checker_ptr spell_checker) {
    free_spell_check_parameter(spell_checker->parameter);
    free_n_gram(spell_checker->n_gram);
    free_hash_map2(spell_checker->merged_words, free_, free_);
    free_hash_map2(spell_checker->split_words, free_, free_);
    free_(spell_checker);
}

/**
 * Checks the morphological analysis of the given word in the given index. If there is no misspelling, it returns
 * the longest root word of the possible analyses.
 * @param sentence Sentence to be analyzed.
 * @param index Index of the word
 * @return If the word is misspelled, null; otherwise the longest root word of the possible analyses.
 */
char * check_analysis_and_set_root_for_word_at_index(N_gram_spell_checker spell_checker,
                                                     Sentence_ptr sentence,
                                                     int index) {
    if (index < sentence->words->size) {
        char* word_name = array_list_get(sentence->words, index);
        Regular_expression_ptr regular1 = create_regular_expression(".*\\d+.*");
        Regular_expression_ptr regular2 = create_regular_expression(".*[a-zA-ZçöğüşıÇÖĞÜŞİ]+.*");
        if ((full_matches(regular1, word_name) && full_matches(regular2, word_name) && str_contains(word_name, "'")) || word_size(word_name) < spell_checker.parameter->min_word_length) {
            free_regular_expression(regular1);
            free_regular_expression(regular2);
            return word_name;
        }
        free_regular_expression(regular1);
        free_regular_expression(regular2);
        Fsm_parse_list_ptr parse_list1 = morphological_analysis(spell_checker.fsm, word_name);
        if (parse_list1->fsm_parses->size != 0) {
            if (spell_checker.parameter->root_n_gram) {
                char* result = get_parse_with_longest_root_word(parse_list1)->root->name;
                free_fsm_parse_list(parse_list1);
                return result;
            }
            free_fsm_parse_list(parse_list1);
            return word_name;
        } else {
            free_fsm_parse_list(parse_list1);
            String_ptr upper_case_word_name = to_capital(word_name);
            Fsm_parse_list_ptr parse_list2 = morphological_analysis(spell_checker.fsm, upper_case_word_name->s);
            free_string_ptr(upper_case_word_name);
            if (parse_list2->fsm_parses->size != 0) {
                if (spell_checker.parameter->root_n_gram) {
                    char* result = get_parse_with_longest_root_word(parse_list2)->root->name;
                    free_fsm_parse_list(parse_list2);
                    return result;
                }
                free_fsm_parse_list(parse_list2);
                return word_name;
            }
            free_fsm_parse_list(parse_list2);
        }
    }
    return NULL;
}

/**
 * Checks the morphological analysis of the given word. If there is no misspelling, it returns
 * the longest root word of the possible analysis.
 *
 * @param word Word to be analyzed.
 * @return If the word is misspelled, null; otherwise the longest root word of the possible analysis.
 */
char *check_analysis_and_set_root(N_gram_spell_checker spell_checker, char *word) {
    Fsm_parse_list_ptr parse_list1 = morphological_analysis(spell_checker.fsm, word);
    if (parse_list1->fsm_parses->size != 0) {
        if (spell_checker.parameter->root_n_gram) {
            char* result = get_parse_with_longest_root_word(parse_list1)->root->name;
            free_fsm_parse_list(parse_list1);
            return result;
        } else {
            free_fsm_parse_list(parse_list1);
            return word;
        }
    }
    free_fsm_parse_list(parse_list1);
    String_ptr upper_case_word_name = to_capital(word);
    Fsm_parse_list_ptr parse_list2 = morphological_analysis(spell_checker.fsm, upper_case_word_name->s);
    free_string_ptr(upper_case_word_name);
    if (parse_list2->fsm_parses->size != 0) {
        if (spell_checker.parameter->root_n_gram) {
            char* result = get_parse_with_longest_root_word(parse_list2)->root->name;
            free_fsm_parse_list(parse_list2);
            return result;
        } else {
            free_fsm_parse_list(parse_list2);
            return word;
        }
    }
    free_fsm_parse_list(parse_list2);
    return NULL;
}

/**
 * The spellCheck method takes a Sentence as an input and loops i times where i ranges from 0 to size of words in given sentence.
 * Then, it calls morphologicalAnalysis method with each word and assigns it to the FsmParseList, if the size of
 * FsmParseList is equal to the 0, it adds current word to the candidateList and assigns it to the candidates ArrayList.
 * <p>
 * Later on, it loops through candidates ArrayList and calls morphologicalAnalysis method with each word and
 * assigns it to the FsmParseList. Then, it gets the root from FsmParseList. For the first time, it defines a previousRoot
 * by calling getProbability method with root, and for the following times it calls getProbability method with previousRoot and root.
 * Then, it finds out the best probability and the corresponding candidate as best candidate and adds it to the result Sentence.
 * <p>
 * If the size of FsmParseList is not equal to 0, it directly adds the current word to the result Sentence and finds
 * the previousRoot directly from the FsmParseList.
 *
 * @param sentence Sentence type input.
 * @return Sentence result.
 */
Sentence_ptr spell_check_n_gram(N_gram_spell_checker spell_checker, Sentence_ptr sentence) {
    char *word, *best_root, *previous_root = NULL, *root, *next_root;
    Candidate_ptr best_candidate;
    double previous_probability, next_probability, best_probability;
    Array_list_ptr candidates = create_array_list();
    Sentence_ptr result = create_sentence();
    root = check_analysis_and_set_root_for_word_at_index(spell_checker, sentence, 0);
    next_root = check_analysis_and_set_root_for_word_at_index(spell_checker, sentence, 1);
    for (int i = 0; i < sentence->words->size; i++) {
        char* next_word = NULL;
        char* previous_word = NULL;
        char* next_next_word = NULL;
        char* previous_previous_word = NULL;
        word = array_list_get(sentence->words, i);
        if (i > 0) {
            previous_word = array_list_get(sentence->words, i - 1);
        }
        if (i > 1) {
            previous_previous_word = array_list_get(sentence->words, i - 2);
        }
        if (i < sentence->words->size - 1) {
            next_word = array_list_get(sentence->words, i + 1);
        }
        if (i < sentence->words->size - 2) {
            next_next_word = array_list_get(sentence->words, i + 2);
        }
        if (forced_misspell_check(spell_checker.fsm, word, result)) {
            previous_root = check_analysis_and_set_root_for_word_at_index(spell_checker, result, result->words->size - 1);
            root = next_root;
            next_root = check_analysis_and_set_root_for_word_at_index(spell_checker, sentence, i + 2);
            continue;
        }
        if (forced_backward_merge_check(spell_checker.merged_words, word, result, previous_word) ||
            forced_suffix_merge_check(spell_checker.fsm, word, result, previous_word)) {
            previous_root = check_analysis_and_set_root_for_word_at_index(spell_checker, result, result->words->size - 1);
            root = check_analysis_and_set_root_for_word_at_index(spell_checker, sentence, i + 1);
            next_root = check_analysis_and_set_root_for_word_at_index(spell_checker, sentence, i + 2);
            continue;
        }
        if (forced_forward_merge_check(spell_checker.merged_words, word, result, next_word) ||
            forced_hyphen_merge_check(spell_checker.fsm, word, result, previous_word, next_word)) {
            i++;
            previous_root = check_analysis_and_set_root_for_word_at_index(spell_checker, result, result->words->size - 1);
            root = check_analysis_and_set_root_for_word_at_index(spell_checker, sentence, i + 1);
            next_root = check_analysis_and_set_root_for_word_at_index(spell_checker, sentence, i + 2);
            continue;
        }
        if (forced_split_check(spell_checker.split_words, word, result) || forced_shortcut_check(word, result)) {
            previous_root = check_analysis_and_set_root_for_word_at_index(spell_checker, result, result->words->size - 1);
            root = next_root;
            next_root = check_analysis_and_set_root_for_word_at_index(spell_checker, sentence, i + 2);
            continue;
        }
        if (spell_checker.parameter->suffix_check) {
            if (forced_de_da_split_check(spell_checker.fsm, word, result) || forced_question_suffix_split_check(spell_checker.fsm, word, result) || forced_suffix_split_check(spell_checker.fsm, word, result)) {
                previous_root = check_analysis_and_set_root_for_word_at_index(spell_checker, result, result->words->size - 1);
                root = next_root;
                next_root = check_analysis_and_set_root_for_word_at_index(spell_checker, sentence, i + 2);
                continue;
            }
        }
        Fsm_parse_list_ptr parse_list = morphological_analysis(spell_checker.fsm, word);
        if (root == NULL ||
            (word_size(word) <= spell_checker.parameter->min_word_length && parse_list->fsm_parses->size == 0)) {
            free_fsm_parse_list(parse_list);
            array_list_clear(candidates, (void (*)(void *)) free_candidate);
            if (root == NULL) {
                free_array_list(candidates, (void (*)(void *)) free_candidate);
                candidates = candidate_list_simple(spell_checker.fsm, word);
                Array_list_ptr split_candidates = split_candidates_list(spell_checker.fsm, word);
                array_list_add_all(candidates, split_candidates);
                free_array_list(split_candidates, NULL);
            }
            Array_list_ptr mergedCandidates = merged_candidates_list(spell_checker.fsm, previous_word, word, next_word);
            array_list_add_all(candidates, mergedCandidates);
            free_array_list(mergedCandidates, NULL);
            best_candidate = create_candidate(word, NO_CHANGE);
            best_root = word;
            best_probability = spell_checker.parameter->threshold;
            for (int j = 0; i < candidates->size; j++) {
                Candidate_ptr candidate = array_list_get(candidates, j);
                if (candidate->operator == SPELL_CHECK || candidate->operator == MISSPELLED_REPLACE ||
                    candidate->operator == CONTEXT_BASED || candidate->operator == TRIE_BASED) {
                    root = check_analysis_and_set_root(spell_checker, candidate->name);
                }
                if (candidate->operator == BACKWARD_MERGE && previous_word != NULL) {
                    String_ptr st = create_string3(previous_word, word);
                    root = check_analysis_and_set_root(spell_checker, st->s);
                    free_string_ptr(st);
                    if (previous_previous_word != NULL) {
                        previous_root = check_analysis_and_set_root(spell_checker, previous_previous_word);
                    }
                }
                if (candidate->operator == FORWARD_MERGE && next_word != NULL) {
                    String_ptr st = create_string3(word, next_word);
                    root = check_analysis_and_set_root(spell_checker, st->s);
                    free_string_ptr(st);
                    if (next_next_word != NULL) {
                        next_root = check_analysis_and_set_root(spell_checker, next_next_word);
                    }
                }
                if (previous_root != NULL) {
                    if (candidate->operator == SPLIT) {
                        Array_list_ptr items = str_split(candidate->name, ' ');
                        root = check_analysis_and_set_root(spell_checker, array_list_get(items, 0));
                        free_array_list(items, free_);
                    }
                    previous_probability = get_probability(spell_checker.n_gram, 2, previous_root, root);
                } else {
                    previous_probability = 0.0;
                }
                if (next_root != NULL) {
                    if (candidate->operator == SPLIT) {
                        Array_list_ptr items = str_split(candidate->name, ' ');
                        root = check_analysis_and_set_root(spell_checker, array_list_get(items, 1));
                        free_array_list(items, free_);
                    }
                    next_probability = get_probability(spell_checker.n_gram, 2, root, next_root);
                } else {
                    next_probability = 0.0;
                }
                if (fmax(previous_probability, next_probability) > best_probability || candidates->size == 1) {
                    best_candidate = candidate;
                    best_root = root;
                    best_probability = fmax(previous_probability, next_probability);
                }
            }
            if (best_candidate->operator == FORWARD_MERGE) {
                i++;
            }
            if (best_candidate->operator == BACKWARD_MERGE) {
                sentence_replace_word(result, i - 1, clone_string(best_candidate->name));
            } else {
                if (best_candidate->operator == SPLIT) {
                    add_split_words(best_candidate->name, result);
                } else {
                    sentence_add_word(result, clone_string(best_candidate->name));
                }
            }
            root = best_root;
        } else {
            free_fsm_parse_list(parse_list);
            sentence_add_word(result, word);
        }
        previous_root = root;
        root = next_root;
        next_root = check_analysis_and_set_root_for_word_at_index(spell_checker, sentence, i + 2);
    }
    return result;
}
