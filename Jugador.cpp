#include "Jugador.h"

Jugador::Jugador() : nombre(""), apellido(""), numCamiseta(0)
{

}

Jugador::Jugador(string nombre, string apellido, int numCamiseta)
    : nombre(nombre), apellido(apellido), numCamiseta(numCamiseta)
{

}

Jugador::Jugador(const Jugador& otro)
    : nombre(otro.nombre), apellido(otro.apellido),
      numCamiseta(otro.numCamiseta), statsHist(otro.statsHist)
{

}

string Jugador::getNombre() const
{
    return nombre;
}
string Jugador::getApellido() const
{
    return apellido;
}
int Jugador::getNumCamiseta() const
{
    return numCamiseta;
}

EstadisticasJugador& Jugador::getStatsHist()
{
    return statsHist;
}
const EstadisticasJugador& Jugador::getStatsHist() const
{
    return statsHist;
}

void Jugador::setNombre(string n)
{
    nombre = n;
}
void Jugador::setApellido(string a)
{
    apellido = a;
}
void Jugador::setNumCamiseta(int n)
{
    numCamiseta = n;
}

void Jugador::actualizarStats(const EstadisticasJugador& statsPartido)
{
    statsHist += statsPartido;
}

bool Jugador::operator==(const Jugador& otro) const
{
    return numCamiseta == otro.numCamiseta;
}

long Jugador::memoriaBytes() const
{
    // string guarda internamente sus chars en heap,(aproximamos)
    return sizeof(Jugador) + nombre.size() + apellido.size()
           + statsHist.memoriaBytes();
}
