#pragma once
#include <string>
#include <iostream>
#include "Ranking.h"

enum Divisiones { Bronce, Plata, Oro, Zafiro, Rubi, Esmeralda, Amatista, Perla, Obsidiana, Diamante };

class Division
{
private:
    Divisiones nombreDivision;
    Ranking ranking;

public:
    Division(Divisiones divInicial = Bronce) {
        nombreDivision = divInicial;
    }
    ~Division() {};

    std::string getNombreStr() {
        const std::string nombres[] = { "Bronce", "Plata", "Oro", "Zafiro", "Rubi", "Esmeralda", "Amatista", "Perla", "Obsidiana", "Diamante" };
        return nombres[nombreDivision];
    }

    void agregarParticipante(Usuario* usuario) {
        ranking.agregarUsuario(usuario);
    }

    void mostrarTablaDivision() {
        std::cout << "\n=== LIGA " << getNombreStr() << " ===\n";
        ranking.mostrarRanking();
        std::cout << "========================\n";
    }
};