cmake_minimum_required(VERSION 3.24)
project(SpellChecker)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "-O3")

find_package(morphological_analysis_c REQUIRED)
find_package(n_gram_c REQUIRED)
find_package(corpus_c REQUIRED)

add_library(SpellChecker
        src/Candidate.c
        src/Candidate.h
        src/Operator.h
        src/TrieCandidate.c
        src/TrieCandidate.h
        src/SpellCheckerParameter.c
        src/SpellCheckerParameter.h
        src/SpellCheckerTrieNode.c
        src/SpellCheckerTrieNode.h
        src/SpellCheckerTrie.c
        src/SpellCheckerTrie.h
        src/SimpleSpellChecker.c
        src/SimpleSpellChecker.h
        src/NGramSpellChecker.c
        src/NGramSpellChecker.h
        src/ContextBasedSpellChecker.c
        src/ContextBasedSpellChecker.h
        src/TrieBasedSpellChecker.c
        src/TrieBasedSpellChecker.h)
target_link_libraries(SpellChecker morphological_analysis_c::morphological_analysis_c n_gram_c::n_gram_c corpus_c::corpus_c)
add_executable(SimpleSpellCheckerTest
        src/Candidate.c
        src/Candidate.h
        src/Operator.h
        src/TrieCandidate.c
        src/TrieCandidate.h
        src/SpellCheckerParameter.c
        src/SpellCheckerParameter.h
        src/SpellCheckerTrieNode.c
        src/SpellCheckerTrieNode.h
        src/SpellCheckerTrie.c
        src/SpellCheckerTrie.h
        src/SimpleSpellChecker.c
        src/SimpleSpellChecker.h
        src/NGramSpellChecker.c
        src/NGramSpellChecker.h
        src/ContextBasedSpellChecker.c
        src/ContextBasedSpellChecker.h
        src/TrieBasedSpellChecker.c
        src/TrieBasedSpellChecker.h
        Test/SimpleSpellCheckerTest.c)
target_link_libraries(SimpleSpellCheckerTest morphological_analysis_c::morphological_analysis_c n_gram_c::n_gram_c corpus_c::corpus_c)

add_executable(NGramSpellCheckerTest
        src/Candidate.c
        src/Candidate.h
        src/Operator.h
        src/TrieCandidate.c
        src/TrieCandidate.h
        src/SpellCheckerParameter.c
        src/SpellCheckerParameter.h
        src/SpellCheckerTrieNode.c
        src/SpellCheckerTrieNode.h
        src/SpellCheckerTrie.c
        src/SpellCheckerTrie.h
        src/SimpleSpellChecker.c
        src/SimpleSpellChecker.h
        src/NGramSpellChecker.c
        src/NGramSpellChecker.h
        src/ContextBasedSpellChecker.c
        src/ContextBasedSpellChecker.h
        src/TrieBasedSpellChecker.c
        src/TrieBasedSpellChecker.h
        Test/NGramSpellCheckerTest.c)

target_link_libraries(NGramSpellCheckerTest morphological_analysis_c::morphological_analysis_c n_gram_c::n_gram_c corpus_c::corpus_c)
