#pragma once
#include <iostream>

class Usuario;

class Producto
{
protected:
    std::string nombre;
    std::string detalle;
    int costo;

public:
    Producto(std::string _nombre, std::string _detalle, int _costo)
        : nombre(_nombre), detalle(_detalle), costo(_costo) {
    }
    virtual ~Producto() {}

    virtual void activar(Usuario* usuario) {};

    std::string getNombre() { return nombre; }
    std::string getDetalle() { return detalle; }
    int getCosto() { return costo; }
};