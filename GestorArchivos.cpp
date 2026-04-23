#include "GestorArchivos.h"
#include <fstream>
#include <iostream>
using namespace std;

GestorArchivos::GestorArchivos()
    : rutaCSV("selecciones_clasificadas_mundial.csv"),
    rutaJugadores("jugadores_stats.csv")
{

}

GestorArchivos::GestorArchivos(string rutaCSV, string rutaJugadores)
    : rutaCSV(rutaCSV), rutaJugadores(rutaJugadores)
{

}

string GestorArchivos::trim(const string& s) const
{
    int inicio = 0;
    int fin = (int)s.size() - 1;
    while (inicio <= fin && (s[inicio]==' '||s[inicio]=='\t'
                             ||s[inicio]=='\r'||s[inicio]=='\n'))
        inicio++;
    while (fin >= inicio && (s[fin]==' '||s[fin]=='\t'
                             ||s[fin]=='\r'||s[fin]=='\n'))
        fin--;
    if (inicio > fin) return "";
    return s.substr(inicio, fin - inicio + 1);
}

// Separador: punto y coma (;)
int GestorArchivos::parsearLinea(const string& linea, string* campos,
                                 int maxCampos) const
{
    int count = 0;
    string campo = "";
    for (int i = 0; i < (int)linea.size() && count < maxCampos; i++)
    {
        if (linea[i] == ';') {
            campos[count++] = trim(campo);
            campo = "";
        } else {
            campo += linea[i];
        }
    }
    if (count < maxCampos && !campo.empty())
    {
        campos[count++] = trim(campo);
    }
    return count;
}

Confederacion* GestorArchivos::obtenerConfederacion(
    Lista<Confederacion*>& confs, const string& nombre) const
{
    for (int i = 0; i < confs.getTamanio(); i++)
    {
        if (confs[i]->getNombre() == nombre)
            return confs[i];
    }
    Confederacion* nueva = new Confederacion(nombre, nombre);
    confs.agregar(nueva);
    return nueva;
}

bool GestorArchivos::cargarEquipos(Lista<Equipo*>& equipos,
                                   Lista<Confederacion*>& confederaciones,
                                   long& iteraciones) const
{
    ifstream archivo(rutaCSV);
    if (!archivo.is_open()) {
        cout << "Error: no se pudo abrir " << rutaCSV << endl;
        cout << "Asegurese de que el archivo CSV esta en la misma carpeta del ejecutable." << endl;
        return false;
    }

    string linea;
    getline(archivo, linea); // Saltar linea 1: titulo
    getline(archivo, linea); // Saltar linea 2: encabezados

    while (getline(archivo, linea))
    {
        iteraciones++;
        if (trim(linea).empty()) continue;

        string campos[10];
        int n = parsearLinea(linea, campos, 10);
        if (n < 9) continue; // necesitamos al menos 9 campos

        // 0:Ranking 1:Pais 2:DirTecnico 3:Federacion 4:Confederacion
        // 5:GF 6:GC 7:PG 8:PE 9:PP
        int ranking = 0;
        for (char c : campos[0])
            if (c>='0' && c<='9') ranking = ranking*10+(c-'0');

        string pais       = campos[1];
        string dirTecnico = campos[2];
        string confNombre = campos[4];

        int gf=0,gc=0,pg=0,pe=0,pp=0;
        for (char c : campos[5]) if(c>='0'&&c<='9') gf=gf*10+(c-'0');
        for (char c : campos[6]) if(c>='0'&&c<='9') gc=gc*10+(c-'0');
        for (char c : campos[7]) if(c>='0'&&c<='9') pg=pg*10+(c-'0');
        for (char c : campos[8]) if(c>='0'&&c<='9') pe=pe*10+(c-'0');
        if (n > 9)
            for (char c : campos[9]) if(c>='0'&&c<='9') pp=pp*10+(c-'0');

        Confederacion* conf = obtenerConfederacion(confederaciones, confNombre);
        Equipo* equipo = new Equipo(pais, dirTecnico, ranking, conf);

        equipo->getStatsHistorico().golesAFavor   = gf;
        equipo->getStatsHistorico().golesEnContra = gc;
        equipo->getStatsHistorico().partGanados   = pg;
        equipo->getStatsHistorico().partEmpatados = pe;
        equipo->getStatsHistorico().partPerdidos  = pp;
        equipo->getStatsHistorico().tarjAmarillas = 0;
        equipo->getStatsHistorico().tarjRojas     = 0;
        equipo->getStatsHistorico().faltas        = 0;

        equipo->generarPlantilla();
        equipos.agregar(equipo);
        iteraciones++;
    }

    archivo.close();
    cout << "Equipos cargados: " << equipos.getTamanio() << endl;
    return true;
}

bool GestorArchivos::guardarJugadores(const Lista<Equipo*>& equipos,
                                      long& iteraciones) const
{
    ofstream archivo(rutaJugadores);
    if (!archivo.is_open())
    {
        cout << "Error: no se pudo crear " << rutaJugadores << endl;
        return false;
    }

    archivo << "pais,camiseta,goles,partidos,minutos,asistencias,"
            << "amarillas,rojas,faltas" << endl;

    for (int i = 0; i < equipos.getTamanio(); i++)
    {
        iteraciones++;
        Equipo* eq = equipos[i];
        const Lista<Jugador*>& plantilla = eq->getPlantilla();
        for (int j = 0; j < plantilla.getTamanio(); j++)
        {
            iteraciones++;
            Jugador* jug = plantilla[j];
            const EstadisticasJugador& st = jug->getStatsHist();
            archivo << eq->getPais()        << ","
                    << jug->getNumCamiseta() << ","
                    << st.goles             << ","
                    << st.partJugados       << ","
                    << st.minutosJugados    << ","
                    << st.asistencias       << ","
                    << st.tarjAmarillas     << ","
                    << st.tarjRojas         << ","
                    << st.faltas            << endl;
        }
    }

    archivo.close();
    cout << "Estadisticas de jugadores guardadas en " << rutaJugadores << endl;
    return true;
}

bool GestorArchivos::cargarJugadores(Lista<Equipo*>& equipos,
                                     long& iteraciones) const
{
    ifstream archivo(rutaJugadores);
    if (!archivo.is_open())
    {
        cout << "Archivo de jugadores no encontrado. Se usaran estadisticas generadas." << endl;
        return false;
    }

    string linea;
    getline(archivo, linea); // Saltar encabezado

    while (getline(archivo, linea))
    {
        iteraciones++;
        if (trim(linea).empty()) continue;

        string campos[9];
        // Este archivo usa coma
        int count = 0;
        string campo = "";
        for (int i = 0; i < (int)linea.size() && count < 9; i++)
        {
            if (linea[i] == ',')
            {
                campos[count++] = trim(campo); campo = "";
            }
            else campo += linea[i];
        }
        if (count < 9 && !campo.empty()) campos[count++] = trim(campo);
        if (count < 9) continue;

        string pais = campos[0];
        int camiseta=0,goles=0,parts=0,mins=0,asis=0,ama=0,roj=0,falt=0;
        for (char c:campos[1]) if(c>='0'&&c<='9') camiseta=camiseta*10+(c-'0');
        for (char c:campos[2]) if(c>='0'&&c<='9') goles=goles*10+(c-'0');
        for (char c:campos[3]) if(c>='0'&&c<='9') parts=parts*10+(c-'0');
        for (char c:campos[4]) if(c>='0'&&c<='9') mins=mins*10+(c-'0');
        for (char c:campos[5]) if(c>='0'&&c<='9') asis=asis*10+(c-'0');
        for (char c:campos[6]) if(c>='0'&&c<='9') ama=ama*10+(c-'0');
        for (char c:campos[7]) if(c>='0'&&c<='9') roj=roj*10+(c-'0');
        for (char c:campos[8]) if(c>='0'&&c<='9') falt=falt*10+(c-'0');

        for (int i = 0; i < equipos.getTamanio(); i++)
        {
            iteraciones++;
            if (equipos[i]->getPais() == pais)
            {
                Lista<Jugador*>& plantilla = equipos[i]->getPlantilla();
                for (int j = 0; j < plantilla.getTamanio(); j++)
                {
                    iteraciones++;
                    if (plantilla[j]->getNumCamiseta() == camiseta)
                    {
                        EstadisticasJugador& st = plantilla[j]->getStatsHist();
                        st.goles=goles; st.partJugados=parts;
                        st.minutosJugados=mins; st.asistencias=asis;
                        st.tarjAmarillas=ama; st.tarjRojas=roj;
                        st.faltas=falt;
                        break;
                    }
                }
                break;
            }
        }
    }

    archivo.close();
    cout << "Estadisticas de jugadores cargadas correctamente." << endl;
    return true;
}

long GestorArchivos::memoriaBytes() const
{
    return sizeof(GestorArchivos) + rutaCSV.size() + rutaJugadores.size();
}
