#ifndef GRUPO_H
#define GRUPO_H

// ============================================================
// Grupo.h
// Representa un grupo de la fase clasificatoria.
// Cada grupo tiene 4 equipos y 6 partidos (todos vs todos).
// ============================================================

#include <string>
#include "Lista.h"
#include "Equipo.h"
#include "Partido.h"
using namespace std;

class Grupo {
private:
    char letra;                  // A, B, C... L
    Lista<Equipo*> equipos;      // 4 equipos por grupo
    Lista<Partido*> partidos;    // 6 partidos por grupo

public:
    Grupo();
    Grupo(char letra);
    Grupo(const Grupo& otro);
    ~Grupo();

    // Getters
    char getLetra() const;
    Lista<Equipo*>& getEquipos();
    const Lista<Equipo*>& getEquipos() const;
    Lista<Partido*>& getPartidos();
    const Lista<Partido*>& getPartidos() const;

    // Agrega un equipo al grupo
    void agregarEquipo(Equipo* equipo);

    // Agrega un partido al grupo
    void agregarPartido(Partido* partido);

    // Verifica si un equipo puede entrar al grupo
    // según las restricciones de confederación del enunciado:
    // - No dos equipos de la misma confederación (salvo UEFA máx 2)
    bool puedeAgregar(Equipo* equipo) const;

    // Genera los 6 partidos del grupo (todos vs todos)
    // Asigna fechas respetando:
    // - Máx 4 partidos por día
    // - Ningún equipo juega más de 1 partido en 3 días
    // fechaInicio: "20/06/2026"
    void generarPartidos(const string& fechaInicio, long& iteraciones);

    // Simula todos los partidos del grupo
    void simularPartidos(long& iteraciones);

    // Ordena los equipos por puntos (para tabla de clasificación)
    // Criterios de desempate: dif goles, goles a favor, sorteo
    void ordenarEquipos(long& iteraciones);

    // Imprime la tabla de clasificación del grupo
    void imprimirTabla() const;

    // Imprime todos los partidos del grupo
    void imprimirPartidos() const;

    long memoriaBytes() const;
};

#endif
