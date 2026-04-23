#include "Partido.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
using namespace std;

// Constantes de la ecuación (1) del enunciado
static const float ALPHA = 0.6f;
static const float BETA  = 0.4f;
static const float MU    = 1.35f;

// Probabilidades del enunciado
static const float P_AMARILLA1 = 0.06f;
static const float P_AMARILLA2 = 0.0115f;
static const float P_GOL_JUG   = 0.04f;
static const float P_FALTA1    = 0.13f;
static const float P_FALTA2    = 0.0275f;
static const float P_FALTA3    = 0.007f;

// Genera un float aleatorio entre 0 y 1
static float randFloat()
{
    return (float)rand() / (float)RAND_MAX;
}

Partido::Partido()
    : fecha(""), hora(""), sede(""),
      equipo1(nullptr), equipo2(nullptr), arbitros(3),
      ganador(nullptr), prorroga(false), simulado(false)
{

}

Partido::Partido(string fecha, string hora, string sede,
                 Equipo* eq1, Equipo* eq2)
    : fecha(fecha), hora(hora), sede(sede),
      equipo1(eq1), equipo2(eq2), arbitros(3),
      ganador(nullptr), prorroga(false), simulado(false)
{

}

Partido::Partido(const Partido& otro)
    : fecha(otro.fecha), hora(otro.hora), sede(otro.sede),
      equipo1(otro.equipo1), equipo2(otro.equipo2),
      arbitros(3), statsEq1(otro.statsEq1), statsEq2(otro.statsEq2),
      ganador(otro.ganador), prorroga(otro.prorroga), simulado(otro.simulado)
{
    for (int i = 0; i < otro.arbitros.getTamanio(); i++)
    {
        arbitros.agregar(otro.arbitros.obtener(i));
    }
}

Partido::~Partido() {
    // Los árbitros son compartidos, no los liberamos aquí
}

// ---- Getters ----
string Partido::getFecha() const
{
    return fecha;
}
string Partido::getHora() const
{
    return hora;
}
string Partido::getSede() const
{
    return sede;
}
Equipo* Partido::getEquipo1() const
{
    return equipo1;
}
Equipo* Partido::getEquipo2() const
{
    return equipo2;
}
Lista<Arbitro*>& Partido::getArbitros()
{
    return arbitros;
}
EstadisticasPartido& Partido::getStatsEq1()
{
    return statsEq1;
}
EstadisticasPartido& Partido::getStatsEq2()
{
    return statsEq2;
}
Equipo* Partido::getGanador() const
{
    return ganador;
}
bool Partido::isProrroga() const
{
    return prorroga;
}
bool Partido::isSimulado() const
{
    return simulado;
}

//Setters
void Partido::setFecha(string f)
{
    fecha = f;
}
void Partido::setHora(string h)
{
    hora = h;
}
void Partido::setSede(string s)
{
    sede = s;
}
void Partido::agregarArbitro(Arbitro* a)
{
    arbitros.agregar(a);
}

//Ecuación (1): λ_A = µ * (GF_A/µ)^α * (GC_B/µ)^β
float Partido::calcularLambda(Equipo* atacante, Equipo* defensor) const
{
    float gfA = atacante->getStatsHistorico().promedioGF();
    float gcB = defensor->getStatsHistorico().promedioGC();

    // Evita división por cero o log de cero
    if (gfA <= 0) gfA = 0.5f;
    if (gcB <= 0) gcB = 0.5f;

    float lambda = MU * pow(gfA / MU, ALPHA) * pow(gcB / MU, BETA);
    return lambda;
}

// Elige 11 jugadores aleatorios de la plantilla
Lista<EstJugPartido*>* Partido::elegirConvocados(Equipo* equipo, int minutos)
{
    Lista<EstJugPartido*>* convocados = new Lista<EstJugPartido*>(11);
    Lista<Jugador*>& plantilla = equipo->getPlantilla();
    int n = plantilla.getTamanio(); // 26

    // Copia índices disponibles
    int* indices = new int[n];
    for (int i = 0; i < n; i++) indices[i] = i;

    // Fisher-Yates shuffle parcial: selecciona 11 al azar
    for (int i = 0; i < 11; i++) {
        int j = i + rand() % (n - i);
        // Intercambia indices[i] e indices[j]
        int tmp = indices[i];
        indices[i] = indices[j];
        indices[j] = tmp;

        EstJugPartido* ejp = new EstJugPartido(plantilla[indices[i]], minutos);
        convocados->agregar(ejp);
    }

    delete[] indices;
    return convocados;
}

//Simula goles y los distribuye entre jugadores
int Partido::simularGoles(float lambda, Lista<EstJugPartido*>& jugadores,
                          long& iters)
{
    int golesEquipo = 0;
    int golesRestantes = (int)round(lambda); // Goles esperados redondeados

    // Intenta asignar goles a jugadores con probabilidad 4% c/u
    // hasta alcanzar los goles esperados
    for (int i = 0; i < jugadores.getTamanio() && golesRestantes > 0; i++)
    {
        iters++;
        if (randFloat() < P_GOL_JUG)
        {
            jugadores[i]->goles++;
            golesEquipo++;
            golesRestantes--;
        }
    }

    // Si no se distribuyeron todos, el primer jugador toma el resto
    // (garantiza que el marcador refleje λ)
    if (golesRestantes > 0 && jugadores.getTamanio() > 0)
    {
        jugadores[0]->goles += golesRestantes;
        golesEquipo += golesRestantes;
    }

    return golesEquipo;
}

// Simula tarjetas y faltas
void Partido::simularTarjetasYFaltas(Lista<EstJugPartido*>& jugadores,
                                     long& iters)
{
    for (int i = 0; i < jugadores.getTamanio(); i++)
    {
        iters++;
        EstJugPartido* ejp = jugadores[i];

        // Amarillas
        if (randFloat() < P_AMARILLA1)
        {
            ejp->tarjAmarillas++;
            // Segunda amarilla → roja
            if (randFloat() < P_AMARILLA2)
            {
                ejp->tarjAmarillas++;
                ejp->tarjRojas++;
            }
        }

        // Faltas
        if (randFloat() < P_FALTA1)
        {
            ejp->faltas++;
            if (randFloat() < P_FALTA2)
            {
                ejp->faltas++;
                if (randFloat() < P_FALTA3)
                {
                    ejp->faltas++;
                }
            }
        }
    }
}

//Calcula posesión proporcional al ranking FIFA
// Menor ranking = mejor equipo = más posesión
void Partido::calcularPosesion()
{
    int r1 = equipo1->getRankingFIFA();
    int r2 = equipo2->getRankingFIFA();

    // Inversión: si ranking=1 (mejor), tiene más posesión
    // Posesion eq1 = r2 / (r1 + r2) * 100
    float total = (float)(r1 + r2);
    statsEq1.posesionBall = (r2 / total) * 100.0f;
    statsEq2.posesionBall = (r1 / total) * 100.0f;
}

//Rompe empate en fases eliminatorias
Equipo* Partido::romperEmpate(long& iters)
{
    iters++;
    int r1 = equipo1->getRankingFIFA();
    int r2 = equipo2->getRankingFIFA();

    // Probabilidad de que eq1 gane es proporcional a r2/(r1+r2)
    // (mejor ranking = número menor = más probabilidad)
    float probEq1 = (float)r2 / (float)(r1 + r2);

    if (randFloat() < probEq1)
    {
        statsEq1.golesAFavor++; // Ajusta marcador
        return equipo1;
    } else {
        statsEq2.golesAFavor++;
        return equipo2;
    }
}

// Simulación de partido de fase de grupos
void Partido::simular(long& iteraciones)
{
    if (simulado) return;

    // 1. Elegir 11 convocados por equipo (90 minutos)
    Lista<EstJugPartido*>* conv1 = elegirConvocados(equipo1, 90);
    Lista<EstJugPartido*>* conv2 = elegirConvocados(equipo2, 90);

    // 2. Calcular λ para cada equipo
    float lambda1 = calcularLambda(equipo1, equipo2);
    float lambda2 = calcularLambda(equipo2, equipo1);

    // 3. Simular goles
    int g1 = simularGoles(lambda1, *conv1, iteraciones);
    int g2 = simularGoles(lambda2, *conv2, iteraciones);

    statsEq1.golesAFavor  = g1;
    statsEq1.golesEnContra = g2;
    statsEq2.golesAFavor  = g2;
    statsEq2.golesEnContra = g1;

    // 4. Simular tarjetas y faltas
    simularTarjetasYFaltas(*conv1, iteraciones);
    simularTarjetasYFaltas(*conv2, iteraciones);

    // 5. Calcular posesión
    calcularPosesion();

    // 6. Transferir convocados a las stats del partido
    for (int i = 0; i < conv1->getTamanio(); i++)
        statsEq1.agregarJugador(conv1->obtener(i));
    for (int i = 0; i < conv2->getTamanio(); i++)
        statsEq2.agregarJugador(conv2->obtener(i));

    delete conv1;
    delete conv2;

    // 7. Determinar ganador (puede ser empate en grupos)
    if (g1 > g2)       ganador = equipo1;
    else if (g2 > g1)  ganador = equipo2;
    else               ganador = nullptr; // Empate

    // 8. Actualizar puntos de grupo
    if (ganador == equipo1)
    {
        equipo1->agregarPuntosGrupo(3);
        equipo2->agregarPuntosGrupo(0);
    } else if (ganador == equipo2)
    {
        equipo1->agregarPuntosGrupo(0);
        equipo2->agregarPuntosGrupo(3);
    } else
    {
        equipo1->agregarPuntosGrupo(1);
        equipo2->agregarPuntosGrupo(1);
    }
    equipo1->agregarGolesGrupo(g1, g2);
    equipo2->agregarGolesGrupo(g2, g1);

    // 9. Actualizar estadísticas históricas
    bool eq1Gano   = (ganador == equipo1);
    bool eq1Empato = (ganador == nullptr);
    bool eq2Gano   = (ganador == equipo2);
    bool eq2Empato = eq1Empato;

    EstadisticasEquipo deltaEq1 = statsEq1.toEstadisticasEquipo(eq1Gano, eq1Empato);
    EstadisticasEquipo deltaEq2 = statsEq2.toEstadisticasEquipo(eq2Gano, eq2Empato);
    equipo1->actualizarStats(deltaEq1);
    equipo2->actualizarStats(deltaEq2);

    // 10. Actualizar estadísticas históricas de jugadores
    for (int i = 0; i < statsEq1.jugadores.getTamanio(); i++)
    {
        iteraciones++;
        EstJugPartido* ejp = statsEq1.jugadores[i];
        ejp->jugador->actualizarStats(ejp->toEstadisticasJugador());
    }
    for (int i = 0; i < statsEq2.jugadores.getTamanio(); i++)
    {
        iteraciones++;
        EstJugPartido* ejp = statsEq2.jugadores[i];
        ejp->jugador->actualizarStats(ejp->toEstadisticasJugador());
    }

    simulado = true;
}

//Simulación eliminatoria (sin empate posible)
void Partido::simularEliminatorio(long& iteraciones)
{
    // Primero simula como partido normal
    simular(iteraciones);

    // Si hubo empate, va a prórroga
    if (ganador == nullptr) {
        prorroga = true;

        // Actualiza minutos a 120 para todos los jugadores
        for (int i = 0; i < statsEq1.jugadores.getTamanio(); i++)
        {
            statsEq1.jugadores[i]->minutosJugados = 120;
        }
        for (int i = 0; i < statsEq2.jugadores.getTamanio(); i++)
        {
            statsEq2.jugadores[i]->minutosJugados = 120;
        }

        // Rompe el empate por ranking FIFA
        ganador = romperEmpate(iteraciones);
    }
}

// Imprime datos del partido
void Partido::imprimir() const
{
    cout << "========================================" << endl;
    cout << "Fecha: " << fecha << " | Hora: " << hora << endl;
    cout << "Sede: " << sede << endl;
    cout << equipo1->getPais() << " vs " << equipo2->getPais() << endl;
    cout << "Resultado: " << statsEq1.golesAFavor
         << " - " << statsEq2.golesAFavor << endl;
    if (prorroga) cout << "(Resuelto en prórroga)" << endl;
    if (ganador)  cout << "Ganador: " << ganador->getPais() << endl;
    else          cout << "Resultado: Empate" << endl;
    cout << "Posesión: " << equipo1->getPais() << " "
         << statsEq1.posesionBall << "% | "
         << equipo2->getPais() << " "
         << statsEq2.posesionBall << "%" << endl;
}

//Imprime goleadores por número de camiseta
void Partido::imprimirGoleadores() const {
    cout << "Goleadores " << equipo1->getPais() << ": ";
    for (int i = 0; i < statsEq1.jugadores.getTamanio(); i++)
    {
        EstJugPartido* ejp = statsEq1.jugadores[i];
        if (ejp->goles > 0)
            cout << "#" << ejp->jugador->getNumCamiseta()
                 << "(" << ejp->goles << ") ";
    }
    cout << endl;

    cout << "Goleadores " << equipo2->getPais() << ": ";
    for (int i = 0; i < statsEq2.jugadores.getTamanio(); i++)
    {
        EstJugPartido* ejp = statsEq2.jugadores[i];
        if (ejp->goles > 0)
            cout << "#" << ejp->jugador->getNumCamiseta()
                 << "(" << ejp->goles << ") ";
    }
    cout << endl;
}

long Partido::memoriaBytes() const
{
    long mem = sizeof(Partido) + fecha.size() + hora.size() + sede.size();
    mem += arbitros.memoriaBytes();
    mem += statsEq1.memoriaBytes();
    mem += statsEq2.memoriaBytes();
    return mem;
}
