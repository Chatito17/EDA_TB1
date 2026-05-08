#pragma once
#include <iostream>
#include <string>

#include "Tienda.h"
#include "Usuario.h"
#include "Producto.h"
#include "Division.h"

class Controladora
{
private:
    Usuario* usuarioActual;
    
    //Tienda
    Tienda* tienda;

    //Division
    Division* division;

    //Idiomas/Examenes
    

public:
    Controladora() {
        usuarioActual = new Usuario();
        tienda = new Tienda();
    }

    ~Controladora() {
        delete usuarioActual;
        delete tienda;
    }

    void iniciar() {
        int opcionMenu;
        do {
            std::cout << "\n[ MENU PRINCIPAL ]\n";
            std::cout << "1. Ver Perfil\n";
            std::cout << "2. Entrar a la Tienda\n";
            std::cout << "3. Salir del programa\n";
            std::cout << "Elige una opcion: ";
            std::cin >> opcionMenu;

            switch (opcionMenu) {
            case 1:
                usuarioActual->verPerfil();
                break;
            case 2:
                menuTienda();
                break;
            case 3:
                std::cout << "Saliendo del aplicativo...\n";
                break;
            default:
                std::cout << "Opcion no valida.\n";
            }
        } while (opcionMenu != 3);
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
};