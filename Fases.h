#ifndef FASES_H
#define FASES_H

// ============================================================
// Fases.h
// FaseEliminatoria: clase base para todas las fases
// FaseGrupos: maneja los 12 grupos y el sorteo
// FaseDirecta: maneja R16, R8, QF, SF, Final
// ============================================================

#include <string>
#include "Lista.h"
#include "Equipo.h"
#include "Partido.h"
#include "Grupo.h"
using namespace std;

// ------------------------------------------------------------
// FaseEliminatoria: base común para todas las fases
// ------------------------------------------------------------
class FaseEliminatoria {
protected:
    string nombre;
    Lista<Partido*> partidos;
    Lista<Equipo*> clasificados;

public:
    FaseEliminatoria();
    FaseEliminatoria(string nombre);
    FaseEliminatoria(const FaseEliminatoria& otra);
    virtual ~FaseEliminatoria();

    string getNombre() const;
    Lista<Partido*>& getPartidos();
    Lista<Equipo*>& getClasificados();

    void agregarPartido(Partido* p);
    void agregarClasificado(Equipo* e);

    // Simula todos los partidos de esta fase
    virtual void simular(long& iteraciones);

    // Imprime resultados de la fase
    virtual void imprimir() const;

    long memoriaBytes() const;
};

// ------------------------------------------------------------
// FaseGrupos: maneja los 12 grupos y el sorteo de bombos
// ------------------------------------------------------------
class FaseGrupos : public FaseEliminatoria {
private:
    Lista<Grupo*> grupos;
    Lista<Equipo*> bombos[4];  // 4 bombos de 12 equipos c/u
    string fechaInicio;

    // Ordena equipos por ranking FIFA para armar bombos
    void ordenarPorRanking(Lista<Equipo*>& lista, long& iteraciones);

    // Arma los 4 bombos según el ranking
    void armarBombos(Lista<Equipo*>& equipos, long& iteraciones);

    // Saca un equipo aleatorio de un bombo respetando
    // las restricciones de confederación para el grupo dado
    Equipo* sacarDeBoomo(int numBombo, Grupo* grupo, long& iteraciones);

public:
    FaseGrupos();
    FaseGrupos(string fechaInicio);
    FaseGrupos(const FaseGrupos& otra);
    ~FaseGrupos();

    Lista<Grupo*>& getGrupos();

    // Realiza el sorteo y forma los 12 grupos
    // equipos: lista de los 48 equipos cargados del CSV
    void realizarSorteo(Lista<Equipo*>& equipos, long& iteraciones);

    // Simula todos los partidos de todos los grupos
    void simular(long& iteraciones) override;

    // Construye tabla de clasificación y determina los 32 clasificados
    // Retorna la lista de 32 equipos clasificados en orden
    void determinarClasificados(long& iteraciones);

    // Imprime todos los grupos y tablas
    void imprimir() const override;

    // Imprime los partidos configurados para R16
    void imprimirPartidosR16(const Lista<Partido*>& partidosR16) const;

    long memoriaBytes() const;
};

// ------------------------------------------------------------
// FaseDirecta: R16, R8, QF, SF, Final
// ------------------------------------------------------------
class FaseDirecta : public FaseEliminatoria {
private:
    bool esFinal;           // true si es la final
    Partido* tercerPuesto;  // Solo existe en la fase de semifinales

public:
    FaseDirecta();
    FaseDirecta(string nombre, bool esFinal = false);
    FaseDirecta(const FaseDirecta& otra);
    ~FaseDirecta();

    bool getEsFinal() const;
    Partido* getTercerPuesto() const;
    void setTercerPuesto(Partido* p);

    // Configura los partidos de esta fase con fecha fija 10/07/2026
    void configurarPartidos(Lista<Equipo*>& equipos, long& iteraciones);

    // Simula todos los partidos eliminatorios
    void simular(long& iteraciones) override;

    // Imprime resultados
    void imprimir() const override;

    long memoriaBytes() const;
};

#endif
