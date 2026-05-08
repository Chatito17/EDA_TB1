#pragma once
#include "Ranking.h"
enum Divisiones { Bronce, Plata, Oro, Zafiro, Rubi, Esmeralda, Amatista, Perla, Obsidiana, Diamante };

class Division
{
private:
	Divisiones nombreDivision;
	Ranking ranking;
public:
	Division() {};
	~Division() {};
};