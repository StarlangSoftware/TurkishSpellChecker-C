//
// Created by Olcay Taner YILDIZ on 14.01.2024.
//

#ifndef SPELLCHECKER_SIMPLESPELLCHECKER_H
#define SPELLCHECKER_SIMPLESPELLCHECKER_H

#include <ArrayList.h>
#include <Sentence.h>
#include <Pair.h>
#include "SpellCheckerParameter.h"

static char* shortcuts[] = {"cc", "cm2", "cm", "gb", "ghz", "gr", "gram", "hz", "inc", "inch",
                            "inç", "kg", "kw", "kva", "litre", "lt", "m2", "m3", "mah", "mb",
                            "metre", "mg", "mhz", "ml", "mm", "mp", "ms", "mt", "mv", "tb",
                            "tl", "va", "volt", "watt", "ah", "hp", "oz", "rpm", "dpi", "ppm",
                            "ohm", "kwh", "kcal", "kbit", "mbit", "gbit", "bit", "byte", "mbps",
                            "gbps", "cm3", "mm2", "mm3", "khz", "ft", "db", "sn"};


static char* conditional_shortcuts[] = {"g", "v", "m", "l", "w", "s"};

static char* question_suffix_list[] = {"mi", "mı", "mu", "mü", "miyim", "misin", "miyiz", "midir",
                                       "miydi", "mıyım", "mısın", "mıyız", "mıdır", "mıydı", "muyum",
                                       "musun", "muyuz", "mudur", "muydu", "müyüm", "müsün", "müyüz",
                                       "müdür", "müydü", "miydim", "miydin", "miydik", "miymiş",
                                       "mıydım", "mıydın", "mıydık", "mıymış", "muydum", "muydun",
                                       "muyduk", "muymuş", "müydüm", "müydün", "müydük", "müymüş",
                                       "misiniz", "mısınız", "musunuz", "müsünüz", "miyimdir",
                                       "misindir", "miyizdir", "miydiniz", "miydiler", "miymişim",
                                       "miymişiz", "mıyımdır", "mısındır", "mıyızdır", "mıydınız",
                                       "mıydılar", "mıymışım", "mıymışız", "muyumdur", "musundur",
                                       "muyuzdur", "muydunuz", "muydular", "muymuşum", "muymuşuz",
                                       "müyümdür", "müsündür", "müyüzdür", "müydünüz", "müydüler",
                                       "müymüşüm", "müymüşüz", "miymişsin", "miymişler", "mıymışsın",
                                       "mıymışlar", "muymuşsun", "muymuşlar", "müymüşsün", "müymüşler",
                                       "misinizdir", "mısınızdır", "musunuzdur", "müsünüzdür"};

struct simple_spell_checker{
    Fsm_morphological_analyzer_ptr fsm;
    Spell_checker_parameter_ptr parameter;
    Hash_map_ptr merged_words;
    Hash_map_ptr split_words;
};

typedef struct simple_spell_checker Simple_spell_checker;

typedef Simple_spell_checker *Simple_spell_checker_ptr;

Simple_spell_checker_ptr create_simple_spell_checker(Fsm_morphological_analyzer_ptr fsm);

void free_simple_spell_checker(Simple_spell_checker_ptr spell_checker);

Array_list_ptr generate_candidate_list_simple(char* word);

Array_list_ptr candidate_list_simple(Fsm_morphological_analyzer_ptr fsm, char* word);

void load_dictionaries(Simple_spell_checker_ptr spell_checker);

bool forced_misspell_check(Fsm_morphological_analyzer_ptr fsm,
                           char* word,
                           Sentence_ptr sentence);

bool forced_backward_merge_check(Hash_map_ptr merge_words,
                                 char* word,
                                 Sentence_ptr result,
                                 char* previous_word);

char* get_correct_form_from_list(char* word, Hash_map_ptr dictionary);

bool forced_forward_merge_check(Hash_map_ptr merge_words,
                                char* word,
                                Sentence_ptr result,
                                char* next_word);

bool forced_split_check(Hash_map_ptr split_words,
                        char* word,
                        Sentence_ptr result);

Pair_ptr get_split_pair(char* word);

void add_split_words(char* multi_word, Sentence_ptr result);

bool forced_suffix_split_check(Fsm_morphological_analyzer_ptr fsm,
                               char* word,
                               Sentence_ptr result);

bool forced_question_suffix_split_check(Fsm_morphological_analyzer_ptr fsm,
                                        char* word,
                                        Sentence_ptr result);

bool forced_hyphen_merge_check(Fsm_morphological_analyzer_ptr fsm,
                               char* word,
                               Sentence_ptr result,
                               char* previous_word,
                               char* next_word);

bool forced_de_da_split_check(Fsm_morphological_analyzer_ptr fsm,
                              char* word,
                              Sentence_ptr result);

bool forced_suffix_merge_check(Fsm_morphological_analyzer_ptr fsm,
                               char* word,
                               Sentence_ptr result,
                               char* previous_word);

Array_list_ptr split_candidates_list(Fsm_morphological_analyzer_ptr fsm, char* word);

Array_list_ptr merged_candidates_list(Fsm_morphological_analyzer_ptr fsm,
                                      char* previous_word,
                                      char* word,
                                      char* next_word);

bool forced_shortcut_check(char* word, Sentence_ptr result);

Sentence_ptr spell_check_simple(Simple_spell_checker_ptr spell_checker, Sentence_ptr sentence);

Hash_map_ptr load_merged_dictionary();

Hash_map_ptr load_split_dictionary();

#endif //SPELLCHECKER_SIMPLESPELLCHECKER_H
