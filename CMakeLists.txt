cmake_minimum_required(VERSION 3.5)
project(programowanie_rownolegle)

enable_testing()

find_package(Threads)
find_package(OpenMP)

if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()


find_package(MPI)

include_directories(${MPI_INCLUDE_PATH})

#LAB 1
add_executable(lab_1_zad_1 lab_1/moj_program.c pomiar_czasu/pomiar_czasu.c)

#LAB 2
add_executable(lab_2_zad_1 lab_2/clone.c)
add_executable(lab_2_zad_2 lab_2/fork.c)

#LAB 3
add_executable(lab_3_zad_1 lab_3/zad_1/pthreads_detach_kill.c)
add_executable(lab_3_zad_2 lab_3/zad_2/pthreads_self.c)
add_executable(lab_3_zad_3 lab_3/zad_3/pthreads_struct.c)

#LAB 4
add_executable(lab_4_zad_1 lab_4/zad_1/pub_sym_1.c)
add_executable(lab_4_zad_2 lab_4/zad_2/pub_sym_2.c)

#LAB 5
add_executable(lab_5_zad_1 lab_5/zad_1/pthreads_suma.c)
add_executable(lab_5_zad_2 lab_5/zad_2/pthreads_suma.c pomiar_czasu/pomiar_czasu.c)

#LAB 7
add_executable(lab_7_zad_1 lab_7/zad_1/prod_cons.c lab_7/zad_1/kolejka.c)
add_executable(lab_7_zad_2 lab_7/zad_2/czyt_pis.c lab_7/zad_2/czytelnia.c)
target_link_libraries(lab_7_zad_2 ${CMAKE_THREAD_LIBS_INIT})
add_executable(lab_7_zad_3 lab_7/zad_3/czyt_pis.c lab_7/zad_3/czytelnia.c)
target_link_libraries(lab_7_zad_3 ${CMAKE_THREAD_LIBS_INIT})

#LAB 9
add_executable(lab_9_zad_1 lab_9/zad_1/openmp_watki_zmienne.c)
add_executable(lab_9_zad_2 lab_9/zad_2/openmp_petle.c)

#LAB 10
add_executable(lab_10_zad_1 lab_10/zad_1/petle.c)
target_link_libraries(lab_10_zad_1 m)
add_executable(lab_10_zad_2 lab_10/zad_2/openmp_mat_vec.c lab_10/zad_2/mat_vec.c)

#lab 11
add_executable(lab_11_zad_1 lab_11/zad_1/test_sortowanie.c lab_11/zad_1/merge_sort_openmp.c lab_11/zad_1/sortowanie_seq.c)
add_executable(lab_11_zad_2 lab_11/zad_2/test_search_max_openmp.c lab_11/zad_2/search_max_openmp.c)
target_link_libraries(lab_11_zad_2 m)
add_executable(lab_11_zad_3 lab_11/zad_3/calka.c)
target_link_libraries(lab_11_zad_3 m)

#lab 12
add_executable(lab_12_zad_2 lab_12/zad_2/MPI_simple.c)
target_link_libraries(lab_12_zad_2 mpi)

add_executable(lab_12_zad_3 lab_12/zad_3/MPI_simple.c)
target_link_libraries(lab_12_zad_3 mpi)

#lab 13
add_executable(lab_13_zad_1 lab_13/zad_1/heat.c)
target_link_libraries(lab_13_zad_1 mpi)

add_executable(lab_13_zad_2 lab_13/zad_2/pi.c)
target_link_libraries(lab_13_zad_2 mpi)
