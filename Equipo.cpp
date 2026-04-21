#include "Equipo.h"
#include <string>
using namespace std;

Equipo::Equipo()
    : pais(""), dirTecnico(""), rankingFIFA(0), confederacion(nullptr),
      plantilla(26), puntosGrupo(0), golesGrupo(0), golesEnContraGrupo(0)
{

}

Equipo::Equipo(string pais, string dirTecnico, int ranking, Confederacion* conf)
    : pais(pais), dirTecnico(dirTecnico), rankingFIFA(ranking),
      confederacion(conf), plantilla(26),
      puntosGrupo(0), golesGrupo(0), golesEnContraGrupo(0)
{

}

Equipo::Equipo(const Equipo& otro)
    : pais(otro.pais), dirTecnico(otro.dirTecnico),
      rankingFIFA(otro.rankingFIFA), confederacion(otro.confederacion),
      plantilla(26), statsHistorico(otro.statsHistorico),
      puntosGrupo(otro.puntosGrupo), golesGrupo(otro.golesGrupo),
      golesEnContraGrupo(otro.golesEnContraGrupo) {

    for (int i = 0; i < otro.plantilla.getTamanio(); i++)
    {
        Jugador* j = new Jugador(*otro.plantilla.obtener(i));
        plantilla.agregar(j);
    }
}

Equipo::~Equipo()
{
    for (int i = 0; i < plantilla.getTamanio(); i++)
    {
        delete plantilla[i];
    }
}

// Getters
string Equipo::getPais() const
{
    return pais;
}
string Equipo::getDirTecnico() const
{
    return dirTecnico;
}
int Equipo::getRankingFIFA() const
{
    return rankingFIFA;
}
Confederacion* Equipo::getConfederacion() const
{
    return confederacion;
}
Lista<Jugador*>& Equipo::getPlantilla()
{
    return plantilla;
}
const Lista<Jugador*>& Equipo::getPlantilla() const
{
    return plantilla;
}
EstadisticasEquipo& Equipo::getStatsHistorico()
{
    return statsHistorico;
}
const EstadisticasEquipo& Equipo::getStatsHistorico() const
{
    return statsHistorico;
}
int Equipo::getPuntosGrupo() const
{
    return puntosGrupo;
}
int Equipo::getGolesGrupo() const
{
    return golesGrupo;
}
int Equipo::getGolesEnContraGrupo() const
{
    return golesEnContraGrupo;
}
int Equipo::getDifGolesGrupo() const { return golesGrupo - golesEnContraGrupo; }

//Setters
void Equipo::setPais(string p)
{
    pais = p;
}
void Equipo::setDirTecnico(string dt)
{
    dirTecnico = dt;
}
void Equipo::setRankingFIFA(int r)
{
    rankingFIFA = r;
}
void Equipo::setConfederacion(Confederacion* c)
{
    confederacion = c;
}

void Equipo::generarPlantilla()
{
    int totalGoles = statsHistorico.golesAFavor;
    int golesBase  = totalGoles / 26;
    int sobrante   = totalGoles % 26;

    for (int i = 1; i <= 26; i++)
    {
        string nom = "nombre" + to_string(i);
        string ape = "apellido" + to_string(i);
        Jugador* j = new Jugador(nom, ape, i);

        // Distribuir goles uniformemente
        int golesJugador = golesBase + (i <= sobrante ? 1 : 0);
        j->getStatsHist().goles = golesJugador;

        plantilla.agregar(j);
    }
}

//Puntos y goles de fase de grupos
void Equipo::agregarPuntosGrupo(int puntos)
{
    puntosGrupo += puntos;
}
void Equipo::agregarGolesGrupo(int gf, int gc)
{
    golesGrupo += gf;
    golesEnContraGrupo += gc;
}
void Equipo::reiniciarContadoresGrupo()
{
    puntosGrupo = 0;
    golesGrupo  = 0;
    golesEnContraGrupo = 0;
}

// Actualiza stats históricas
void Equipo::actualizarStats(const EstadisticasEquipo& delta)
{
    statsHistorico += delta;
}

// Operador
bool Equipo::operator==(const Equipo& otro) const
{
    return pais == otro.pais;
}

// Memoria
long Equipo::memoriaBytes() const
{
    long mem = sizeof(Equipo) + pais.size() + dirTecnico.size();
    mem += statsHistorico.memoriaBytes();
    mem += plantilla.memoriaBytes();
    for (int i = 0; i < plantilla.getTamanio(); i++)
    {
        mem += plantilla.obtener(i)->memoriaBytes();
    }
    return mem;
}
