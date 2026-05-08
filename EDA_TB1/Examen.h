#pragma once
#ifndef EXAMEN_H
#define EXAMEN_H

#include "Pregunta.h"
#include "Pila.h"
#include "ResultadoDetallado.h"
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
    int tiempoSegundos;  // tiempo total empleado (simulado)
    int cantPreguntas;

    // ---- Lógica de ejecución compartida entre hijos ----
    // Extrae preguntas de la pila, pregunta una por una y
    // registra cada respuesta en el ResultadoDetallado.
    // Complejidad: O(n) donde n = cantPreguntas
    ResultadoDetallado ejecutarRonda(
        Pila<Pregunta*>& pila,
        const std::string& nombreExamen,
        const std::string& fecha)
    {
        correctos = 0;
        incorrectos = 0;

        ResultadoDetallado resultado(nombreExamen, cantPreguntas, fecha);

        std::cout << "\n========================================\n";
        std::cout << "  " << nombreExamen << "\n";
        std::cout << "  Preguntas: " << cantPreguntas << "\n";
        std::cout << "========================================\n\n";

        int num = 1;
        while (!pila.estaVacia()) {
            Pregunta* p = pila.pop();   // polimorfismo: pop() + dispatch

            std::cout << "Pregunta " << num++ << "/" << cantPreguntas << ":\n";
            p->mostrar();               // virtual: muestra según tipo

            std::string respuesta;
            std::getline(std::cin, respuesta);

            bool ok = p->comprobar(respuesta); // virtual dispatch

            if (ok) {
                std::cout << "  -> Correcto!\n\n";
                correctos++;
            }
            else {
                std::cout << "  -> Incorrecto. Respuesta: "
                    << p->getRespuestaCorrecta() << "\n\n";
                incorrectos++;
            }

            resultado.registrarRespuesta(
                p->getEnunciado(), respuesta,
                p->getRespuestaCorrecta(), ok);
        }

        std::cout << "----------------------------------------\n";
        std::cout << "  Resultado: "
            << correctos << "/" << cantPreguntas << "\n";
        std::cout << "----------------------------------------\n\n";

        return resultado;
    }

public:
    explicit Examen(int cantPreguntas)
        : correctos(0), incorrectos(0),
        tiempoSegundos(0), cantPreguntas(cantPreguntas) {
    }

    virtual ~Examen() {}

    // Método virtual puro: cada hijo implementa su lógica propia
    virtual ResultadoDetallado hacerExamen(Pila<Pregunta*>& preguntas) = 0;

    virtual std::string getNombre() const = 0;

    int getCorrectos()    const { return correctos; }
    int getIncorrectos()  const { return incorrectos; }
    int getCantPreguntas() const { return cantPreguntas; }
};


// ============================================================
// ExamenCertificado — 10 preguntas
// Al aprobar (>=70%), emite un mensaje de certificado.
// Es el examen más exigente de la plataforma.
// ============================================================
class ExamenCertificado : public Examen {
public:
    ExamenCertificado() : Examen(10) {}

    std::string getNombre() const override {
        return "Examen Certificado";
    }

    ResultadoDetallado hacerExamen(Pila<Pregunta*>& preguntas) override {
        ResultadoDetallado res = ejecutarRonda(preguntas, getNombre(), "2025-06-15");

        // Lógica exclusiva del certificado: emitir certificado si aprueba
        double porcentaje = (double)res.getPuntaje() / cantPreguntas * 100.0;
        if (porcentaje >= 70.0) {
            std::cout << "*** CERTIFICADO EMITIDO ***\n";
            std::cout << "Has superado el " << (int)porcentaje
                << "% y obtienes tu certificado de idioma.\n\n";
        }
        else {
            std::cout << "Puntaje insuficiente para certificado ("
                << (int)porcentaje << "%). Necesitas 70%.\n\n";
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
private:
    int etapaActual;  // etapa que se intenta desbloquear

public:
    explicit ExamenEtapa(int etapa = 1) : Examen(7), etapaActual(etapa) {}

    std::string getNombre() const override {
        return "Examen de Etapa";
    }

    ResultadoDetallado hacerExamen(Pila<Pregunta*>& preguntas) override {
        ResultadoDetallado res = ejecutarRonda(preguntas, getNombre(), "2025-06-15");

        // Lógica exclusiva: saltar a la siguiente etapa si aprueba
        double porcentaje = (double)res.getPuntaje() / cantPreguntas * 100.0;
        if (porcentaje >= 60.0) {
            std::cout << "ETAPA " << etapaActual << " SUPERADA.\n";
            std::cout << "Has saltado a la Etapa " << (etapaActual + 1)
                << " de la plataforma.\n\n";
        }
        else {
            std::cout << "No alcanzaste el 60% para saltar de etapa.\n\n";
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
private:
    std::string leccionActual;  // lección que se evalúa

public:
    explicit ExamenNivel(const std::string& leccion = "Leccion 1")
        : Examen(5), leccionActual(leccion) {
    }

    std::string getNombre() const override {
        return "Examen de Nivel";
    }

    ResultadoDetallado hacerExamen(Pila<Pregunta*>& preguntas) override {
        ResultadoDetallado res = ejecutarRonda(preguntas, getNombre(), "2025-06-15");

        // Lógica exclusiva: avanzar lección si aprueba
        double porcentaje = (double)res.getPuntaje() / cantPreguntas * 100.0;
        if (porcentaje >= 60.0) {
            std::cout << leccionActual << " completada.\n";
            std::cout << "Avanzas a la siguiente leccion.\n\n";
        }
        else {
            std::cout << "Repasa la " << leccionActual
                << " e intentalo de nuevo.\n\n";
        }
        return res;
    }
};

#endif // EXAMEN_H