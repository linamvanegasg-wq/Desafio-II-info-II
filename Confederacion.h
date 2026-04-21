#ifndef CONFEDERACION_H
#define CONFEDERACION_H
#include <string>
using namespace std;

class Confederacion
{
private:
    string nombre;  // Ej: "UEFA", "CONMEBOL"
    string codigo;  // Ej: "UEFA", "CAF"

public:
    // Arreglo dimanico
    Confederacion();

    // parámetros
    Confederacion(string nombre, string codigo);

    // copia
    Confederacion(const Confederacion& otra);

    // Getters
    string getNombre() const;
    string getCodigo() const;

    // Setters
    void setNombre(string nombre);
    void setCodigo(string codigo);

    // Comparación de confederaciones
    bool operator==(const Confederacion& otra) const;
};

#endif
