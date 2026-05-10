#pragma once
#include <string>
#include "LinkedList.h"
#include "Nivel.h"

class Seccion {
private:
    std::string nombreSeccion;
    LinkedList<Nivel*> niveles;

public:
    Seccion(std::string nombre) : nombreSeccion(nombre) {}

    ~Seccion() {
        for (int i = 0; i < niveles.getLongitud(); i++) {
            delete niveles.GetPos(i);
        }
    }

    std::string getNombre() { return nombreSeccion; }

    void agregarNivel(Nivel* n) {
        niveles.AddLast(n);
    }

    Nivel* getNivel(int index) {
        return niveles.GetPos(index);
    }

    int getCantidadNiveles() { return niveles.getLongitud(); }
};