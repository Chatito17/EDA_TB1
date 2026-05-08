#pragma once

#include "Pregunta.h"
#include "Pila.h"
#include <string>
#include <fstream>
#include <sstream>
#include <functional>
#include <cstdlib>
#include <ctime>
#include <iostream>

class BancoPreguntas {
private:
    Pregunta** preguntas;   // arreglo dinámico de punteros
    int cantidad;    // preguntas cargadas
    int capacidad;   // tamaño actual del arreglo

    // Duplica capacidad cuando el arreglo se llena - O(n)
    void expandir() {
        int nuevaCap = capacidad * 2;
        Pregunta** nuevo = new Pregunta * [nuevaCap];
        for (int i = 0; i < cantidad; i++) nuevo[i] = preguntas[i];
        delete[] preguntas;
        preguntas = nuevo;
        capacidad = nuevaCap;
    }

public:
    BancoPreguntas() : cantidad(0), capacidad(16) {
        preguntas = new Pregunta * [capacidad];
        std::srand((unsigned)std::time(nullptr));
    }

    // Destructor: libera cada pregunta y el arreglo - O(n)
    ~BancoPreguntas() {
        for (int i = 0; i < cantidad; i++) delete preguntas[i];
        delete[] preguntas;
    }

    // Agrega una pregunta; el banco toma posesión del puntero - amortizado O(1)
    void agregar(Pregunta* p) {
        if (cantidad == capacidad) expandir();
        preguntas[cantidad++] = p;
    }

    int getCantidad() const { return cantidad; }

    // ================================================================
    // seleccionarParaExamen — integra Lambda 2
    //
    // Filtra con el criterio recibido (lambda), baraja los aptos con
    // Fisher-Yates sobre un arreglo dinámico temporal (sin vector) y
    // retorna una Pila<Pregunta*> con exactamente 'n' preguntas.
    //
    // Complejidad: O(total) filtrar + O(aptos) barajar
    // ================================================================
    Pila<Pregunta*> seleccionarParaExamen(
        int n,
        std::function<bool(Pregunta*)> criterio) const
    {
        // --- Lambda 2 en acción: filtra preguntas aptas ---
        // Arreglo dinámico temporal (sin vector)
        Pregunta** aptas = new Pregunta * [cantidad];
        int cantAptas = 0;

        for (int i = 0; i < cantidad; i++) {
            if (criterio(preguntas[i]))
                aptas[cantAptas++] = preguntas[i];
        }

        if (cantAptas < n) {
            std::cout << "[BancoPreguntas] Solo hay " << cantAptas
                << " preguntas aptas; se pedian " << n
                << ". Usando " << cantAptas << ".\n";
            n = cantAptas;
        }

        // Barajar Fisher-Yates sobre aptas[]
        for (int i = cantAptas - 1; i > 0; i--) {
            int j = std::rand() % (i + 1);
            Pregunta* tmp = aptas[i]; aptas[i] = aptas[j]; aptas[j] = tmp;
        }

        // Apilar las primeras n aptas
        Pila<Pregunta*> pila;
        for (int i = 0; i < n; i++) pila.push(aptas[i]);

        delete[] aptas;
        return pila;
    }

    // Debug: muestra todo el banco - O(n)
    void mostrarTodo() const {
        std::cout << "=== Banco (" << cantidad << " preguntas) ===" << std::endl;
        for (int i = 0; i < cantidad; i++) {
            std::cout << (i + 1) << ". ";
            preguntas[i]->mostrar();
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
