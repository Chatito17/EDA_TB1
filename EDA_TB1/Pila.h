#pragma once

template <typename T>
class Nodo {
public:
	T dato;
	Nodo<T>* siguiente;
	Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};

template <typename T>
class Pila {
private:
	Nodo<T>* cima;
public:
	Pila() : cima(nullptr) {}

	void push(T valor) {
		Nodo<T>* nuevo = new Nodo<T>(valor);
		nuevo->siguiente = cima;
		cima = nuevo;
	}

	T pop() {
		if (cima == nullptr) return T();
		Nodo<T>* temp = cima;
		T valor = temp->dato;
		cima = cima->siguiente;
		delete temp;
		return valor;
	}

	T top() {
		return cima ? cima->dato : T();
	}

	bool isEmpty() {
		return cima == nullptr;
	}
};