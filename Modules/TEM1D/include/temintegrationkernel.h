/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     02/10/2011
  @version  $Id: temintegrationkernel.h 190 2014-10-16 23:02:09Z tirons $
 **/

#ifndef __TEMINTEGRATIONKERNEL_H
#define __TEMINTEGRATIONKERNEL_H

#include "integrationkernel.h"
#include "emearth1d.h"

namespace Lemma {


// ===================================================================
//        Class:  TemIntegrationKernel
/// \brief  Integration kernel for calculation of TEM data
/// \details Calculates the integration kernel for the sine transform
// ===================================================================

class TemIntegrationKernel : public IntegrationKernel<Real>{

	//friend std::ostream &operator<<(std::ostream &stream,
	//	const TemIntegrationKernel &ob);

	public:

	// ====================  LIFECYCLE     =======================

		static TemIntegrationKernel* New();

		void Delete();

	// ====================  OPERATORS     =======================

	// ====================  OPERATIONS    =======================

	// ====================  ACCESS        =======================

		/// Calculates the integration argument.  Passed a frequency.
		Real Argument(const Real&x, const int& iRelated);

		int GetNumRel();

		/// Attach EMEarth1D object to TemIntegrationKernel
		void SetEMEarth1D(EMEarth1D* earth);

		/// Attach Transmitter object
		void SetTransmitter(WireAntenna *antennae);

		/// Attach Dipole object
		void SetDipole(DipoleSource* dipolesource);

		/// Attach Receiver object
		void SetReceiver(ReceiverPoints *receiver);

        /**
         *   @param[in] comp is the component to integrate
         */
        void SetComponent(const FIELDCOMPONENT& comp);
	// ====================  INQUIRY       =======================



	protected:

	// ====================  LIFECYCLE     =======================

		/// Default protected constructor.
		TemIntegrationKernel (const std::string& name);

		/// Default protected constructor.
		~TemIntegrationKernel ();

		void Release();

	// ====================  DATA MEMBERS  =========================

        FIELDCOMPONENT  component;

		EMEarth1D* EmEarthInt;

		WireAntenna* Trans;

		DipoleSource* DipoleS;

		ReceiverPoints* Receivers;

	private:

}; // -----  end of class  TemIntegrationKernel  -----

} // End of namespace Lemma

#endif // __TEMINTEGRATIONKERNEL_H
