#pragma once
#include <iostream>
#include <string>

#include "Tienda.h"
#include "Usuario.h"
#include "Producto.h"
#include "Division.h"
#include "BancoPreguntas.h"
#include "Examen.h"

class Controladora
{
private:
    Usuario* usuarioActual;
    Tienda* tienda;
    Division* ligaActual;
    BancoPreguntas* banco;

    // Almacenamos a los bots para poder borrar su memoria al cerrar
    LinkedList<Usuario*> competidores;

public:
    Controladora() {
        usuarioActual = new Usuario("Cristian");
        tienda = new Tienda();
        ligaActual = new Division(Bronce);
        banco = new BancoPreguntas();

        poblarBancoDePreguntas();
        iniciarSimulacionLiga();
    }

    ~Controladora() {
        delete usuarioActual;
        delete tienda;
        delete ligaActual;
        delete banco;
        // Limpiar memoria de bots
        for (int i = 0; i < competidores.Length(); i++) {
            delete competidores.GetPos(i);
        }
    }

    void iniciar() {
        int opcionMenu;
        do {
            std::cout << "\n[ MENU PRINCIPAL - CLON DUOLINGO ]\n";
            std::cout << "1. Ver Perfil\n";
            std::cout << "2. Entrar a la Tienda\n";
            std::cout << "3. Rendir Examen (Ganar EXP)\n";
            std::cout << "4. Ver Clasificacion de la Liga\n";
            std::cout << "5. Salir del programa\n";
            std::cout << "Elige una opcion: ";
            std::cin >> opcionMenu;
            std::cin.ignore();

            switch (opcionMenu) {
            case 1:
                usuarioActual->verPerfil();
                break;
            case 2:
                menuTienda();
                break;
            case 3:
                rendirExamen();
                break;
            case 4:
                ligaActual->mostrarTablaDivision();
                break;
            case 5:
                std::cout << "Saliendo del aplicativo...\n";
                break;
            default:
                std::cout << "Opcion no valida.\n";
            }
        } while (opcionMenu != 5);
    }

private:
    void poblarBancoDePreguntas() {
        banco->agregar(new PreguntaEscribir("Traduce 'Manzana' al ingles", "Apple"));
        banco->agregar(new PreguntaEscribir("Traduce 'Perro' al ingles", "Dog"));
        banco->agregar(new PreguntaEscribir("Traduce 'Gato' al ingles", "Cat"));
        banco->agregar(new PreguntaEscribir("Traduce 'Libro' al ingles", "Book"));
        banco->agregar(new PreguntaEscribir("Traduce 'Agua' al ingles", "Water"));
    }

    void iniciarSimulacionLiga() {
        // Agregamos al usuario real a la división
        ligaActual->agregarParticipante(usuarioActual);

        // Creamos bots con EXP simulada para que haya competencia
        Usuario* bot1 = new Usuario("Duo_Lover"); bot1->sumarExp(150);
        Usuario* bot2 = new Usuario("EnglishPro"); bot2->sumarExp(80);
        Usuario* bot3 = new Usuario("Gamer2025"); bot3->sumarExp(30);

        competidores.AddLast(bot1);
        competidores.AddLast(bot2);
        competidores.AddLast(bot3);

        ligaActual->agregarParticipante(bot1);
        ligaActual->agregarParticipante(bot2);
        ligaActual->agregarParticipante(bot3);
    }

    void menuTienda() {
        int opcionTienda;
        do {
            tienda->mostrarCatalogo();
            std::cout << "Ingresa el numero del producto a comprar: ";
            std::cin >> opcionTienda;

            if (opcionTienda > 0) {
                Producto* productoElegido = tienda->getProducto(opcionTienda - 1);
                if (productoElegido != nullptr) {
                    usuarioActual->comprarProducto(productoElegido);
                }
                else {
                    std::cout << "Producto inexistente.\n";
                }
            }
        } while (opcionTienda != 0);
    }

    void rendirExamen() {
        Pila<Pregunta*> preguntasExamen = banco->seleccionarParaExamen(5, [](Pregunta* p) {
            return true;
            });

        ExamenNivel examen("Leccion Rapida");
        ResultadoDetallado res = examen.hacerExamen(preguntasExamen);

        // Lógica fundamental de Duolingo: A más respuestas correctas, más experiencia
        if (res.getPuntaje() > 0) {
            int expGanada = res.getPuntaje() * 15; // 15 de EXP por acierto
            usuarioActual->sumarExp(expGanada);
            std::cout << ">>> Has ganado " << expGanada << " de EXP! Revise la liga para ver si subio de puesto <<<\n";
        }

        if (res.getPuntaje() >= 3) {
            std::cout << ">>> Has ganado 50 gemas extra de recompensa! <<<\n";
            usuarioActual->sumarGemas(50);
        }
    }
};