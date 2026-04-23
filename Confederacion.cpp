#include "Confederacion.h"

Confederacion::Confederacion() : nombre(""), codigo("")
{

}

Confederacion::Confederacion(string nombre, string codigo)
    : nombre(nombre), codigo(codigo)
{

}

Confederacion::Confederacion(const Confederacion& otra)
    : nombre(otra.nombre), codigo(otra.codigo)
{

}

string Confederacion::getNombre() const
{
    return nombre;
}
string Confederacion::getCodigo() const
{
    return codigo;
}

void Confederacion::setNombre(string n)
{
    nombre = n;
}
void Confederacion::setCodigo(string c)
{
    codigo = c;
}

bool Confederacion::operator==(const Confederacion& otra)
    const
{
    return codigo == otra.codigo;
}
