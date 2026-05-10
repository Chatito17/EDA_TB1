#pragma once

#include <string>
#include <algorithm>
#include <iostream>
#include "LinkedList.h"

class Pregunta {
protected:
    std::string enunciado;
    std::string respuestaCorrecta;

public:
    Pregunta(std::string enunciado, std::string respuesta) {
        this->enunciado = enunciado;
        this->respuestaCorrecta = respuesta;
    }

    virtual ~Pregunta() {}

    virtual bool comprobar(std::string entrada) = 0;
    virtual void mostrar() = 0;

    std::string getEnunciado() { return enunciado; }
    std::string getRespuestaCorrecta() { return respuestaCorrecta; }
};

class PreguntaEscribir : public Pregunta {
public:
    PreguntaEscribir(std::string enunciado, std::string respuesta)
        : Pregunta(enunciado, respuesta) {
    }

    bool comprobar(std::string entrada) override {
      
        // Uso de LAMBDA
        auto normalizar = [](std::string s) -> std::string {
            size_t ini = s.find_first_not_of(" \t\r\n");
            if (ini == std::string::npos) return "";
            size_t fin = s.find_last_not_of(" \t\r\n");
            s = s.substr(ini, fin - ini + 1);
            std::transform(s.begin(), s.end(), s.begin(),
                [](unsigned char c) { return std::tolower(c); });
            return s;
            };
        return normalizar(entrada) == normalizar(respuestaCorrecta);
    }

    void mostrar() override {
        std::cout << "  [Escribir] " << enunciado << std::endl;
    }
};

struct ParRelacion {
    std::string concepto;
    std::string definicion;

    ParRelacion(std::string c, std::string d) {
        concepto = c;
        definicion = d;
    }
};

class PreguntaRelacionar : public Pregunta {
private:
    LinkedList<ParRelacion*> listaPares;

public:
    PreguntaRelacionar(std::string enunciado, std::string respuesta)
        : Pregunta(enunciado, respuesta) {
    }

    ~PreguntaRelacionar() override {
        for (int i = 0; i < listaPares.getLongitud(); i++) {
            delete listaPares.GetPos(i);
        }
    }

    void agregarPar(std::string concepto, std::string definicion) {
        ParRelacion* nuevoPar = new ParRelacion(concepto, definicion);
        listaPares.AddLast(nuevoPar);
    }

    bool comprobar(std::string entrada) override {
        return entrada == respuestaCorrecta;
    }

    void mostrar() override {
        std::cout << "  [Relacionar] " << enunciado << std::endl;
        std::cout << "  Concepto  |  Opciones de definicion" << std::endl;

        for (int i = 0; i < listaPares.getLongitud(); i++) {
            ParRelacion* par = listaPares.GetPos(i);
            std::cout << "    " << (i + 1) << ". "
                << par->concepto << "   -->   "
                << par->definicion << std::endl;
        }
        std::cout << "  Escribe la secuencia correcta: ";
    }

    int getCantidad() { return listaPares.getLongitud(); }
};