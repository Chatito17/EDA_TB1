#pragma once

template <typename T>

class Node {
public:
	T data;
	Node<T>* next;
	
	Node(T data) : data(data), next(nullptr) {}
	Node(T data, Node<T>* next) : data(data), next(next) {}
};

template <typename T>
class LinkedList {
private:
	Node<T>* head;
	int length;

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

	int Length() {
		return length;
	}

	bool IsEmpty() {
		return length == 0;
	}

	void AddFirst(T data) {
		Node<T>* node = new Node<T>(data, IsEmpty() ? nullptr : head);
		head = node;
		length++;
	}

	void AddPos(T data, int pos) {
		if (pos == 0) 
			AddFirst(data);
		else
		{
			Node<T>* nodeBefore = NodeAt(pos - 1);
			Node<T>* nodeAfter = nodeBefore->next;
			Node<T>* node = new Node<T>(data, nodeAfter);
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
		if (pos == 0) 
			RemoveFirst();
		else {
			Node<T>* nodeBefore = NodeAt(pos - 1);
			Node<T>* nodeErase = NodeAt(pos);
			if (nodeBefore != nullptr && nodeErase != nullptr) {
				nodeBefore->next = nodeErase->next;
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
			Node<T>* nodePreviousLast = NodeAt(length - 2);
			if (nodePreviousLast != nullptr) {
				Node<T>* nodeLast = nodePreviousLast->next;
				nodePreviousLast->next = nullptr;
				delete nodeLast;
				length--;
			}
		}
	}

	T GetFirst() {
		Node<T>* first = NodeAt(0);
		return first != nullptr ? first->data : T();
	}

	T GetPos(int pos) {
		Node<T>* node = NodeAt(pos);
		return node != nullptr ? node->data : T();
	}

	T GetLast() {
		Node<T>* last = NodeAt(length - 1);
		return last != nullptr ? last->data : T();
	}
	Node<T>* NodeAt(int pos) {
		if (pos >= length) {
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
};