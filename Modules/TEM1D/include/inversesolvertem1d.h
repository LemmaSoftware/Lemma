/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     03/23/2011
  @version  $Id: inversesolvertem1d.h 201 2015-01-03 00:07:47Z tirons $
 **/

#ifndef _INVERSESOLVERTEM1D__H
#define _INVERSESOLVERTEM1D__H

#include "inversesolver.h"
#include "layeredearthem.h"
#include "WireAntenna.h"
#include "dipolesource.h"
#include "receiverpoints.h"
#include "instrumenttem.h"
#include "cg.h"

namespace Lemma {
	// ===================================================================
	//        Class:  InverseSolverTEM1D
	/// \brief  Inversion for 1D TEM data
	/// \details
	// ===================================================================
	class InverseSolverTEM1D : public InverseSolver {

		friend std::ostream &operator<<(std::ostream &stream,
			const InverseSolverTEM1D &ob);

		public:

		// ====================  LIFECYCLE     =======================

		static InverseSolverTEM1D* New();

		void Delete();

		// ====================  OPERATORS     =======================

		// ====================  OPERATIONS    =======================

		/// Perform the inverse calculation
		void Calculate();

		// ====================  ACCESS        =======================

		/// Attach starting model
		void AttachStartMod(LayeredEarthEM* inpstartmod);

		/// Attach recovered model
		void AttachRecMod(LayeredEarthEM* inprecmod);

		/// Attach wire antenna source
		void AttachWireAntenna(WireAntenna* inpwireantenna);

		/// Attach dipole source
		void AttachDipoleSource(DipoleSource* inpdipsource);

		/// Attach receiver points
		void AttachReceiver(ReceiverPoints* inpreceiver);

		/// Set the time gates
		void SetTimes(VectorXr &inptimes);

		/// Show forward modelled data
		void ShowSoln();

		/// Write recovered model to file
		void WriteSoln(const std::string &filename);

		/// Set free parameter indices: layer, conductivity
		void SetFreeParams(const VectorXi &layers, const VectorXi &cond);

		/// Attach measured data for misfit comparison
		void AttachMeasuredData(const MatrixXr &data);

		// ====================  INQUIRY       =======================
		/// Report number of iterations
		int NumberOfIterations();

		/// Report success/failure
		bool Success();

		/// Return phi m vector
		VectorXr GetPhiMVector();

		/// Return phi d vector
		VectorXr GetPhiDVector();


		protected:

		/// Some crap trevor uses
		void FillInG(const Vector3r& pos, const Vector3r& step);

		/// Calculate Jacobian
		MatrixXr CalcJac(LayeredEarthEM* m0);

		// ====================  LIFECYCLE     =======================

		/// Default protected constructor.
		InverseSolverTEM1D (const std::string &name);

		/// Default protected constructor.
		~InverseSolverTEM1D ();

        void Release();

		// ====================  DATA MEMBERS  =========================

		WireAntenna* Antenna;

		DipoleSource* Dipole;

		ReceiverPoints* Receiver;

		LayeredEarthEM* StartingMod;

		LayeredEarthEM* RecoveredMod;

		InstrumentTem *InstrumentFor;

		MatrixXr MeasuredData;

		VectorXr Times;

		/// Vector of indices for which layer conductivities are solved for
		VectorXi SCondIndex;

		/// Vecor of indices for which layer thicknesses are solved for
		VectorXi SThickIndex;

		/// Temporary vector for returning linearised step
		VectorXr RecModStep;

		private:

}; // -----  end of class  InverseSolverTEM1D  -----
} // End of namespace Lemma
#endif // _INVERSESOLVERTEM1D__H
