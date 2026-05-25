#include "bloc.h"
#include <cstdlib>
#include <ctime>

Bloc::Bloc() :m_columna(COLUMNA_INICI), m_fila(-1)
{
	for (int i = 0; i < MIDA_BLOC; i++)
	{
		m_candies[i] = randomType();
	}
}

Bloc::Bloc(int columna, int fila, const CandyType types[MIDA_BLOC]) :
	m_columna(columna), m_fila(fila)
{
	for (int i = 0; i < MIDA_BLOC; i++)
	{
		m_candies[i] = types[i];
	}
}

CandyType Bloc::getCandyType(int pos) const
{
	return m_candies[pos];
}
CandyType Bloc::randomType()
{
	int n = std::rand() % static_cast<int>(CandyType::COUNT);
	return static_cast<CandyType>(n);
}

int Bloc::getColumna() const{ return m_columna; }
int Bloc::getFila() const { return m_fila; }

void Bloc::esquerra()
{
	m_columna--;
}

void Bloc::dreta()
{
	m_columna++;
}

void Bloc::baixar()
{
	m_fila++;
}

void Bloc::cicle()
{
	//123 _> 231
	CandyType primer = m_candies[0];
	for (int i = 0; i < MIDA_BLOC - 1; i++)
	{
		m_candies[i] = m_candies[i + 1];
	}
	m_candies[MIDA_BLOC - 1] = primer;
}

bool Bloc::estaDintre() const
{
	if (m_fila >= 0)
	{
		return true;
	}
	return false;
}