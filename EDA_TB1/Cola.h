#pragma once
#include <iostream>
#include <functional>

template <typename T>
class NodoCola {
public:
    T dato;
    NodoCola<T>* siguiente;
    NodoCola(T valor) : dato(valor), siguiente(nullptr) {}
};

template <typename T>
class Cola {
private:
    NodoCola<T>* frente;
    NodoCola<T>* final;
    int longitud;

public:
    Cola() : frente(nullptr), final(nullptr), longitud(0) {}

    ~Cola() {
        while (!estaVacia()) {
            desencolar();
        }
    }

    // Método 1: Encolar
    void encolar(T valor) {
        NodoCola<T>* nuevo = new NodoCola<T>(valor);
        if (estaVacia()) {
            frente = final = nuevo;
        }
        else {
            final->siguiente = nuevo;
            final = nuevo;
        }
        longitud++;
    }

    // Método 2: Desencolar
    T desencolar() {
        if (estaVacia()) return T();
        NodoCola<T>* temp = frente;
        T valor = temp->dato;
        frente = frente->siguiente;
        if (frente == nullptr) final = nullptr;
        delete temp;
        longitud--;
        return valor;
    }

    bool estaVacia() const { return frente == nullptr; }

    // Método 3 (Criterio B y D): Procesar con Lambda
    void procesarTodos(std::function<void(T)> accion) {
        NodoCola<T>* actual = frente;
        while (actual != nullptr) {
            accion(actual->dato);
            actual = actual->siguiente;
        }
    }
};