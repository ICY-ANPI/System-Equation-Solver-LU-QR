/**
 * Copyright (C) 2018
 * Área Académica de Ingeniería en Computadoras, ITCR, Costa Rica
 *
 * This file is part of the numerical analysis lecture CE3102 at TEC
 *
 * @Author: 
 * @Date  : 03.03.2018
 */

#include <cmath>
#include <limits>
#include <functional>
#include <algorithm>

#include "Exception.hpp"
#include "Matrix.hpp"

#ifndef ANPI_LU_DOOLITTLE_HPP
#define ANPI_LU_DOOLITTLE_HPP

namespace anpi {



template<class M>
void printMat(M c){
	std::cout << "MATRIX START HERE!" << std::endl;
	std::cout << "==================="<< std::endl;

	for (size_t i = 0; i < c.rows(); i++){
		for(size_t j = 0; j < c.cols();j++){
			std::cout << c[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << "==================="<< std::endl;
}


/**
 * Auxiliary method used to debug LU decomposition.
 *
 * It separates a packed LU matrix into the lower triangular matrix
 * L and the upper triangular matrix U, such that the diagonal of L
 * is composed by 1's.
 */
template<typename T>
void unpackDoolittle(const Matrix<T>& LU,
		Matrix<T>& L,
		Matrix<T>& U) {



	//Allocating L and U matrices
	L.allocate(LU.rows(),LU.cols());
	U.allocate(LU.rows(),LU.cols());


	//for each row
	for (size_t i = 0; i < LU.rows(); ++i) {
		//for each column
		for (size_t j = 0; j < LU.cols(); ++j) {
			//under the diagonal
			if(i > j){
				U[i][j] = T(0);
				L[i][j] = LU[i][j];
			}
			//at diagonal
			else if (i == j){
				L[i][j] = T(1);
				U[i][j] = LU[i][j];
			}
			//over the diagonal.
			else{
				L[i][j] = T(0);
				U[i][j] = LU[i][j];

			}
		}
	}


	//throw anpi::Exception("To be implemented yet");
}



/**
 * This function change the rows values at r1 and r2 swaping it.
 */

template<typename T>
void luSwapRows(Matrix<T>& LU,size_t r1,size_t r2){
	//temporary element
	T element;
	for(size_t i = 0; i < LU.cols(); i++){
		//swaping the current element at column i, between the rows.
		element = LU[r1][i];
		LU[r1][i] = LU[r2][i];
		LU[r2][i] = element;
	}
}



/**
 * Decompose the matrix A into a lower triangular matrix L and an
 * upper triangular matrix U.  The matrices L and U are packed into
 * a single matrix LU.
 *
 * The L matrix will have in the Doolittle's LU decomposition a
 * diagonal of 1's
 *
 * @param[in] A a square matrix
 * @param[out] LU matrix encoding the L and U matrices
 * @param[out] permut permutation vector, holding the indices of the
 *             original matrix falling into the corresponding element.
 *             For example if permut[5]==3 holds, then the fifth row
 *             of the LU decomposition in fact is dealing with the third
 *             row of the original matrix.
 *
 * @throws anpi::Exception if matrix cannot be decomposed, or input
 *         matrix is not square.
 */

template<typename T>
void luDoolittle(const Matrix<T>& A,
		Matrix<T>& LU,
		std::vector<size_t>& permut) {


	//Verifying if the matrix is a 0x0 matrix
	if (A.cols() <= size_t(0) || A.rows() <= size_t(0))
		throw anpi::Exception("0x0 matrix found");

	//verifying if the matrix A is a squared matrix
	if (A.cols() != A.rows())
		throw anpi::Exception("The input matrix isn't a square matrix.");


	//setting the permut vector as 1,2,3,4.... n-1, with n the size of matrix nxn
	for (size_t x = 0; x < A.rows();x++)
		permut.push_back(x);


	//Allocating the matrix LU
	LU.allocate(A.rows(), A.rows());
	//Setting the initial values of LU
	LU = A;



	for (size_t col = 0; col < A.cols(); col++) {
		//max value of the current column
		T max = T(0);
		//index of the max value at current column
		size_t max_index = col;


		//In the following loop the max value and max index of column 'col' are setted
		for(size_t row = col; row < A.rows();row++){
			//searching the max absolute value
			if (std::abs(max) < std::abs(LU[row][col])){
				//setting the max value and max index
				max = LU[row][col];
				max_index = row;
			}
		}
		//We can swap the current row and the row which we have found the max value of the
		//current col. Remember, the current col is also the current row.
		if (max_index > col){
			//modifying the permutation vector
			permut[max_index] = permut[col];
			permut[col] = max_index;
			//Swaping the rows
			luSwapRows(LU,max_index,col);
		}

		//Setting LU for each row, where row > col.
		for(size_t row = col+1; row < A.rows();row++){
			//verifing if the current element is zero.
			if (std::abs(LU[col][col]) == T(0))
				throw anpi::Exception("Division by zero detected, LU matrix couldn't be created");

			//Making the factor.
			T factor = LU[row][col]/LU[col][col];


			//U part of LU matrix
			for (size_t k = col+1; k < A.cols(); k++) {
				LU[row][k] -= LU[col][k]*factor;
			}
			//L part of U matrix
			LU[row][col] = factor;

		}


	}

	//throw anpi::Exception("To be implemented yet");

}

}

#endif

