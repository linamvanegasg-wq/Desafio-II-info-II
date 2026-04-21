#ifndef PARTIDO_H
#define PARTIDO_H
#include <string>
#include "Lista.h"
#include "Equipo.h"
#include "Arbitro.h"
#include "EstadisticasPartido.h"
using namespace std;

class Partido
{
private:
    string fecha;
    string hora;
    string sede;
    Equipo* equipo1;
    Equipo* equipo2;
    Lista<Arbitro*> arbitros;
    EstadisticasPartido statsEq1;
    EstadisticasPartido statsEq2;
    Equipo* ganador;
    bool prorroga;
    bool simulado;
    float calcularLambda(Equipo* atacante, Equipo* defensor) const;
    int simularGoles(float lambda, Lista<EstJugPartido*>& jugadores, long& iters);

    //Tarjetas y faltas para los 11 jugadores
    void simularTarjetasYFaltas(Lista<EstJugPartido*>& jugadores, long& iters);

    // Calcula posesión de balón
    void calcularPosesion();

    // Elige aleatoriamente 11 jugadores de la plantilla (26)
    Lista<EstJugPartido*>* elegirConvocados(Equipo* equipo, int minutos);

    // Hace desempate en fases eliminatorias usando ranking FIFA
    Equipo* romperEmpate(long& iters);

public:
    Partido();
    Partido(string fecha, string hora, string sede,
            Equipo* eq1, Equipo* eq2);
    Partido(const Partido& otro);
    ~Partido();

    // Getters
    string getFecha() const;
    string getHora() const;
    string getSede() const;
    Equipo* getEquipo1() const;
    Equipo* getEquipo2() const;
    Lista<Arbitro*>& getArbitros();
    EstadisticasPartido& getStatsEq1();
    EstadisticasPartido& getStatsEq2();
    Equipo* getGanador() const;
    bool isProrroga() const;
    bool isSimulado() const;

    // Setters
    void setFecha(string fecha);
    void setHora(string hora);
    void setSede(string sede);
    void agregarArbitro(Arbitro* arbitro);

    // Simulador del partido en la fase de grupos
    void simular(long& iteraciones);

    // Simulador del partido eliminatorio
    void simularEliminatorio(long& iteraciones);

    // Imprime en pantalla los datos del partido
    void imprimir() const;

    // Imprime los goleadores (número de camiseta) de cada equipo
    void imprimirGoleadores() const;

    long memoriaBytes() const;
};

#endif
