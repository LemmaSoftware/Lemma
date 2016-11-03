/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     12/02/2009
  @version  $Id: emearth1d.h 266 2015-04-01 03:24:00Z tirons $
 **/

#ifndef __EMEARTH1D_H
#define __EMEARTH1D_H



// forward declare these due to include cycle
//#include "LayeredEarthEM.h"
//#include "DipoleSource.h"
//#include "FieldPoints.h"
//#include "WireAntenna.h"
//#include "PolygonalWireAntenna.h"
//#include "KernelEM1DManager.h"

#include "KernelEM1DSpec.h"
#include "hankeltransformgaussianquadrature.h"
#include "hankeltransformhankel2.h"
#include "FHTKey.h"
#include "FHTKey51.h"
#include "FHTKey101.h"
#include "QWEKey.h"
#include "CubicSplineInterpolator.h"

#ifdef HAVEBOOSTPROGRESS
#include "boost/progress.hpp"
#endif

namespace Lemma {

    class WireAntenna;
    class PolygonalWireAntenna;
    class FieldPoints;
    class DipoleSource;
    class LayeredEarthEM;

    // =======================================================================
    //        Class:  EmEarth1D
    /// \brief  Implimentation of 1D EM solution.
    /// \details  We've done a lot of different things.
    // =======================================================================
    class EMEarth1D : public LemmaObject {

		friend std::ostream &operator<<(std::ostream &stream,
			const EMEarth1D &ob);

        struct ctor_key{};

        public:

            //friend class KernelEm1D;

            // ====================  LIFECYCLE     ===========================

            /** Default protected constructor. */
            EMEarth1D ( const ctor_key& );

            /** Default protected constructor. */
			EMEarth1D ( const YAML::Node& node, const ctor_key& );

            /** Default protected constructor. */
            ~EMEarth1D ();

            /**
             *  Returns pointer to new EMEarth1D. Location is
             *  initialized to (0,0,0) type and polarization are
             *  initialized  to nonworking values that will throw
             *  exceptions if used.
             */
            static std::shared_ptr<EMEarth1D> NewSP();

            /** stream debugging info to std::out
             */
            void Query();

            /** YAML Serializing method
             */
            YAML::Node Serialize() const;

            //static EMEarth1D* DeSerialize(const YAML::Node& node);

            // ====================  OPERATORS     ===========================

            // ====================  OPERATIONS    ===========================

            /// Calculates the field(s) due to an ungrounded dipole source
            /// Calls FORTRAN library em1d (em1dnew.for)
#ifdef COMPILE_FORTRAN
            void MakeCalc();
#endif

            /** C++ wrapper for em1dnew.for, serial */
            void MakeCalc3();

            /** Calculates the field(s) due to a wire antennae */
            void CalculateWireAntennaFields(bool progressbar=false);

            // ====================  ACCESS        ===========================

            /** Attaches an antennae */
            void AttachWireAntenna(WireAntenna *antennae);

            /** Attaches a dipole for calculation */
            void AttachDipoleSource( std::shared_ptr<DipoleSource> dipole);

            /** Attaches a layered earth model for calculation */
            void AttachLayeredEarthEM( std::shared_ptr<LayeredEarthEM> Earth);

            /** Attaches a set of receiver points for calculation */
            void AttachFieldPoints( std::shared_ptr<FieldPoints> Receivers);

            /** Sets the fields that are calcultated, E,H or BOTH */
            void SetFieldsToCalculate(const FIELDCALCULATIONS &calc);

            /** Sets the method to use to evaluate the Hankel integral,
             */
            void SetHankelTransformMethod(const HANKELTRANSFORMTYPE &type);

            // ====================  INQUIRY       ===========================

        protected:

            // ====================  OPERATIONS    ===========================

            /** Used internally, this is the innermost loop of the MakeCalc3,
             *  and CalculateWireAntennaField routines.
             */
            void SolveSingleTxRxPair(const int &irec,
                    std::shared_ptr<HankelTransform> Hankel,
                    const Real &wavef, const int &ifreq,
                    std::shared_ptr<DipoleSource> tDipole);

            /** Used internally, this is the innermost loop of the MakeCalc3,
             *  and CalculateWireAntennaField routines.
             */
            void SolveLaggedTxRxPair(const int &irec, std::shared_ptr<Hankel2> Hankel,
                    const Real &wavef, const int &ifreq,
                    std::shared_ptr<PolygonalWireAntenna> antenna);

            /** Removes all connections */
            void DetachAll();

            // ====================  DATA MEMBERS  ===========================

            /** Computes field due to dipole */
            std::shared_ptr<DipoleSource>        Dipole;

            /** Earth model (Cole-cole) */
            std::shared_ptr<LayeredEarthEM>      Earth;

            /** Receiver points */
            std::shared_ptr<FieldPoints>         Receivers;

            /** Wire antennae tx */
            std::shared_ptr<WireAntenna>         Antenna;

            /** What fields are wanted */
            FIELDCALCULATIONS    FieldsToCalculate;

            /** The type of Hankel transform to use, default to digital
             *  filtering
             */
            HANKELTRANSFORMTYPE  HankelType;

            /** Counter for number of caclulations made
             */
            int icalcinner;

            /** Counter for number of caclulations made
             */
            int icalc;

    }; // -----  end of class  EMEarth1D  -----

    /////////////////////////////////////////
    // Exception classes

    /** If a Receivers Class is NULL valued, throw this.
     */
    class NullReceivers : public std::runtime_error {
            /** Thrown when Receivers pointer is NULL
             */
            public: NullReceivers();
    };

    /** If an Antenna is NULL valued, throw this error.
     */
    class NullAntenna : public std::runtime_error {
            /** Thrown when an antenna pointer is NULL
             */
            public: NullAntenna();
    };

    /** If an Instrument is NULL valued, throw this error.
     */
    class NullInstrument : public std::runtime_error {
            /** thrown when an instrument pointer is NULL.
             *  @param[in] ptr is a pointer to the class throwing the exception.
             */
            public: NullInstrument(LemmaObject* ptr);
    };

    /** If a dipole source is specified, but a method calling a wire antenna is
     * called, throw this.
     */
    class DipoleSourceSpecifiedForWireAntennaCalc : public std::runtime_error {
            /** Thrown when a dipole source is specified when a wire antenna is
             * expected
             */
            public: DipoleSourceSpecifiedForWireAntennaCalc();
    };

} // Namespace Lemma

#endif // __EMEARTH1D_H
