cmake_minimum_required(VERSION 3.5)
project(programowanie_rownolegle)

find_package(Threads)

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


