#include "pch.h"
#include "GrafSolucio.h"

bool operator==(const Coordinate& a, const Coordinate& b) {
	return a.lat == b.lat && a.lon == b.lon;
}

Coordinate GrafSolucio::getNode(int index)
{
	return m_nodes[index];
}

vector<Coordinate> GrafSolucio::getNodes()
{
	return m_nodes;
}

int GrafSolucio::obtenirIndex(Coordinate coord)
{
	// Recorremos la lista de nodos
	for (int i = 0; i < m_nodes.size(); i++) // Si encontramos el nodo con la coordenada especificada, devolvemos su índice.
		if (m_nodes[i] == coord)
			return i;
	// Si no se encontró el nodo, devolvemos -1.
	return -1;
}

GrafSolucio::GrafSolucio(const vector<CamiBase*> llistaCamins) : m_numNodes(0), m_numArestes(0)
{
	// Inicializar el vector de nodos.
	m_nodes = vector<Coordinate>();
	// Agregar los nodos del camino a la lista de nodos, evitando los nodos repetidos.
	for (auto& it_camins : llistaCamins)
	{
		if (it_camins != nullptr)
		{
			for (auto& it_nodes : it_camins->getCamiCoords())
			{
				vector<Coordinate>::iterator it = std::find(m_nodes.begin(), m_nodes.end(), it_nodes);
				if (it == m_nodes.end()) //No repetits
					m_nodes.emplace_back(it_nodes);
			}
		}
	}

	// Inicializar el vector de adyacencias.
	m_numNodes = m_nodes.size();
	m_veins.resize(m_numNodes);

	// Agregar las aristas del camino al grafo.
	for (auto& it_camins : llistaCamins)
	{
		vector<Coordinate> vecOfCoord = it_camins->getCamiCoords();
		for (int i = 0; i < vecOfCoord.size() - 1; i++)
		{
			int indexActual = obtenirIndex(vecOfCoord[i]);
			int indexSeguent = obtenirIndex(vecOfCoord[i + 1]);
			if (indexActual != -1 && indexSeguent != -1)
			{
				double pes = Util::DistanciaHaversine(m_nodes[indexActual], m_nodes[indexSeguent]);
				m_veins[indexActual].emplace_back(pair<int, double>(indexSeguent, pes));
				m_veins[indexSeguent].emplace_back(pair<int, double>(indexActual, pes));
				m_numArestes++;
			}
		}
	}
}

void GrafSolucio::inserirAresta(int posNode1, int posNode2, double pes) 
{
	// Agregamos la arista al vector de vecinas del nodo 1
	m_veins[posNode1].push_back(make_pair(posNode2, pes));

	// Si el grafo es no dirigido, también debemos agregar la arista
	// al vector de vecinas del nodo 2
	m_veins[posNode2].push_back(make_pair(posNode1, pes));

	// Aumentamos el contador de aristas
	m_numArestes++;
}

void GrafSolucio::dijkstra(int node1, int node2, vector<double>& dist, vector<int>& anterior)
{
	//Inicialización de estrucutras para guardar los datos.
	vector<bool> visitat(m_numNodes, false);
	dist.resize(m_numNodes, DISTMAX);
	dist[node1] = 0;
	anterior.resize(m_numNodes, 0);
	int j = node1;
	while (j != node2)
	{
		visitat[j] = true;
		for (auto& vei : m_veins[j])
		{
			int pos = vei.first;
			double pes = vei.second;
			if (dist[j] + pes < dist[pos])
			{
				dist[pos] = dist[j] + pes;
				anterior[pos] = j;
			}
		}
		j = minDistance(dist, visitat);
	}
}

int GrafSolucio::minDistance(const vector<double>& dist, const vector<bool>& visitat) const
{
	double min = DISTMAX;
	int minIndex = -1;
	for (int i = 0; i < m_numNodes; i++)
	{
		if (dist[i] <= min && !visitat[i])
		{
			min = dist[i];
			minIndex = i;
		}
	}
	return minIndex;
}

