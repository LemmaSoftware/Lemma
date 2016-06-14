/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     01/14/2013
  @version  $Id$
 **/

#ifndef __INSTRUMENTFEM_H
#define __INSTRUMENTFEM_H

#include "instrument.h"
#include "emearth1d.h"
#include "WireAntenna.h"
#include "PolygonalWireAntenna.h"
#include "receiverpoints.h"
#include "dipolesource.h"
#include "layeredearthem.h"
#include "datafem.h"

namespace Lemma {

// ===================================================================
//        Class:  InstrumentFem
/// \brief
/// \details
// ===================================================================
	class InstrumentFem : public Instrument {
		friend std::ostream &operator<<(std::ostream &stream,
			const InstrumentFem &ob);


	public:

	// ====================  LIFECYCLE     =======================

		static InstrumentFem* New();

		void Delete();

	// ====================  OPERATORS     =======================

	// ====================  OPERATIONS    =======================

		/// Make the forward model calculation
		void MakeCalculation();

		/// Forward model from a vector of inputs
		void ForwardModelProfile();

	// ====================  ACCESS        =======================
	//
		/// Set layered earth model
		void EMEarthModel(LayeredEarthEM* Earth);

		/// Set dipole source
		void SetDipoleSource(DipoleSource* dipolesource);

		/// Set Field Points
		void SetReceiverPoints(ReceiverPoints* receiver);

		/// Copy parameters from data object
		void AlignWithData(DataFEM* inpdata);

		/// Set a data object for output
		void SetOutputData(DataFEM* outputdata);



	// ====================  INQUIRY       =======================


	protected:

	// ====================  LIFECYCLE     =======================

		/// Default protected constructor.
		InstrumentFem (const std::string &name);

		/// Default protected constructor.
		~InstrumentFem ();

		void Release();

	// ====================  DATA MEMBERS  =========================

	LayeredEarthEM* EarthModel;

	DipoleSource* Dipole;

	ReceiverPoints* Receiver;

	DataFEM* InputData;

	DataFEM* OutputData;

	/// Vector of dipole sources - may be superceded by boost
	std::vector<DipoleSource*> Transmitters;

	/// Vector of receivers - may be superceded by boost
	/// not necessary--one set of receivers for all dipoles,
	/// except the number of frequencies is built into the receiver
	std::vector<ReceiverPoints*> Receivers;

	/// Vector of Earth Models - may be superceded by boost
	//std::vector<LayeredEarthEM*> EarthModels;

	/// Number of frequencies
	int nFreq;

	/// Number of observation locations
	int nObs;

	/// Array of frequencies
	VectorXr freq;

	/// Position vector, with z as height above the ground surface (pos)
	Vector3Xr xyz;

	/// Array of Tx orientations (x,y,z) for each freq
	Eigen::Matrix<ORIENTATION,Eigen::Dynamic,1> TxOrientation;

	/// Array of Rx orientations (x,y,z) for each freq
	Eigen::Matrix<ORIENTATION,Eigen::Dynamic,1> RxOrientation;

	/// Transmitter moments, one for each frequency
	VectorXr TxMom;

	/// Tx/Rx Separation in local, right hand system with x aligned with flight
	Vector3Xr TxRxSep;

	/// Scale factor as described in EM1DFM manual
	VectorXr ScaleFac;

	private:

}; // -----  end of class  InstrumentFem  -----
} // end of namespace Lemma
#endif
