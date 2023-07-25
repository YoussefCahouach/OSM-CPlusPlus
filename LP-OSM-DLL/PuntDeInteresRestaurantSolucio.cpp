#include "pch.h"
#include "PuntDeInteresRestaurantSolucio.h"

unsigned int PuntDeInteresRestaurantSolucio::getColor()
{
	if (m_tipusCuina == "pizza" && m_facilitaAcces == true)
		return 0x03FCBA;
	else
		if (m_tipusCuina == "chinese")
			return 0xA6D9F7;
		else
			if (m_facilitaAcces == true)
				return 0x251351;
			else
				return PuntDeInteresBase::getColor(); // En cualquier otro caso, devuelve el color predeterminado
}