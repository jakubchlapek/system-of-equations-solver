#include "matrix.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXTAB 

void Complement( double** pTabO, double** pTabI, int nRow, int nCol, int nDim );
void ComplMatrix( double** pTabD, double** pTab, int nDim );

int CreateTab( double*** pTab, int nDim ) //!!! zwraca 0  gdy error lub  1 - gdy ok
{
	*pTab = ( double** )malloc( nDim * sizeof( double* ) );
	if( !*pTab ) return 0;
	memset( *pTab, 0, nDim * sizeof( double* ) );

	double** pTemp = *pTab;

	for( int i = 0; i < nDim; i++, pTemp++ )
	{
		*pTemp = (double*)malloc( nDim * sizeof( double ) );
		if( !*pTemp ) return 0;
		memset( *pTemp, 0, nDim * sizeof( double ) );
	}
	return 1;
}

void DeleteMatrix( double*** pTab, int nDim ) // *pTab=NULL na konuc
{
	double** p = *pTab;
	for( int i = 0; i < nDim; i++ )
		free( *p++ );

	free( *pTab );
	*pTab = NULL;
}

void TransMatrix( double** pTab, int nDim )
{
	double** pTemp1 = pTab;
	for( int i = 0; i < nDim - 1; i++, pTemp1++ )
	{
		double* pTemp2 = *pTemp1 + i + 1;
		for( int j = i + 1; j < nDim; j++, pTemp2++ )			// transponowanie macierzy
		{											// bez pomocniczej, raz wolno indeksowac		
			double temp = *pTemp2;				// pTab[j][i] gdzie i numer wiersza a j numer kolumny
			*pTemp2 = pTab[j][i];				// dla elementu ktory jest transponowany
			pTab[j][i] = temp;
		}
	}
}

void PrintMatrix( double** pTab, int nDim ) // drukuje macierz 
{
	for( int i = 0; i < nDim; i++ )
	{
		double* v = *pTab++;
		for( int j = 0; j < nDim; j++ )
			printf( "%.2f ", *v++ );
		printf( "\n" );
	}
}


// wyznacznik rekurencyjnie 
//rozwiniecie wzgl 0-go wiersza 
// dla 2x2 uzyc znanego wzoru - mozna zaindeksowac
double Det( double** pTab, int nDim )
{
	if( nDim == 1 ) return **pTab;
	if( nDim == 2 ) return ((**pTab * pTab[1][1]) - (pTab[0][1] * pTab[1][0]));

	double** pTemp = NULL;
	if( !CreateTab( &pTemp, nDim - 1 ) )
	{
		perror( "Allocation Error: Det\n\n" );
		return 0;
	}

	double det = 0;
	int sign = 1;
	double* pT = *pTab;

	for( int i = 0; i < nDim; i++, pT++ )
	{
		Complement( pTemp, pTab, 0, i, nDim );
		det += *pT * Det( pTemp, nDim - 1 ) * sign;
		sign = -sign;
	}

	DeleteMatrix( &pTemp, nDim - 1 );
	return det;
}

// macierz odwrotna, na we obliczony wczesniej w main() det  (tu w funkcji juz na pewno musi byc det!=0)
void InverseMatrix( double** pInv, double** pTab, int nDim, double det )
{
	ComplMatrix( pInv, pTab, nDim );
	TransMatrix( pInv, nDim );
	for( int i = 0; i < nDim; i++ )
	{
		double* v = *pInv++;
		for( int j = 0; j < nDim; j++ )
		{
			*v++ /= det;
		}
	}
}

void LayoutEqu( double** pInv, double* pB, double* pRes, int nDim )// rozwiazuje ukl rownan
{
	for( int i = 0; i < nDim; i++, pRes++ )
	{
		double* pInvTemp = *pInv++;
		double* pBTemp = pB;
		for( int j = 0; j < nDim; j++, pInvTemp++, pBTemp++ )
			*pRes += (*pInvTemp) * (*pBTemp);
	}
}

void Complement( double** pTabO, double** pTabI, int nRow, int nCol, int nDim )
{
	for( int i = 0; i < nDim; i++, pTabI++ )
	{
		if( i == nRow ) continue;
		double* in = *pTabI;
		double* out = *pTabO++;

		for( int j = 0; j < nDim; j++, in++ )
		{
			if( j == nCol ) continue;
			*out++ = *in;
		}
	}
}

void ComplMatrix( double** pTabD, double** pTab, int nDim )
{
	double** pTempTab = NULL;
	if( !CreateTab( &pTempTab, nDim - 1 ) )
	{
		printf( "Allocation Error: ComplTab\n" );
		return;
	}

	for( int i = 0; i < nDim; i++ )
	{
		int sign = (i % 2) ? -1 : 1;
		double* x = *pTabD++;
		for( int j = 0; j < nDim; j++ )
		{
			Complement( pTempTab, pTab, i, j, nDim );
			*x++ = sign * Det( pTempTab, nDim - 1 );
			sign = -sign;
		}
	}

	DeleteMatrix( &pTempTab, nDim - 1 );
}