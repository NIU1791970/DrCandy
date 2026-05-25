#ifndef BLOC_H
#define BLOC_H

#include "candy.h"
#include <array>

const int COLUMNA_INICI = 5;
const int MIDA_BLOC = 3;

class Bloc 
{
public:
	Bloc();
	Bloc(int columna, int fila, const CandyType types[MIDA_BLOC]);

	int getColumna() const;
	int getFila() const;
	CandyType getCandyType(int pos) const;

	void esquerra();
	void dreta();
	void baixar();
	void cicle();

	bool estaDintre() const;
private:
	int m_columna;
	int m_fila; //fila del caramel de mes avall
	CandyType m_candies[MIDA_BLOC];
	
	CandyType randomType();
};
#endif