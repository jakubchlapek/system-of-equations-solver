#include <iostream>
#include <stdio.h>
#include "matrix.h"

#define _DEBUG_
#define ZERO 1e-200

void ReadData( FILE* fin, double** pMatrix, double* pVec, int nDim );
int CreateVector( double** pVec, int nDim );
void PrintVector( double* pVec, int nDim );


int main( int argc, char** argv )
{
	// sprwdzic argumenty main
	//sprawdzenie parametru main() - rozmiar tablicy
	if( argc != 2 )
	{
		printf( "Usage: %s <input_file>\n", argv[0] );
		return 1;
	}

	// odtworzyc plik do odczytu (tekstowy)
	FILE* fin = NULL;

	if( (fin = fopen( argv[1], "rt" )) == NULL )
	{
		printf( "Error opening input file %s\n", argv[1] );
		return 1;
	}

	// odczytac rozmiar nDim   fscanf( fin, "%d\n", &nDim ); // nDim param WY   fin plik po otwarciu do odczytu
	int nDim = 0;
	fscanf( fin, "%d", &nDim );
	if( fabs( nDim ) < ZERO )
	{
		printf( "Error: Matrix dimension is 0" );
		return 7;
	}

#ifdef _DEBUG_
	printf( "Wymiar macierzy: %d\n\n", nDim );
#endif

	// wykreowac macierz pMatrix ukladu nDim x nDim
	double** pTab = NULL;
	if( !CreateTab( &pTab, nDim ) )
	{
		perror( "Allocation Error: pPattern\n\n" );
		return 2;
	}

	// wykreowac wektor wyrazow wolnych b
	double* pVector = NULL;
	if( !CreateVector( &pVector, nDim ) )
	{
		perror( "Allocation Error: pVector\n\n" );
		return 3;
	}

	// wczytac dane ( ReadData() )

	ReadData( fin, pTab, pVector, nDim );

	//wydruk kontrolny (warunkowa kompilacja)
#ifdef _DEBUG_
	printf( "Wczytana macierz: \n\n" );
	PrintMatrix( pTab, nDim );
	printf( "\nWektor wyrazow wolnych: \n\n" );
	PrintVector( pVector, nDim );
#endif

	// obl wyznacznik
	double det = Det( pTab, nDim );

	//wydruk kontrolny (warunkowa kompilacja)
#ifdef _DEBUG_
	printf( "\nWyznacznik: %.3f \n\n", det );
#endif

	if( fabs( det ) < ZERO )
	{
		printf( "Error: Det equal to 0\n" );
		return 4;
	}

	// obrocic macierz
	double** MatInv = NULL;
	if( !CreateTab( &MatInv, nDim ) )
	{
		printf( "Allocation Error: MatInv\n\n" );
		return 5;
	}

	InverseMatrix( MatInv, pTab, nDim, det );

	//wydruk kontrolny  (warunkowa kompilacja)
#ifdef _DEBUG_
	printf( "Macierz odwrotna: \n\n" );
	PrintMatrix( MatInv, nDim );
	printf( "\n" );
#endif

	// wykreowac wektor wynikowy
	double* VecRes = NULL;
	if( !CreateVector( &VecRes, nDim ) ) {
		printf( "Allocation Error: VecRes\n\n" );
		return 6;
	}

	// rozwiazac ukl
	LayoutEqu( MatInv, pVector, VecRes, nDim );

	// wyniki
	printf( "Macierz wynikowa: \n\n" );
	PrintVector( VecRes, nDim );

	// zwolnic pamiec !!!
	DeleteMatrix( &pTab, nDim );
	DeleteMatrix( &MatInv, nDim );
	free( VecRes );
	free( pVector );

	return 0;
}


void ReadData( FILE* fin, double** pMatrix, double* pVec, int nDim ) {

	for( int i = 0; i < nDim; i++ )
	{
		double* pTemp = *pMatrix++;
		for( int j = 0; j < nDim; j++ )
		{
			fscanf( fin, "%lf ", pTemp++ );
		}
		fscanf( fin, "%lf", pVec++ );
	}

}

int CreateVector( double** pVec, int nDim ) {
	*pVec = ( double* )malloc( nDim * sizeof( double ) );
	if( !*pVec ) {
		return 0;
	}

	memset( *pVec, 0, nDim * sizeof( double ) );

	return 1;
}

void PrintVector( double* pTab, int nDim ) {
	for( int i = 0; i < nDim; i++ ) {
		printf( "%.2lf \n", *pTab++ );
	}
	return;
}