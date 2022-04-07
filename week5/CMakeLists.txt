cmake_minimum_required(VERSION 3.21)
set(PROJ_NAME week5)
project(${PROJ_NAME})

set(CMAKE_CXX_STANDARD 17)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../week2)
add_compile_options(-pthread)


set(JOS_PERM josephus_permutation.cpp)
set(SV_COPY_ASSIGN simple_vector_copy_assignment.cpp simple_vector.cpp)
set(SPLIT_SENTENCES split_into_sentences.cpp)
set(MERGE_SORT merge_sort_3.cpp)
set(PRIOR_CLCTN priority_collection_2.cpp)
set(EXP_KEY_WORDS explore_key_words.cpp)
set(CONCUR_MAP concurrent_map.cpp)


add_executable(${PROJ_NAME} ${SV_COPY_ASSIGN})
