/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     02/07/2011
  @version  $Id: digitalfiltercostrans.h 87 2013-09-05 22:44:05Z tirons $
 **/

#ifndef __DIGITALFILTERCOSTRANS_H
#define __DIGITALFILTERCOSTRANS_H

#include "DigitalFilterIntegratorAnderson.h"

namespace Lemma {

	// ===================================================================
	//        Class:  FastCosTransformAnderson
	/** \brief Cosine Transform via digital filters
	    \details Based on Andersons' Hankel transform, this calculates
	 			the cosine transform of a discrete function
				using a digital filter approach.  Filter weights are
				hard-coded.
				Calculates transform of the form:
				\f[ \int_0^\infty f(x) \cos (x\omega)~dx
				\f]
    */

	// ===================================================================
	class FastCosTransformAnderson : public DigitalFilterIntegratorAnderson<Real> {

		friend std::ostream &operator<<(std::ostream &stream, const
			FastCosTransformAnderson &ob);

		public:

			// ====================  LIFECYCLE     =======================

			static std::shared_ptr< FastCosTransformAnderson > NewSP();

            /// Default protected constructor.
			FastCosTransformAnderson (const ctor_key& key);

			/// Default protected constructor.
			~FastCosTransformAnderson ();

			// ====================  OPERATORS     =======================

			// ====================  OPERATIONS    =======================

			// ====================  ACCESS        =======================

			// ====================  INQUIRY       =======================

		protected:

			// ====================  LIFECYCLE     =======================

			/// Set all the filter weights
			void SetFilterWeights();

			// ====================  DATA MEMBERS  =========================

			//Eigen::Matrix<Real, 787, 1> FilterWeights;

		private:

    }; // -----  end of class  FastCosTransformAnderson  -----

} //end of namespace Lemma

#endif // __DIGITALFILTERCOSTRANS_H

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */
