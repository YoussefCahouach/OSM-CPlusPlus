#include "pch.h"
#include "PuntDeInteresBotigaSolucio.h"

unsigned int PuntDeInteresBotigaSolucio::getColor()
{
	if (m_tipusBotiga == "supermarket")
		return 0xA5BE00;
	else
		if (m_tipusBotiga == "tobacco")
			return 0xFFAD69;
		else
			if (m_tipusBotiga == "bakery" && (m_openingHours.find("06:00-22:00") != string::npos) && m_wheelchair == "yes")
				return 0x4CB944;
			else
				if (m_tipusBotiga == "bakery")
					return 0xE85D75;
				else
					return 0xEFD6AC; // En cualquier otro caso, devuelve 0xEFD6AC
}