#include "Arbitro.h"

Arbitro::Arbitro() : codigo(""), nombre("")
{

}

Arbitro::Arbitro(string codigo, string nombre)
    : codigo(codigo), nombre(nombre)
{

}

Arbitro::Arbitro(const Arbitro& otro)
    : codigo(otro.codigo), nombre(otro.nombre)
{

}

string Arbitro::getCodigo() const
{
    return codigo;
}
string Arbitro::getNombre() const
{
    return nombre;
}

void Arbitro::setCodigo(string c)
{
    codigo = c;
}
void Arbitro::setNombre(string n)
{
    nombre = n;
}

bool Arbitro::operator==(const Arbitro& otro) const
{
    return codigo == otro.codigo;
}

long Arbitro::memoriaBytes() const
{
    return sizeof(Arbitro) + codigo.size() + nombre.size();
}
