#pragma once
#include <string>
#include "LinkedList.h"
#include "Etapa.h"

class Curso {
private:
    std::string idioma;
    LinkedList<Etapa*> etapas;

public:
    Curso(std::string _idioma) : idioma(_idioma) {}

    ~Curso() {
        for (int i = 0; i < etapas.getLongitud(); i++) {
            delete etapas.GetPos(i);
        }
    }

    std::string getIdioma() { return idioma; }

    void agregarEtapa(Etapa* e) {
        etapas.AddLast(e);
    }

    Etapa* getEtapa(int index) {
        return etapas.GetPos(index);
    }

    int getCantidadEtapas() { return etapas.  getLongitud(); }
};