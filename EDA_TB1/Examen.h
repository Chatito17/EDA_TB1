#pragma once

#include "Pregunta.h"
#include "Pila.h"
#include "ResultadoDetallado.h"
#include "Usuario.h" 
#include <string>
#include <iostream>


// ============================================================
// Clase Base Abstracta: Examen
// Contiene la lógica común de tiempo y puntaje.
// Cada hijo define cantPreguntas y el comportamiento especial
// de su tipo (certificado, etapa o nivel).
// ============================================================
class Examen {
protected:
    int correctos;
    int incorrectos;
    int cantPreguntas;
    int multiplicadorExp; // NUEVO: Encapsulamiento
    int recomGemas;

    // ---- Lógica de ejecución compartida entre hijos ----
    // Extrae preguntas de la pila, pregunta una por una y
    // registra cada respuesta en el ResultadoDetallado.
    // Complejidad: O(n) donde n = cantPreguntas
    ResultadoDetallado ejecutarRonda(
        Pila<Pregunta*>& pila,
        const std::string& nombreExamen,
        Usuario* user)
    {
        correctos = 0;
        incorrectos = 0;

        ResultadoDetallado resultado(nombreExamen, cantPreguntas);

        std::cout << "\n========================================\n";
        std::cout << "  " << nombreExamen << "\n";
        std::cout << "  Preguntas: " << cantPreguntas << "\n";
        std::cout << "========================================\n\n";

        int num = 1;
        // El examen termina si se acaban las preguntas o si las vidas llegan a 0
        while (!pila.estaVacia() && user->getVidas() > 0) {
            Pregunta* p = pila.pop();

            std::cout << "Pregunta " << num++ << "/" << cantPreguntas << ":\n";
            p->mostrar();

            std::string respuesta;
            std::getline(std::cin, respuesta);

            bool ok = p->comprobar(respuesta);

            if (ok) {
                std::cout << "  -> Correcto!\n\n";
                correctos++;
            }
            else {
                std::cout << "  -> Incorrecto. Respuesta: " << p->getRespuestaCorrecta() << "\n";
                user->restarVida(); // Penalización real
                std::cout << "  -> Pierdes un corazon. Vidas restantes: " << user->getVidas() << "\n\n";
                incorrectos++;
            }

            resultado.registrarRespuesta(p->getEnunciado(), respuesta, p->getRespuestaCorrecta(), ok);
        }

        if (user->getVidas() <= 0) {
            std::cout << "\n[!] Te has quedado sin vidas. Examen abortado.\n\n";
        }

        return resultado;
    }

public:
    explicit Examen(int cantPreguntas, int multExp, int rGemas)
        : correctos(0), incorrectos(0),
        cantPreguntas(cantPreguntas), multiplicadorExp(multExp), recomGemas(rGemas) {
    }

    virtual ~Examen() {}

    // El método hacerExamen ahora debe recibir al Usuario
    virtual ResultadoDetallado hacerExamen(Pila<Pregunta*>& preguntas, Usuario* user) = 0;
    virtual std::string getNombre() const = 0;

    int getCantPreguntas() const { return cantPreguntas; }
    int getMultiplicadorExp() const { return multiplicadorExp; }
    int getRecomGemas() const { return recomGemas; }
};


// ============================================================
// ExamenCertificado — 10 preguntas
// Al aprobar (>=70%), emite un mensaje de certificado.
// Es el examen más exigente de la plataforma.
// ============================================================
class ExamenCertificado : public Examen {
public:
    // Parametros: 10 preguntas, Multiplicador x25, 100 gemas
    ExamenCertificado() : Examen(10, 25, 100) {}

    std::string getNombre() const override { return "Examen Certificado"; }

    ResultadoDetallado hacerExamen(Pila<Pregunta*>& preguntas, Usuario* user) override {
        ResultadoDetallado res = ejecutarRonda(preguntas, getNombre(), user);
        double porcentaje = (double)res.getPuntaje() / cantPreguntas * 100.0;
        if (porcentaje >= 70.0) {
            std::cout << "*** CERTIFICADO EMITIDO ***\n";
        }
        return res;
    }
};


// ============================================================
// ExamenEtapa — 7 preguntas
// Al aprobar (>=60%), desbloquea la siguiente etapa de la plataforma,
// permitiendo saltar secciones completas.
// ============================================================
class ExamenEtapa : public Examen {

public:
    // Parametros: 7 preguntas, Multiplicador x20, 70 gemas
    ExamenEtapa() : Examen(7, 20, 70) {}

    std::string getNombre() const override { return "Examen de Etapa"; }

    ResultadoDetallado hacerExamen(Pila<Pregunta*>& preguntas, Usuario* user) override {
        ResultadoDetallado res = ejecutarRonda(preguntas, getNombre(), user);
        double porcentaje = (double)res.getPuntaje() / cantPreguntas * 100.0;
        if (porcentaje >= 60.0) {
            std::cout << "ETAPA SUPERADA. Has saltado secciones completas.\n";
        }
        return res;
    }
};


// ============================================================
// ExamenNivel — 5 preguntas
// Al aprobar (>=60%), avanza al usuario a la siguiente lección.
// Son los exámenes rápidos del día a día.
// ============================================================

class ExamenNivel : public Examen {

public:
    // Parametros: 5 preguntas, Multiplicador x15, 50 gemas
    ExamenNivel()
        : Examen(5, 15, 50) {
    }

    std::string getNombre() const override { return "Examen de Nivel"; }

    ResultadoDetallado hacerExamen(Pila<Pregunta*>& preguntas, Usuario* user) override {
        ResultadoDetallado res = ejecutarRonda(preguntas, getNombre(), user);
        return res;
    }
};
