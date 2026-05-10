#pragma once
#include <iostream>
#include <fstream>
#include "Usuario.h"
#include "LinkedList.h" // Se usa la lista enlazada creada por ti

class Ranking
{
private:
    LinkedList<Usuario*> listaUsuarios;


    int particion(Usuario** arr, int low, int high) {
        Usuario* pivot = arr[high];
        int i = (low - 1);

        for (int j = low; j <= high - 1; j++) {
            // Orden descendente por Experiencia
            if (arr[j]->getExp() >= pivot->getExp()) {
                i++;
                Usuario* temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        Usuario* temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;
        return (i + 1);
    }
    void quickSort(Usuario** arr, int low, int high) {
        if (low < high) {
            int pi = particion(arr, low, high);
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }

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

        // Extraemos a un arreglo para ordenar rápidamente O(1) en acceso
        Usuario** arr = new Usuario * [n];
        for (int i = 0; i < n; i++) {
            arr[i] = listaUsuarios.GetPos(i);
        }

        quickSort(arr, 0, n - 1);

        // Reconstruimos la lista ordenada
        Node<Usuario*>* nodoActual = listaUsuarios.NodeAt(0); // Tomamos la cabeza
        for (int i = 0; i < n; i++) {
            if (nodoActual != nullptr) {
                nodoActual->data = arr[i];
                nodoActual = nodoActual->next;
            }
        }
        delete[] arr;
    }
    void guardarTabla(std::string nombreLiga) {
        ordenarRanking();
        std::ofstream archivo("Clasificacion_" + nombreLiga + ".txt");

        if (archivo.is_open()) {
            archivo << "=== RANKING FINAL " << nombreLiga << " ===\n";
            for (int i = 0; i < listaUsuarios.Length(); i++) {
                Usuario* u = listaUsuarios.GetPos(i);
                archivo << i + 1 << ". " << u->getNombre() << " | EXP: " << u->getExp() << "\n";
            }
            archivo.close();
        }
    }
};