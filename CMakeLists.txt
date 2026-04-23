cmake_minimum_required(VERSION 3.16)
project(UdeAWorldCup)

set(CMAKE_CXX_STANDARD 17)

add_executable(UdeAWorldCup
    main.cpp
    Confederacion.cpp
    Estadisticas.cpp
    Jugador.cpp
    Equipo.cpp
    Arbitro.cpp
    EstadisticasPartido.cpp
    Partido.cpp
    GestorArchivos.cpp
    Grupo.cpp
    Fases.cpp
    Torneo.cpp
    Torneo.h

)
