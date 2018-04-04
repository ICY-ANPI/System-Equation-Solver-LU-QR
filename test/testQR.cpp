/*
 * testQR.cpp
 *
 *  Created on: Apr 2, 2018
 *      Author: cristian
 */






#include <boost/test/unit_test.hpp>

#include "QRdescomposition.hpp"

#include <iostream>
#include <exception>
#include <cstdlib>
#include <complex>

#include <functional>

#include <cmath>

namespace anpi {
  namespace test {

    /// Test the given closed root finder
    template<typename T>
    void qrTest(const std::function<void(const Matrix<T>&,
                                         Matrix<T>&,
										 Matrix<T>&)>& iqr) {

      // The result
    	Matrix<T> Q;
    	// The result
    	Matrix<T> R;
      // Test decomposition
      {
        // same matrix as before, but already permuted to force a clean decomposition
        //anpi::Matrix<T> A = { { 2, 0,1,2},{-1,-2,1,2},{ 1, 1,1,1},{-1,-1,0,1} };
        //anpi::Matrix<T> A = { { 2, 0,1,2},{-1,-2,1,2},{ 1, 1,1,1},{-1,-1,0,1} };
    	anpi::Matrix<T> A = { { 12,	-51,	4,	51,	84 ,	-95},
    						  {  0.000000000045,	17,  -68,	84, 118, 	 42+554},
							  {  6,	1+7,  -6,	97, 781, 	 784},
							  {  4,	16,  -8,	84, -554, 	 98},
							  {  56,	167,  -68,	84, 788, 	 -402},
							  {  6,	67,  -688,	814, 78, 	 4854654}
    						};
        iqr(A,Q,R);
        Matrix<T> Ar=Q*R;

        std::cout << "THE MATRIX A IS: " << std::endl;
        anpi::printMat<Matrix<T>>(A);

        std::cout << "THE MATRIX Ar IS: " << std::endl;

    	anpi::printMat<Matrix<T>>(Ar);



    	std::cout << "THE MATRIX E IS: " << std::endl;

    	Matrix<T> E = A-Ar;
    	anpi::printMat<Matrix<T>>(E);

        const T eps = std::numeric_limits<T>::epsilon()*T(10);

        BOOST_CHECK(Ar.rows()==A.rows());
        BOOST_CHECK(Ar.cols()==A.cols());
        bool iguales = true;
        for (size_t i=0;i<Ar.rows();++i) {
          for (size_t j=0;j<Ar.cols();++j) {

        	  std::cout << std::abs(Ar(i,j)-A(i,j)) << " " << eps << std::endl;
            BOOST_CHECK(std::abs(Ar(i,j)-A(i,j)) < eps);
            iguales = iguales && std::abs(Ar(i,j)-A(i,j)) < eps;

          }
        }
        if (iguales){
        	std::cout << "Ambas matrices son iguales Ar y A" << std::endl;
        }

      }
    }

  } // test
}  // anpi

BOOST_AUTO_TEST_SUITE( QR )

BOOST_AUTO_TEST_CASE(qr)
{
  anpi::test::qrTest<float>(anpi::qr<float>);
  anpi::test::qrTest<double>(anpi::qr<double>);
}


BOOST_AUTO_TEST_SUITE_END()
