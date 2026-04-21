#include "Estadisticas.h"


EstadisticasEquipo::EstadisticasEquipo()
    : golesAFavor(0), golesEnContra(0), partGanados(0),
      partEmpatados(0), partPerdidos(0),
      tarjAmarillas(0), tarjRojas(0), faltas(0)
{

}

EstadisticasEquipo::EstadisticasEquipo(int gf, int gc, int pg, int pe, int pp)
    : golesAFavor(gf), golesEnContra(gc), partGanados(pg),
      partEmpatados(pe), partPerdidos(pp),
      tarjAmarillas(0), tarjRojas(0), faltas(0)
{

}

EstadisticasEquipo::EstadisticasEquipo(const EstadisticasEquipo& o)
    : golesAFavor(o.golesAFavor), golesEnContra(o.golesEnContra),
      partGanados(o.partGanados), partEmpatados(o.partEmpatados),
      partPerdidos(o.partPerdidos), tarjAmarillas(o.tarjAmarillas),
      tarjRojas(o.tarjRojas), faltas(o.faltas)
{

}

EstadisticasEquipo& EstadisticasEquipo::operator+=(const EstadisticasEquipo& o)
{
    golesAFavor    += o.golesAFavor;
    golesEnContra  += o.golesEnContra;
    partGanados    += o.partGanados;
    partEmpatados  += o.partEmpatados;
    partPerdidos   += o.partPerdidos;
    tarjAmarillas  += o.tarjAmarillas;
    tarjRojas      += o.tarjRojas;
    faltas         += o.faltas;
    return *this;
}

float EstadisticasEquipo::promedioGF() const
{
    int pj = partJugados();
    if (pj == 0) return 0.0f;
    return (float)golesAFavor / pj;
}

float EstadisticasEquipo::promedioGC() const
{
    int pj = partJugados();
    if (pj == 0) return 0.0f;
    return (float)golesEnContra / pj;
}

int EstadisticasEquipo::difGoles() const
{
    return golesAFavor - golesEnContra;
}

int EstadisticasEquipo::partJugados() const
{
    return partGanados + partEmpatados + partPerdidos;
}

long EstadisticasEquipo::memoriaBytes() const
{
    return sizeof(EstadisticasEquipo);
}

//EstadisticasJugador

EstadisticasJugador::EstadisticasJugador()
    : partJugados(0), goles(0), minutosJugados(0),
      asistencias(0), tarjAmarillas(0), tarjRojas(0), faltas(0)
{

}

EstadisticasJugador::EstadisticasJugador(const EstadisticasJugador& o)
    : partJugados(o.partJugados), goles(o.goles),
      minutosJugados(o.minutosJugados), asistencias(o.asistencias),
      tarjAmarillas(o.tarjAmarillas), tarjRojas(o.tarjRojas),
      faltas(o.faltas)
{

}

EstadisticasJugador& EstadisticasJugador::operator+=(const EstadisticasJugador& o)
{
    partJugados    += o.partJugados;
    goles          += o.goles;
    minutosJugados += o.minutosJugados;
    asistencias    += o.asistencias;
    tarjAmarillas  += o.tarjAmarillas;
    tarjRojas      += o.tarjRojas;
    faltas         += o.faltas;
    return *this;
}

long EstadisticasJugador::memoriaBytes() const
{
    return sizeof(EstadisticasJugador);
}
