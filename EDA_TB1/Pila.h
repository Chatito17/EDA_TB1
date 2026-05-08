#pragma once
class Nodo {
public:
	int dato;
	Nodo* siguiente;
	Nodo(int valor) {
		dato = valor;
		siguiente = nullptr;
	}
};

class Pila {
private:
	Nodo* cima;
public:
	Pila() {
		cima = nullptr;
	}

	void push(int valor) {
		Nodo* nuevo = new Nodo(valor);
		nuevo->siguiente = cima;
		cima = nuevo;
	}

	void pop() {
		if (cima == nullptr) return;
		Nodo* temp = cima;
		cima = cima->siguiente;
		delete temp;
	}

	int top() {
		return cima ? cima->dato : -1;
	}

	bool isEmpty() {
		return cima == nullptr;
	}
};
