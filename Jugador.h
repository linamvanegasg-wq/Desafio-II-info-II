#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>
#include "Estadisticas.h"
using namespace std;

class Jugador
{
private:
    string nombre;
    string apellido;
    int numCamiseta;
    EstadisticasJugador statsHist;  // Estadísticas acumuladas

public:
    Jugador();
    Jugador(string nombre, string apellido, int numCamiseta);

    // Crea una copia
    Jugador(const Jugador& otro);

    // Getters
    string getNombre() const;
    string getApellido() const;
    int getNumCamiseta() const;
    EstadisticasJugador& getStatsHist();
    const EstadisticasJugador& getStatsHist() const;

    // Setters
    void setNombre(string nombre);
    void setApellido(string apellido);
    void setNumCamiseta(int num);

    // Suma estadísticas de un partido al historial del jugador
    void actualizarStats(const EstadisticasJugador& statsPartido);

    // Comparar por número de camiseta
    bool operator==(const Jugador& otro) const;

    // Memoria que ocupa
    long memoriaBytes() const;
};

#endif
