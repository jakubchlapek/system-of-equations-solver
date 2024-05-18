#ifndef _MATRIX_H
#define _MATRIX_H

int  CreateTab( double*** pTab, int nDim );
void DeleteMatrix( double*** pTab, int nDim );
void TransMatrix( double** pTab, int nDim );
void InverseMatrix( double** pInv, double** pTab, int nDim, double det );
double Det( double** pTab, int nDim );
void LayoutEqu( double** pInv, double* pB, double* pRes, int nDim );
void PrintMatrix( double** pTab, int nDim );

#endif