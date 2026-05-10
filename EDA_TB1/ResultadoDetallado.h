#pragma once
#include <string>
#include <functional>
#include <iostream>

class ResultadoDetallado {
private:
    std::string nombreExamen;
    int puntaje;
    int totalPreguntas;

    std::string* enunciados;
    std::string* respuestasUsuario;
    std::string* respuestasCorrectas;
    bool* sonCorrectas;

    int cantDetalles;
    int capDetalles;

    void expandir() {
        int nuevaCap = capDetalles * 2;

        std::string* ne = new std::string[nuevaCap];
        std::string* nru = new std::string[nuevaCap];
        std::string* nrc = new std::string[nuevaCap];
        bool* nsc = new bool[nuevaCap];

        for (int i = 0; i < cantDetalles; i++) {
            ne[i] = enunciados[i];
            nru[i] = respuestasUsuario[i];
            nrc[i] = respuestasCorrectas[i];
            nsc[i] = sonCorrectas[i];
        }

        delete[] enunciados;
        delete[] respuestasUsuario;
        delete[] respuestasCorrectas;
        delete[] sonCorrectas;

        enunciados = ne;
        respuestasUsuario = nru;
        respuestasCorrectas = nrc;
        sonCorrectas = nsc;
        capDetalles = nuevaCap;
    }

    void liberar() {
        delete[] enunciados;
        delete[] respuestasUsuario;
        delete[] respuestasCorrectas;
        delete[] sonCorrectas;
    }

    // Alloca los 4 arreglos con la capacidad indicada
    void alocar(int cap) {
        enunciados = new std::string[cap];
        respuestasUsuario = new std::string[cap];
        respuestasCorrectas = new std::string[cap];
        sonCorrectas = new bool[cap];
        capDetalles = cap;
    }

public:
    ResultadoDetallado()
        : puntaje(0), totalPreguntas(0),
        enunciados(nullptr), respuestasUsuario(nullptr),
        respuestasCorrectas(nullptr), sonCorrectas(nullptr),
        cantDetalles(0), capDetalles(0) {
    }

    ResultadoDetallado(std::string& nombre, int total)
        : nombreExamen(nombre), puntaje(0), totalPreguntas(total), cantDetalles(0)
    {
        alocar(total > 0 ? total : 8);
    }

    ResultadoDetallado(ResultadoDetallado& otro) : nombreExamen(otro.nombreExamen), puntaje(otro.puntaje), totalPreguntas(otro.totalPreguntas),
        cantDetalles(otro.cantDetalles)
    {
        alocar(otro.capDetalles);
        for (int i = 0; i < cantDetalles; i++) {
            enunciados[i] = otro.enunciados[i];
            respuestasUsuario[i] = otro.respuestasUsuario[i];
            respuestasCorrectas[i] = otro.respuestasCorrectas[i];
            sonCorrectas[i] = otro.sonCorrectas[i];
        }
    }
    ResultadoDetallado& operator=(const ResultadoDetallado& otro) {
        if (this != &otro) {
            liberar();
            nombreExamen = otro.nombreExamen;
            puntaje = otro.puntaje;
            totalPreguntas = otro.totalPreguntas;
            cantDetalles = otro.cantDetalles;
            alocar(otro.capDetalles);
            for (int i = 0; i < cantDetalles; i++) {
                enunciados[i] = otro.enunciados[i];
                respuestasUsuario[i] = otro.respuestasUsuario[i];
                respuestasCorrectas[i] = otro.respuestasCorrectas[i];
                sonCorrectas[i] = otro.sonCorrectas[i];
            }
        }
        return *this;
    }

    ~ResultadoDetallado() { liberar(); }

    void registrarRespuesta(std::string& enunciado, std::string& respUsuario, std::string& respCorrecta, bool correcta){
        if (cantDetalles == capDetalles) expandir();
        enunciados[cantDetalles] = enunciado;
        respuestasUsuario[cantDetalles] = respUsuario;
        respuestasCorrectas[cantDetalles] = respCorrecta;
        sonCorrectas[cantDetalles] = correcta;
        cantDetalles++;
        if (correcta) puntaje++;
    }

    int getPuntaje() { return puntaje; }
    int getTotalPreguntas() { return totalPreguntas; }
    std::string getNombreExamen() { return nombreExamen; }

    void mostrar() const {
        std::cout << nombreExamen
            << "  ->  " << puntaje << "/" << totalPreguntas
            << std::endl;
    }

    void mostrarDetalle() const {
        mostrar();
        for (int i = 0; i < cantDetalles; i++) {
            std::cout << "  P" << (i + 1) << ": "
                << (sonCorrectas[i] ? "[OK] " : "[XX] ")
                << enunciados[i];
            if (!sonCorrectas[i])
                std::cout << "  (correcto: " << respuestasCorrectas[i] << ")";
            std::cout << std::endl;
        }
    }
};

void mergePunteros(
    ResultadoDetallado** arr, int izq, int mid, int der,
    std::function<bool(const ResultadoDetallado*,
        const ResultadoDetallado*)> cmp)
{
    int n1 = mid - izq + 1;
    int n2 = der - mid;

    ResultadoDetallado** L = new ResultadoDetallado * [n1];
    ResultadoDetallado** R = new ResultadoDetallado * [n2];

    for (int i = 0; i < n1; i++) L[i] = arr[izq + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = izq;
    while (i < n1 && j < n2)
        arr[k++] = cmp(L[i], R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeSortPunteros(ResultadoDetallado** arr, int izq, int der, std::function<bool(const ResultadoDetallado*, const ResultadoDetallado*)> cmp){
    if (izq >= der) return;  
    int mid = izq + (der - izq) / 2; 
    mergeSortPunteros(arr, izq, mid, cmp);
    mergeSortPunteros(arr, mid + 1, der, cmp);      
    mergePunteros(arr, izq, mid, der, cmp);      
}

void ordenarResultados( ResultadoDetallado** arr, int n, std::function<bool(const ResultadoDetallado*, const ResultadoDetallado*)> cmp){
    if (n <= 1) return;
    mergeSortPunteros(arr, 0, n - 1, cmp);
}
