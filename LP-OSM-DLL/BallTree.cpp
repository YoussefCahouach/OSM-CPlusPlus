#include "pch.h"
#include "BallTree.h"
using namespace std;

void BallTree::construirArbre(const std::vector<Coordinate>& coordenades)
{
	// 1. Calcula el punto central de los nodos (punto C)
	std::vector<Coordinate> coordAux = coordenades;
	Coordinate C = Util::calcularPuntCentral(coordAux);
	m_pivot = C;

	if (coordenades.size() == 1) //Es una fulla
	{
		m_coordenades = coordenades;
		m_pivot = m_coordenades[0];
	}
	else
	{

		// 2. Calcula todas las distancias de los nodos respecto al punto C
		m_coordenades = coordenades;
		std::vector<double> distanciasC;
		for (const auto& coord : coordenades)
		{
			double dist = Util::DistanciaHaversine(C, coord);
			distanciasC.push_back(dist);
		}
		// 3. Cogemos el punto más lejano (Punto A)
		int indexA = std::distance(distanciasC.begin(), std::max_element(distanciasC.begin(), distanciasC.end())); //python moment
		Coordinate A = coordenades[indexA];
		m_radi = (*std::max_element(distanciasC.begin(), distanciasC.end()));

		// 4. Calcula todas las distancias de los nodos respecto al punto A
		std::vector<double> distanciasA;
		for (const auto& coord : coordenades) {
			double dist = Util::DistanciaHaversine(A, coord);
			distanciasA.push_back(dist);
		}

		// 5. Cogemos el punto más lejano (Punto B)
		int indexB = std::distance(distanciasA.begin(), std::max_element(distanciasA.begin(), distanciasA.end()));
		Coordinate B = coordenades[indexB];

		// 6. Para cada nodo:
		vector<Coordinate> vecOfCoordLeft;
		vector<Coordinate> vecOfCoordRight;
		for (const auto& coord : coordenades) {
			// 6.1 Calculamos la distancia del nodo al punto A (D1)
			double D1 = Util::DistanciaHaversine(A, coord);

			// 6.2 Calcula la distancia del nodo al punto B (D2).
			double D2 = Util::DistanciaHaversine(B, coord);

			// 6.3 Si D1 < D2, asignamos el nodo a la bola izquierda.
			if (D1 < D2)
				vecOfCoordLeft.push_back(coord);

			// 6.4 Si D1 >= D2, asignamos el nodo a la bola derecha.
			if (D1 >= D2)
				vecOfCoordRight.push_back(coord);
		}

		//Push de los vectores.
		if (!vecOfCoordLeft.empty())
		{
			m_left = new BallTree;
			m_left->setArrel(this);
			m_left->construirArbre(vecOfCoordLeft);
		}
		if (!vecOfCoordRight.empty())
		{
			m_right = new BallTree;
			m_right->setArrel(this);
			m_right->construirArbre(vecOfCoordRight);
		}
	}
}

void BallTree::inOrdre(std::vector<std::list<Coordinate>>& out)
{
	// TODO: TASCA 2
	if (this != nullptr)
	{
		//Parte izquierda.
		if (m_left != nullptr)
			m_left->inOrdre(out);

		//Out
		list<Coordinate> llistaCoord;
		std::move(m_coordenades.begin(), m_coordenades.end(), std::back_inserter(llistaCoord));
		out.push_back(llistaCoord);

		//Parte derecha.
		if (m_right != nullptr)
			m_right->inOrdre(out);
	}

}
void BallTree::preOrdre(std::vector<std::list<Coordinate>>& out)
{
	if (this != nullptr)
	{
		//Out
		list<Coordinate> llistaCoord;
		std::move(m_coordenades.begin(), m_coordenades.end(), std::back_inserter(llistaCoord));
		out.push_back(llistaCoord);

		//Parte izquierda.
		if (m_left != nullptr)
			m_left->preOrdre(out);

		//Parte derecha.
		if (m_right != nullptr)
			m_right->preOrdre(out);

	}
}

void BallTree::postOrdre(std::vector<std::list<Coordinate>>& out) {

	if (this != nullptr)
	{
		//Parte izquierda.
		if (m_left != nullptr)
			m_left->postOrdre(out);

		//Parte derecha.
		if (m_right != nullptr)
			m_right->postOrdre(out);

		//Out
		list<Coordinate> llistaCoord;
		std::move(m_coordenades.begin(), m_coordenades.end(), std::back_inserter(llistaCoord));
		out.push_back(llistaCoord);
	}
}

Coordinate BallTree::nodeMesProper(Coordinate targetQuery, Coordinate& Q, BallTree* ball) {
	// TODO: TASCA 3
	// Inicializamos Q con una coordenada que tenga una latitud y una longitud igual a 0.0
	// sólo cuando se llame a la función desde el exterior del árbol

	if (ball != nullptr)
	{
		//1. Calcula la distancia del punto central de la bola respecto al pdi (D1).
		double D1 = Util::DistanciaHaversine(ball->getPivot(), targetQuery);

		//2. Calcula la distancia del punto central de la bola respecto al Q(D2).
		double D2 = Util::DistanciaHaversine(ball->getPivot(), Q);

		//3. Si D1 – bola.radi >= D2 --> devuelve Q.
		if ((D1 - ball->getRadi()) >= D2)
			return Q;

		//4. Si la bola es una hoja del árbol, actualiza Q si es el nodo camino más cercano al punto de interés, de los puntos que forman la bola.
		if (ball->getEsquerre() == nullptr && ball->getDreta() == nullptr) //Significa que es una hoja
		{
			for (auto& it : ball->getCoordenades())
			{
				if (Util::DistanciaHaversine(targetQuery, Q) > Util::DistanciaHaversine(targetQuery, it))
					Q = it;
			}
		}
		else //5. Sino:
		{
			double Da = 0.0;
			double Db = 0.0;

			//5.1. Calcula la distancia pdi respecto al punto central de la bola izquierda(Da).
			if (ball->getEsquerre() != nullptr)
				Da = Util::DistanciaHaversine(ball->getEsquerre()->getPivot(), targetQuery);

			//5.2. Calcula la distancia pdi respecto del punto central de la bola derecha(Db).
			if (ball->getDreta() != nullptr)
				Db = Util::DistanciaHaversine(ball->getDreta()->getPivot(), targetQuery);

			
			if (Da < Db) //5.3. Si Da < Db, comienza la búsqueda por la bola izquierdo, y después, a la derecha.
			{
				Q = nodeMesProper(targetQuery, Q, ball->getEsquerre());
				Q = nodeMesProper(targetQuery, Q, ball->getDreta());
			}
			else //5.4. Si Da >= Db, comienza la búsqueda por la bola derecha, y después, a la izquierda.
			{
				Q = nodeMesProper(targetQuery, Q, ball->getDreta());
				Q = nodeMesProper(targetQuery, Q, ball->getEsquerre());
			}
		}
	}

	return Q;
}



