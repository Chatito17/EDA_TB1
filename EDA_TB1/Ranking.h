#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Usuario.h"
class Ranking
{
private:
	std::vector<Usuario> listaUsuarios;


public:
	Ranking() {};
	~Ranking() {};
	void agregarUsuario(Usuario usuario)
	{
		listaUsuarios.push_back(usuario);
	}
	void mostrarPrimeros()
	{
		std::cout << listaUsuarios[0].getNombre();
	}
	void mostrarRanking()
	{
		for (int i = 0; i < listaUsuarios.size(); i++) std::cout << listaUsuarios[i].getNombre() << std::endl;
	}
	void ordenarRanking()
	{

	}


};

