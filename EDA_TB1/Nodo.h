#pragma once
template <typename T>
class Nodo {
public:
	T dato;
	Nodo<T>* siguiente;
	Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};
