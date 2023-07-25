#pragma once

#include <string>
#include "Common.h"

class PuntDeInteresBase {

private:
	Coordinate m_coord;
	std::string m_name;

public:
	PuntDeInteresBase(); //Constructor básico.
	PuntDeInteresBase(Coordinate coord, std::string name); //Constructor de un punto de interés, no es una tienda ni un restaurante.

	//Getters
	Coordinate getCoord();
	virtual std::string getName();
	virtual unsigned int getColor();
};
