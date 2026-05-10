#pragma once
#include <string>
#include <iostream>
#include "LinkedList.h"

struct DetalleIntento {
    std::string enunciado;
    std::string respuestaUsuario;
    std::string respuestaCorrecta;
    bool sonCorrectas;

    DetalleIntento(std::string e, std::string ru, std::string rc, bool ok)
        : enunciado(e), respuestaUsuario(ru), respuestaCorrecta(rc), sonCorrectas(ok) {
    }
};

class ResultadoDetallado {
private:
    std::string nombreExamen;
    int puntaje;
    int totalPreguntas;

    LinkedList<DetalleIntento*> listaDetalles;

public:
    ResultadoDetallado() {
        nombreExamen = "Sin nombre";
        puntaje = 0;
        totalPreguntas = 0;
    }

    ResultadoDetallado(std::string nombre, int total) {
        nombreExamen = nombre;
        totalPreguntas = total;
        puntaje = 0;
    }

    ~ResultadoDetallado() {
        for (int i = 0; i < listaDetalles.getLongitud(); i++) {
            delete listaDetalles.GetPos(i);
        }
    }
    void registrarRespuesta(std::string enunciado, std::string respUsuario, std::string respCorrecta, bool correcta) {
        DetalleIntento* nuevo = new DetalleIntento(enunciado, respUsuario, respCorrecta, correcta);
        listaDetalles.AddLast(nuevo);
        if (correcta) puntaje++;
    }

    int getPuntaje() { return puntaje; }
    int getTotalPreguntas() { return totalPreguntas; }
    std::string getNombreExamen() { return nombreExamen; }

    void mostrar() {
        std::cout << nombreExamen << " -> " << puntaje << "/" << totalPreguntas << std::endl;
    }

    void mostrarDetalle() {
        mostrar();
        for (int i = 0; i < listaDetalles.getLongitud(); i++) {
            DetalleIntento* d = listaDetalles.GetPos(i);
            std::cout << "  P" << (i + 1) << ": "
                << (d->sonCorrectas ? "[OK] " : "[XX] ")
                << d->enunciado;
            if (!d->sonCorrectas)
                std::cout << "  (correcto: " << d->respuestaCorrecta << ")";
            std::cout << std::endl;
        }
    }

    // Algoritmo MERGE SORT / Recursividad

    void merge(DetalleIntento** arr, int izq, int mid, int der) {
        int n1 = mid - izq + 1;
        int n2 = der - mid;

        DetalleIntento** L = new DetalleIntento * [n1];
        DetalleIntento** R = new DetalleIntento * [n2];

        for (int i = 0; i < n1; i++) L[i] = arr[izq + i];
        for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

        int i = 0, j = 0, k = izq;
        while (i < n1 && j < n2) {
            if (L[i]->enunciado <= R[j]->enunciado)
                arr[k++] = L[i++];
            else
                arr[k++] = R[j++];
        }
        while (i < n1) arr[k++] = L[i++];
        while (j < n2) arr[k++] = R[j++];

        delete[] L;
        delete[] R;
    }

    void mergeSort(DetalleIntento** arr, int izq, int der) {
        if (izq < der) {
            int mid = izq + (der - izq) / 2;
            mergeSort(arr, izq, mid);
            mergeSort(arr, mid + 1, der);
            merge(arr, izq, mid, der);
        }
    }

    void ordenarPorEnunciado() {
        int n = listaDetalles.getLongitud();
        if (n <= 1) return;

        DetalleIntento** arr = new DetalleIntento * [n];
        for (int i = 0; i < n; i++) arr[i] = listaDetalles.GetPos(i);

        mergeSort(arr, 0, n - 1);

        for (int i = 0; i < n; i++) {
            listaDetalles.ModifyPos(arr[i], i);
        }
        delete[] arr;
    }
};