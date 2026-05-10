#pragma once
#include "Pregunta.h"
#include "Pila.h"
#include "LinkedList.h"

#include <string>
#include <fstream>
#include <sstream>
#include <functional>
#include <cstdlib>
#include <ctime>
#include <iostream>

class BancoPreguntas {
private:
    LinkedList<Pregunta*> preguntas;

public:
    BancoPreguntas() {
        std::srand(std::time(0));
    }

    // Destructor: libera cada pregunta y el arreglo - O(n)
    ~BancoPreguntas() {
        for (int i = 0; i < preguntas.getLongitud(); i++) {
            Pregunta* p = preguntas.GetPos(i);
            if (p != nullptr) {
                delete p;
            }
        }
    }

    // Agrega una pregunta; el banco toma posesión del puntero - amortizado O(1)
    void agregar(Pregunta* p) {
        preguntas.AddLast(p);
    }
    int getCantidad() {
        return preguntas.getLongitud();
    }

    // ================================================================
    // seleccionarParaExamen — integra Lambda 2
    //
    // Filtra con el criterio recibido (lambda), baraja los aptos con
    // Fisher-Yates sobre un arreglo dinámico temporal (sin vector) y
    // retorna una Pila<Pregunta*> con exactamente 'n' preguntas.
    //
    // Complejidad: O(total) filtrar + O(aptos) barajar
    // ================================================================
    Pila<Pregunta*> seleccionarParaExamen(int n, std::function<bool(Pregunta*)> criterio) const
    {
        LinkedList<Pregunta*> aptas;
        for (int i = 0; i < preguntas.getLongitud(); i++) {
            Pregunta* p = preguntas.GetPos(i);
            if (criterio(p)) {
                aptas.AddLast(p); // Apuntamos las que cumplen
            }
        }

        int cantAptas = aptas.getLongitud();
        if (cantAptas < n) {
            std::cout << "[BancoPreguntas] Solo hay " << cantAptas
                << " preguntas aptas; se pedian " << n
                << ". Usando " << cantAptas << ".\n";
            n = cantAptas;
        }

        Pila<Pregunta*> pila;

        // 2. Extraer aleatoriamente directo de la LinkedList temporal a la Pila
        while (n > 0 && !aptas.estaVacia()) {
            int randomIndex = std::rand() % aptas.getLongitud();
            Pregunta* p = aptas.GetPos(randomIndex);

            pila.push(p);                  // La metemos al examen
            aptas.RemovePos(randomIndex);  // La quitamos para no repetirla en la selección
            n--;
        }

        return pila;
    }

    // Debug: muestra todo el banco - O(n)
    void mostrarTodo() {
        std::cout << "=== Banco (" << preguntas.getLongitud() << " preguntas) ===" << std::endl;
        for (int i = 0; i < preguntas.getLongitud(); i++) {
            std::cout << (i + 1) << ". ";
            preguntas.GetPos(i)->mostrar();
        }
    }
    void cargarDesdeArchivo(const std::string& rutaArchivo) {
        std::ifstream archivo(rutaArchivo);
        if (!archivo.is_open()) {
            std::cout << "[!] Error: No se encontro el archivo de preguntas (" << rutaArchivo << ").\n";
            return;
        }

        std::string linea;
        while (std::getline(archivo, linea)) {
            // Formato esperado en el txt: Enunciado|RespuestaCorrecta
            size_t posDivisor = linea.find('|');
            if (posDivisor != std::string::npos) {
                std::string enunciado = linea.substr(0, posDivisor);
                std::string respuesta = linea.substr(posDivisor + 1);

                // Agregamos dinámicamente al banco
                agregar(new PreguntaEscribir(enunciado, respuesta));
            }
        }
        archivo.close();
    }
};
