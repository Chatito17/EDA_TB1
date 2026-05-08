#pragma once
#include <iostream>
#include <string>
#include "Producto.h"

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
    }
    ~Usuario() {}

    std::string getNombre() { return apodo; }
    int getExp() { return exp; }

    void sumarVidas(int cantidad) { vidas += cantidad; }
    void sumarGemas(int cantidad) { gemas += cantidad; }
    void setProtectorRacha(bool estado) { proteRacha = estado; }
    void setUsuarioPlus(bool estado) { usuarioPlus = estado; }
    void activarDuplicador() { dobleExp = true; std::cout << " [Buff] EXP x2 Activada!\n"; }

  
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
    }
};