#include "pch.h"
#include "MapaSolucio.h"
#include <map>
#include <unordered_map>

void MapaSolucio::getPdis(std::vector<PuntDeInteresBase*>& pdis)
{
	vector<PuntDeInteresBase*>::iterator it;
	for (it = m_puntInteres.begin(); it != m_puntInteres.end(); it++)
		pdis.push_back((*it));

}

void MapaSolucio::getCamins(std::vector<CamiBase*>& camins)
{
	vector<CamiBase*>::iterator it;
	for (it = m_camins.begin(); it != m_camins.end(); it++)
		camins.push_back((*it)->clone());

}

void MapaSolucio::desarCoordenades(Coordinate& coord, std::vector<PAIR_ATTR_VALUE> atributs)
{
	coord.lat = 0.0;
	coord.lon = 0.0;

	for (const auto& attr : atributs)
	{
		if (attr.first == "lat")
		{
			coord.lat = std::stod(attr.second);
		}
		else if (attr.first == "lon")
		{
			coord.lon = std::stod(attr.second);
		}

		if (coord.lat != 0.0 && coord.lon != 0.0)
		{
			break;
		}
	}

}
void MapaSolucio::cercaCamiRef(string ref, vector<XmlElement> xmlElements, Coordinate& c)
{
	bool atributTrobat = false;

	for (int iXML = 0; iXML < xmlElements.size() && !atributTrobat; ++iXML)
	{
		if (xmlElements[iXML].id_element == "node")
		{
			for (int jAtribut = 0; jAtribut < xmlElements[iXML].atributs.size(); ++jAtribut)
			{
				if (xmlElements[iXML].atributs[jAtribut].first == "id" && xmlElements[iXML].atributs[jAtribut].second == ref)
				{
					desarCoordenades(c, xmlElements[iXML].atributs);
					atributTrobat = true;
					break;
				}
			}
		}
	}

	if (!atributTrobat)
	{
		c.lat = 0.0;
		c.lon = 0.0;
	}
}
PuntDeInteresBase* MapaSolucio::possibleUndef(std::vector<CHILD_NODE> fills, Coordinate& c, bool& fill)
{
	// Create a map of tag names to their values.
	std::map<std::string, std::string> tagValues;

	// This vector will hold the names of all the tags that we want to check for.
	std::vector<std::string> tagsToCheck = { "highway", "public_Transport", "access", "entrances" };

	// Initialize the fill flag to true. We will set it to false if we find any of the tags in the 'tagsToCheck' vector.
	fill = true;

	// Loop through all the fills and extract the tag names and values.
	for (auto& fill_data : fills)
	{
		if (fill_data.first == "tag")
		{
			auto tagNameValue = Util::kvDeTag(fill_data.second);
			auto tagName = tagNameValue.first;
			auto tagValue = tagNameValue.second;

			// If this tag has a name and a non-empty value, store it in
			// the tagValues map.
			if (!tagName.empty() && !tagValue.empty())
			{
				tagValues[tagName] = tagValue;
			}

			// If this tag is one of the tags we want to check for, set
			// the fill flag to false.
			if (std::find(tagsToCheck.begin(), tagsToCheck.end(), tagName) != tagsToCheck.end())
			{
				fill = false;
			}
		}
	}

	std::string name = "";
	// Check if the "name" tag is present in the tagValues map.
	if (tagValues.count("name") > 0)
		name = tagValues["name"];

	// Create a new PuntDeInteresBase object with the given coordinate and name.
	return new PuntDeInteresBase(c, name);

}
PuntDeInteresBotigaSolucio* MapaSolucio::cercaBotiga(std::vector<CHILD_NODE> fills, Coordinate& c)
{
	// Create local variables.
	std::string name;
	std::string tipusBotiga;
	std::string wheelchair;
	std::string openingHours;

	// Loop through all elements in the 'fills' vector
	for (auto& fill : fills) {
		// If the current element is a 'tag' element
		if (fill.first == "tag")
		{
			auto valorTag = Util::kvDeTag(fill.second);
			if (valorTag.first == "name" && !valorTag.second.empty())
			{
				name = valorTag.second;
			}
			else
				if (valorTag.first == "shop")
				{
					tipusBotiga = valorTag.second;
				}
				else
					if (valorTag.second == "yes" && valorTag.first == "wheelchair")
					{
						wheelchair = "yes";
					}
					else
						if (valorTag.first == "opening_hours" && valorTag.second.find("06:00-22:00") != std::string::npos)
						{
							openingHours = valorTag.second;
						}

			if (!name.empty() && !tipusBotiga.empty() && !wheelchair.empty() && !openingHours.empty())
				break;
		}
	}

	return new PuntDeInteresBotigaSolucio(c, name, tipusBotiga, openingHours, wheelchair);
}
PuntDeInteresRestaurantSolucio* MapaSolucio::cercaRestaurant(std::vector<CHILD_NODE> fills, Coordinate& c)
{
	int i = 0;
	int fillsSize = fills.size();
	bool nomTrobat = false;
	bool accesTrobat = false;
	bool tipusTrobat = false;
	string name = "";
	string tipusCuina = "";
	bool facilitaAcces = false;
	std::map<std::string, std::string> tags;

	while (i < fillsSize)
	{
		if (fills[i].first == "tag")
		{
			std::pair<std::string, std::string> valorTag = Util::kvDeTag(fills[i].second);
			tags[valorTag.first] = valorTag.second;
		}

		if (!nomTrobat && tags.count("name") && tags["name"] != "")
		{
			name = tags["name"];
			nomTrobat = true;
		}
		else if (!accesTrobat && tags.count("wheelchair") && tags["wheelchair"] == "yes")
		{
			facilitaAcces = true;
			accesTrobat = true;
		}
		else if (!tipusTrobat && tags.count("cuisine"))
		{
			tipusCuina = tags["cuisine"];
			tipusTrobat = true;
		}

		if (nomTrobat && accesTrobat && tipusTrobat)
			break;

		i++;
	}

	return new PuntDeInteresRestaurantSolucio(c, name, tipusCuina, facilitaAcces);
}


void MapaSolucio::parsejaXmlElements(std::vector<XmlElement>& xmlElements)
{
	string ref = "";
	Coordinate coord;
	bool fillTrobat = false;
	bool camiTrobat = false;
	PuntDeInteresBase* puntInteresBase;
	std::pair<std::string, std::string> valorTag;

	if (!m_puntInteres.empty())
	{
		for (auto it : m_puntInteres)
			delete (it);
		m_puntInteres.clear();
	}
	if (!m_camins.empty())
	{
		for (auto it : m_camins)
			delete (it);
		m_camins.clear();
	}
	for (XmlElement& element : xmlElements)
	{
		if (element.id_element == "node")
		{
			int jFill = 0;
			fillTrobat = false;
			puntInteresBase = nullptr;

			while (!(fillTrobat) && jFill < element.fills.size())
			{
				if (element.fills[jFill].first == "tag")
				{
					desarCoordenades(coord, element.atributs);
					valorTag = Util::kvDeTag(element.fills[jFill].second);
					if (valorTag.first == "shop")
					{
						if (puntInteresBase == nullptr)
						{
							puntInteresBase = cercaBotiga(element.fills, coord);
						}
						if (puntInteresBase->getName() != "")
						{
							fillTrobat = true;
						}

					}
					else if (valorTag.second == "restaurant")
					{
						if (puntInteresBase == nullptr)
						{
							puntInteresBase = cercaRestaurant(element.fills, coord);
						}
						if (puntInteresBase->getName() != "")
						{
							fillTrobat = true;
						}
					}
					else
					{
						if (jFill == element.fills.size() - 1)
						{
							puntInteresBase = possibleUndef(element.fills, coord, fillTrobat);
						}
					}
				}
				if (!fillTrobat)
					jFill++;
			}
			if (fillTrobat)
			{
				m_puntInteres.push_back(puntInteresBase);
			}
		}
		else
			if (element.id_element == "way")
			{
				int jCami = 0;
				camiTrobat = false;
				while (jCami < element.fills.size())
				{
					if (element.fills[jCami].first == "tag")
					{
						valorTag = Util::kvDeTag(element.fills[jCami].second);
						if (valorTag.first == "highway")
						{
							CamiSolucio* cami = new CamiSolucio();
							for (const auto& fill : element.fills)
							{
								if (fill.first == "nd" && fill.second[0].first == "ref")
								{
									ref = fill.second[0].second;
									cercaCamiRef(ref, xmlElements, coord);
									(*cami).afegirCoordenades(coord);
								}
							}
							m_camins.push_back(cami);
							camiTrobat = true;
							break;
						}
					}
					jCami++;
				}
			}
	}
}
CamiBase* MapaSolucio::buscaCamiMesCurt(PuntDeInteresBase* desde, PuntDeInteresBase* a) {
	// Comprueba si los puntos de interés recibidos son válidos
	if (desde == nullptr || a == nullptr) 
		return nullptr;

	// Obtenemos todos los caminos del mapa.
	vector<CamiBase*> camins;
	getCamins(camins);

	// Construimos un grafo con los caminos del mapa.
	GrafSolucio graf(camins);

	// Creamos dos objetos CamiSolucio para almacenar el camino encontrado.
	CamiBase* caminoInvertido = new CamiSolucio;
	CamiBase* caminoFinal = new CamiSolucio;

	// Inicializamos la coordenada Q con latitud y longitud iguales a 0.0.
	Coordinate QOrg = Coordinate{ 0.0, 0.0 };
	Coordinate QDest = Coordinate{ 0.0, 0.0 };
	
	// Llamamos a la función nodeMesProper para encontrar el nodo más cercano al punto de destino en el árbol.
	BallTree arbol;
	arbol.construirArbre(graf.getNodes());

	// Obtenemos las coordenadas de ambos puntos de interés.
	QOrg = arbol.nodeMesProper(desde->getCoord(), QOrg, &arbol);
	QDest = arbol.nodeMesProper(a->getCoord(), QDest, &arbol);
	
	// Obtenemos los índices de los puntos de origen y destino en el grafo.
	int indexOrigen = graf.obtenirIndex(QOrg);
	int indexDestino = graf.obtenirIndex(QDest);
	
	// Inicializamos los vectores de distancias y rutas anteriores.
	vector<double> dist;
	vector<int> anterior;

	// Aplicamos la función dijkstra para encontrar el camino más corto entre los dos puntos.
	graf.dijkstra(indexOrigen, indexDestino, dist, anterior);

	
	// Construye un objeto CamiSolucio con el resultado del camino más corto
	int pos = indexDestino;
	caminoInvertido->afegirCoordenades(graf.getNode(pos)); 
	while (pos != indexOrigen) 
	{
		caminoInvertido->afegirCoordenades(graf.getNode(anterior[pos]));
		pos = anterior[pos];
	}

	// Invertimos el orden de las coordenadas del camino encontrado.
	stack<Coordinate> pila;
	for (int i = 0; i < caminoInvertido->getCamiCoords().size(); i++)
		pila.push(caminoInvertido->getCamiCoords()[i]);
	while (!pila.empty())
	{
		caminoFinal->afegirCoordenades(pila.top());
		pila.pop();
	}
		
	// Finalmente, devolvemos el camino encontrado.
	return caminoFinal;
}
