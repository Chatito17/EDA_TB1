#pragma once

template <typename T>
class Nodo {
public:
	T dato;
	Nodo<T>* siguiente;
	Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};

template <typename T>
class LinkedList {
private:
	Node<T>* head;
	int length;

	Node<T>* NodeAt(int pos) {
		if (pos >= length) {
			std::cout << "No existe posicion, debe ser menor al tamanio de la lista...\n";
			return nullptr;
		}
		int index = 0;
		Node<T>* aux = head;
		while (index < pos) {
			aux = aux->next;
			index++;
		}
		return aux;
	}

public:
	LinkedList() : head(nullptr), length(0) {}
	~LinkedList() {
		Node<T>* aux = head;
		Node<T>* aux2;
		while (aux != nullptr) {
			aux2 = aux->next;
			delete aux;
			aux = aux2;
		}
	}

	void Print() {
		if (IsEmpty()) {
			std::cout << "La lista enlazada esta Vacia...\n";
			return;
		}
		Node<T>* aux = head;
		while (aux != nullptr) {
			std::cout << aux->data << " ";
			aux = aux->next;
		}
		std::cout << std::endl;
	}

	// La longitud
	int Length() {
		return length;
	}

	// Es vacío
	bool IsEmpty() {
		return length == 0;
	}

	void AddFirst(T data) {
		Node<T>* node = new Node<T>(data, IsEmpty() ? nullptr : head);
		head = node;
		length++;
	}

	void AddPos(T data, int pos) {
		// Ubicamos la posición anterior
		if (pos == 0) // Agrego al inicio
			AddFirst(data);
		else
		{
			Node<T>* nodeBefore = NodeAt(pos - 1);
			Node<T>* nodeAfter = nodeBefore->next;
			// El nodo nuevo apunta a la siguiente posición
			Node<T>* node = new Node<T>(data, nodeAfter);
			// El nodo anterior apunta al nuevo nodo.
			nodeBefore->next = node;
		}
		length++;
	}

	void AddLast(T data) {
		Node<T>* node = new Node<T>(data);
		if (IsEmpty()) {
			head = node;
		}
		else {
			// Buscamos el último nodo y lo agregamos
			Node<T>* nodeLast = NodeAt(length - 1);
			nodeLast->next = node;
		}
		length++;
	}

	void ModifyFirst(T data) {
		Node<T>* currentNode = NodeAt(0);
		if (currentNode != nullptr) {
			currentNode->data = data;
		}
	}

	void ModifyPos(T data, int pos) {
		Node<T>* currentNode = NodeAt(pos);
		if (currentNode != nullptr) {
			currentNode->data = data;
		}
	}

	void ModifyLast(T data) {
		Node<T>* currentNode = NodeAt(length - 1);
		if (currentNode != nullptr) {
			currentNode->data = data;
		}
	}

	void RemoveFirst() {
		if (IsEmpty()) {
			std::cout << "No se puede eliminar un nodo en una lista enlazada vacía\n";
			return;
		}
		Node<T>* aux = head;
		head = head->next;
		delete aux;
		length--;
	}

	void RemovePos(int pos) {
		if (IsEmpty()) {
			std::cout << "No se puede eliminar un nodo en una lista enlazada vacía\n";
			return;
		}
		if (pos == 0) // Remuevo el inicio
			RemoveFirst();
		else {
			Node<T>* nodeBefore = NodeAt(pos - 1);
			Node<T>* nodeErase = NodeAt(pos);
			if (nodeBefore != nullptr && nodeErase != nullptr) {
				nodeBefore->next = nodeErase->next;
				// Podemos eliminar con tranquilidad el nodo elegido
				delete nodeErase;
				length--;
			}
		}
	}

	void RemoveLast() {
		if (IsEmpty()) {
			std::cout << "No se puede eliminar un nodo en una lista enlazada vacía\n";
			return;
		}
		if (length == 1) {
			Node<T>* aux = head;
			head = nullptr;
			delete aux;
			length--;
		}
		else {
			// Busca el penúltimo nodo, lo guardamos
			Node<T>* nodePreviousLast = NodeAt(length - 2);
			if (nodePreviousLast != nullptr) {
				// Penúltimo nodo apunte a nulo y hacemos que el último nodo
				// sea eliminado.
				Node<T>* nodeLast = nodePreviousLast->next;
				nodePreviousLast->next = nullptr;
				delete nodeLast;
				length--;
			}
		}
	}

	T GetPos(int pos) {
		Node<T>* node = NodeAt(pos);
		return node != nullptr ? node->data : T();
	}
};