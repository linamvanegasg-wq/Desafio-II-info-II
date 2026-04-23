#ifndef ARBITRO_H
#define ARBITRO_H
#include <string>
using namespace std;

class Arbitro
{
private:
    string codigo;
    string nombre;

public:
    Arbitro();
    Arbitro(string codigo, string nombre);
    Arbitro(const Arbitro& otro);

    string getCodigo() const;
    string getNombre() const;

    void setCodigo(string codigo);
    void setNombre(string nombre);

    bool operator==(const Arbitro& otro) const;

    long memoriaBytes() const;
};

#endif
