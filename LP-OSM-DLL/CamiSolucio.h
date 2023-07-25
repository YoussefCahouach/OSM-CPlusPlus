#pragma once
#include "CamiBase.h"
#include <vector>
using namespace std;

class CamiSolucio : public CamiBase
{
public:
	CamiSolucio() {} //Constructor básico.
	~CamiSolucio() {} //Destructor.
	
	vector<Coordinate> getCamiCoords(); //Obtiene un vector de coordenadas que representan el camino almacenado en la clase
	
	void afegirCoordenades(Coordinate c) { m_camiCoords.push_back(c); } // Agrega una coordenada al final del vector de coordenadas del camino
	
	CamiSolucio* clone() { return new CamiSolucio(*this); } //Clone

private:
	vector<Coordinate> m_camiCoords;
};

