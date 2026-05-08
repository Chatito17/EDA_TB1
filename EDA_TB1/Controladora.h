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
            std::cout << "3. Rendir Examen\n";
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
                menuExamen();
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
        // --- PREGUNTAS DE ESCRIBIR ---
        banco->agregar(new PreguntaEscribir("Traduce 'Manzana' al ingles", "apple"));
        banco->agregar(new PreguntaEscribir("Traduce 'Perro' al ingles", "dog"));
        banco->agregar(new PreguntaEscribir("Traduce 'Gato' al ingles", "cat"));
        banco->agregar(new PreguntaEscribir("Traduce 'Libro' al ingles", "book"));
        banco->agregar(new PreguntaEscribir("Traduce 'Agua' al ingles", "water"));
        banco->agregar(new PreguntaEscribir("Traduce 'Rojo' al ingles", "red"));
        banco->agregar(new PreguntaEscribir("Traduce 'Azul' al ingles", "blue"));
        banco->agregar(new PreguntaEscribir("Traduce 'Verde' al ingles", "green"));

        // --- PREGUNTAS DE RELACIONAR ---
        // Concepto 1 -> Opcion 3 | Concepto 2 -> Opcion 1 | Concepto 3 -> Opcion 2 (Respuesta: 312)
        std::string conceptos1[] = { "Hello", "Goodbye", "Please" };
        std::string definiciones1[] = { "1. Adios", "2. Por favor", "3. Hola" };
        banco->agregar(new PreguntaRelacionar(
            "Escribe la secuencia de numeros de definicion correctos (Ej. 312)",
            "312", conceptos1, definiciones1, 3));

        // Concepto 1 -> Opcion 2 | Concepto 2 -> Opcion 3 | Concepto 3 -> Opcion 1 (Respuesta: 231)
        std::string conceptos2[] = { "One", "Two", "Three" };
        std::string definiciones2[] = { "1. Tres", "2. Uno", "3. Dos" };
        banco->agregar(new PreguntaRelacionar(
            "Escribe la secuencia de numeros de definicion correctos (Ej. 231)",
            "231", conceptos2, definiciones2, 3));

        std::string conceptos3[] = { "Car", "House", "Tree" };
        std::string definiciones3[] = { "1. Casa", "2. Arbol", "3. Coche" };
        banco->agregar(new PreguntaRelacionar(
            "Escribe la secuencia de numeros de definicion correctos (Ej. 312)",
            "312", conceptos3, definiciones3, 3));
    }

    void iniciarSimulacionLiga() {
        ligaActual->agregarParticipante(usuarioActual);
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

    // --- NUEVO METODO PARA ELEGIR TIPO DE EXAMEN ---
    void menuExamen() {
        int tipoExamen;
        std::cout << "\n=== SELECCION DE EXAMEN ===\n";
        std::cout << "1. Examen de Nivel (5 preguntas)\n";
        std::cout << "2. Examen de Etapa (7 preguntas)\n";
        std::cout << "3. Examen de Certificado (10 preguntas)\n";
        std::cout << "Elige el tipo de examen: ";
        std::cin >> tipoExamen;
        std::cin.ignore(); // Limpiar el buffer para la lectura de las respuestas

        Examen* examen = nullptr;
        int numPreguntas = 0;
        int multiplicadorExp = 15;
        int recomGemas = 50;

        // Polimorfismo: instanciamos el examen según la elección
        switch (tipoExamen) {
        case 1:
            examen = new ExamenNivel("Leccion Rapida");
            numPreguntas = 5;
            multiplicadorExp = 15;
            recomGemas = 50;
            break;
        case 2:
            examen = new ExamenEtapa(1);
            numPreguntas = 7;
            multiplicadorExp = 20;
            recomGemas = 70;
            break;
        case 3:
            examen = new ExamenCertificado();
            numPreguntas = 10;
            multiplicadorExp = 25;
            recomGemas = 100;
            break;
        default:
            std::cout << "Opcion invalida, seleccionando Examen de Nivel por defecto.\n";
            examen = new ExamenNivel("Leccion Rapida");
            numPreguntas = 5;
            break;
        }

        // Se extraen exactamente la cantidad de preguntas requeridas para el examen
        Pila<Pregunta*> preguntasExamen = banco->seleccionarParaExamen(numPreguntas, [](Pregunta* p) {
            return true; // Acepta todas
            });

        // Ejecuta el examen con la lógica propia del hijo
        ResultadoDetallado res = examen->hacerExamen(preguntasExamen);

        // Subir EXP en base a la dificultad del examen
        if (res.getPuntaje() > 0) {
            int expGanada = res.getPuntaje() * multiplicadorExp;
            usuarioActual->sumarExp(expGanada);
            std::cout << ">>> Has ganado " << expGanada << " de EXP! Revisa la clasificacion <<<\n";
        }

        // Si acierta más de la mitad, se lleva gemas de recompensa proporcionales
        if (res.getPuntaje() >= (numPreguntas / 2 + 1)) {
            std::cout << ">>> Has ganado " << recomGemas << " gemas extra por tu buen rendimiento! <<<\n";
            usuarioActual->sumarGemas(recomGemas);
        }

        delete examen; // Liberamos la memoria del polimorfismo
    }
};