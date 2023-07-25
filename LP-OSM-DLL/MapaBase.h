#pragma once
#include "pch.h"
#include "Common.h"
#include "PuntDeInteresBase.h"
#include "CamiBase.h"

class MapaBase {

public:
	MapaBase() {} //Constructor básico.
	virtual ~MapaBase() {} //Destructor.

	// Metodos a añadir de la primera parte.
	virtual void getPdis(std::vector<PuntDeInteresBase*>&) = 0;
	virtual void getCamins(std::vector<CamiBase*>&) = 0;
	virtual void parsejaXmlElements(std::vector<XmlElement>& xmlElements) = 0;

	// Metodo adicional para implementar en la segunda parte.
	virtual CamiBase* buscaCamiMesCurt(PuntDeInteresBase* desde, PuntDeInteresBase* a) = 0;
};

