cmake_minimum_required(VERSION 3.19)
project(philo C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(philo
        ft_atoi.c
        main.c
        philo.h)
