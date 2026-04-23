#include "Grupo.h"
#include <iostream>
#include <cstdlib>
using namespace std;

Grupo::Grupo() : letra('?'), equipos(4), partidos(6)
{

}

Grupo::Grupo(char letra) : letra(letra), equipos(4), partidos(6)
{

}

Grupo::Grupo(const Grupo& otro)
    : letra(otro.letra), equipos(4), partidos(6)
{
    for (int i = 0; i < otro.equipos.getTamanio(); i++)
        equipos.agregar(otro.equipos.obtener(i));
    for (int i = 0; i < otro.partidos.getTamanio(); i++)
        partidos.agregar(otro.partidos.obtener(i));
}

Grupo::~Grupo()
{
    for (int i = 0; i < partidos.getTamanio(); i++)
        delete partidos[i];
}

char Grupo::getLetra() const
{
    return letra;
}
Lista<Equipo*>& Grupo::getEquipos()
{
    return equipos;
}
const Lista<Equipo*>& Grupo::getEquipos() const
{
    return equipos;
}
Lista<Partido*>& Grupo::getPartidos()
{
    return partidos;
}
const Lista<Partido*>& Grupo::getPartidos() const
{
    return partidos;
}

void Grupo::agregarEquipo(Equipo* equipo)
{
    equipos.agregar(equipo);
}
void Grupo::agregarPartido(Partido* partido)
{
    partidos.agregar(partido);
}

//Verifica restricciones de confederación
bool Grupo::puedeAgregar(Equipo* equipo) const
{
    string confNueva = equipo->getConfederacion()->getNombre();
    int countConf = 0;

    for (int i = 0; i < equipos.getTamanio(); i++)
    {
        string confExist = equipos[i]->getConfederacion()->getNombre();
        if (confExist == confNueva)
        {
            countConf++;
            // UEFA puede tener máximo 2, las demás máximo 1
            if (confNueva == "UEFA")
            {
                if (countConf >= 2) return false;
            } else {
                return false;
            }
        }
    }
    return true;
}

//Suma días a una fecha en formato "DD/MM/AAAA"
static string sumarDias(const string& fecha, int dias)
{
    // Extraer día, mes, año
    int dia  = (fecha[0]-'0')*10 + (fecha[1]-'0');
    int mes  = (fecha[3]-'0')*10 + (fecha[4]-'0');
    int anio = (fecha[6]-'0')*1000 + (fecha[7]-'0')*100
             + (fecha[8]-'0')*10   + (fecha[9]-'0');

    // Días por mes (simplificado)
    int diasMes[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};

    dia += dias;
    while (dia > diasMes[mes])
    {
        dia -= diasMes[mes];
        mes++;
        if (mes > 12)
        {
            mes = 1; anio++;
        }
    }

    // Formatear resultado
    string result = "";
    result += (char)('0' + dia/10);
    result += (char)('0' + dia%10);
    result += '/';
    result += (char)('0' + mes/10);
    result += (char)('0' + mes%10);
    result += '/';
    result += (char)('0' + anio/1000);
    result += (char)('0' + (anio/100)%10);
    result += (char)('0' + (anio/10)%10);
    result += (char)('0' + anio%10);
    return result;
}

//Genera los 6 partidos del grupo con fechas válidas
void Grupo::generarPartidos(const string& fechaInicio, long& iteraciones)
{
    // Los 6 partidos de un grupo de 4 equipos (A vs B, A vs C, etc.)
    // Pares: (0,1), (0,2), (0,3), (1,2), (1,3), (2,3)
    int pares[6][2] = {{0,1},{2,3},{0,2},{1,3},{0,3},{1,2}};

    // Distribuir en días respetando:
    // - Máx 4 partidos por día (global, aquí por grupo son 2 por día máx)
    // - Ningún equipo juega más de 1 partido en 3 días
    // Usamos días: 0, 1, 4, 5, 9, 10 desde fechaInicio
    // Esto garantiza al menos 3 días entre partidos del mismo equipo
    int diasPartido[6] = {0, 0, 3, 3, 6, 6};

    for (int i = 0; i < 6; i++)
    {
        iteraciones++;
        string fecha = sumarDias(fechaInicio, diasPartido[i]);
        Equipo* eq1 = equipos[pares[i][0]];
        Equipo* eq2 = equipos[pares[i][1]];

        Partido* p = new Partido(fecha, "00:00", "nombreSede", eq1, eq2);

        // Agregar árbitros fijos
        Arbitro* a1 = new Arbitro("codArbitro1", "codArbitro1");
        Arbitro* a2 = new Arbitro("codArbitro2", "codArbitro2");
        Arbitro* a3 = new Arbitro("codArbitro3", "codArbitro3");
        p->agregarArbitro(a1);
        p->agregarArbitro(a2);
        p->agregarArbitro(a3);

        partidos.agregar(p);
    }
}

//Simula todos los partidos del grupo
void Grupo::simularPartidos(long& iteraciones) {
    for (int i = 0; i < partidos.getTamanio(); i++)
    {
        iteraciones++;
        partidos[i]->simular(iteraciones);
    }
}

//Ordena equipos por puntos (burbuja)
// Criterios: puntos → dif goles → goles a favor → sorteo
void Grupo::ordenarEquipos(long& iteraciones)
{
    int n = equipos.getTamanio();
    for (int i = 0; i < n-1; i++)
    {
        for (int j = 0; j < n-i-1; j++)
        {
            iteraciones++;
            Equipo* a = equipos[j];
            Equipo* b = equipos[j+1];

            bool intercambiar = false;

            if (a->getPuntosGrupo() < b->getPuntosGrupo())
            {
                intercambiar = true;
            } else if (a->getPuntosGrupo() == b->getPuntosGrupo())
            {
                if (a->getDifGolesGrupo() < b->getDifGolesGrupo())
                {
                    intercambiar = true;
                } else if (a->getDifGolesGrupo() == b->getDifGolesGrupo())
                {
                    if (a->getGolesGrupo() < b->getGolesGrupo())
                    {
                        intercambiar = true;
                    } else if (a->getGolesGrupo() == b->getGolesGrupo())
                    {
                        // Sorteo aleatorio
                        intercambiar = (rand() % 2 == 0);
                    }
                }
            }

            if (intercambiar) {
                equipos[j]   = b;
                equipos[j+1] = a;
            }
        }
    }
}

//Imprime tabla de clasificación
void Grupo::imprimirTabla() const
{
    cout << "\n=== GRUPO " << letra << " ===" << endl;
    cout << "Equipo           | PJ | PG | PE | PP | GF | GC | DG | PTS" << endl;
    cout << "-----------------------------------------------------------------" << endl;
    for (int i = 0; i < equipos.getTamanio(); i++)
    {
        Equipo* eq = equipos[i];
        int pj = eq->getPuntosGrupo()/3*1; // Aproximado
        cout << eq->getPais();
        // Espaciado
        for (int s = eq->getPais().size(); s < 17; s++) cout << " ";
        cout << "|  " << 3 // partidos jugados en grupo
             << " |  " << eq->getPuntosGrupo()/3
             << " |  " << 0
             << " |  " << 0
             << " |  " << eq->getGolesGrupo()
             << " |  " << eq->getGolesEnContraGrupo()
             << " |  " << eq->getDifGolesGrupo()
             << " | "  << eq->getPuntosGrupo()
             << endl;
    }
}

//Imprime todos los partidos
void Grupo::imprimirPartidos() const
{
    cout << "\n--- Partidos Grupo " << letra << " ---" << endl;
    for (int i = 0; i < partidos.getTamanio(); i++)
    {
        partidos[i]->imprimir();
        partidos[i]->imprimirGoleadores();
    }
}

long Grupo::memoriaBytes() const
{
    long mem = sizeof(Grupo);
    mem += equipos.memoriaBytes();
    mem += partidos.memoriaBytes();
    for (int i = 0; i < partidos.getTamanio(); i++)
        mem += partidos[i]->memoriaBytes();
    return mem;
}
