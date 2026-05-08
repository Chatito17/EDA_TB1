#pragma once
#include <iostream>
#include "Usuario.h"
#include "LinkedList.h" // Se usa la lista enlazada creada por ti

class Ranking
{
private:
    LinkedList<Usuario*> listaUsuarios;

public:
    Ranking() {};
    ~Ranking() {};

    void agregarUsuario(Usuario* usuario) {
        listaUsuarios.AddLast(usuario);
    }

    void mostrarPrimeros() {
        if (!listaUsuarios.IsEmpty()) {
            std::cout << "Lider: " << listaUsuarios.GetFirst()->getNombre()
                << " (" << listaUsuarios.GetFirst()->getExp() << " EXP)\n";
        }
    }

    void mostrarRanking() {
        ordenarRanking(); // Siempre se ordena antes de mostrar la tabla
        for (int i = 0; i < listaUsuarios.Length(); i++) {
            Usuario* u = listaUsuarios.GetPos(i);
            std::cout << i + 1 << ". " << u->getNombre() << " \t| EXP: " << u->getExp() << "\n";
        }
    }

    void ordenarRanking() {
        int n = listaUsuarios.Length();
        if (n <= 1) return;

        // Ordenamiento burbuja adaptado para tu LinkedList (Descendente por EXP)
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                Usuario* u1 = listaUsuarios.GetPos(j);
                Usuario* u2 = listaUsuarios.GetPos(j + 1);

                if (u1->getExp() < u2->getExp()) {
                    // Se intercambian las posiciones si el de abajo tiene más EXP
                    listaUsuarios.ModifyPos(u2, j);
                    listaUsuarios.ModifyPos(u1, j + 1);
                }
            }
        }
    }
};