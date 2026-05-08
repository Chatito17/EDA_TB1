#ifndef PREGUNTA_H
#define PREGUNTA_H
#pragma once

#include <string>
#include <algorithm>
#include <cctype>
#include <iostream>

// ============================================================
// Clase Base Abstracta: Pregunta
// Contiene el enunciado y la respuesta correcta.
// Las clases hijas implementan comprobar() y mostrar().
// ============================================================
class Pregunta {
protected:
    std::string enunciado;
    std::string respuestaCorrecta;

public:
    Pregunta(const std::string& enunciado, const std::string& respuesta)
        : enunciado(enunciado), respuestaCorrecta(respuesta) {
    }

    virtual ~Pregunta() {}

    // Método virtual puro -> clase abstracta (no instanciable)
    virtual bool comprobar(std::string entrada) = 0;
    virtual void mostrar() const = 0;

    std::string getEnunciado()        const { return enunciado; }
    std::string getRespuestaCorrecta() const { return respuestaCorrecta; }
};


// ============================================================
// PreguntaEscribir
// El usuario escribe una traducción o palabra directamente.
// Integra Lambda 1: normaliza la entrada antes de comparar
// (quita espacios extremos y convierte a minúsculas).
// ============================================================
class PreguntaEscribir : public Pregunta {
public:
    PreguntaEscribir(const std::string& enunciado,
        const std::string& respuesta)
        : Pregunta(enunciado, respuesta) {
    }

    // Complejidad: O(n), n = longitud de la cadena
    bool comprobar(std::string entrada) override {
        // Lambda 1: trim de extremos + lowercase
        auto normalizar = [](std::string s) -> std::string {
            size_t ini = s.find_first_not_of(" \t");
            if (ini == std::string::npos) return "";
            size_t fin = s.find_last_not_of(" \t");
            s = s.substr(ini, fin - ini + 1);
            std::transform(s.begin(), s.end(), s.begin(),
                [](unsigned char c) { return std::tolower(c); });
            return s;
            };
        return normalizar(entrada) == normalizar(respuestaCorrecta);
    }

    void mostrar() const override {
        std::cout << "  [Escribir] " << enunciado << std::endl;
    }
};


// ============================================================
// PreguntaRelacionar
// Presenta N conceptos y N definiciones para emparejar.
// Se usan arreglos dinámicos (sin vector) para manejar los pares.
// El usuario escribe el número de la definición que corresponde
// al concepto señalado en el enunciado.
// ============================================================
class PreguntaRelacionar : public Pregunta {
private:
    std::string* conceptos;    // arreglo dinámico columna izquierda
    std::string* definiciones; // arreglo dinámico columna derecha
    int          cantidad;     // número de pares

public:
    // Constructor: recibe arreglos y los copia internamente
    PreguntaRelacionar(const std::string& enunciado,
        const std::string& respuesta,
        const std::string* conceptos,
        const std::string* definiciones,
        int cantidad)
        : Pregunta(enunciado, respuesta), cantidad(cantidad)
    {
        // Copia profunda: no dependemos de arreglos externos
        this->conceptos = new std::string[cantidad];
        this->definiciones = new std::string[cantidad];
        for (int i = 0; i < cantidad; i++) {
            this->conceptos[i] = conceptos[i];
            this->definiciones[i] = definiciones[i];
        }
    }

    // Destructor: libera los arreglos dinámicos
    ~PreguntaRelacionar() override {
        delete[] conceptos;
        delete[] definiciones;
    }

    // Complejidad: O(1) — solo compara el índice ingresado
    bool comprobar(std::string entrada) override {
        return entrada == respuestaCorrecta;
    }

    void mostrar() const override {
        std::cout << "  [Relacionar] " << enunciado << std::endl;
        std::cout << "  Concepto  |  Opciones de definicion" << std::endl;
        for (int i = 0; i < cantidad; i++) {
            std::cout << "    " << (i + 1) << ". "
                << conceptos[i] << "   -->   "
                << definiciones[i] << std::endl;
        }
        std::cout << "  Escribe el numero correcto: ";
    }

    int getCantidad() const { return cantidad; }
};

#endif // PREGUNTA_H