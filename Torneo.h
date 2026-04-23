#ifndef TORNEO_H
#define TORNEO_H
#include <string>
#include "Lista.h"
#include "Equipo.h"
#include "Confederacion.h"
#include "GestorArchivos.h"
#include "Fases.h"
using namespace std;

// MedidorRecursos
// Requisito funcional V del enunciado
class MedidorRecursos {
private:
    long iteraciones;
    long memoriaTotal;

public:
    MedidorRecursos();

    void resetear();
    void agregarIteraciones(long n);
    void setMemoria(long bytes);

    long getIteraciones() const;
    long getMemoria() const;

    // Imprime las métricas en pantalla
    void imprimir(const string& etapa) const;

    long memoriaBytes() const;
};

// Torneo
// Coordina todas las fases del mundial
class Torneo
{
private:
    Lista<Equipo*> equipos;
    Lista<Confederacion*> confederaciones;
    GestorArchivos gestorArchivos;
    FaseGrupos* faseGrupos;
    FaseDirecta* r16;
    FaseDirecta* r8;
    FaseDirecta* qf;
    FaseDirecta* sf;
    FaseDirecta* finalTorneo;
    MedidorRecursos medidor;
    bool cargado; // true si ya se cargaron los equipos

    // Calcula la memoria total de todos los objetos
    long calcularMemoriaTotal() const;

    // Configura los partidos de R16 según las reglas del enunciado
    void configurarR16(long& iteraciones);

public:
    Torneo();
    ~Torneo();

    // I - Carga de datos desde CSV
    void cargarDatos();

    // II - Conformación de grupos
    void conformarGrupos();

    // III a - Simular fase de grupos
    void simularFaseGrupos();

    // III b - Transición a R16 (tabla + configurar partidos)
    void transicionR16();

    // III c - Simular resto de fases
    void simularFaseDirecta();

    // IV - Estadísticas finales del torneo
    void mostrarEstadisticas();

    // Getters para el menú
    bool isCargado() const;

    long memoriaBytes() const;
};

#endif
