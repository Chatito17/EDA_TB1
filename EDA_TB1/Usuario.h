#pragma once
#include <iostream>
#include <string>
#include <fstream>

#include "Producto.h"
#include "Curso.h"
#include "Cola.h"
#include "Etapa.h"
#include "Seccion.h"

class Usuario
{
private:
    std::string apodo;
    int vidas;
    int racha;
    int antiracha;
    int gemas;
    int exp;
    bool proteRacha;
    bool usuarioPlus;
    bool dobleExp;

    Curso* cursoActual;
    int indiceEtapaActual;
    int indiceSeccionActual;
    int indiceNivelActual;

    Cola<std::string> notificaciones;

public:
    Usuario(std::string _apodo) {
        apodo = _apodo;
        vidas = 3;
        racha = 1;
        antiracha = 0;
        gemas = 100;
        exp = 0;
        proteRacha = false;
        usuarioPlus = false;
        dobleExp = false;

        cursoActual = nullptr;
        indiceEtapaActual = 0;
        indiceSeccionActual = 0;
        indiceNivelActual = 0;
    }
    ~Usuario() {}

    std::string getNombre() { return apodo; }
    int getExp() { return exp; }

    void sumarVidas(int cantidad) { vidas += cantidad; }
    void sumarGemas(int cantidad) { gemas += cantidad; }
    void setProtectorRacha(bool estado) { proteRacha = estado; }
    void setUsuarioPlus(bool estado) { usuarioPlus = estado; }
    void activarDuplicador() { dobleExp = true; std::cout << " [Buff] EXP x2 Activada!\n"; }

    void guardarProgreso() {
        std::ofstream archivo("Usuario_" + apodo + ".txt");
        if (archivo.is_open()) {
            archivo << apodo << "\n" << vidas << "\n" << racha << "\n"
                << gemas << "\n" << exp << "\n"
                << indiceEtapaActual << "\n" << indiceSeccionActual << "\n" << indiceNivelActual << "\n";
            archivo.close();
        }
    }
    void cargarProgreso() {
        std::ifstream archivoE("Usuario_" + apodo + ".txt");
        if (archivoE.is_open()) {
            std::getline(archivoE, apodo);
            archivoE >> vidas >> racha >> gemas >> exp >> indiceEtapaActual >> indiceSeccionActual >> indiceNivelActual;
            archivoE.close();
            std::cout << "Datos cargados exitosamente, " << apodo << "!\n";
          
        }
        else {
            std::cout << "No se encontraron los datos, creando usuario nuevo...\n";
            std::ofstream archivoS("Usuario_" + apodo + ".txt");
            
            if (archivoS.is_open()) {
                archivoS << apodo << "\n" << 3 << "\n" << 0 << "\n"
                    << 50 << "\n" << 0 << "\n"
                    << 1 << "\n" << 1 << "\n" << 1 << "\n";
                archivoS.close();
            }
            else {
                std::cout << "Error critico, no se puede crear archivo (calaverita).\n";
            }
            
        }
     
        
    }
    void sumarExp(int cantidad) {
        if (dobleExp) cantidad *= 2;
        exp += cantidad;
    }

    bool gastarGemas(int cantidad) {
        if (gemas >= cantidad) {
            gemas -= cantidad;
            return true;
        }
        return false;
    }
    int getVidas() { return vidas; }
    void restarVida() {
        if (vidas > 0) vidas--;
    }

    void comprarProducto(Producto* p) {
        if (p && gastarGemas(p->getCosto())) {
            std::cout << "\n>>> Compraste con exito: " << p->getNombre() << " <<<\n";
            p->activar(this);
        }
        else if (p) {
            std::cout << "\n[!] No tienes suficientes gemas para " << p->getNombre() << ".\n";
        }
    }

    void verPerfil() {
        std::cout << "\n=== PERFIL DE USUARIO ===\n";
        std::cout << "Apodo: " << apodo << "\n";
        std::cout << "Vidas: " << vidas << "\n";
        std::cout << "Gemas: " << gemas << "\n";
        std::cout << "Racha: " << racha << "\n";
        std::cout << "EXP: " << exp << "\n";
        std::cout << "Protector de Racha: " << (proteRacha ? "Activado" : "Desactivado") << "\n";
        std::cout << "Usuario Plus: " << (usuarioPlus ? "Si" : "No") << "\n";
        std::cout << "=========================\n";
        leerNotificaciones();
    }
    void inscribirseCurso(Curso* curso) {
        cursoActual = curso;
        indiceEtapaActual = 0;
        indiceSeccionActual = 0;
        indiceNivelActual = 0;
        std::cout << "\n>>> Te has inscrito exitosamente al curso de " << curso->getIdioma() << " <<<\n";
    }
    Curso* getCursoActual() { return cursoActual; }
    int getEtapaActual() { return indiceEtapaActual; }
    int getSeccionActual() { return indiceSeccionActual; }
    int getNivelActual() { return indiceNivelActual; }

    void avanzarNivel() {

        if (!cursoActual) return;
        if (indiceEtapaActual >= cursoActual->getCantidadEtapas()) return;

        indiceNivelActual++;

        // CORRECCIÓN: Faltaba definir etapaActualObj para prevenir el Null Pointer / Crasheo
        Etapa* etapaActualObj = cursoActual->getEtapa(indiceEtapaActual);
        Seccion* secActual = etapaActualObj->getSeccion(indiceSeccionActual);

        // Si superamos todos los niveles de esta sección...
        if (indiceNivelActual >= secActual->getCantidadNiveles()) {
            indiceNivelActual = 0;
            indiceSeccionActual++;

            // Si superamos todas las secciones de la etapa...
            if (indiceSeccionActual >= etapaActualObj->getCantidadSecciones()) {
                indiceSeccionActual = 0;
                indiceEtapaActual++; // Pasamos a la siguiente gran etapa del curso
                std::cout << "\n>>> ¡Has avanzado a una nueva ETAPA! <<<\n";
            }
            else {
                std::cout << "\n>>> ¡Has desbloqueado una nueva SECCIÓN! <<<\n";
            }
        }
    }

    void verProgreso() {
        if (!cursoActual) {
            std::cout << "No estas inscrito en ningun curso.\n";
            return;
        }
        std::cout << "\n--- PROGRESO ACTUAL ---\n";
        std::cout << "Curso: " << cursoActual->getIdioma() << "\n";
        std::cout << "Etapa: " << indiceEtapaActual + 1 << "\n";
        std::cout << "Seccion: " << indiceSeccionActual + 1 << "\n";
        std::cout << "Nivel: " << indiceNivelActual + 1 << "\n";
        std::cout << "-----------------------\n";
    }

    void agregarNotificacion(std::string msj) {
        notificaciones.encolar(msj);
    }

    void leerNotificaciones() {
        if (notificaciones.estaVacia()) {
            std::cout << "No tienes notificaciones nuevas.\n";
            return;
        }
        std::cout << "\n--- BANDEJA DE ENTRADA ---\n";

        // Uso de Lambda para cumplir rúbrica
        auto imprimirMsj = [](std::string msj) {
            std::cout << "[Mensaje] " << msj << "\n";
            };

        notificaciones.procesarTodos(imprimirMsj);

        // Vaciamos la cola tras leer
        while (!notificaciones.estaVacia()) {
            notificaciones.desencolar();
        }
        std::cout << "--------------------------\n";
    }
};