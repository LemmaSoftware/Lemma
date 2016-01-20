/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     02/10/2011
  @version  $Id: instrumenttem.h 201 2015-01-03 00:07:47Z tirons $
 **/

#ifndef __INSTRUMENTTEM_H
#define __INSTRUMENTTEM_H

#include "instrument.h"
#include "emearth1d.h"
#include "WireAntenna.h"
#include "PolygonalWireAntenna.h"
#include "receiverpoints.h"
#include "dipolesource.h"
#include "layeredearthem.h"
#include "digitalfiltercostrans.h"
#include "digitalfiltersintrans.h"
#include "temintegrationkernel.h"
#include "CubicSplineInterpolator.h"

namespace Lemma {

enum ReceiverType { INDUCTIVE, MAGNETOMETER, ELECTRIC };

// ===================================================================
//        Class:  InstrumentTem
/// \brief  TEM Instrument Class
/// \details Provides utility to forward model TEM data in Lemma.
// ===================================================================
class InstrumentTem : public Instrument {

	friend std::ostream &operator<<(std::ostream &stream,
		const InstrumentTem &ob);

	public:

	// ====================  LIFECYCLE     =======================

		static InstrumentTem* New();

		void Delete();

	// ====================  OPERATORS     =======================

	// ====================  OPERATIONS    =======================

		/// Perform the forward model calculation
		void MakeDirectCalculation( const HANKELTRANSFORMTYPE& hType );

        /** Perform the forward model calculation, use lagged convolutions to
         *  speed up the calculations
         */
		void MakeLaggedCalculation( const HANKELTRANSFORMTYPE& hType );

	// ====================  ACCESS        =======================

		/** Sets pulse parameters as a linearly segmented graph, so
         *  for example a triangle wave needs three points in Amps and
         *  times, a trapazoidal needs for. The pulse is assumed (defaults) to be
         *  bipolar, only the positive polarity should be specified.
         *  @param[in] Amp is the waveform current in Amps
         *  @param[in] times are the abscissa values of the waveform
         *  @param[in] bipolar specifies whether or not the pulse is bipolar
         */
		void SetPulse( const VectorXr& Amp, const VectorXr& times, bool bipolar=true );

		/// Sets layered earth model
		void EMEarthModel(LayeredEarthEM* Earth);

		/// Sets transmitter parameters
		void SetTransmitLoop(WireAntenna *antennae);

		/// Sets dipole source parameters
		void SetDipoleSource(DipoleSource* dipolesource);

		/// Sets receiver points and parameters
		void SetReceiver(ReceiverPoints* Receivers);

        /** Sets the type of receiver/measurement
         */
        void SetReceiverType( const ReceiverType& rtype  );

		/// Return calculated data
		MatrixXr GetMeasurements();

		/**
            Set time gates, assumes gates widths are delta (instantaneous)
            @param[in] times gate centres units are in seconds
         */

		void SetTimes(const VectorXr &times);

        /** Sets time gates centres and widths, both in seconds. The window is assumed to be
         *  a simple boxcar. Simpson's rule is used to integrate the response
         * @param[in] times gate centres units are in seconds
         * @param[in] widths gate widths units are in seconds
         */
        void SetTimeGates(const VectorXr &centres, const VectorXr& widths);

		/// Get number of times
		int GetNumberOfTimes();

        /** Sets reference time for time gate to pulse time offset.
         */
        void SetReferenceTime(const Real& RefTime);

		/// Attach EMEarth1D
		//void AttachEMEarth1D(EMEarth1D *EMEarth);

	// ====================  INQUIRY       =======================



	protected:

	// ====================  LIFECYCLE     =======================

		/// Default protected constructor.
		InstrumentTem (const std::string &name);

		/// Default protected constructor.
		~InstrumentTem ();

		void Release();

    // ====================  OPERATIONS    =======================

        /**
         *  Converts impulse B response (step db/dt) into inductive db/dt
         *  response for general waveform.
         */
        void FoldAndConvolve( CubicSplineInterpolator* Spline );

        /** Convolves the transmitter waveform(s), takes the impulse response
            and uses simpson's rule to evaluate the integral.
            @param[in] Spline is the Cubic Spline object representing the solution.
            @param[in] t is the evaluation time.
         */
        Real ConvolveWaveform( CubicSplineInterpolator* Spline, const Real& t );

        /** Subtracts previous waveform(s) from current impulse response, and re-splines.
         */
        void SubtractPrevious( CubicSplineInterpolator* Spline );


	// ====================  DATA MEMBERS  =========================

		//EMEarth1D* EmEarth;

		WireAntenna* Antenna;

		ReceiverPoints* Receiver;

		LayeredEarthEM* EarthMod;

		DipoleSource* Dipole;

        bool     bipolarWaveform;

        /** Number of previous pules to model */
        int      NPreviousPulse;

        /** Reference time (relative to pulse abscissa) for time gates */
        Real     RefTime;

        /** Pulse period, if bimodal 1/2 repitition period */
        Real     PulseT;

        /** Transmitter repitition frequency */
        Real     TxFreq;

        /** Receiver type */
        ReceiverType  RType;

        MatrixXr ModelledData;

        /** Centre of time gate */
		VectorXr TimeGates;

        /** Width of time gates */
        VectorXr GateWidths;

        /** Pulse waveform */
        VectorXr Waveform;

        /** Current Amplitude of pulse */
        VectorXr  TxAmp;

        /** Current -delta of pulse */
        VectorXr  TxDelta;

        /** Current derivative of pulse */
        VectorXr  TxDiff;

        /** times associated with TxAmp segments */
        VectorXr  TxAbs;

	private:

}; // -----  end of class  InstrumentTem  -----

} // End of namespace Lemma

#endif // __TEMINSTRUMENT_H

