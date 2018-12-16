/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     02/18/2011
  @version  $Id: digitalfiltersintrans.h 87 2013-09-05 22:44:05Z tirons $
 **/

#ifndef __FSTANDERSON
#define __FSTANDERSON

#include "DigitalFilterIntegratorAnderson.h"

namespace Lemma {

	// ===================================================================
	//        Class:  FastSinTransformAnderson
	/** \brief Sine Transform via digital filters
	    \details Based on Andersons' Hankel transform, this calculates
	 			the sine transform of a discrete function
				using a digital filter approach.  Filter weights are
				hard-coded.
				Calculates transform of the form:
				\f[ \int_0^\infty f(x) \sin (x\omega)~dx
				\f]
    */

	// ===================================================================
	class FastSinTransformAnderson : public DigitalFilterIntegratorAnderson<Real> {

		friend std::ostream &operator<<(std::ostream &stream, const
			FastSinTransformAnderson &ob);

		public:

			// ====================  LIFECYCLE     =======================

            /// Default locked constructor.
			FastSinTransformAnderson (const ctor_key& key);

			/// Default destructor
			~FastSinTransformAnderson ();

			static std::shared_ptr<FastSinTransformAnderson> NewSP();


			// ====================  OPERATORS     =======================

			// ====================  OPERATIONS    =======================

			// ====================  ACCESS        =======================

			// ====================  INQUIRY       =======================

            /** Returns the name of the underlying class, similiar to Python's type */
            virtual std::string GetName() const { return this->CName; }

		protected:

			// ====================  LIFECYCLE     =======================

			/// Set all the filter weights
			void SetFilterWeights();

			// ====================  DATA MEMBERS  =========================

			//Eigen::Matrix<Real, 787, 1> FilterWeights;

		private:

            /** ASCII string representation of the class name */
            static constexpr auto CName = "FastSinTransformAnderson";

    }; // -----  end of class  FastSinTransformAnderson  -----

} //end of namespace Lemma

#endif // __FSTANDERSON_H

