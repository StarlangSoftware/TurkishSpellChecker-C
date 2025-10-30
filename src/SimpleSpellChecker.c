//
// Created by Olcay Taner YILDIZ on 14.01.2024.
//

#include <StringUtils.h>
#include <Dictionary/Word.h>
#include <Dictionary/TxtDictionary.h>
#include <FsmMorphologicalAnalyzer.h>
#include <Memory/Memory.h>
#include <FileUtils.h>
#include <string.h>
#include "SimpleSpellChecker.h"
#include "Language/TurkishLanguage.h"
#include "Candidate.h"
#include <RegularExpression.h>
#include <stdlib.h>

/**
 * The generateCandidateList method takes a String as an input. Firstly, it creates a String consists of lowercase Turkish letters
 * and an vector candidates. Then, it loops i times where i ranges from 0 to the length of given word. It gets substring
 * from 0 to ith index and concatenates it with substring from i+1 to the last index as a new String called deleted. Then, adds
 * this String to the candidates vector. Secondly, it loops j times where j ranges from 0 to length of
 * lowercase letters String and adds the jth character of this String between substring of given word from 0 to ith index
 * and the substring from i+1 to the last index, then adds it to the candidates vector. Thirdly, it loops j
 * times where j ranges from 0 to length of lowercase letters String and adds the jth character of this String between
 * substring of given word from 0 to ith index and the substring from i to the last index, then adds it to the candidates vector.
 *
 * @param word String input.
 * @return vector candidates.
 */
Array_list_ptr generate_candidate_list_simple(char *word) {
    char* s = TURKISH_LOWERCASE_LETTERS;
    Array_list_ptr candidates = create_array_list();
    char* english_characters = "wqx";
    for (int i = 0; i < word_size(word); i++) {
        String_ptr tmpi = char_at(word, i);
        if (i < word_size(word) - 1){
            String_ptr swapped = substring(word, 0, i);
            String_ptr tmp = char_at(word, i + 1);
            string_append_s(swapped, tmp);
            free_string_ptr(tmp);
            string_append_s(swapped, tmpi);
            tmp = substring2(word, i + 2);
            string_append_s(swapped, tmp);
            free_string_ptr(tmp);
            array_list_add(candidates, create_candidate(swapped->s, SPELL_CHECK));
            free_string_ptr(swapped);
        }
        if (str_contains(LETTERS, tmpi->s) || str_contains(english_characters, tmpi->s)){
            String_ptr deleted = substring(word, 0, i);
            String_ptr tmp = substring2(word, i + 1);
            string_append_s(deleted, tmp);
            free_string_ptr(tmp);
            array_list_add(candidates, create_candidate(deleted->s, SPELL_CHECK));
            free_string_ptr(deleted);
            for (int j = 0; j < word_size(s); j++) {
                String_ptr replaced = substring(word, 0, i);
                tmp = char_at(s, j);
                string_append_s(replaced, tmp);
                free_string_ptr(tmp);
                tmp = substring2(word, i + 1);
                string_append_s(replaced, tmp);
                free_string_ptr(tmp);
                array_list_add(candidates, create_candidate(replaced->s, SPELL_CHECK));
                free_string_ptr(replaced);
            }
            for (int j = 0; j < word_size(s); j++) {
                String_ptr added = substring(word, 0, i);
                tmp = char_at(s, j);
                string_append_s(added, tmp);
                free_string_ptr(tmp);
                tmp = substring2(word, i);
                string_append_s(added, tmp);
                free_string_ptr(tmp);
                array_list_add(candidates, create_candidate(added->s, SPELL_CHECK));
                free_string_ptr(added);
            }
        }
        free_string_ptr(tmpi);
    }
    return candidates;
}

/**
 * The candidateList method takes a Word as an input and creates a candidates vector by calling generateCandidateList
 * method with given word. Then, it loop i times where i ranges from 0 to size of candidates vector and creates a
 * FsmParseList by calling morphologicalAnalysis with each item of candidates vector. If the size of
 * FsmParseList is 0, it then removes the ith item.
 *
 * @param word Word input.
 * @return candidates vector.
 */
Array_list_ptr candidate_list_simple(Fsm_morphological_analyzer_ptr fsm, char *word) {
    Array_list_ptr candidates = create_array_list();
    Txt_dictionary_ptr dictionary = fsm->dictionary;
    Array_list_ptr first_candidates = generate_candidate_list_simple(word);
    for (int i = 0; i < first_candidates->size; i++) {
        Candidate_ptr firstCandidate = array_list_get(first_candidates, i);
        Fsm_parse_list_ptr fsmParseList = morphological_analysis(fsm, firstCandidate->name);
        if (fsmParseList->fsm_parses->size != 0) {
            array_list_add(candidates, create_candidate(firstCandidate->name, firstCandidate->operator));
        } else {
            char* newCandidate = get_correct_form(dictionary, firstCandidate->name);
            Fsm_parse_list_ptr fsmParseList2 = morphological_analysis(fsm, newCandidate);
            if (newCandidate != NULL && fsmParseList2->fsm_parses->size > 0){
                array_list_add(candidates, create_candidate(newCandidate, MISSPELLED_REPLACE));
            }
            free_fsm_parse_list(fsmParseList2);
        }
        free_fsm_parse_list(fsmParseList);
    }
    free_array_list(first_candidates, (void (*)(void *)) free_candidate);
    return candidates;
}

/**
 * A constructor of {@link SimpleSpellChecker} class which takes an {@link FsmMorphologicalAnalyzer} as an input and
 * assigns it to the fsm variable. Then it creates a new {@link SpellCheckerParameter} and assigns it to the parameter.
 * Finally, it calls the loadDictionaries method.
 *
 * @param fsm {@link FsmMorphologicalAnalyzer} type input.
 */
Simple_spell_checker_ptr create_simple_spell_checker(Fsm_morphological_analyzer_ptr fsm) {
    Simple_spell_checker_ptr result = malloc_(sizeof(Simple_spell_checker));
    result->fsm = fsm;
    result->parameter = create_spell_checker_parameter();
    result->merged_words = load_merged_dictionary();
    result->split_words = load_split_dictionary();
    return result;
}

/**
 * Frees memory allocated for a SimpleSpellChecker object. Deallocates memory for merged words and split words lists.
 * @param spell_checker SpellChecker object to be deallocated.
 */
void free_simple_spell_checker(Simple_spell_checker_ptr spell_checker) {
    free_spell_check_parameter(spell_checker->parameter);
    free_hash_map2(spell_checker->merged_words, free_, free_);
    free_hash_map2(spell_checker->split_words, free_, free_);
    free_(spell_checker);
}

/**
 * Loads the merged word list from the merged words file. Reads the input file line by line. Each line consists of
 * three words, the first two words are to be merged words and the third word is the merged version of those two words.
 * @return Hash map containing the merged words. Key is the two words separated with space, value if the merged word.
 */
Hash_map_ptr load_merged_dictionary(){
    Hash_map_ptr result = create_string_hash_map();
    Array_list_ptr lines = read_lines("merged.txt");
    for (int i = 0; i < lines->size; i++){
        char* line = array_list_get(lines, i);
        Array_list_ptr items = str_split(line, ' ');
        String_ptr tmp = create_string4(array_list_get(items, 0), " ", array_list_get(items, 1));
        hash_map_insert(result, tmp->s, clone_string(array_list_get(items, 2)));
        free_(tmp);
        free_array_list(items, free_);
    }
    free_array_list(lines, free_);
    return result;
}

/**
 * Loads the split word list from the merged words file. Reads the input file line by line. Each line consists of
 * three words, the first word is the split version of two words, second and third words are the split words.
 * @return Hash map containing the split words. Key is the split word, value is the two split words separated with space.
 */
Hash_map_ptr load_split_dictionary(){
    Hash_map_ptr result = create_string_hash_map();
    Array_list_ptr lines = read_lines("split.txt");
    for (int i = 0; i < lines->size; i++){
        char* line = array_list_get(lines, i);
        Array_list_ptr items = str_split(line, ' ');
        String_ptr tmp = create_string2(array_list_get(items, 1));
        for (int j = 2; j < items->size; j++){
            string_append(tmp, " ");
            string_append(tmp, array_list_get(items, j));
        }
        hash_map_insert(result, clone_string(array_list_get(items, 0)), tmp->s);
        free_(tmp);
        free_array_list(items, free_);
    }
    free_array_list(lines, free_);
    return result;
}

/**
 * Checks if the given word is a misspelled word according to the misspellings list,
 * and if it is, then replaces it with its correct form in the given sentence.
 *
 * @param word   the Word to check for misspelling
 * @param result the Sentence that the word belongs to
 * @return true if the word was corrected, false otherwise
 */
bool forced_misspell_check(Fsm_morphological_analyzer_ptr fsm,
                           char *word,
                           Sentence_ptr sentence) {
    char* forced_replacement = get_correct_form(fsm->dictionary, word);
    if (forced_replacement != NULL){
        sentence_add_word(sentence, clone_string(forced_replacement));
        return true;
    }
    return false;
}

/**
 * Checks if the given word and its preceding word need to be merged according to the merged list.
 * If the merge is needed, the word and its preceding word are replaced with their merged form in the given sentence.
 *
 * @param word         the Word to check for merge
 * @param result       the Sentence that the word belongs to
 * @param previous_word the preceding Word of the given Word
 * @return true if the word was merged, false otherwise
 */
bool forced_backward_merge_check(Hash_map_ptr merged_words,
                                 char *word,
                                 Sentence_ptr result,
                                 char *previous_word) {
    if (previous_word != NULL){
        String_ptr st = create_string4(array_list_get(result->words, result->words->size - 1), " ", word);
        char* forced_replacement = get_correct_form_from_list(st->s, merged_words);
        free_string_ptr(st);
        if (forced_replacement != NULL){
            sentence_replace_word(result, result->words->size - 1, clone_string(forced_replacement));
            return true;
        }
    }
    return false;
}

/**
 * Returns the correct form of a given word by looking it up in the provided dictionary.
 *
 * @param word   the name of the word to look up in the dictionary
 * @param dictionary the dictionary to use for looking up the word
 * @return the correct form of the word, as stored in the dictionary, or null if the word is not found
 */
char *get_correct_form_from_list(char *word, Hash_map_ptr dictionary) {
    if (hash_map_contains(dictionary, word)){
        return hash_map_get(dictionary, word);
    }
    return NULL;
}

/**
 * Checks if the given word and its next word need to be merged according to the merged list.
 * If the merge is needed, the word and its next word are replaced with their merged form in the given sentence.
 *
 * @param word     the Word to check for merge
 * @param result   the Sentence that the word belongs to
 * @param nextWord the next Word of the given Word
 * @return true if the word was merged, false otherwise
 */
bool forced_forward_merge_check(Hash_map_ptr merged_words,
                                char *word,
                                Sentence_ptr result,
                                char *next_word) {
    if (next_word != NULL){
        String_ptr st = create_string4(word, " ", next_word);
        char* forced_replacement = get_correct_form_from_list(st->s, merged_words);
        free_string_ptr(st);
        if (forced_replacement != NULL){
            sentence_add_word(result, clone_string(forced_replacement));
            return true;
        }
    }
    return false;
}

/**
 * Checks if the given word needs to be split according to the split list.
 * If the split is needed, the word is replaced with its split form in the given sentence.
 *
 * @param word   the Word to check for split
 * @param result the Sentence that the word belongs to
 * @return true if the word was split, false otherwise
 */
bool forced_split_check(Hash_map_ptr split_words,
                        char *word,
                        Sentence_ptr result) {
    char* forced_replacement = get_correct_form_from_list(word, split_words);
    if (forced_replacement != NULL){
        add_split_words(forced_replacement, result);
        return true;
    }
    return false;
}

/**
 * Splits a word into two parts, a key and a value, based on the first non-numeric/non-punctuation character.
 *
 * @param word the Word object to split
 * @return an {@link AbstractMap.SimpleEntry} object containing the key (numeric/punctuation characters) and the value (remaining characters)
 */
Pair_ptr get_split_pair(char *word) {
    Pair_ptr pair;
    int j;
    for (j = 0; j < word_size(word); j++){
        String_ptr ch = char_at(word, j);
        if ((strcmp(ch->s, "0") >= 0 && strcmp(ch->s, "9") <= 0) || string_in_list(ch->s, (char*[]){".", ","}, 2)){
            free_string_ptr(ch);
        } else {
            free_string_ptr(ch);
            break;
        }
    }
    String_ptr first = substring(word, 0, j);
    String_ptr second = substring2(word, j);
    pair = create_pair(first->s, second->s);
    free_string_ptr(first);
    free_string_ptr(second);
    return pair;
}

/**
 * Given a multiword form, splits it and adds it to the given sentence.
 *
 * @param multiWord multiword form to split
 * @param result    the Sentence to add the split words to
 */
void add_split_words(char *multi_word, Sentence_ptr result) {
    Array_list_ptr words = str_split(multi_word, ' ');
    for (int i = 0; i < words->size; i++){
        sentence_add_word(result, array_list_get(words, i));
    }
    free_array_list(words, NULL);
}

/**
 * Checks whether the given Word can be split into a proper noun and a suffix, with an apostrophe in between
 * and adds the split result to the Sentence if it's valid.
 *
 * @param word the Word to check for forced suffix split.
 * @param result the Sentence that the word belongs to
 * @return true if the split is successful, false otherwise.
 */
bool forced_suffix_split_check(Fsm_morphological_analyzer_ptr fsm,
                               char *word,
                               Sentence_ptr result) {
    Fsm_parse_list_ptr parse_list = morphological_analysis(fsm, word);
    if (parse_list->fsm_parses->size > 0){
        free_fsm_parse_list(parse_list);
        return false;
    }
    free_fsm_parse_list(parse_list);
    for (int i = 1; i < word_size(word); i++){
        String_ptr capital = to_capital(word);
        String_ptr probable_proper_noun = substring(capital->s, 0, i);
        free_string_ptr(capital);
        String_ptr probable_suffix = substring2(word, i);
        String_ptr apostrophe_word = create_string4(probable_proper_noun->s, "'", probable_suffix->s);
        free_string_ptr(probable_suffix);
        char* lowercase = to_lowercase(probable_proper_noun->s);
        free_string_ptr(probable_proper_noun);
        Txt_word_ptr  txt_word = get_word_txt(fsm->dictionary, lowercase);
        free_(lowercase);
        if (txt_word != NULL && is_proper_noun(txt_word)){
            parse_list = morphological_analysis(fsm, apostrophe_word->s);
            if (parse_list->fsm_parses->size > 0){
                free_fsm_parse_list(parse_list);
                free_string_ptr(apostrophe_word);
                sentence_add_word(result, clone_string(apostrophe_word->s));
                return true;
            }
            free_fsm_parse_list(parse_list);
            free_string_ptr(apostrophe_word);
        } else {
            free_string_ptr(apostrophe_word);
        }
    }
    return false;
}

/**
 * Checks whether the current word ends with a valid question suffix and split it if it does.
 * It splits the word with the question suffix and adds the two new words to the sentence.
 * If the split is valid, it returns true.
 *
 * @param word   current Word
 * @param result the Sentence that the word belongs to
 * @return true if split is valid, false otherwise
 */
bool forced_question_suffix_split_check(Fsm_morphological_analyzer_ptr fsm,
                                        char *word,
                                        Sentence_ptr result) {
    Fsm_parse_list_ptr parse_list = morphological_analysis(fsm, word);
    if (parse_list->fsm_parses->size > 0){
        free_fsm_parse_list(parse_list);
        return false;
    }
    free_fsm_parse_list(parse_list);
    for (int i = 0; i < 84; i++){
        char* question_suffix = question_suffix_list[i];
        if (ends_with(word, question_suffix)){
            String_ptr split_word_name = substring(word, 0, str_find_c(word, question_suffix));
            parse_list = morphological_analysis(fsm, split_word_name->s);
            if (parse_list->fsm_parses->size == 0){
                free_fsm_parse_list(parse_list);
                free_string_ptr(split_word_name);
                return false;
            }
            Txt_word_ptr root = get_word_txt(fsm->dictionary, get_parse_with_longest_root_word(parse_list)->root->name);
            if (!is_code(root)){
                sentence_add_word(result, clone_string(split_word_name->s));
                sentence_add_word(result, clone_string(question_suffix));
                free_string_ptr(split_word_name);
                free_fsm_parse_list(parse_list);
                return true;
            }
            free_string_ptr(split_word_name);
            free_fsm_parse_list(parse_list);
        }
    }
    return false;
}

/**
 * Checks whether the next word and the previous word can be merged if the current word is a hyphen,
 * an en-dash or an em-dash.
 * If the previous word and the next word exist and they are valid words,
 * it merges the previous word and the next word into a single word and add the new word to the sentence
 * If the merge is valid, it returns true.
 *
 * @param word         current Word
 * @param result       the Sentence that the word belongs to
 * @param previousWord the Word before current word
 * @param nextWord     the Word after current word
 * @return true if merge is valid, false otherwise
 */
bool forced_hyphen_merge_check(Fsm_morphological_analyzer_ptr fsm,
                               char *word,
                               Sentence_ptr result,
                               char *previous_word,
                               char *next_word) {
    if (string_in_list(word, (char*[]) {"-", "–", "—"}, 3)){
        Regular_expression_ptr regular = create_regular_expression("[a-zA-ZçöğüşıÇÖĞÜŞİ]+");
        if (previous_word != NULL && next_word != NULL && full_matches(regular, previous_word) && full_matches(regular, next_word)){
            String_ptr new_word_name = create_string4(previous_word, "-", next_word);
            Fsm_parse_list_ptr parse_list = morphological_analysis(fsm, new_word_name->s);
            if (parse_list->fsm_parses->size > 0){
                sentence_replace_word(result, result->words->size - 1, clone_string(new_word_name->s));
                free_string_ptr(new_word_name);
                free_regular_expression(regular);
                free_fsm_parse_list(parse_list);
                return true;
            }
            free_string_ptr(new_word_name);
            free_fsm_parse_list(parse_list);
        }
        free_regular_expression(regular);
    }
    return 0;
}

/**
 * Checks if the given word has a "da" or "de" suffix that needs to be split according to a predefined set of rules.
 * If the split is needed, the word is replaced with its bare form and "da" or "de" in the given sentence.
 *
 * @param word   the Word to check for "da" or "de" split
 * @param result the Sentence that the word belongs to
 * @return true if the word was split, false otherwise
 */
bool forced_de_da_split_check(Fsm_morphological_analyzer_ptr fsm,
                              char *word,
                              Sentence_ptr result) {
    String_ptr capitalized_word_name = to_capital(word);
    Txt_word_ptr txt_word = NULL;
    if (ends_with(word, "da") || ends_with(word, "de")){
        Fsm_parse_list_ptr parse_list1 = morphological_analysis(fsm, word);
        Fsm_parse_list_ptr parse_list2 = morphological_analysis(fsm, capitalized_word_name->s);
        if (parse_list1->fsm_parses->size == 0 && parse_list2->fsm_parses->size == 0){
            String_ptr new_word_name = substring(word, 0, word_size(word) - 2);
            Fsm_parse_list_ptr parse_list = morphological_analysis(fsm, new_word_name->s);
            char* lowercase = to_lowercase(new_word_name->s);
            Txt_word_ptr txt_new_word = get_word_txt(fsm->dictionary, lowercase);
            if (txt_new_word != NULL && is_proper_noun(txt_new_word)){
                String_ptr new_word_name_capitalized = to_capital(new_word_name->s);
                String_ptr tmp1 = create_string4(new_word_name_capitalized->s, "'", "da");
                Fsm_parse_list_ptr parse_list3 = morphological_analysis(fsm, tmp1->s);
                if (parse_list3->fsm_parses->size > 0){
                    sentence_add_word(result, clone_string(tmp1->s));
                } else {
                    free_string_ptr(tmp1);
                    tmp1 = create_string4(new_word_name_capitalized->s, "'", "de");
                    sentence_add_word(result, clone_string(tmp1->s));
                }
                free_string_ptr(tmp1);
                free_fsm_parse_list(parse_list3);
                free_string_ptr(new_word_name_capitalized);
                free_(lowercase);
                free_fsm_parse_list(parse_list);
                free_string_ptr(new_word_name);
                free_fsm_parse_list(parse_list1);
                free_fsm_parse_list(parse_list2);
                free_string_ptr(capitalized_word_name);
                return true;
            }
            if (parse_list->fsm_parses->size > 0){
               txt_word  = get_word_txt(fsm->dictionary, get_parse_with_longest_root_word(parse_list)->root->name);
            }
            if (txt_word != NULL && !is_code(txt_word)){
                sentence_add_word(result, clone_string(new_word_name->s));
                String_ptr last = last_vowel(new_word_name->s);
                if (is_back_vowel(last->s)){
                    if (not_obeys_vowel_harmony_during_agglutination(txt_word)){
                        sentence_add_word(result, clone_string("de"));
                    } else {
                        sentence_add_word(result, clone_string("da"));
                    }
                } else {
                    if (not_obeys_vowel_harmony_during_agglutination(txt_word)){
                        sentence_add_word(result, clone_string("da"));
                    } else {
                        sentence_add_word(result, clone_string("de"));
                    }
                }
                free_string_ptr(last);
                free_(lowercase);
                free_fsm_parse_list(parse_list);
                free_string_ptr(new_word_name);
                free_fsm_parse_list(parse_list1);
                free_fsm_parse_list(parse_list2);
                free_string_ptr(capitalized_word_name);
                return true;
            }
            free_(lowercase);
            free_fsm_parse_list(parse_list);
            free_string_ptr(new_word_name);
        }
        free_fsm_parse_list(parse_list1);
        free_fsm_parse_list(parse_list2);
    }
    free_string_ptr(capitalized_word_name);
    return false;
}

/**
 * Checks if the given word is a suffix like 'li' or 'lik' that needs to be merged with its preceding word which is a number.
 * If the merge is needed, the word and its preceding word are replaced with their merged form in the given sentence.
 *
 * @param word         the Word to check for merge
 * @param sentence     the Sentence that the word belongs to
 * @param previousWord the preceding Word of the given Word
 * @return true if the word was merged, false otherwise
 */
bool forced_suffix_merge_check(Fsm_morphological_analyzer_ptr fsm,
                               char *word,
                               Sentence_ptr result,
                               char *previous_word) {
    char* li_list[] = {"li", "lı", "lu", "lü"};
    char* lik_list[] = {"lik", "lık", "luk", "lük"};
    if (string_in_list(word, li_list , 4) || string_in_list(word, lik_list , 4)){
        Regular_expression_ptr regular = create_regular_expression("\\d+");
        if (previous_word != NULL && full_matches(regular, previous_word)){
            for (int i = 0; i < 4; i++){
                String_ptr tmp = create_string4(previous_word, "'", li_list[i]);
                Fsm_parse_list_ptr parse_list = morphological_analysis(fsm, tmp->s);
                if (parse_list->fsm_parses->size > 0){
                    sentence_replace_word(result, result->words->size - 1, clone_string(tmp->s));
                    free_regular_expression(regular);
                    free_fsm_parse_list(parse_list);
                    free_string_ptr(tmp);
                    return true;
                }
                free_fsm_parse_list(parse_list);
                free_string_ptr(tmp);
            }
            for (int i = 0; i < 4; i++){
                String_ptr tmp = create_string4(previous_word, "'", lik_list[i]);
                Fsm_parse_list_ptr parse_list = morphological_analysis(fsm, tmp->s);
                if (parse_list->fsm_parses->size > 0){
                    sentence_replace_word(result, result->words->size - 1, clone_string(tmp->s));
                    free_regular_expression(regular);
                    free_fsm_parse_list(parse_list);
                    free_string_ptr(tmp);
                    return true;
                }
                free_fsm_parse_list(parse_list);
                free_string_ptr(tmp);
            }
        }
        free_regular_expression(regular);
    }
    return false;
}

/**
 * Generates a list of split candidates for the given word.
 *
 * @param word The Word currently being checked.
 * @return A list of split candidates.
 */
Array_list_ptr split_candidates_list(Fsm_morphological_analyzer_ptr fsm, char *word) {
    Array_list_ptr split_candidates = create_array_list();
    for (int i = 4; i < word_size(word) - 3; i++){
        String_ptr first = substring(word, 0, i);
        String_ptr second = substring2(word, i);
        Fsm_parse_list_ptr parse_list1 = morphological_analysis(fsm, first->s);
        Fsm_parse_list_ptr parse_list2 = morphological_analysis(fsm, second->s);
        if (parse_list1->fsm_parses->size > 0 && parse_list2->fsm_parses->size > 0){
            String_ptr candidate = create_string4(first->s, " ", second->s);
            array_list_add(split_candidates, create_candidate(candidate->s, SPLIT));
            free_string_ptr(candidate);
        }
        free_fsm_parse_list(parse_list1);
        free_fsm_parse_list(parse_list2);
        free_string_ptr(first);
        free_string_ptr(second);
    }
    return split_candidates;
}

/**
 * Generates a list of merged candidates for the word and previous and next words.
 *
 * @param previousWord The previous Word in the sentence.
 * @param word         The Word currently being checked.
 * @param nextWord     The next Word in the sentence.
 * @return A list of merged candidates.
 */
Array_list_ptr merged_candidates_list(Fsm_morphological_analyzer_ptr fsm,
                                      char *previous_word,
                                      char *word,
                                      char *next_word) {
    Fsm_parse_list_ptr parse_list;
    Array_list_ptr merged_candidates = create_array_list();
    Candidate_ptr backward_merge_candidate = NULL;
    Candidate_ptr forward_merge_candidate = NULL;
    if (previous_word != NULL){
        String_ptr st = create_string3(previous_word, word);
        parse_list = morphological_analysis(fsm, st->s);
        if (parse_list->fsm_parses->size != 0){
            backward_merge_candidate = create_candidate(st->s, BACKWARD_MERGE);
            array_list_add(merged_candidates, backward_merge_candidate);
        }
        free_fsm_parse_list(parse_list);
        free_string_ptr(st);
    }
    if (next_word != NULL){
        String_ptr st = create_string3(word, next_word);
        parse_list = morphological_analysis(fsm, st->s);
        if (parse_list->fsm_parses->size != 0){
            forward_merge_candidate = create_candidate(st->s, FORWARD_MERGE);
            array_list_add(merged_candidates, forward_merge_candidate);
        }
        free_fsm_parse_list(parse_list);
        free_string_ptr(st);
    }
    return merged_candidates;
}

/**
 * Checks if the given word is a shortcut form, such as "5kg" or "2.5km".
 * If it is, it splits the word into its number and unit form and adds them to the given sentence.
 *
 * @param word   the Word to check for shortcut split
 * @param result the Sentence that the word belongs to
 * @return true if the word was split, false otherwise
 */
bool forced_shortcut_check(char *word, Sentence_ptr result) {
    int j;
    bool is_shortcut = false;
    for (j = 0; j < word_size(word); j++){
        String_ptr ch = char_at(word, j);
        if (!(strcmp(ch->s, "0") >= 0 && strcmp(ch->s, "9") <= 0) && !string_in_list(ch->s, (char*[]){".", ","}, 2)){
            break;
        }
    }
    if (j != 0 && j != word_size(word)){
        String_ptr first = substring(word, 0, j);
        String_ptr second = substring2(word, j);
        if (string_in_list(second->s, shortcuts, 57) || string_in_list(second->s, conditional_shortcuts, 6)){
            is_shortcut = true;
            sentence_add_word(result, clone_string(first->s));
            sentence_add_word(result, clone_string(second->s));
        }
        free_string_ptr(first);
        free_string_ptr(second);
    }
    return is_shortcut;
}

/**
 * The spellCheck method takes a Sentence as an input and loops i times where i ranges from 0 to size of words in given sentence.
 * Then, it calls morphologicalAnalysis method with each word and assigns it to the FsmParseList, if the size of
 * FsmParseList is equal to the 0, it adds current word to the candidateList and assigns it to the candidates ArrayList.
 * if the size of candidates greater than 0, it generates a random number and selects an item from candidates ArrayList with
 * this random number and assign it as newWord. If the size of candidates is not greater than 0, it directly assigns the
 * current word as newWord. At the end, it adds the newWord to the result Sentence.
 *
 * @param sentence Sentence type input.
 * @return Sentence result.
 */
Sentence_ptr spell_check_simple(Simple_spell_checker_ptr spell_checker, Sentence_ptr sentence) {
    char* word;
    char* new_word;
    unsigned long random_candidate;
    Array_list_ptr candidates;
    Sentence_ptr result = create_sentence();
    for (int i = 0; i < sentence->words->size; i++) {
        word = array_list_get(sentence->words, i);
        char* next_word = NULL;
        char* previous_word = NULL;
        if (i > 0){
            previous_word = array_list_get(sentence->words, i - 1);
        }
        if (i < sentence->words->size - 1){
            next_word = array_list_get(sentence->words, i + 1);
        }
        if (forced_misspell_check(spell_checker->fsm, word, result) || forced_backward_merge_check(spell_checker->merged_words, word, result, previous_word) || forced_suffix_merge_check(spell_checker->fsm, word, result, previous_word)){
            continue;
        }
        if (forced_forward_merge_check(spell_checker->merged_words, word, result, next_word) || forced_hyphen_merge_check(spell_checker->fsm, word, result, previous_word, next_word)){
            i++;
            continue;
        }
        if (forced_split_check(spell_checker->split_words, word, result) || forced_shortcut_check(word, result) || forced_de_da_split_check(spell_checker->fsm, word, result) || forced_question_suffix_split_check(spell_checker->fsm, word, result) || forced_suffix_split_check(spell_checker->fsm, word, result)){
            continue;
        }
        Fsm_parse_list_ptr parse_list1 = morphological_analysis(spell_checker->fsm, word);
        String_ptr capital = to_capital(word);
        Fsm_parse_list_ptr parse_list2 = morphological_analysis(spell_checker->fsm, capital->s);
        free_string_ptr(capital);
        if (parse_list1->fsm_parses->size == 0 && parse_list2->fsm_parses->size == 0) {
            candidates = merged_candidates_list(spell_checker->fsm, previous_word, word, next_word);
            if (candidates->size == 0) {
                free_array_list(candidates, (void (*)(void *)) free_candidate);
                candidates = candidate_list_simple(spell_checker->fsm, word);
            }
            if (candidates->size == 0) {
                Array_list_ptr split_candidates = split_candidates_list(spell_checker->fsm, word);
                array_list_add_all(candidates, split_candidates);
                free_array_list(split_candidates, NULL);
            }
            if (candidates->size != 0) {
                random_candidate = random() % candidates->size;
                Candidate_ptr candidate = array_list_get(candidates, random_candidate);
                new_word = candidate->name;
                if (candidate->operator == BACKWARD_MERGE){
                    sentence_replace_word(result, i - 1, clone_string(new_word));
                    continue;
                }
                if (candidate->operator == FORWARD_MERGE){
                    i++;
                }
                if (candidate->operator == SPLIT){
                    add_split_words(new_word, result);
                    continue;
                }
            } else {
                new_word = word;
            }
            free_array_list(candidates, (void (*)(void *)) free_candidate);
        } else {
            new_word = word;
        }
        free_fsm_parse_list(parse_list1);
        free_fsm_parse_list(parse_list2);
        sentence_add_word(result, clone_string(new_word));
    }
    return result;
}
