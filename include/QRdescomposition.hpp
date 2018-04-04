/*
 * QRdescomposition.hpp
 *
 *  Created on: Apr 2, 2018
 *      Author: cristian
 */


#include <cmath>
#include <limits>
#include <functional>

#include <vector>

#include "Exception.hpp"
#include "Matrix.hpp"


#include "LUDoolittle.hpp"




#ifndef INCLUDE_QRDESCOMPOSITION_HPP_
#define INCLUDE_QRDESCOMPOSITION_HPP_






namespace anpi {

using namespace std;


template<typename T>
void qr( const Matrix<T>& A, Matrix<T>& Q, Matrix<T>& R);



template<typename T>
void qr( const Matrix<T>& A, Matrix<T>& Q, Matrix<T>& R){

	//find the H matrix
	Q.allocate(A.rows(),A.cols());
	R.allocate(A.rows(),A.cols());

	//Q = A;

	for (size_t i = 0; i < A.rows(); i++) {
		for (size_t j = 0; j < A.rows(); j++) {
			if (i==j)
				Q[i][j] = T(1);
			else
				Q[i][j] = T(0);
		}
	}

	Matrix<T> H;
	H.allocate(A.rows(),A.cols());

	Matrix<T> D;
	D = A;

	for (size_t diagonal = 0; diagonal+1  < A.rows(); diagonal++) {

		//save the vector X
		vector<T> vectorX;
		T moduleX(T(0)),current;
		for (size_t i = diagonal; i < A.rows(); i++) {
			current = D[i][diagonal];
			vectorX.push_back(current);
			moduleX += current*current;
		}
		//vectorX[0] =  vectorX[0] -  moduleX;
		T sign = vectorX[0] <= T(0)? T(1): T(-1);
		T tmp = vectorX[0];
		vectorX[0] =  vectorX[0] + sign* std::sqrt(moduleX);
		//setting the vector U
		moduleX += vectorX[0]*vectorX[0] - tmp*tmp;



		for (size_t i = 0; i < A.rows(); i++)
			for (size_t j = 0; j < A.rows(); j++)
					H[i][j] = T(i==j);

		for (size_t i = diagonal; i < A.rows(); i++) {
			for (size_t j = diagonal; j < A.rows(); j++) {
				H[i][j] = T(-2) *(vectorX[i-diagonal]* (vectorX[j-diagonal])/moduleX) + T(i==j);
			}
		}

		Q = H*Q;
		//esto debe ir aqui
		D = Q*A;

	}

	R = Q*A;


	//tranponiendo matriz Qt
	T temp;
	for (size_t i = 0; i < A.rows(); i++) {
		for (size_t j = i; j < A.rows(); j++) {
			temp = Q[i][j];
			Q[i][j] = Q[j][i];
			Q[j][i] = temp;
		}
	}

	printMat<Matrix<T>>(R);
	printMat<Matrix<T>>(Q);





	//throw Exception("QR: unimplemented method!");

}






}//end namespace anpi





#endif /* INCLUDE_QRDESCOMPOSITION_HPP_ */
