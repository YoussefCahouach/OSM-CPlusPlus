#pragma once

#include "Common.h"
#include <vector>

class CamiBase {
public:
	virtual std::vector<Coordinate> getCamiCoords() = 0; // Obtiene un vector de coordenadas que representan el camino almacenado en la clase
	virtual void afegirCoordenades(Coordinate c) = 0; // Agrega una coordenada al final del vector de coordenadas del camino

	virtual CamiBase* clone() = 0; //Clon
};

