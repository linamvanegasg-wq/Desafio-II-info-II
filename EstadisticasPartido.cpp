#include "EstadisticasPartido.h"


EstJugPartido::EstJugPartido()
    : jugador(nullptr), goles(0), tarjAmarillas(0),
      tarjRojas(0), faltas(0), minutosJugados(90)
{

}

EstJugPartido::EstJugPartido(Jugador* j, int minutos)
    : jugador(j), goles(0), tarjAmarillas(0),
      tarjRojas(0), faltas(0), minutosJugados(minutos)
{

}

EstJugPartido::EstJugPartido(const EstJugPartido& otro)
    : jugador(otro.jugador), goles(otro.goles),
      tarjAmarillas(otro.tarjAmarillas), tarjRojas(otro.tarjRojas),
      faltas(otro.faltas), minutosJugados(otro.minutosJugados)
{

}

// Convierte las stats del partido a formato histórico del jugador
EstadisticasJugador EstJugPartido::toEstadisticasJugador() const
{
    EstadisticasJugador ej;
    ej.partJugados    = 1;
    ej.goles          = goles;
    ej.minutosJugados = minutosJugados;
    ej.asistencias    = 0; // El enunciado dice dejarlas en 0
    ej.tarjAmarillas  = tarjAmarillas;
    ej.tarjRojas      = tarjRojas;
    ej.faltas         = faltas;
    return ej;
}

long EstJugPartido::memoriaBytes() const
{
    return sizeof(EstJugPartido);
}

//EstadisticasPartido

EstadisticasPartido::EstadisticasPartido()
    : golesAFavor(0), golesEnContra(0),
      posesionBall(0.0f), jugadores(11)
{

}

EstadisticasPartido::EstadisticasPartido(const EstadisticasPartido& otra)
    : golesAFavor(otra.golesAFavor), golesEnContra(otra.golesEnContra),
      posesionBall(otra.posesionBall), jugadores(11)
{
    for (int i = 0; i < otra.jugadores.getTamanio(); i++)
    {
        EstJugPartido* ejp = new EstJugPartido(*otra.jugadores.obtener(i));
        jugadores.agregar(ejp);
    }
}

EstadisticasPartido::~EstadisticasPartido()
{
    for (int i = 0; i < jugadores.getTamanio(); i++)
    {
        delete jugadores[i];
    }
}

void EstadisticasPartido::agregarJugador(EstJugPartido* ejp)
{
    jugadores.agregar(ejp);
}

// Convierte las stats del partido a formato histórico del equipo
EstadisticasEquipo EstadisticasPartido::toEstadisticasEquipo(
        bool gano, bool empato) const
{
    EstadisticasEquipo eq;
    eq.golesAFavor   = golesAFavor;
    eq.golesEnContra = golesEnContra;
    eq.partGanados   = gano   ? 1 : 0;
    eq.partEmpatados = empato ? 1 : 0;
    eq.partPerdidos  = (!gano && !empato) ? 1 : 0;

    // Suma tarjetas y faltas de todos los jugadores
    for (int i = 0; i < jugadores.getTamanio(); i++)
    {
        eq.tarjAmarillas += jugadores.obtener(i)->tarjAmarillas;
        eq.tarjRojas     += jugadores.obtener(i)->tarjRojas;
        eq.faltas        += jugadores.obtener(i)->faltas;
    }
    return eq;
}

long EstadisticasPartido::memoriaBytes() const
{
    long mem = sizeof(EstadisticasPartido);
    mem += jugadores.memoriaBytes();
    for (int i = 0; i < jugadores.getTamanio(); i++)
    {
        mem += jugadores.obtener(i)->memoriaBytes();
    }
    return mem;
}
