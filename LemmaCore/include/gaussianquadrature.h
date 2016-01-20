/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     10/07/2010
  @version  $Id: gaussianquadrature.h 193 2014-11-10 23:51:41Z tirons $
 **/

#ifndef __gaussianquadrature_h
#define __gaussianquadrature_h

#include "LemmaObject.h"

namespace Lemma {


	// ===================================================================
	//        Class:  gaussianquadrature
	/// \brief Numerical integration via Legendre-Gauss Quadrature.
	/// \details Returns integration result, weights, and abscissae.  This
	/// script computes the Legendre-Gauss nodes and weights on an interval
	/// [a,b] with truncation order N.  This class is heavily derived from
	/// lgwt.m by Greg von Winckel (2/25/2004).
	// ===================================================================
	class gaussianquadrature {

		public:

			friend std::ostream &operator<<(std::ostream &stream,
						const gaussianquadrature &ob);

			// ====================  LIFECYCLE     =======================

			static gaussianquadrature* New();

			void Delete();

			// ====================  OPERATORS     =======================

			// ====================  OPERATIONS    =======================
			/// Perform Gaussian Quadrature Integration
			void Integrate();

			/// Calculate abscissae and weights
			void CalcAW();

			// ====================  ACCESS        =======================
			/// Set number of points and limits
			void SetFreqs(const int& nfreq, const Real& a, const Real& b);

			/// Eigen Vector of function values to be integrated over
			void SetFunc(const VectorXr& fx);

			// ====================  INQUIRY       =======================
			/// Returns Eigen Vector of Abscissae
			VectorXr GetAbscissae();

			/// Returns Eigen Vector of Weights
			VectorXr GetWeights();

			/// Returns integration result
			Real GetResult();


		protected:

			// ====================  LIFECYCLE     =======================

			/// Default protected constructor.
			gaussianquadrature ();

			/// Default protected constructor.
			~gaussianquadrature ();



			// ====================  DATA MEMBERS  =========================
			/// Interval
			Real dx;

			/// Machine precision
			Real machineeps;

			/// Bookkeeping variable
			int N;

			/// Bookkeeping variable
			int N1;

			/// Bookkeeping variable
			int N2;

			/// Lower bound
			Real a;

			/// Upper bound
			Real b;

			/// Placeholder vector
			VectorXr ytemp;

			/// Abscissae
			VectorXr xu;

			/// Intermediate Abscissae vector
			VectorXr y;

			/// Intermediate Abscissae vector
			VectorXr y0;

			/// Legendre-Gauss Vandermonde Matrix
			MatrixXr L;

			/// Derivative of LGVM
			VectorXr Lp;

			/// Intermediate Matrix
			VectorXr Ltemp;

			/// Weights Vector
			VectorXr wgq;

			///Function to integrate
			VectorXr funcx;

			/// Integration result
			Real intres;

		private:

	}; // -----  end of class  gaussianquadrature  -----
} // end of namespace Lemma
#endif // __gaussianquadrature_h

