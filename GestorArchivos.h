#ifndef GESTORARCHIVOS_H
#define GESTORARCHIVOS_H

// ============================================================
// GestorArchivos.h
// Lee el CSV de equipos y guarda/carga estadísticas de jugadores
// NO usa librerías de terceros (solo fstream, string estándar)
// ============================================================

#include <string>
#include "Lista.h"
#include "Equipo.h"
#include "Confederacion.h"
using namespace std;

class GestorArchivos {
private:
    // Ruta del CSV principal de equipos
    string rutaCSV;

    // Ruta del archivo de estadísticas de jugadores
    string rutaJugadores;

    // Elimina espacios al inicio y al final de un string
    // (necesario porque el CSV puede tener espacios extra)
    string trim(const string& s) const;

    // Separa una línea del CSV por comas y retorna los campos
    // Llena el arreglo 'campos' y retorna cuántos campos encontró
    int parsearLinea(const string& linea, string* campos, int maxCampos) const;

    // Busca o crea una confederación en la lista
    // Evita duplicados (UEFA no se crea dos veces)
    Confederacion* obtenerConfederacion(Lista<Confederacion*>& confs,
                                        const string& nombre) const;

public:
    GestorArchivos();
    GestorArchivos(string rutaCSV, string rutaJugadores);

    // Lee el CSV y llena la lista de equipos y confederaciones
    // Retorna true si se leyó correctamente
    bool cargarEquipos(Lista<Equipo*>& equipos,
                       Lista<Confederacion*>& confederaciones,
                       long& iteraciones) const;

    // Guarda las estadísticas de todos los jugadores en archivo propio
    // Formato: pais,camiseta,goles,partidos,minutos,asistencias,amarillas,rojas,faltas
    bool guardarJugadores(const Lista<Equipo*>& equipos,
                          long& iteraciones) const;

    // Carga las estadísticas de jugadores desde el archivo
    bool cargarJugadores(Lista<Equipo*>& equipos,
                         long& iteraciones) const;

    long memoriaBytes() const;
};

#endif
