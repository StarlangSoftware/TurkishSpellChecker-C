cmake_minimum_required(VERSION 3.24)
project(SpellChecker)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "-O3")

find_package(morphological_analysis_c REQUIRED)
find_package(n_gram_c REQUIRED)

add_library(SpellChecker
        Candidate.c
        Candidate.h
        Operator.h
        TrieCandidate.c
        TrieCandidate.h
        SpellCheckerParameter.c
        SpellCheckerParameter.h
        SpellCheckerTrieNode.c
        SpellCheckerTrieNode.h
        SpellCheckerTrie.c
        SpellCheckerTrie.h
        SimpleSpellChecker.c
        SimpleSpellChecker.h
        NGramSpellChecker.c
        NGramSpellChecker.h
        ContextBasedSpellChecker.c
        ContextBasedSpellChecker.h
        TrieBasedSpellChecker.c
        TrieBasedSpellChecker.h)
target_link_libraries(SpellChecker morphological_analysis_c::morphological_analysis_c n_gram_c::n_gram_c)
