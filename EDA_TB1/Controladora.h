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
        usuarioActual = new Usuario("Anfitrion");

        usuarioActual->cargarProgreso();

        tienda = new Tienda();
        ligaActual = new Division(Bronce);

        inicializarCursos();
        iniciarSimulacionLiga();
    }

    ~Controladora() {
        usuarioActual->guardarProgreso();

        ligaActual->getRanking()->guardarTabla(ligaActual->getNombreStr());

        delete usuarioActual;
        delete tienda;
        delete ligaActual;

        for (int i = 0; i < competidores.getLongitud(); i++) {
            delete competidores.GetPos(i);
        }
        for (int i = 0; i < catalogoCursos.getLongitud(); i++) {
            delete catalogoCursos.GetPos(i);
        }
    }

    void iniciar() {
        int opcionMenu;
        do {
            std::cout << "\n=== MENU PRINCIPAL ===\n";
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
                usuarioActual->verProgreso();
                break;
            case 2:
                menuCursos();
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

   void menuExamen() {
        if (usuarioActual->getVidas() <= 0) {
            std::cout << "\nNo tienes vidas, intentelo despues.\n";
            return;
        }

        Curso* c = usuarioActual->getCursoActual();
        if (c == nullptr) {
            std::cout << "\nDebes inscribirte a un curso primero.\n";
            return;
        }
        if (usuarioActual->getEtapaActual() >= c->getCantidadEtapas()) {
            std::cout << "\nFelicidades! Ya has completado todo el curso de " << c->getIdioma() << ".\n";
            return;
        }

        int tipoExamen;
        std::cout << "\n=== SELECCION DE EXAMEN ===\n";
        std::cout << "1. Examen de Nivel\n";
        std::cout << "2. Examen de Etapa\n";
        std::cout << "3. Examen de Certificado\n";
        std::cout << "Elige el tipo de examen: ";
        std::cin >> tipoExamen;
        std::cin.ignore();

        Examen* examen = nullptr;

        switch (tipoExamen) {
        case 1:
            examen = new ExamenNivel();
            break;
        case 2:
            examen = new ExamenEtapa();
            break;
        case 3:
            examen = new ExamenCertificado();
            break;
        default:
            std::cout << "Opcion invalida, seleccionando Examen de Nivel por defecto.\n";
            examen = new ExamenNivel();
            break;
        }

        Etapa* eActual = c->getEtapa(usuarioActual->getEtapaActual());
        Seccion* sActual = eActual->getSeccion(usuarioActual->getSeccionActual());
        Nivel* nActual = sActual->getNivel(usuarioActual->getNivelActual());
        BancoPreguntas* bancoLocal = nActual->getBancoPreguntas();

        Pila<Pregunta*> preguntasExamen = bancoLocal->seleccionarParaExamen(examen->getCantPreguntas(), [](Pregunta* p) {
            return true;
            });

        ResultadoDetallado res = examen->hacerExamen(preguntasExamen, usuarioActual);

        delete examen;
    }
    void menuCursos() {
        std::cout << "\n=== CATALOGO DE CURSOS ===\n";
        if (catalogoCursos.estaVacia()) {
            std::cout << "No hay cursos disponibles por el momento.\n";
            return;
        }

        for (int i = 0; i < catalogoCursos.getLongitud(); i++) {
            std::cout << i + 1 << ". Curso de " << catalogoCursos.GetPos(i)->getIdioma() << "\n";
        }
        std::cout << "0. Volver\n";
        std::cout << "==========================\n";
        std::cout << "Elige un curso para inscribirte: ";

        int opcion;
        std::cin >> opcion;
        std::cin.ignore();

        if (opcion > 0 && opcion <= catalogoCursos.getLongitud()) {
            Curso* cursoElegido = catalogoCursos.GetPos(opcion - 1);
            usuarioActual->inscribirseCurso(cursoElegido);
        }
    }
    void inicializarCursos() {
        Curso* cursoIngles = new Curso("Ingles");
        Etapa* etapa1 = new Etapa("Principiante");
        Seccion* sec1 = new Seccion("Saludos e Introduccion");
        Nivel* nivel1 = new Nivel("Leccion 1: Lo basico");

        nivel1->getBancoPreguntas()->cargarDesdeArchivo("Preguntas_Ingles.txt");

        sec1->agregarNivel(nivel1);
        etapa1->agregarSeccion(sec1);
        cursoIngles->agregarEtapa(etapa1);
        catalogoCursos.AddLast(cursoIngles);

        if (usuarioActual->getCursoActual() == nullptr) {
            usuarioActual->inscribirseCurso(cursoIngles);
        }
    }
};