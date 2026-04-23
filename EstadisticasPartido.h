#ifndef ESTADISTICASPARTIDO_H
#define ESTADISTICASPARTIDO_H
#include "Lista.h"
#include "Jugador.h"

// Estadísticas de un jugador en un partido
class EstJugPartido
{
public:
    Jugador* jugador;
    int goles;
    int tarjAmarillas;
    int tarjRojas;
    int faltas;
    int minutosJugados;

    EstJugPartido();
    EstJugPartido(Jugador* j, int minutos = 90);
    EstJugPartido(const EstJugPartido& otro);

    // Actualizar el histórico
    EstadisticasJugador toEstadisticasJugador() const;

    long memoriaBytes() const;
};

// Estadísticas de un equipo en un partido
class EstadisticasPartido
{
public:
    int golesAFavor;
    int golesEnContra;
    float posesionBall;             // Porcentaje 0.0 - 100.0
    Lista<EstJugPartido*> jugadores; // Los 11 convocados

    EstadisticasPartido();
    EstadisticasPartido(const EstadisticasPartido& otra);
    ~EstadisticasPartido();

    // Agrega un jugador convocado a este partido
    void agregarJugador(EstJugPartido* ejp);

    // Convierte a EstadisticasEquipo para actualizar el histórico
    // gano: true si este equipo ganó, empato: true si empató
    EstadisticasEquipo toEstadisticasEquipo(bool gano, bool empato) const;

    long memoriaBytes() const;
};

#endif
