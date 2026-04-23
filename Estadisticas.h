#ifndef ESTADISTICAS_H
#define ESTADISTICAS_H

class EstadisticasEquipo
{
public:
    int golesAFavor;
    int golesEnContra;
    int partGanados;
    int partEmpatados;
    int partPerdidos;
    int tarjAmarillas;
    int tarjRojas;
    int faltas;

    EstadisticasEquipo();
    EstadisticasEquipo(int gf, int gc, int pg, int pe, int pp);

    // Copia Estadisticas
    EstadisticasEquipo(const EstadisticasEquipo& otra);

    // Suma las estadísticas de otro partido a este y lo actualizada despues de un partido

    EstadisticasEquipo& operator+=(const EstadisticasEquipo& otra);

    // Hace el promedio de goles a favor
    float promedioGF() const;

    // Hce el promedio de goles en contra
    float promedioGC() const;

    // Identifica la diferencia de goles
    int difGoles() const;

    // Indica los jugados jugados
    int partJugados() const;

    // Memoria en bytes que ocupa
    long memoriaBytes() const;
};

// Estadisticas de un jugador

class EstadisticasJugador
{
public:
    int partJugados;
    int goles;
    int minutosJugados;
    int asistencias;
    int tarjAmarillas;
    int tarjRojas;
    int faltas;

    // estadisticas desde cero
    EstadisticasJugador();

    // Copia de las estadisticas
    EstadisticasJugador(const EstadisticasJugador& otra);

    // Suma estadísticas de un partido al histórico
    EstadisticasJugador& operator+=(const EstadisticasJugador& otra);

    // Memoria en bytes
    long memoriaBytes() const;
};

#endif
