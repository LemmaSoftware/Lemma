/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     02/19/2011
  @version  $Id: modelreadertem1dubc.h 87 2013-09-05 22:44:05Z tirons $
 **/

#ifndef __MODELREADERTEM1DUBC_H
#define __MODELREADERTEM1DUBC_H

#include "modelreadertem1d.h"

namespace Lemma {

// ===================================================================
//        Class:  ModelReaderTem1DUBC
/// \brief Model reader class for forward modelling 1D TEM data
/// \details Reads UBC 1D TEM forward model format
// ===================================================================
class ModelReaderTem1DUBC : public ModelReaderTem1D {

	friend std::ostream &operator<<(std::ostream &stream,
		const ModelReaderTem1DUBC &ob);

	public:

		// ====================  LIFECYCLE     =======================

		static ModelReaderTem1DUBC* New();

		void Delete();

		// ====================  OPERATORS     =======================

		// ====================  OPERATIONS    =======================

		void ReadParameters(const std::string &params);

		// ====================  ACCESS        =======================

		/// Attaches a LayeredEarthEM object
		void SetEMEarth1D(LayeredEarthEM* earth);

		/// Attaches a WireAntenna object
		void SetTransmitter(WireAntenna *antennae);

		/// Attaches a ReceiverPoints object
		void SetReceiver(ReceiverPoints* receiver);

		VectorXr GetTimes();

		// ====================  INQUIRY       =======================


	protected:

		// ====================  LIFECYCLE     =======================

		/// Default protected constructor.
		ModelReaderTem1DUBC (const std::string &name);

		/// Default protected constructor.
		~ModelReaderTem1DUBC ();

		void Release();

		/// Read in the Earth Model file and attach
		void ReadModel();

		/// Read in the instrument parameters and attach
		void ReadInstrument();

		/// Read in the waveform file (future)
		void ReadWaveform();

		// ====================  DATA MEMBERS  =========================

		std::string modelfile;

		std::string instrumentfile;

		std::string waveformfile;

		WireAntenna* Trans;

		ReceiverPoints* Receivers;

		LayeredEarthEM* EarthMod;

		/// Simulated noise flag
		bool addednoise;

		/// Simulated noise percentage
		Real noisepercent;

		/// Baseline threshold percentage
		Real noisethresh;

		/// Seed for random number generator
		Real noiseseed;

		VectorXr receivermoment;

		VectorXr modtimes;

	private:

}; // -----  end of class  ModelReaderTem1DUBC  -----
} // end of namespace lemma
#endif // __MODELREADERTEM1DUBC_H
