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
        // 2. GUARDAR PROGRESO AL SALIR
        usuarioActual->guardarProgreso();

        // 3. GUARDAR EL RANKING DE LA LIGA AL SALIR
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
                    usuarioActual->agregarNotificacion("Has comprado: " + productoElegido->getNombre());
                }
                else {
                    std::cout << "Producto inexistente.\n";
                }
            }
        } while (opcionTienda != 0);

    }

    // --- NUEVO METODO PARA ELEGIR TIPO DE EXAMEN ---
    void menuExamen() {
        // Validación 1: Verificar Vidas
        if (usuarioActual->getVidas() <= 0) {
            std::cout << "\n[!] No te quedan corazones. Ve a la tienda a comprar vidas extra para continuar aprendiendo.\n";
            return;
        }

        // Validación 2: Verificar inscripción al curso
        Curso* c = usuarioActual->getCursoActual();
        if (c == nullptr) {
            std::cout << "\n[!] Debes inscribirte a un curso primero (Opcion 2 del menu).\n";
            return;
        }
        if (usuarioActual->getEtapaActual() >= c->getCantidadEtapas()) {
            std::cout << "\n[!] ¡Felicidades! Ya has completado todo el curso de " << c->getIdioma() << ".\n";
            return;
        }

        int tipoExamen;
        std::cout << "\n=== SELECCION DE EXAMEN ===\n";
        std::cout << "1. Examen de Nivel (Leccion Normal)\n";
        std::cout << "2. Examen de Etapa (Salto rapido)\n";
        std::cout << "3. Examen de Certificado (Final)\n";
        std::cout << "Elige el tipo de examen: ";
        std::cin >> tipoExamen;
        std::cin.ignore();

        Examen* examen = nullptr;

        // Instanciación Polimórfica (Ya no hardcodeamos las variables aquí)
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
            tipoExamen = 1;
            break;
        }

        Etapa* eActual = c->getEtapa(usuarioActual->getEtapaActual());
        Seccion* sActual = eActual->getSeccion(usuarioActual->getSeccionActual());
        Nivel* nActual = sActual->getNivel(usuarioActual->getNivelActual());
        BancoPreguntas* bancoLocal = nActual->getBancoPreguntas();

        // Extraer las preguntas del banco local según la cantidad dictada por el examen
        Pila<Pregunta*> preguntasExamen = bancoLocal->seleccionarParaExamen(examen->getCantPreguntas(), [](Pregunta* p) {
            return true;
            });

        // Ejecutar enviando al usuario
        ResultadoDetallado res = examen->hacerExamen(preguntasExamen, usuarioActual);

        // Lógica de Recompensas y Avance
        int expGanada = res.getPuntaje() * examen->getMultiplicadorExp();
        if (expGanada > 0) {
            usuarioActual->sumarExp(expGanada);
            std::cout << ">>> Has ganado " << expGanada << " de EXP! Revisa el Ranking de Ligas <<<\n";
        }

        // Se aprueba con más del 50%
        int mitad = (examen->getCantPreguntas() / 2) + 1;

        if (res.getPuntaje() >= mitad && usuarioActual->getVidas() > 0) {
            std::cout << "\n>>> ¡PRUEBA SUPERADA! <<<\n";
            if (tipoExamen == 1) {
                usuarioActual->avanzarNivel();
            }
            // Aquí puedes agregar la lógica si pasa el de Etapa para llamar a usuarioActual->avanzarEtapa()
            usuarioActual->agregarNotificacion("¡Felicidades! Ganaste " + std::to_string(examen->getRecomGemas()) + " gemas.");
            usuarioActual->sumarGemas(examen->getRecomGemas());
        }
        else if (usuarioActual->getVidas() > 0) {
            std::cout << "\n>>> No lograste el puntaje minimo. ¡Sigue intentando! <<<\n";
        }

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

        // --- 4. CARGAMOS LAS PREGUNTAS DESDE EL ARCHIVO TXT ---
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