#pragma once
#include <string>
#include "LinkedList.h"
#include "Seccion.h"

class Etapa {
private:
    std::string nombreEtapa;
    LinkedList<Seccion*> secciones;

public:
    Etapa(std::string nombre) : nombreEtapa(nombre) {}

    ~Etapa() {
        for (int i = 0; i < secciones.Length(); i++) {
            delete secciones.GetPos(i);
        }
    }

    std::string getNombre() { return nombreEtapa; }

    void agregarSeccion(Seccion* s) {
        secciones.AddLast(s);
    }

    Seccion* getSeccion(int index) {
        return secciones.GetPos(index);
    }

    int getCantidadSecciones() { return secciones.Length(); }
};