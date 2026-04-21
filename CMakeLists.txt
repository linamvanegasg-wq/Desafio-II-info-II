cmake_minimum_required(VERSION 3.16)

project(Desafioll-infoll LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(Desafioll-infoll
  main.cpp
  Lista.h
  Confederacion.h
  Confederacion.cpp
  Estadisticas.h
  Estadisticas.cpp
  Jugador.h
  Jugador.cpp
  Equipo.h
  Equipo.cpp
  Arbitro.h
  Arbitro.cpp
  EstadisticasPartido.h
  EstadisticasPartido.cpp
  Partido.h
  Partido.cpp
)
target_link_libraries(Desafioll-infoll PRIVATE Qt${QT_VERSION_MAJOR}::Core)

include(GNUInstallDirs)
install(TARGETS Desafioll-infoll
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
