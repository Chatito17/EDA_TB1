#pragma once
#include <iostream>
#include <string>
#include "Producto.h"
#include "Usuario.h"

class ActivarPlus : public Producto
{
public:
    ActivarPlus() : Producto("Super Plus", "Activa los beneficios Plus", 80) {}
    void activar(Usuario* user) override {
        user->setUsuarioPlus(true);
    }
};