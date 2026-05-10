#pragma once
#include <string>
#include <functional>
#include <iostream>

// ============================================================
// Clase 9: ResultadoDetallado
//
// Registra el intento completo de un examen guardando, por cada
// pregunta respondida: su enunciado, la respuesta del usuario,
// la respuesta correcta y si fue acertada.
//
// Se usan 4 arreglos dinámicos paralelos (sin struct auxiliar,
// sin std::vector) indexados por posición [0..cantDetalles-1].
// ============================================================
class ResultadoDetallado {
private:
    // ----- Datos globales del intento -----
    std::string nombreExamen;
    int puntaje;
    int totalPreguntas;

    // ----- Arreglos paralelos: un slot por pregunta respondida -----
    // Cada índice i representa la pregunta i-ésima del examen.
    std::string* enunciados;        // enunciado de la pregunta
    std::string* respuestasUsuario; // lo que escribió el usuario
    std::string* respuestasCorrectas;
    bool* sonCorrectas;

    int cantDetalles; // preguntas ya registradas
    int capDetalles;  // capacidad actual de los 4 arreglos

    // Duplica la capacidad de los 4 arreglos cuando se llenan - O(n)
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

    ResultadoDetallado(const std::string& nombre, int total)
        : nombreExamen(nombre), puntaje(0), totalPreguntas(total), cantDetalles(0)
    {
        alocar(total > 0 ? total : 8); // preasigna justo lo necesario
    }

    // Constructor de copia — regla de tres - O(n)
    ResultadoDetallado(const ResultadoDetallado& otro)
        : nombreExamen(otro.nombreExamen), puntaje(otro.puntaje),
        totalPreguntas(otro.totalPreguntas),
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

    // Operador de asignación — regla de tres - O(n)
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

    // Registra una respuesta; incrementa puntaje si fue correcta - amortizado O(1)
    void registrarRespuesta(const std::string& enunciado,
        const std::string& respUsuario,
        const std::string& respCorrecta,
        bool correcta)
    {
        if (cantDetalles == capDetalles) expandir();
        enunciados[cantDetalles] = enunciado;
        respuestasUsuario[cantDetalles] = respUsuario;
        respuestasCorrectas[cantDetalles] = respCorrecta;
        sonCorrectas[cantDetalles] = correcta;
        cantDetalles++;
        if (correcta) puntaje++;
    }

    // ---- Getters ----
    int getPuntaje()         const { return puntaje; }
    int getTotalPreguntas()  const { return totalPreguntas; }
    std::string getNombreExamen()    const { return nombreExamen; }

    // Resumen de una línea - O(1)
    void mostrar() const {
        std::cout << nombreExamen
            << "  ->  " << puntaje << "/" << totalPreguntas
            << std::endl;
    }

    // Desglose pregunta a pregunta - O(n)
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


// ============================================================
// MergeSort para arreglo dinámico de ResultadoDetallado*
//
// Trabaja sobre un arreglo de punteros ResultadoDetallado*
// para evitar copias costosas al reordenar.
// Recibe Lambda 3 como comparador.
//
// Complejidad temporal:  O(n log n) — todos los casos
// Complejidad espacial:  O(n) arreglo auxiliar + O(log n) pila de recursión
// ============================================================

// -- merge: combina dos mitades ya ordenadas --
static void mergePunteros(
    ResultadoDetallado** arr, int izq, int mid, int der,
    std::function<bool(const ResultadoDetallado*,
        const ResultadoDetallado*)> cmp)
{
    int n1 = mid - izq + 1;
    int n2 = der - mid;

    // Arreglos auxiliares dinámicos (sin vector)
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

// -- Recursión principal --
// BASE: sub-arreglo de tamaño <= 1 (ya ordenado)
// PASO: divide a la mitad, ordena cada parte, une con merge
static void mergeSortPunteros(
    ResultadoDetallado** arr, int izq, int der,
    std::function<bool(const ResultadoDetallado*,
        const ResultadoDetallado*)> cmp)
{
    if (izq >= der) return;                           // Caso base

    int mid = izq + (der - izq) / 2;                 // Punto medio seguro
    mergeSortPunteros(arr, izq, mid, cmp);            // Ordena mitad izquierda
    mergeSortPunteros(arr, mid + 1, der, cmp);        // Ordena mitad derecha
    mergePunteros(arr, izq, mid, der, cmp);           // Combina
}

// -- Wrapper público --
inline void ordenarResultados(
    ResultadoDetallado** arr, int n,
    std::function<bool(const ResultadoDetallado*,
        const ResultadoDetallado*)> cmp)
{
    if (n <= 1) return;
    mergeSortPunteros(arr, 0, n - 1, cmp);
}
