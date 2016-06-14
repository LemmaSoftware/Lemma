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

#ifndef __DIGITALFILTERSINTRANS_H
#define __DIGITALFILTERSINTRANS_H

#include "digitalfilterintegrator.h"

namespace Lemma {

	// ===================================================================
	//        Class:  DigitalFilterSinTrans
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
	class DigitalFilterSinTrans : public DigitalFilterIntegrator<Real> {

		friend std::ostream &operator<<(std::ostream &stream, const
			DigitalFilterSinTrans &ob);

		public:

			// ====================  LIFECYCLE     =======================

			static DigitalFilterSinTrans* New();

			void Delete();

			// ====================  OPERATORS     =======================

			// ====================  OPERATIONS    =======================

			// ====================  ACCESS        =======================

			// ====================  INQUIRY       =======================

		protected:

			// ====================  LIFECYCLE     =======================

			/// Default protected constructor.
			DigitalFilterSinTrans (const std::string& name);

			/// Default protected constructor.
			~DigitalFilterSinTrans ();

			void Release();

			/// Set all the filter weights
			void SetFilterWeights();

			// ====================  DATA MEMBERS  =========================

			//Eigen::Matrix<Real, 787, 1> FilterWeights;

		private:

    }; // -----  end of class  DigitalFilterSinTrans  -----

} //end of namespace Lemma

#endif // __DIGITALFILTERSINTRANS_H

