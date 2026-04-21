#ifndef EQUIPO_H
#define EQUIPO_H
#include <string>
#include "Lista.h"
#include "Jugador.h"
#include "Estadisticas.h"
#include "Confederacion.h"
using namespace std;

class Equipo
{
private:
    string pais;
    string dirTecnico;
    int rankingFIFA;
    Confederacion* confederacion;
    Lista<Jugador*> plantilla;
    EstadisticasEquipo statsHistorico;

    // Puntos acumulados en la fase de grupos-para la tabla de clasificación
    int puntosGrupo;
    // Goles acumulados solo en fase de grupos --para el desempate
    int golesGrupo;
    int golesEnContraGrupo;

public:
    Equipo();
    Equipo(string pais, string dirTecnico, int ranking, Confederacion* conf);

    // Crea una copia
    Equipo(const Equipo& otro);

    // Libera los jugadores de la plantilla
    ~Equipo();

    // Getters
    string getPais() const;
    string getDirTecnico() const;
    int getRankingFIFA() const;
    Confederacion* getConfederacion() const;
    Lista<Jugador*>& getPlantilla();
    const Lista<Jugador*>& getPlantilla() const;
    EstadisticasEquipo& getStatsHistorico();
    const EstadisticasEquipo& getStatsHistorico() const;
    int getPuntosGrupo() const;
    int getGolesGrupo() const;
    int getGolesEnContraGrupo() const;
    int getDifGolesGrupo() const;

    // Setters
    void setPais(string pais);
    void setDirTecnico(string dt);
    void setRankingFIFA(int ranking);
    void setConfederacion(Confederacion* conf);

    // Genera la plantilla... nombres "nombre1"..."nombre26", goles repartidos uniformemente
    void generarPlantilla();

    // Agrega los puntos despues de un partido de grupo
    void agregarPuntosGrupo(int puntos);
    void agregarGolesGrupo(int gf, int gc);

    // Reinicia el contador de la fase de grupos
    void reiniciarContadoresGrupo();

    // Actualiza las estadísticas históricas del equipo despues de un partido
    void actualizarStats(const EstadisticasEquipo& delta);

    // Comparar por paises
    bool operator==(const Equipo& otro) const;

    // Memoria
    long memoriaBytes() const;
};

#endif
