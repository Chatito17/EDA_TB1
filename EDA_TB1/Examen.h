#pragma once
#include "Pregunta.h"
#include "Pila.h"
#include "ResultadoDetallado.h"
#include "Usuario.h" 
#include <string>
#include <iostream>

class Examen {
protected:
    int correctos;
    int incorrectos;
    int cantPreguntas;
    int multiplicadorExp;
    int recomGemas;

    ResultadoDetallado ejecutarRonda( Pila<Pregunta*>& pila, const std::string& nombreExamen, Usuario* user)
    {
        correctos = 0;
        incorrectos = 0;

        ResultadoDetallado resultado(nombreExamen, cantPreguntas);

        std::cout << "\n========================================\n";
        std::cout << "  " << nombreExamen << "\n";
        std::cout << "  Preguntas: " << cantPreguntas << "\n";
        std::cout << "========================================\n\n";

        int num = 1;
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
                user->restarVida();
                std::cout << "  -> Pierdes un corazon. Vidas restantes: " << user->getVidas() << "\n\n";
                incorrectos++;
            }

            resultado.registrarRespuesta(p->getEnunciado(), respuesta, p->getRespuestaCorrecta(), ok);
        }

        if (user->getVidas() <= 0) {
            std::cout << "\nTe has quedado sin vidas. Examen abortado.\n\n";
        }

        return resultado;
    }

    void otorgarRecompensas(Usuario* user, int puntaje, bool aprobado) {
        int expGanada = puntaje * multiplicadorExp;
        if (expGanada > 0) {
            user->sumarExp(expGanada);
            std::cout << "Has ganado " << expGanada << " de EXP!\n";
        }

        if (aprobado) {
            user->sumarGemas(recomGemas);
            std::cout << "Has ganado " << recomGemas << " gemas por aprobar!\n";
        }
    }

public:
    explicit Examen(int cantPreguntas, int multExp, int rGemas)
        : correctos(0), incorrectos(0),
        cantPreguntas(cantPreguntas), multiplicadorExp(multExp), recomGemas(rGemas) {
    }

    virtual ~Examen() {}

    virtual ResultadoDetallado hacerExamen(Pila<Pregunta*>& preguntas, Usuario* user) = 0;
    virtual std::string getNombre() const = 0;

    int getCantPreguntas() const { return cantPreguntas; }
    int getMultiplicadorExp() const { return multiplicadorExp; }
    int getRecomGemas() const { return recomGemas; }
};

class ExamenCertificado : public Examen {
public:
    ExamenCertificado() : Examen(10, 25, 100) {}
    std::string getNombre() const override { return "Examen Certificado"; }

    ResultadoDetallado hacerExamen(Pila<Pregunta*>& preguntas, Usuario* user) override {
        ResultadoDetallado res = ejecutarRonda(preguntas, getNombre(), user);

        double porcentaje = (double)res.getPuntaje() / cantPreguntas * 100.0;
        bool aprobado = (porcentaje >= 70.0 && user->getVidas() > 0);

        if (aprobado) {
            std::cout << "\n=== CERTIFICADO EMITIDO CON EXITO ===\n";
        }
        else if (user->getVidas() > 0) {
            std::cout << "\nNo lograste el puntaje minimo del 70%. ¡Sigue intentando!\n";
        }

        otorgarRecompensas(user, res.getPuntaje(), aprobado);
        return res;
    }
};

class ExamenEtapa : public Examen {
public:
    ExamenEtapa() : Examen(7, 20, 70) {}
    std::string getNombre() const override { return "Examen de Etapa"; }

    ResultadoDetallado hacerExamen(Pila<Pregunta*>& preguntas, Usuario* user) override {
        ResultadoDetallado res = ejecutarRonda(preguntas, getNombre(), user);

        double porcentaje = (double)res.getPuntaje() / cantPreguntas * 100.0;
        bool aprobado = (porcentaje >= 60.0 && user->getVidas() > 0);

        if (aprobado) {
            std::cout << "\nETAPA SUPERADA. Has saltado secciones completas.\n";
        }
        else if (user->getVidas() > 0) {
            std::cout << "\nNo lograste el puntaje minimo del 60%. ¡Sigue intentando!\n";
        }

        otorgarRecompensas(user, res.getPuntaje(), aprobado);
        return res;
    }
};

class ExamenNivel : public Examen {
public:
    ExamenNivel() : Examen(5, 15, 50) {}
    std::string getNombre() const override { return "Examen de Nivel"; }

    ResultadoDetallado hacerExamen(Pila<Pregunta*>& preguntas, Usuario* user) override {
        ResultadoDetallado res = ejecutarRonda(preguntas, getNombre(), user);

        int mitad = (cantPreguntas / 2) + 1;
        bool aprobado = (res.getPuntaje() >= mitad && user->getVidas() > 0);

        if (aprobado) {
            std::cout << "\nPRUEBA SUPERADA\n";
            user->avanzarNivel();
        }
        else if (user->getVidas() > 0) {
            std::cout << "\nNo lograste el puntaje minimo. ¡Sigue intentando!\n";
        }

        otorgarRecompensas(user, res.getPuntaje(), aprobado);
        return res;
    }
};