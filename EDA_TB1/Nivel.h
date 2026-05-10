#pragma once
#include <string>
#include "BancoPreguntas.h"

class Nivel {
private:
    std::string nombreNivel;
    BancoPreguntas* banco;

public:
    Nivel(std::string nombre) : nombreNivel(nombre) {
        banco = new BancoPreguntas();
    }

    ~Nivel() {
        delete banco;
    }

    std::string getNombre() { return nombreNivel; }

    void agregarPregunta(Pregunta* p) {
        banco->agregar(p);
    }

    BancoPreguntas* getBancoPreguntas() {
        return banco;
    }
};