#pragma once

#include <string>
#include "Common.h"

class PuntDeInteresBase {

private:
	Coordinate m_coord;
	std::string m_name;

public:
	PuntDeInteresBase(); //Constructor b�sico.
	PuntDeInteresBase(Coordinate coord, std::string name); //Constructor de un punto de inter�s, no es una tienda ni un restaurante.

	//Getters
	Coordinate getCoord();
	virtual std::string getName();
	virtual unsigned int getColor();
};
