#pragma once

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
public:
	Pila() : cima(nullptr) {}

	void push(T valor) {
		NodoPila<T>* nuevo = new NodoPila<T>(valor);
		nuevo->siguiente = cima;
		cima = nuevo;
	}

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

	bool estaVacia() {
		return cima == nullptr;
	}
};