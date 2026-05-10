#pragma once
#include "Pregunta.h"
#include "Pila.h"
#include "LinkedList.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <cstdlib> 
#include <ctime>

class BancoPreguntas {
private:
    LinkedList<Pregunta*> preguntas;

public:
    BancoPreguntas() {
        std::srand(std::time(0));
    }
    ~BancoPreguntas() {
        for (int i = 0; i < preguntas.getLongitud(); i++) {
            Pregunta* p = preguntas.GetPos(i);
            if (p != nullptr) {
                delete p;
            }
        }
    }
    void agregar(Pregunta* p) {
        preguntas.AddLast(p);
    }
    int getCantidad() {
        return preguntas.getLongitud();
    }

    //Uso de LAMBDA

    template<typename Criterio>
    Pila<Pregunta*> seleccionarParaExamen(int n, Criterio criterio)
    {
        LinkedList<Pregunta*> aptas;
        for (int i = 0; i < preguntas.getLongitud(); i++) {
            Pregunta* p = preguntas.GetPos(i);
            if (criterio(p)) {
                aptas.AddLast(p);
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


        while (n > 0 && !aptas.estaVacia()) {
            int randomIndi = std::rand() % aptas.getLongitud();
            Pregunta* p = aptas.GetPos(randomIndi);

            pila.push(p); 
            aptas.RemovePos(randomIndi); 
            n--;
        }

        return pila;
    }

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
            std::cout << "Error: No se encontro el archivo de preguntas (" << rutaArchivo << ").\n";
            return;
        }

        std::string linea;
        while (std::getline(archivo, linea)) {
            size_t posDivisor = linea.find('|');
            if (posDivisor != std::string::npos) {
                std::string enunciado = linea.substr(0, posDivisor);
                std::string respuesta = linea.substr(posDivisor + 1);

                agregar(new PreguntaEscribir(enunciado, respuesta));
            }
        }
        archivo.close();
    }
};
