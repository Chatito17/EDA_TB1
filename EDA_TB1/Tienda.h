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
    void heapify(Producto** arr, int n, int i) {
        int masGrande = i;
        int izq = 2 * i + 1;
        int der = 2 * i + 2;

        // Si el hijo izquierdo es mayor
        if (izq < n && arr[izq]->getCosto() > arr[masGrande]->getCosto())
            masGrande = izq;

        // Si el hijo derecho es mayor
        if (der < n && arr[der]->getCosto() > arr[masGrande]->getCosto())
            masGrande = der;

        // Si el más grande no es la raíz, intercambiamos y aplicamos recursión
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

        // 1. Extraemos a un arreglo nativo
        Producto** arr = new Producto * [n];
        for (int i = 0; i < n; i++) {
            arr[i] = catalogo.GetPos(i);
        }

        // 2. Construimos el Heap (reordenamos el arreglo)
        for (int i = n / 2 - 1; i >= 0; i--)
            heapify(arr, n, i);

        // 3. Extraemos elementos del heap uno por uno
        for (int i = n - 1; i > 0; i--) {
            // Movemos la raíz (el mayor) al final
            Producto* temp = arr[0];
            arr[0] = arr[i];
            arr[i] = temp;

            // Llamamos heapify a la raíz reduciendo el heap
            heapify(arr, i, 0);
        }

        // 4. Reconstruimos la LinkedList secuencialmente O(N)
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