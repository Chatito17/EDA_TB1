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
        for (int i = 0; i < catalogo.Length(); i++) {
            Producto* p = catalogo.GetPos(i);
            if (p != nullptr)
                delete p;
        }
    }

    void mostrarCatalogo() {
        std::cout << "\n=== TIENDA ===\n";
        for (int i = 0; i < catalogo.Length(); i++) {
            std::cout << i + 1 << ". " << catalogo.GetPos(i)->getNombre()
                << " - " << catalogo.GetPos(i)->getDetalle()
                << " (Costo: " << catalogo.GetPos(i)->getCosto() << " gemas)\n";
        }
        std::cout << "0. Volver al menu\n";
        std::cout << "=======================\n";
    }

    Producto* getProducto(int indice) {
        if (indice >= 0 && indice < catalogo.Length()) {
            return catalogo.GetPos(indice);
        }
        return nullptr;
    }
};