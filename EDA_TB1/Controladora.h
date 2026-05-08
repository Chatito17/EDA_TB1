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
    Division* division;
    BancoPreguntas* banco;
    

public:
    Controladora() {
        // Se corrige el constructor pasando el apodo
        usuarioActual = new Usuario("putito");
        tienda = new Tienda();
        division = new Division();
        banco = new BancoPreguntas();

        // 1. Poblamos el banco de preguntas al iniciar
        banco->agregar(new PreguntaEscribir("Traduce 'Manzana' al ingles", "Apple"));
        banco->agregar(new PreguntaEscribir("Traduce 'Perro' al ingles", "Dog"));
        banco->agregar(new PreguntaEscribir("Traduce 'Gato' al ingles", "Cat"));
        banco->agregar(new PreguntaEscribir("Traduce 'Libro' al ingles", "Book"));
        banco->agregar(new PreguntaEscribir("Traduce 'Agua' al ingles", "Water"));
    }

    ~Controladora() {
        delete usuarioActual;
        delete tienda;
        delete division;
        delete banco;
    }

    void iniciar() {
        int opcionMenu;
        do {
            std::cout << "\n[ MENU PRINCIPAL ]\n";
            std::cout << "1. Ver Perfil\n";
            std::cout << "2. Entrar a la Tienda\n";
            std::cout << "3. Rendir Examen de Nivel\n";
            std::cout << "4. Salir del programa\n";
            std::cout << "Elige una opcion: ";
            std::cin >> opcionMenu;
            std::cin.ignore(); // Limpiar el buffer para el getline de los examenes

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
                std::cout << "Saliendo del aplicativo...\n";
                break;
            default:
                std::cout << "Opcion no valida.\n";
            }
        } while (opcionMenu != 4);
    }

private:
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
        // Obtenemos 5 preguntas aleatorias del banco usando tu propia logica lambda
        Pila<Pregunta*> preguntasExamen = banco->seleccionarParaExamen(5, [](Pregunta* p) {
            return true; // Criterio: acepta todas las preguntas
            });

        // Ejecutamos el polimorfismo de Examen
        ExamenNivel examen("Leccion de Ingles Basico");
        ResultadoDetallado res = examen.hacerExamen(preguntasExamen);

        // Integracion: Recompensamos al usuario si aprobo
        if (res.getPuntaje() >= 3) { // 3 de 5 correcto
            std::cout << ">>> Has ganado 50 gemas por tu buen rendimiento! <<<\n";
            // Para que esto funcione, necesitas agregar un metodo sumarGemas(int) en Usuario.h
        }
    }
};