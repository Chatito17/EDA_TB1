#pragma once
#include <iostream>
#include <string>

#include "Tienda.h"
#include "Usuario.h"
#include "Producto.h"
#include "Division.h"
#include "BancoPreguntas.h"
#include "Examen.h"
#include "Curso.h"

class Controladora
{
private:
    Usuario* usuarioActual;
    Tienda* tienda;
    Division* ligaActual;

    LinkedList<Usuario*> competidores;
    LinkedList<Curso*> catalogoCursos;

public:
    Controladora() {
        usuarioActual = new Usuario("Cristian");
        tienda = new Tienda();
        ligaActual = new Division(Bronce);

        inicializarCursos();
        iniciarSimulacionLiga();
    }

    ~Controladora() {
        delete usuarioActual;
        delete tienda;
        delete ligaActual;
        for (int i = 0; i < competidores.Length(); i++) {
            delete competidores.GetPos(i);
        }
        
        for (int i = 0; i < catalogoCursos.Length(); i++) {
            delete catalogoCursos.GetPos(i);
        }
    }

    void iniciar() {
        int opcionMenu;
        do {
            std::cout << "\n[ MENU PRINCIPAL - CLON DUOLINGO ]\n";
            std::cout << "1. Ver Perfil y Progreso actual\n";
            std::cout << "2. Explorar e Inscribirse a Cursos\n"; 
            std::cout << "3. Entrar a la Tienda\n";
            std::cout << "4. Rendir Examen\n";
            std::cout << "5. Ver Clasificacion de la Liga\n";
            std::cout << "6. Salir del programa\n";
            std::cout << "Elige una opcion: ";
            std::cin >> opcionMenu;
            std::cin.ignore();

            switch(opcionMenu) {
            case 1:
                usuarioActual->verPerfil();
                usuarioActual->verProgreso(); // Muestra nivel, etapa, etc.
                break;
            case 2:
                menuCursos(); // Llama al nuevo menú
                break;
            case 3:
                menuTienda();
                break;
            case 4:
                menuExamen();
                break;
            case 5:
                ligaActual->mostrarTablaDivision();
                break;
            case 6:
                std::cout << "Saliendo del aplicativo...\n";
                break;
            default:
                std::cout << "Opcion no valida.\n";
            }
        } while (opcionMenu != 6);
    }

private:
    void inicializarCursos() {
        Curso* cursoIngles = new Curso("Ingles");

        Etapa* etapa1 = new Etapa("Principiante");
        Seccion* sec1 = new Seccion("Saludos e Introduccion");

        Nivel* nivel1 = new Nivel("Leccion 1: Lo basico");
        // Agregamos preguntas al Nivel 1 en vez de al banco global
        nivel1->agregarPregunta(new PreguntaEscribir("Traduce 'Manzana' al ingles", "apple"));
        nivel1->agregarPregunta(new PreguntaEscribir("Traduce 'Perro' al ingles", "dog"));
        nivel1->agregarPregunta(new PreguntaEscribir("Traduce 'Gato' al ingles", "cat"));
        nivel1->agregarPregunta(new PreguntaEscribir("Traduce 'Agua' al ingles", "water"));
        nivel1->agregarPregunta(new PreguntaEscribir("Traduce 'Libro' al ingles", "book"));

        Nivel* nivel2 = new Nivel("Leccion 2: Colores y Relaciones");
        nivel2->agregarPregunta(new PreguntaEscribir("Traduce 'Rojo' al ingles", "red"));
        nivel2->agregarPregunta(new PreguntaEscribir("Traduce 'Azul' al ingles", "blue"));

        std::string con1[] = { "Hello", "Goodbye", "Please" };
        std::string def1[] = { "1. Adios", "2. Por favor", "3. Hola" };
        nivel2->agregarPregunta(new PreguntaRelacionar(
            "Escribe la secuencia (Ej. 312)", "312", con1, def1, 3));

        // Ensamblamos la estructura
        sec1->agregarNivel(nivel1);
        sec1->agregarNivel(nivel2);
        etapa1->agregarSeccion(sec1);
        cursoIngles->agregarEtapa(etapa1);

        catalogoCursos.AddLast(cursoIngles);

        // Inscribimos al usuario por defecto al primer curso
        usuarioActual->inscribirseCurso(cursoIngles);
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
        Curso* c = usuarioActual->getCursoActual();
        if (c == nullptr) {
            std::cout << "\n[!] Debes inscribirte a un curso primero (Opcion 2 del menu).\n";
            return;
        }
        if (usuarioActual->getEtapaActual() >= c->getCantidadEtapas()) {
            std::cout << "\n[!] ¡Felicidades! Ya has completado todo el curso de " << c->getIdioma() << ".\n";
            return;
        }
        Etapa* eActual = c->getEtapa(usuarioActual->getEtapaActual());
        Seccion* sActual = eActual->getSeccion(usuarioActual->getSeccionActual());
        Nivel* nActual = sActual->getNivel(usuarioActual->getNivelActual());

        BancoPreguntas* bancoLocal = nActual->getBancoPreguntas();

        Pila<Pregunta*> preguntasExamen = bancoLocal->seleccionarParaExamen(numPreguntas, [](Pregunta* p) {
            return true;
            });
        // Ejecuta el examen con la lógica propia del hijo
        ResultadoDetallado res = examen->hacerExamen(preguntasExamen);

        // Subir EXP en base a la dificultad del examen
        if (res.getPuntaje() > 0) {
            int expGanada = res.getPuntaje() * multiplicadorExp;
            usuarioActual->sumarExp(expGanada);
            std::cout << ">>> No has pasado al siguiente nivel! <<<\n";
            std::cout << ">>> Has ganado " << expGanada << " de EXP! Revisa la clasificacion <<<\n";
        }

        if (res.getPuntaje() >= (numPreguntas / 2 + 1)) {
            std::cout << ">>> Has pasado al siguiente nivel! <<<\n";
            if (tipoExamen == 1) {
                usuarioActual->avanzarNivel();
            }
            std::cout << ">>> Has ganado " << recomGemas << " gemas extra por tu buen rendimiento! <<<\n";
            usuarioActual->sumarGemas(recomGemas);
        }

        delete examen;
    }
    void menuCursos() {
        std::cout << "\n=== CATALOGO DE CURSOS ===\n";
        if (catalogoCursos.IsEmpty()) {
            std::cout << "No hay cursos disponibles por el momento.\n";
            return;
        }

        for (int i = 0; i < catalogoCursos.Length(); i++) {
            std::cout << i + 1 << ". Curso de " << catalogoCursos.GetPos(i)->getIdioma() << "\n";
        }
        std::cout << "0. Volver\n";
        std::cout << "==========================\n";
        std::cout << "Elige un curso para inscribirte: ";

        int opcion;
        std::cin >> opcion;
        std::cin.ignore();

        if (opcion > 0 && opcion <= catalogoCursos.Length()) {
            Curso* cursoElegido = catalogoCursos.GetPos(opcion - 1);
            usuarioActual->inscribirseCurso(cursoElegido);
        }
    }
};