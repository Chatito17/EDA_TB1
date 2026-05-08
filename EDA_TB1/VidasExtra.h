#pragma once
#include <iostream>
#include <string>
#include "Producto.h"
#include "Usuario.h"

class VidasExtra : public Producto
{
public:
    VidasExtra() : Producto("Vidas Extra", "Restaura 3 vidas", 30) {}
    void activar(Usuario* user) override {
        user->sumarVidas(3);
    }
};