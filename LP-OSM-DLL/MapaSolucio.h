#pragma once

#include "MapaBase.h"
#include "CamiSolucio.h"
#include "PuntDeInteresBotigaSolucio.h"
#include "PuntDeInteresRestaurantSolucio.h"
#include "BallTree.h"
#include "GrafSolucio.h"
#include "Common.h"
#include "Util.h"
#include <algorithm>
#include <iostream>
using namespace std;

class MapaSolucio : public MapaBase {
public:
	MapaSolucio() {
		//Constructor básic.
	}

	~MapaSolucio() {
		//Destructor.
	}

	// Metodos a implementar de la primera parte.
	void getPdis(std::vector<PuntDeInteresBase*>&);
	void getCamins(std::vector<CamiBase*>&);
	void parsejaXmlElements(std::vector<XmlElement>& xmlElements);

	//Funciones extras para hacer funcionar parsejaXmlElements.
	void desarCoordenades(Coordinate& coord, std::vector<PAIR_ATTR_VALUE> atributs);
	void cercaCamiRef(string ref, vector<XmlElement> xmlElements, Coordinate& c);
	PuntDeInteresBase* possibleUndef(std::vector<CHILD_NODE> fills, Coordinate& c, bool& fill);
	PuntDeInteresBotigaSolucio* cercaBotiga(std::vector<CHILD_NODE> fills, Coordinate& c);
	PuntDeInteresRestaurantSolucio* cercaRestaurant(std::vector<CHILD_NODE> fills, Coordinate& c);

	// Metodo a implementar de la segunda parte.
	CamiBase* buscaCamiMesCurt(PuntDeInteresBase* desde, PuntDeInteresBase* a);

private:
	// TODO: Añade los atributos que creas necesarios para "MapaSolucio".
	vector<PuntDeInteresBase*> m_puntInteres;
	vector<CamiBase*> m_camins;
};
