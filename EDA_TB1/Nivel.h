#pragma once
#include <string>
#include "BancoPreguntas.h"

class Nivel {
private:
    std::string nombreNivel;
    BancoPreguntas* banco; // Preguntas exclusivas de este nivel

public:
    Nivel(std::string nombre) : nombreNivel(nombre) {
        banco = new BancoPreguntas();
    }

    ~Nivel() {
        delete banco; // El BancoPreguntas ya destruye las Preguntas internamente
    }

    std::string getNombre() { return nombreNivel; }

    void agregarPregunta(Pregunta* p) {
        banco->agregar(p);
    }

    BancoPreguntas* getBancoPreguntas() {
        return banco;
    }
};