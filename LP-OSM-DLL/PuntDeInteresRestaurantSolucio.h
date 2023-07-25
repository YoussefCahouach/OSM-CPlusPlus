#pragma once
#include "PuntDeInteresBase.h"
#include <iostream>
#include <string>

using namespace std;

class PuntDeInteresRestaurantSolucio : public PuntDeInteresBase
{
public:
	PuntDeInteresRestaurantSolucio() : m_tipusCuina(""), m_facilitaAcces(false) {} //Constructor básico.

	PuntDeInteresRestaurantSolucio(Coordinate coord, string name, string tipusCuina, bool facilitaAcces) :
		PuntDeInteresBase(coord, name) {
		m_tipusCuina = tipusCuina; m_facilitaAcces = facilitaAcces;
	} //Constructor de un punto de interés, un restaurante.

	PuntDeInteresRestaurantSolucio(PuntDeInteresRestaurantSolucio& p) : PuntDeInteresBase(p.getCoord(), p.getName())
	{
		m_tipusCuina = p.m_tipusCuina; m_facilitaAcces = p.m_facilitaAcces;
	} //Constructor de coordenadas del punto de interés.

	~PuntDeInteresRestaurantSolucio() {} //Destructor

	//Getters
	unsigned int getColor();
	string getTipusCuina() { return m_tipusCuina; }
	bool getFacilitaAcces() { return m_facilitaAcces; }
	string getName() { return PuntDeInteresBase::getName(); }
	
	//Setters
	void setTipusCuina(const string& tipusCuina) { m_tipusCuina = tipusCuina; }
	void setFacilitaAcces(bool facilitaAcces) { m_facilitaAcces = facilitaAcces; }

	//Clon
	PuntDeInteresRestaurantSolucio* clone() { return new PuntDeInteresRestaurantSolucio(*this); }

private:
	string m_tipusCuina;
	bool m_facilitaAcces;
};

