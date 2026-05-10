#pragma once
#include "Producto.h"
#include "VidasExtra.h"
#include "ProtectorRacha.h"
#include "ActivarPlus.h"
#include "DuplicadorExp.h"

#include "LinkedList.h"

class Tienda
{
private:
    LinkedList<Producto*> catalogo;

public:
    Tienda() {
        catalogo.AddLast(new VidasExtra());
        catalogo.AddLast(new ProtectorRacha());
        catalogo.AddLast(new ActivarPlus());
        catalogo.AddLast(new DuplicadorExp());
    }

    ~Tienda() {
        for (int i = 0; i < catalogo.getLongitud(); i++) {
            Producto* p = catalogo.GetPos(i);
            if (p != nullptr)
                delete p;
        }
    }

    // Algoritmo HEAP SORT / Recursividad
    void heapify(Producto** arr, int n, int i) {
        int masGrande = i;
        int izq = 2 * i + 1;
        int der = 2 * i + 2;

        if (izq < n && arr[izq]->getCosto() > arr[masGrande]->getCosto())
            masGrande = izq;

        if (der < n && arr[der]->getCosto() > arr[masGrande]->getCosto())
            masGrande = der;

        if (masGrande != i) {
            Producto* swap = arr[i];
            arr[i] = arr[masGrande];
            arr[masGrande] = swap;
            heapify(arr, n, masGrande);
        }
    }

    void heapSortCatalogo() {
        int n = catalogo.getLongitud();
        if (n <= 1) return;

        Producto** arr = new Producto * [n];
        for (int i = 0; i < n; i++) {
            arr[i] = catalogo.GetPos(i);
        }

        for (int i = n / 2 - 1; i >= 0; i--)
            heapify(arr, n, i);

        for (int i = n - 1; i > 0; i--) {
            Producto* temp = arr[0];
            arr[0] = arr[i];
            arr[i] = temp;

            heapify(arr, i, 0);
        }

        for (int i = 0; i < n; i++) {
            catalogo.ModifyPos(arr[i], i);
        }

        delete[] arr;
    }
    void mostrarCatalogo() {
        heapSortCatalogo();
        std::cout << "\n=== TIENDA ===\n";
        for (int i = 0; i < catalogo.getLongitud(); i++) {
            std::cout << i + 1 << ". " << catalogo.GetPos(i)->getNombre()
                << " - " << catalogo.GetPos(i)->getDetalle()
                << " (Costo: " << catalogo.GetPos(i)->getCosto() << " gemas)\n";
        }
        std::cout << "0. Volver al menu\n";
        std::cout << "=======================\n";
    }

    Producto* getProducto(int indice) {
        if (indice >= 0 && indice < catalogo.getLongitud()) {
            return catalogo.GetPos(indice);
        }
        return nullptr;
    }
};