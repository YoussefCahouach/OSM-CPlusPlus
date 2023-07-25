#pragma once
#include "PuntDeInteresBase.h"
using namespace std;

class PuntDeInteresBotigaSolucio : public PuntDeInteresBase
{
public:
	PuntDeInteresBotigaSolucio() : m_tipusBotiga(""), m_openingHours(""), m_wheelchair("") {} //Constructor básico

	PuntDeInteresBotigaSolucio(Coordinate coord, string name, string tipusBotiga, string openingHours, string wheelchair) :
		PuntDeInteresBase(coord, name), m_tipusBotiga(tipusBotiga), m_openingHours(openingHours), m_wheelchair(wheelchair) { }
	//Constructor del punto de interés, una tienda.

	PuntDeInteresBotigaSolucio(PuntDeInteresBotigaSolucio& p) : PuntDeInteresBase(p.getCoord(), p.getName())
	{
		m_tipusBotiga = p.m_tipusBotiga; m_openingHours = p.m_openingHours; m_wheelchair = p.m_wheelchair;
	} //Constructor de coordenadas del punto de interés.

	~PuntDeInteresBotigaSolucio() {} //Destructor.

	//Getters
	unsigned int getColor();
	string getTipusBotiga() { return m_tipusBotiga; }
	string getName() { return PuntDeInteresBase::getName(); }

	//Setters
	void setTipusBotiga(const string& tipusBotiga) { m_tipusBotiga = tipusBotiga; }

	//Clon
	PuntDeInteresBotigaSolucio* clone() { return new PuntDeInteresBotigaSolucio(*this); }

private:
	string m_tipusBotiga;
	string m_openingHours;
	string m_wheelchair;
};

