#pragma once
#include <iostream>
#include <string>

#include "Producto.h"
#include "Usuario.h"

class DuplicadorExp : public Producto
{
public:
    DuplicadorExp() : Producto("Duplicador de EXP", "Duplica la EXP ganada hoy", 15) {}
    void activar(Usuario* user) override {
        user->activarDuplicador();
    }
};