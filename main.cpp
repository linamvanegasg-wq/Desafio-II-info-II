#include <iostream>
#include "Torneo.h"
using namespace std;

void mostrarMenu() {
    cout << "       UdeAWorldCup - FIFA 2026       " << endl;
    cout << "======================================" << endl;
    cout << "  1. Cargar datos de equipos (CSV)    " << endl;
    cout << "  2. Conformar grupos (sorteo)        " << endl;
    cout << "  3. Simular fase de grupos           " << endl;
    cout << "  4. Transicion a R16                 " << endl;
    cout << "  5. Simular fases directas           " << endl;
    cout << "  6. Estadisticas finales             " << endl;
    cout << "  0. Salir                            " << endl;
    cout << "======================================" << endl;
    cout << "Seleccione una opcion: ";
}

int main()
{
    cout << "   Bienvenido a UdeAWorldCup 2026     " << endl;
    cout << "======================================" << endl;

    Torneo torneo;
    int opcion = -1;

    while (opcion != 0)
    {
        mostrarMenu();
        cin >> opcion;

        switch (opcion)
        {
        case 1:
            torneo.cargarDatos();
            break;
        case 2:
            torneo.conformarGrupos();
            break;
        case 3:
            torneo.simularFaseGrupos();
            break;
        case 4:
            torneo.transicionR16();
            break;
        case 5:
            torneo.simularFaseDirecta();
            break;
        case 6:
            torneo.mostrarEstadisticas();
            break;
        case 0:
            cout << "\nHasta luego. Viva el futbol!" << endl;
            break;
        default:
            cout << "Opcion invalida. Intente de nuevo." << endl;
            break;
        }
    }

    return 0;
}
