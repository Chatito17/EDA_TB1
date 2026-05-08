#pragma once
#include <iostream>
#include <string>
#include "Producto.h"
#include "Usuario.h"

class ProtectorRacha : public Producto
{
public:
    ProtectorRacha() : Producto("Protector de Racha", "Evita perder la racha por un dia", 20) {}
    void activar(Usuario* user) override {
        user->setProtectorRacha(true);
    }
};