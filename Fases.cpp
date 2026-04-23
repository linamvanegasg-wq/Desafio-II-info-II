#include "Fases.h"
#include <iostream>
#include <cstdlib>
using namespace std;

// FaseEliminatoria

FaseEliminatoria::FaseEliminatoria()
    : nombre(""), partidos(10), clasificados(32)
{

}

FaseEliminatoria::FaseEliminatoria(string nombre)
    : nombre(nombre), partidos(10), clasificados(32)
{

}

FaseEliminatoria::FaseEliminatoria(const FaseEliminatoria& otra)
    : nombre(otra.nombre), partidos(10), clasificados(32)
{
    for (int i = 0; i < otra.partidos.getTamanio(); i++)
        partidos.agregar(otra.partidos.obtener(i));
    for (int i = 0; i < otra.clasificados.getTamanio(); i++)
        clasificados.agregar(otra.clasificados.obtener(i));
}

FaseEliminatoria::~FaseEliminatoria()
{
    for (int i = 0; i < partidos.getTamanio(); i++)
        delete partidos[i];
}

string FaseEliminatoria::getNombre() const
{
    return nombre;
}
Lista<Partido*>& FaseEliminatoria::getPartidos() {

    return partidos;
}
Lista<Equipo*>& FaseEliminatoria::getClasificados()
{
    return clasificados;
}

void FaseEliminatoria::agregarPartido(Partido* p)
{
    partidos.agregar(p);
}
void FaseEliminatoria::agregarClasificado(Equipo* e)
{
    clasificados.agregar(e);
}

void FaseEliminatoria::simular(long& iteraciones)
{
    for (int i = 0; i < partidos.getTamanio(); i++) {
        iteraciones++;
        partidos[i]->simularEliminatorio(iteraciones);
        clasificados.agregar(partidos[i]->getGanador());
    }
}

void FaseEliminatoria::imprimir() const
{
    cout << "\n========== " << nombre << " ==========" << endl;
    for (int i = 0; i < partidos.getTamanio(); i++) {
        partidos[i]->imprimir();
        partidos[i]->imprimirGoleadores();
    }
}

long FaseEliminatoria::memoriaBytes() const
{
    long mem = sizeof(FaseEliminatoria) + nombre.size();
    mem += partidos.memoriaBytes();
    mem += clasificados.memoriaBytes();
    for (int i = 0; i < partidos.getTamanio(); i++)
        mem += partidos[i]->memoriaBytes();
    return mem;
}

// FaseGrupos

FaseGrupos::FaseGrupos()
    : FaseEliminatoria("Fase de Grupos"),
      grupos(12), fechaInicio("20/06/2026")
{
    for (int i = 0; i < 4; i++) bombos[i] = Lista<Equipo*>(12);
}

FaseGrupos::FaseGrupos(string fechaInicio)
    : FaseEliminatoria("Fase de Grupos"),
      grupos(12), fechaInicio(fechaInicio)
{
    for (int i = 0; i < 4; i++) bombos[i] = Lista<Equipo*>(12);
}

FaseGrupos::FaseGrupos(const FaseGrupos& otra)
    : FaseEliminatoria(otra), grupos(12),
      fechaInicio(otra.fechaInicio)
{
    for (int i = 0; i < otra.grupos.getTamanio(); i++)
        grupos.agregar(otra.grupos.obtener(i));
    for (int i = 0; i < 4; i++)
    {
        bombos[i] = Lista<Equipo*>(12);
        for (int j = 0; j < otra.bombos[i].getTamanio(); j++)
            bombos[i].agregar(otra.bombos[i].obtener(j));
    }
}

FaseGrupos::~FaseGrupos()
{
    for (int i = 0; i < grupos.getTamanio(); i++)
        delete grupos[i];
}

Lista<Grupo*>& FaseGrupos::getGrupos() { return grupos; }

// Ordena por ranking FIFA (burbuja)
void FaseGrupos::ordenarPorRanking(Lista<Equipo*>& lista, long& iteraciones)
{
    int n = lista.getTamanio();
    for (int i = 0; i < n-1; i++)
    {
        for (int j = 0; j < n-i-1; j++)
        {
            iteraciones++;
            if (lista[j]->getRankingFIFA() > lista[j+1]->getRankingFIFA())
            {
                Equipo* tmp = lista[j];
                lista[j]   = lista[j+1];
                lista[j+1] = tmp;
            }
        }
    }
}

//Arma los 4 bombos
void FaseGrupos::armarBombos(Lista<Equipo*>& equipos, long& iteraciones)
{
    // Ordenar por ranking
    ordenarPorRanking(equipos, iteraciones);

    // EE.UU va directo al bombo 1 (país sede)
    Equipo* eeuu = nullptr;
    int idxEEUU = -1;
    for (int i = 0; i < equipos.getTamanio(); i++)
    {
        iteraciones++;
        if (equipos[i]->getPais() == "United States")
        {
            eeuu = equipos[i];
            idxEEUU = i;
            break;
        }
    }

    // Agregar EE.UU al bombo 1 primero
    if (eeuu)
    {
        bombos[0].agregar(eeuu);
        equipos.eliminar(idxEEUU);
    }

    // Repartir los 47 restantes en bombos de 12
    // Bombo 1: primeros 11 (+ EEUU = 12)
    // Bombo 2: siguientes 12
    // Bombo 3: siguientes 12
    // Bombo 4: últimos 12
    for (int i = 0; i < equipos.getTamanio(); i++)
    {
        iteraciones++;
        if (i < 11)       bombos[0].agregar(equipos[i]);
        else if (i < 23)  bombos[1].agregar(equipos[i]);
        else if (i < 35)  bombos[2].agregar(equipos[i]);
        else              bombos[3].agregar(equipos[i]);
    }
}

//Saca equipo aleatorio de un bombo respetando restricciones
Equipo* FaseGrupos::sacarDeBoomo(int numBombo, Grupo* grupo,
                                  long& iteraciones)
{
    Lista<Equipo*>& bombo = bombos[numBombo];
    int intentos = 0;

    while (bombo.getTamanio() > 0 && intentos < 100)
    {
        iteraciones++;
        intentos++;
        int idx = rand() % bombo.getTamanio();
        Equipo* candidato = bombo[idx];

        if (grupo->puedeAgregar(candidato))
        {
            bombo.eliminar(idx);
            return candidato;
        }
    }

    // Si no se encontró candidato válido, toma el primero disponible
    if (bombo.getTamanio() > 0)
    {
        Equipo* eq = bombo[0];
        bombo.eliminar(0);
        return eq;
    }
    return nullptr;
}

// Realiza el sorteo y forma los 12 grupos
void FaseGrupos::realizarSorteo(Lista<Equipo*>& equipos, long& iteraciones)
{
    armarBombos(equipos, iteraciones);

    // Crear los 12 grupos (A-L)
    char letras[] = {'A','B','C','D','E','F','G','H','I','J','K','L'};
    for (int i = 0; i < 12; i++) {
        iteraciones++;
        grupos.agregar(new Grupo(letras[i]));
    }

    // Asignar un equipo de cada bombo a cada grupo
    for (int bombo = 0; bombo < 4; bombo++)
    {
        for (int g = 0; g < 12; g++)
        {
            iteraciones++;
            Equipo* eq = sacarDeBoomo(bombo, grupos[g], iteraciones);
            if (eq) grupos[g]->agregarEquipo(eq);
        }
    }

    // Generar partidos para cada grupo
    for (int i = 0; i < grupos.getTamanio(); i++) {
        iteraciones++;
        grupos[i]->generarPartidos(fechaInicio, iteraciones);
    }

    // Imprimir grupos conformados
    cout << "\n====== GRUPOS CONFORMADOS ======" << endl;
    for (int i = 0; i < grupos.getTamanio(); i++)
    {
        cout << "\nGRUPO " << grupos[i]->getLetra() << ":" << endl;
        for (int j = 0; j < grupos[i]->getEquipos().getTamanio(); j++)
        {
            Equipo* eq = grupos[i]->getEquipos()[j];
            cout << "  - " << eq->getPais()
                 << " (" << eq->getConfederacion()->getNombre()
                 << ", Ranking " << eq->getRankingFIFA() << ")" << endl;
        }
    }
}

// Simula todos los partidos de grupos
void FaseGrupos::simular(long& iteraciones)
{
    for (int i = 0; i < grupos.getTamanio(); i++)
    {
        iteraciones++;
        grupos[i]->simularPartidos(iteraciones);
        grupos[i]->ordenarEquipos(iteraciones);
    }
}

//Determina los 32 clasificados
void FaseGrupos::determinarClasificados(long& iteraciones)
{
    // 12 primeros + 12 segundos pasan automáticamente
    // 8 mejores terceros también pasan
    Lista<Equipo*> terceros(12);

    for (int i = 0; i < grupos.getTamanio(); i++)
    {
        iteraciones++;
        Lista<Equipo*>& eq = grupos[i]->getEquipos();
        clasificados.agregar(eq[0]); // 1ro
        clasificados.agregar(eq[1]); // 2do
        terceros.agregar(eq[2]);     // 3ro (candidato)
    }

    // Ordenar terceros por puntos y tomar los 8 mejores
    int n = terceros.getTamanio();
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++)
        {
            iteraciones++;
            Equipo* a = terceros[j];
            Equipo* b = terceros[j+1];
            if (a->getPuntosGrupo() < b->getPuntosGrupo())
            {
                terceros[j]   = b;
                terceros[j+1] = a;
            }
        }
    }

    // Agregar los 8 mejores terceros
    for (int i = 0; i < 8 && i < terceros.getTamanio(); i++)
    {
        iteraciones++;
        clasificados.agregar(terceros[i]);
    }
}

void FaseGrupos::imprimir() const
{
    for (int i = 0; i < grupos.getTamanio(); i++)
    {
        grupos[i]->imprimirTabla();
        grupos[i]->imprimirPartidos();
    }
}

long FaseGrupos::memoriaBytes() const
{
    long mem = FaseEliminatoria::memoriaBytes();
    mem += grupos.memoriaBytes();
    for (int i = 0; i < grupos.getTamanio(); i++)
        mem += grupos[i]->memoriaBytes();
    for (int i = 0; i < 4; i++)
        mem += bombos[i].memoriaBytes();
    return mem;
}

// FaseDirecta

FaseDirecta::FaseDirecta()
    : FaseEliminatoria(""), esFinal(false), tercerPuesto(nullptr)
{

}

FaseDirecta::FaseDirecta(string nombre, bool esFinal)
    : FaseEliminatoria(nombre), esFinal(esFinal), tercerPuesto(nullptr)
{

}

FaseDirecta::FaseDirecta(const FaseDirecta& otra)
    : FaseEliminatoria(otra), esFinal(otra.esFinal),
      tercerPuesto(otra.tercerPuesto)
{

}

FaseDirecta::~FaseDirecta()
{
    if (tercerPuesto) delete tercerPuesto;
}

bool FaseDirecta::getEsFinal() const
{
    return esFinal;
}
Partido* FaseDirecta::getTercerPuesto() const
{
    return tercerPuesto;
}
void FaseDirecta::setTercerPuesto(Partido* p)
{
    tercerPuesto = p;
}

// ---- Configura partidos enfrentando equipos de la lista ----
void FaseDirecta::configurarPartidos(Lista<Equipo*>& equipos,
                                      long& iteraciones) {
    // Empareja equipos de 2 en 2: [0]vs[1], [2]vs[3], etc.
    for (int i = 0; i+1 < equipos.getTamanio(); i += 2) {
        iteraciones++;
        Partido* p = new Partido("10/07/2026", "00:00", "nombreSede",
                                  equipos[i], equipos[i+1]);
        Arbitro* a1 = new Arbitro("codArbitro1", "codArbitro1");
        Arbitro* a2 = new Arbitro("codArbitro2", "codArbitro2");
        Arbitro* a3 = new Arbitro("codArbitro3", "codArbitro3");
        p->agregarArbitro(a1);
        p->agregarArbitro(a2);
        p->agregarArbitro(a3);
        partidos.agregar(p);
    }
}

// partidos eliminatorios
void FaseDirecta::simular(long& iteraciones)
{
    clasificados.vaciar();
    for (int i = 0; i < partidos.getTamanio(); i++)
    {
        iteraciones++;
        partidos[i]->simularEliminatorio(iteraciones);
        clasificados.agregar(partidos[i]->getGanador());
    }
}

void FaseDirecta::imprimir() const
{
    cout << "\n========== " << nombre << " ==========" << endl;
    for (int i = 0; i < partidos.getTamanio(); i++)
    {
        partidos[i]->imprimir();
        partidos[i]->imprimirGoleadores();
    }
    if (tercerPuesto)
    {
        cout << "\n--- Partido Tercer Puesto ---" << endl;
        tercerPuesto->imprimir();
        tercerPuesto->imprimirGoleadores();
    }
}

long FaseDirecta::memoriaBytes() const
{
    long mem = FaseEliminatoria::memoriaBytes() + sizeof(FaseDirecta);
    if (tercerPuesto) mem += tercerPuesto->memoriaBytes();
    return mem;
}
