#pragma once
#include <functional>
template <typename T>
class NodoPila {
public:
    T dato;
    NodoPila<T>* siguiente;
    NodoPila(T valor) : dato(valor), siguiente(nullptr) {}
};

template <typename T>
class Pila {
private:
    NodoPila<T>* cima;

    void vaciarRecursivo(NodoPila<T>* nodo) {
        if (nodo == nullptr) return; // Caso base
        vaciarRecursivo(nodo->siguiente); // Llamada recursiva hasta el final
        delete nodo; // Se elimina de abajo hacia arriba
    }
public:
    Pila() : cima(nullptr) {}

    ~Pila() {
        vaciarRecursivo(cima);
        cima = nullptr;
    }

    void push(T valor) {
        NodoPila<T>* nuevo = new NodoPila<T>(valor);
        nuevo->siguiente = cima;
        cima = nuevo;
    }

    // pop() ahora debe retornar el dato para que Pregunta* p = pila.pop() funcione
    T pop() {
        if (cima == nullptr) return T();
        NodoPila<T>* temp = cima;
        T valor = temp->dato;
        cima = cima->siguiente;
        delete temp;
        return valor;
    }

    T top() {
        return cima ? cima->dato : T();
    }

    // Se renombra para coincidir con Examen.h
    bool estaVacia() {
        return cima == nullptr;
    }

    // Método extra 1: Invertir la pila
    void invertir() {
        if (cima == nullptr || cima->siguiente == nullptr) return;
        NodoPila<T>* previo = nullptr;
        NodoPila<T>* actual = cima;
        NodoPila<T>* siguiente = nullptr;
        while (actual != nullptr) {
            siguiente = actual->siguiente;
            actual->siguiente = previo;
            previo = actual;
            actual = siguiente;
        }
        cima = previo;
    }

    // Método extra 2: Filtrar con Lambda
    Pila<T> filtrar(std::function<bool(T)> criterio) {
        Pila<T> resultado;
        Pila<T> temp;

        NodoPila<T>* actual = cima;
        while (actual != nullptr) {
            if (criterio(actual->dato)) {
                temp.push(actual->dato); // Apila temporalmente
            }
            actual = actual->siguiente;
        }
        // Restaurar orden original en el resultado
        while (!temp.estaVacia()) {
            resultado.push(temp.pop());
        }
        return resultado;
    }
};