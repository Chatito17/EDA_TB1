#pragma once

template <typename T>
class NodoList {
public:
	T dato;
	NodoList<T>* siguiente;
	NodoList(T valor) : dato(valor), siguiente(nullptr) {}

	NodoList(T valor, NodoList<T>* sig) : dato(valor), siguiente(sig) {}
};

template <typename T>
class LinkedList {
private:
	NodoList<T>* head;
	int longitud;

public:
	LinkedList() : head(nullptr), longitud(0) {}
	~LinkedList() {
		NodoList<T>* aux = head;
		NodoList<T>* aux2;
		while (aux != nullptr) {
			aux2 = aux->siguiente;
			delete aux;
			aux = aux2;
		}
	}
	NodoList<T>* NodeAt(int pos) {
		if (pos >= longitud) {
			std::cout << "No existe posicion, debe ser menor al tamanio de la lista...\n";
			return nullptr;
		}
		int index = 0;
		NodoList<T>* aux = head;
		while (index < pos) {
			aux = aux->siguiente;
			index++;
		}
		return aux;
	}

	void Print() {
		if (estaVacia()) {
			std::cout << "La lista enlazada esta Vacia...\n";
			return;
		}
		NodoList<T>* aux = head;
		while (aux != nullptr) {
			std::cout << aux->dato << " ";
			aux = aux->siguiente;
		}
		std::cout << std::endl;
	}

	int getLongitud() {
		return longitud;
	}

	bool estaVacia() {
		return longitud == 0;
	}

	void AddFirst(T data) {
		NodoList<T>* node = new NodoList<T>(data, estaVacia() ? nullptr : head);
		head = node;
		longitud++;
	}

	void AddPos(T data, int pos) {
		if (pos == 0) 
			AddFirst(data);
		else
		{
			NodoList<T>* nodeBefore = NodeAt(pos - 1);
			NodoList<T>* nodeAfter = nodeBefore->siguiente;

			NodoList<T>* node = new NodoList<T>(data, nodeAfter);
			
			nodeBefore->siguiente = node;
		}
		longitud++;
	}

	void AddLast(T data) {
		NodoList<T>* node = new NodoList<T>(data);
		if (estaVacia()) {
			head = node;
		}
		else {
			NodoList<T>* nodeLast = NodeAt(longitud - 1);
			nodeLast->siguiente = node;
		}
		longitud++;
	}

	void ModifyFirst(T data) {
		NodoList<T>* currentNode = NodeAt(0);
		if (currentNode != nullptr) {
			currentNode->dato = data;
		}
	}

	void ModifyPos(T data, int pos) {
		NodoList<T>* currentNode = NodeAt(pos);
		if (currentNode != nullptr) {
			currentNode->dato = data;
		}
	}

	void ModifyLast(T data) {
		NodoList<T>* currentNode = NodeAt(longitud - 1);
		if (currentNode != nullptr) {
			currentNode->dato = data;
		}
	}

	void RemoveFirst() {
		if (estaVacia()) {
			std::cout << "No se puede eliminar un nodo en una lista enlazada vacía\n";
			return;
		}
		NodoList<T>* aux = head;
		head = head->siguiente;
		delete aux;
		longitud--;
	}

	void RemovePos(int pos) {
		if (estaVacia()) {
			std::cout << "No se puede eliminar un nodo en una lista enlazada vacía\n";
			return;
		}
		if (pos == 0)
			RemoveFirst();
		else {
			NodoList<T>* nodeBefore = NodeAt(pos - 1);
			NodoList<T>* nodeErase = NodeAt(pos);
			if (nodeBefore != nullptr && nodeErase != nullptr) {
				nodeBefore->siguiente = nodeErase->siguiente;
				delete nodeErase;
				longitud--;
			}
		}
	}

	void RemoveLast() {
		if (estaVacia()) {
			std::cout << "No se puede eliminar un nodo en una lista enlazada vacía\n";
			return;
		}
		if (longitud == 1) {
			NodoList<T>* aux = head;
			head = nullptr;
			delete aux;
			longitud--;
		}
		else {
			NodoList<T>* nodePreviousLast = NodeAt(longitud - 2);
			if (nodePreviousLast != nullptr) {
				NodoList<T>* nodeLast = nodePreviousLast->siguiente;
				nodePreviousLast->siguiente = nullptr;
				delete nodeLast;
				longitud--;
			}
		}
	}

	T GetPos(int pos) {
		NodoList<T>* node = NodeAt(pos);
		return node != nullptr ? node->dato: T();
	}
};