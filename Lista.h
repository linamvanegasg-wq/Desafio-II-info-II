#ifndef LISTA_H
#define LISTA_H

template <typename T>
class Lista
{
private:
    T* Datos;
    int Capacidad;
    int Tamanio;

//Paso1: Cuando se llena, duplica el espacio

    void expandir()
    {
        Capacidad *= 2;
        T* nuevo = new T[Capacidad];
        for (int i = 0; i < Tamanio; i++)
        {
            nuevo[i] = Datos[i];
        }
        delete[] Datos;
        Datos = nuevo;
    }

public:
    // Constructor: capacidad inicial de 4 por defecto
    Lista(int CapInicial = 4)
    {
        Capacidad = CapInicial;
        Tamanio = 0;
        Datos = new T[Capacidad];
    }

    // Constructor de copia — requerido por el enunciado
    Lista(const Lista<T>& otra)
    {
        Capacidad = otra.Capacidad;
        Tamanio = otra.Tamanio;
        Datos = new T[Capacidad];
        for (int i = 0; i < Tamanio; i++)
        {
            Datos[i] = otra.Datos[i];
        }
    }

    // Destructor — libera la memoria del arreglo
    ~Lista()
    {
        delete[] Datos;
    }

    // Operador = (asignación por copia)
    Lista<T>& operator=(const Lista<T>& otra)
    {
        if (this != &otra)
        {
            delete[] Datos;
            Capacidad = otra.Capacidad;
            Tamanio = otra.Tamanio;
            Datos = new T[Capacidad];
            for (int i = 0; i < Tamanio; i++)
            {
                Datos[i] = otra.Datos[i];
            }
        }
        return *this;
    }

    // Paso 2: Agrega un elemento al final
    void agregar(T elemento)
    {
        if (Tamanio == Capacidad) expandir();
        Datos[Tamanio++] = elemento;
    }

    // Paso 3: Obtiene un elemento
    T& obtener(int i)
    {
        if (i < 0 || i >= Tamanio) return Datos[0];
        return Datos[i];
    }

    // Versión const para poder usarla en métodos const
    const T& obtener(int i) const
    {
        if (i < 0 || i >= Tamanio) return Datos[0];
        return Datos[i];
    }

    // Sobrecarga de [] para acceso más cómodo
    T& operator[](int i)
    {
        return obtener(i);
    }
    const T& operator[](int i) const
    {
        return obtener(i);
    }

    // Elimina el elemento en la posición i
    void eliminar(int i)
    {
        if (i < 0 || i >= Tamanio) return;
        // Desplaza los elementos hacia la izquierda
        for (int j = i; j < Tamanio - 1; j++)
        {
            Datos[j] = Datos[j + 1];
        }
        Tamanio--;
    }

    // Retorna cuántos elementos tiene actualmente
    int getTamanio() const
    {
        return Tamanio;
    }

    // Retorna la capacidad actual (útil para medir memoria)
    int getCapacidad() const
    {
        return Capacidad;
    }

    // Vacía la lista sin liberar la memoria del arreglo
    void vaciar()
    {
        Tamanio = 0;
    }

    // Calcula memoria usada por esta lista en bytes
    long memoriaBytes() const
    {
        return (long)(Capacidad * sizeof(T)) + sizeof(Lista<T>);
    }
};

#endif
