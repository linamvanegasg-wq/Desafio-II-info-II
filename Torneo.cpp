#include "Torneo.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

MedidorRecursos::MedidorRecursos() : iteraciones(0), memoriaTotal(0)
{

}

void MedidorRecursos::resetear()
{
    iteraciones = 0;
    memoriaTotal = 0;
}

void MedidorRecursos::agregarIteraciones(long n)
{
    iteraciones += n;
}
void MedidorRecursos::setMemoria(long bytes)
{
    memoriaTotal = bytes;
}
long MedidorRecursos::getIteraciones() const
{
    return iteraciones;
}
long MedidorRecursos::getMemoria() const
{
    return memoriaTotal;
}

void MedidorRecursos::imprimir(const string& etapa) const
{
    cout << "\n--- Medicion de Recursos: " << etapa << " ---" << endl;
    cout << "Iteraciones realizadas: " << iteraciones << endl;
    cout << "Memoria total usada:    " << memoriaTotal << " bytes ("
         << memoriaTotal/1024 << " KB)" << endl;
}

long MedidorRecursos::memoriaBytes() const
{
    return sizeof(MedidorRecursos);
}

// Torneo

Torneo::Torneo()
    : equipos(48), confederaciones(10),
    gestorArchivos("selecciones_clasificadas_mundial.csv",
                   "jugadores_stats.csv"),
    faseGrupos(nullptr), r16(nullptr), r8(nullptr),
    qf(nullptr), sf(nullptr), finalTorneo(nullptr),
    cargado(false)
{
    srand((unsigned int)time(nullptr)); // Inicializa semilla aleatoria
}

Torneo::~Torneo()
{
    // Liberar fases
    if (faseGrupos)  delete faseGrupos;
    if (r16)         delete r16;
    if (r8)          delete r8;
    if (qf)          delete qf;
    if (sf)          delete sf;
    if (finalTorneo) delete finalTorneo;

    // Liberar equipos
    for (int i = 0; i < equipos.getTamanio(); i++)
        delete equipos[i];

    // Liberar confederaciones
    for (int i = 0; i < confederaciones.getTamanio(); i++)
        delete confederaciones[i];
}

bool Torneo::isCargado() const
{
    return cargado;
}

//Calcula memoria total

long Torneo::calcularMemoriaTotal() const
{
    long mem = sizeof(Torneo);
    mem += equipos.memoriaBytes();
    mem += confederaciones.memoriaBytes();
    for (int i = 0; i < equipos.getTamanio(); i++)
        mem += equipos[i]->memoriaBytes();
    for (int i = 0; i < confederaciones.getTamanio(); i++)
        mem += confederaciones[i]->getNombre().size() * 2;
    if (faseGrupos)  mem += faseGrupos->memoriaBytes();
    if (r16)         mem += r16->memoriaBytes();
    if (r8)          mem += r8->memoriaBytes();
    if (qf)          mem += qf->memoriaBytes();
    if (sf)          mem += sf->memoriaBytes();
    if (finalTorneo) mem += finalTorneo->memoriaBytes();
    return mem;
}

// 1 - Carga de datos
void Torneo::cargarDatos()
{
    medidor.resetear();
    long iters = 0;

    cout << "\nCargando datos desde CSV..." << endl;
    bool ok = gestorArchivos.cargarEquipos(equipos, confederaciones, iters);

    if (ok)
    {
        // Intentar cargar stats previas de jugadores
        gestorArchivos.cargarJugadores(equipos, iters);
        cargado = true;
        cout << "Datos cargados correctamente." << endl;
        cout << "Equipos: " << equipos.getTamanio() << endl;
        cout << "Confederaciones: " << confederaciones.getTamanio() << endl;
    } else {
        cout << "Error al cargar datos. Verifique el archivo CSV." << endl;
    }

    medidor.agregarIteraciones(iters);
    medidor.setMemoria(calcularMemoriaTotal());
    medidor.imprimir("Carga de datos");
}

// 2. Conformación de grupos
void Torneo::conformarGrupos()
{
    if (!cargado)
    {
        cout << "Primero debe cargar los datos (opcion 1)." << endl;
        return;
    }

    medidor.resetear();
    long iters = 0;

    if (faseGrupos) delete faseGrupos;
    faseGrupos = new FaseGrupos("20/06/2026");

    // Copia de la lista de equipos para el sorteo
    Lista<Equipo*> copiaEquipos(48);
    for (int i = 0; i < equipos.getTamanio(); i++) {
        iters++;
        copiaEquipos.agregar(equipos[i]);
    }

    faseGrupos->realizarSorteo(copiaEquipos, iters);

    medidor.agregarIteraciones(iters);
    medidor.setMemoria(calcularMemoriaTotal());
    medidor.imprimir("Conformacion de grupos");
}

// 3. a - Simular fase de grupos
void Torneo::simularFaseGrupos()
{
    if (!faseGrupos)
    {
        cout << "Primero debe conformar los grupos (opcion 2)." << endl;
        return;
    }
    // Verificar que los grupos tienen equipos
    if (faseGrupos->getGrupos().getTamanio() == 0 ||
        faseGrupos->getGrupos()[0]->getEquipos().getTamanio() == 0)
    {
        cout << "Los grupos estan vacios. Cargue los datos (1) y conforme grupos (2) primero." << endl;
        return;
    }

    medidor.resetear();
    long iters = 0;

    cout << "\nSimulando fase de grupos..." << endl;
    faseGrupos->simular(iters);
    faseGrupos->imprimir();

    // Guardar stats actualizadas
    gestorArchivos.guardarJugadores(equipos, iters);

    medidor.agregarIteraciones(iters);
    medidor.setMemoria(calcularMemoriaTotal());
    medidor.imprimir("Simulacion fase de grupos");
}

// 3.b - Transición a R16
void Torneo::transicionR16()
{
    if (!faseGrupos) {
        cout << "Primero debe simular la fase de grupos (opcion 3)." << endl;
        return;
    }

    medidor.resetear();
    long iters = 0;

    // Determinar clasificados
    faseGrupos->determinarClasificados(iters);

    cout << "\n====== CLASIFICADOS A R16 ======" << endl;
    Lista<Equipo*>& clas = faseGrupos->getClasificados();
    for (int i = 0; i < clas.getTamanio(); i++)
    {
        iters++;
        cout << i+1 << ". " << clas[i]->getPais()
             << " (Pts: " << clas[i]->getPuntosGrupo() << ")" << endl;
    }

    // Configurar R16
    if (r16) delete r16;
    r16 = new FaseDirecta("Dieciseisavos de Final (R16)");
    r16->configurarPartidos(clas, iters);

    cout << "\n====== PARTIDOS R16 CONFIGURADOS ======" << endl;
    Lista<Partido*>& partR16 = r16->getPartidos();
    for (int i = 0; i < partR16.getTamanio(); i++)
    {
        iters++;
        cout << partR16[i]->getEquipo1()->getPais() << " vs "
             << partR16[i]->getEquipo2()->getPais() << endl;
    }

    medidor.agregarIteraciones(iters);
    medidor.setMemoria(calcularMemoriaTotal());
    medidor.imprimir("Transicion a R16");
}

// 3.c - Simular fases directas
void Torneo::simularFaseDirecta()
{
    if (!r16)
    {
        cout << "Primero debe hacer la transicion a R16 (opcion 4)." << endl;
        return;
    }

    medidor.resetear();
    long iters = 0;

    // R16
    cout << "\nSimulando R16..." << endl;
    r16->simular(iters);
    r16->imprimir();
    gestorArchivos.guardarJugadores(equipos, iters);

    // R8
    if (r8) delete r8;
    r8 = new FaseDirecta("Octavos de Final (R8)");
    r8->configurarPartidos(r16->getClasificados(), iters);
    cout << "\nSimulando R8..." << endl;
    r8->simular(iters);
    r8->imprimir();
    gestorArchivos.guardarJugadores(equipos, iters);

    // QF
    if (qf) delete qf;
    qf = new FaseDirecta("Cuartos de Final (QF)");
    qf->configurarPartidos(r8->getClasificados(), iters);
    cout << "\nSimulando Cuartos de Final..." << endl;
    qf->simular(iters);
    qf->imprimir();
    gestorArchivos.guardarJugadores(equipos, iters);

    //SF
    if (sf) delete sf;
    sf = new FaseDirecta("Semifinales (SF)");
    sf->configurarPartidos(qf->getClasificados(), iters);
    cout << "\nSimulando Semifinales..." << endl;
    sf->simular(iters);
    sf->imprimir();

    // Tercer puesto
    Lista<Equipo*>& clasSF = sf->getClasificados();
    Lista<Partido*>& partSF = sf->getPartidos();

    // Los perdedores de las semifinales juegan el tercer puesto
    Lista<Equipo*> perdedores(2);
    for (int i = 0; i < partSF.getTamanio(); i++)
    {
        iters++;
        Equipo* eq1 = partSF[i]->getEquipo1();
        Equipo* eq2 = partSF[i]->getEquipo2();
        Equipo* gan = partSF[i]->getGanador();
        if (gan != eq1) perdedores.agregar(eq1);
        else            perdedores.agregar(eq2);
    }

    Partido* p3 = new Partido("10/07/2026", "00:00", "nombreSede",
                              perdedores[0], perdedores[1]);
    Arbitro* a1 = new Arbitro("codArbitro1", "codArbitro1");
    Arbitro* a2 = new Arbitro("codArbitro2", "codArbitro2");
    Arbitro* a3 = new Arbitro("codArbitro3", "codArbitro3");
    p3->agregarArbitro(a1);
    p3->agregarArbitro(a2);
    p3->agregarArbitro(a3);
    p3->simularEliminatorio(iters);
    sf->setTercerPuesto(p3);

    cout << "\n--- Partido Tercer Puesto ---" << endl;
    p3->imprimir();
    p3->imprimirGoleadores();

    // Final
    if (finalTorneo) delete finalTorneo;
    finalTorneo = new FaseDirecta("Final", true);
    finalTorneo->configurarPartidos(clasSF, iters);
    cout << "\nSimulando Final..." << endl;
    finalTorneo->simular(iters);
    finalTorneo->imprimir();
    gestorArchivos.guardarJugadores(equipos, iters);

    medidor.agregarIteraciones(iters);
    medidor.setMemoria(calcularMemoriaTotal());
    medidor.imprimir("Simulacion fases directas");
}

// 4. Estadísticas finales
void Torneo::mostrarEstadisticas()
{
    if (!finalTorneo || finalTorneo->getClasificados().getTamanio() == 0)
    {
        cout << "Primero debe simular todo el torneo." << endl;
        return;
    }

    medidor.resetear();
    long iters = 0;

    cout << "\n========== ESTADISTICAS FINALES DEL TORNEO ==========" << endl;

    // 1. Top 4
    Equipo* campeon   = finalTorneo->getClasificados()[0];
    Equipo* subcampeon = finalTorneo->getPartidos()[0]->getEquipo1() == campeon
                             ? finalTorneo->getPartidos()[0]->getEquipo2()
                             : finalTorneo->getPartidos()[0]->getEquipo1();
    Equipo* tercero   = sf->getTercerPuesto()->getGanador();
    Equipo* cuarto    = sf->getTercerPuesto()->getEquipo1() == tercero
                         ? sf->getTercerPuesto()->getEquipo2()
                         : sf->getTercerPuesto()->getEquipo1();

    cout << "\n--- Top 4 ---" << endl;
    cout << "1. " << campeon->getPais()    << endl;
    cout << "2. " << subcampeon->getPais() << endl;
    cout << "3. " << tercero->getPais()    << endl;
    cout << "4. " << cuarto->getPais()     << endl;

    // 2. Máximo goleador del campeón
    cout << "\n--- Maximo goleador del campeon (" << campeon->getPais() << ") ---" << endl;
    Jugador* maxGoleador = nullptr;
    int maxGoles = -1;
    Lista<Jugador*>& plantilla = campeon->getPlantilla();
    for (int i = 0; i < plantilla.getTamanio(); i++)
    {
        iters++;
        int g = plantilla[i]->getStatsHist().goles;
        if (g > maxGoles)
        {
            maxGoles = g;
            maxGoleador = plantilla[i];
        }
    }
    if (maxGoleador)
        cout << maxGoleador->getNombre() << " " << maxGoleador->getApellido()
             << " - " << maxGoles << " goles" << endl;

    // 3. Top 3 goleadores del torneo
    cout << "\n--- Top 3 goleadores del torneo ---" << endl;
    Jugador* top[3] = {nullptr, nullptr, nullptr};
    int topG[3] = {-1, -1, -1};

    for (int i = 0; i < equipos.getTamanio(); i++)
    {
        iters++;
        Lista<Jugador*>& p = equipos[i]->getPlantilla();
        for (int j = 0; j < p.getTamanio(); j++)
        {
            iters++;
            int g = p[j]->getStatsHist().goles;
            if (g > topG[0])
            {
                topG[2]=topG[1]; top[2]=top[1];
                topG[1]=topG[0]; top[1]=top[0];
                topG[0]=g;       top[0]=p[j];
            } else if (g > topG[1])
            {
                topG[2]=topG[1]; top[2]=top[1];
                topG[1]=g;       top[1]=p[j];
            } else if (g > topG[2]) {
                topG[2]=g;       top[2]=p[j];
            }
        }
    }
    for (int i = 0; i < 3; i++)
    {
        if (top[i])
            cout << i+1 << ". " << top[i]->getNombre() << " "
                 << top[i]->getApellido() << " - " << topG[i] << " goles" << endl;
    }

    // 4. Equipo con más goles históricos
    cout << "\n--- Equipo con mas goles historicos ---" << endl;
    Equipo* maxEq = nullptr;
    int maxGolesEq = -1;
    for (int i = 0; i < equipos.getTamanio(); i++)
    {
        iters++;
        int g = equipos[i]->getStatsHistorico().golesAFavor;
        if (g > maxGolesEq)
        {
            maxGolesEq = g;
            maxEq = equipos[i];
        }
    }
    if (maxEq)
        cout << maxEq->getPais() << " con " << maxGolesEq << " goles" << endl;

    // 5. Confederación con mayor presencia en R16, R8, R4
    cout << "\n--- Confederacion con mayor presencia ---" << endl;
    string etapas[] = {"R16", "R8", "QF"};
    FaseDirecta* fases[] = {r16, r8, qf};

    for (int f = 0; f < 3; f++)
    {
        if (!fases[f]) continue;
        iters++;
        // Contar equipos por confederación
        string confNames[10];
        int confCount[10] = {0};
        int numConfs = 0;

        Lista<Equipo*>& cls = fases[f]->getClasificados();
        for (int i = 0; i < cls.getTamanio(); i++)
        {
            iters++;
            string cn = cls[i]->getConfederacion()->getNombre();
            bool found = false;
            for (int j = 0; j < numConfs; j++)
            {
                if (confNames[j] == cn)
                {
                    confCount[j]++;
                    found = true;
                    break;
                }
            }
            if (!found && numConfs < 10)
            {
                confNames[numConfs] = cn;
                confCount[numConfs] = 1;
                numConfs++;
            }
        }

        // Encontrar la mayor
        int maxIdx = 0;
        for (int i = 1; i < numConfs; i++) {
            if (confCount[i] > confCount[maxIdx]) maxIdx = i;
        }
        cout << etapas[f] << ": " << confNames[maxIdx]
             << " (" << confCount[maxIdx] << " equipos)" << endl;
    }

    medidor.agregarIteraciones(iters);
    medidor.setMemoria(calcularMemoriaTotal());
    medidor.imprimir("Estadisticas finales");
}

long Torneo::memoriaBytes() const {
    return calcularMemoriaTotal();
}
