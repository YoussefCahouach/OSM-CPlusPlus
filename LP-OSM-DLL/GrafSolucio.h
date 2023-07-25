#pragma once
#include <string>
#include <vector>
#include <list>
#include <limits>
#include <cstdio>
#include <stack>
#include <limits>
#include <algorithm>
#include "CamiSolucio.h"
#include "Common.h"
#include "Util.h"
using namespace std;

class GrafSolucio
{
public:
	GrafSolucio() : m_numNodes(0), m_numArestes(0) {} //Constructor básico.
	GrafSolucio(const vector<CamiBase*> llistaCamins); //Constructor que recibe un vector de punteros a objetos de la clase CamiBase
	~GrafSolucio() {} //Destructor.
	
	void inserirAresta(int posNode1, int posNode2, double pes); //Insertar arista entre dos nodos en el grafo.
	int obtenirIndex(Coordinate coord); //Obtener el índice de un nodo dado su coordenada.
	void dijkstra(int node1, int node2, vector<double>& dist, vector<int>& anterior); //Calcula la ruta más corta entre dos nodos.
	int minDistance(const vector<double>& dist, const vector<bool>& visitat) const; //Encuentra el nodo con la distancia mínima.
	
	//Getters
	int getNumNodes() { return m_numNodes; }
	Coordinate getNode(int index);
	vector<Coordinate> getNodes();

private:
	vector<list<pair<int, double>>> m_veins; //Para cada vecino <index_node,peso> (peso=1 si no ponderat)
	vector<Coordinate> m_nodes;
	int m_numNodes;
	int m_numArestes;
	const double DISTMAX = DBL_MAX;
};



	
	
	


