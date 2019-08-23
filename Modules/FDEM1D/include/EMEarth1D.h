/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     12/02/2009
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

#include "HankelTransformFactory.h"

#include "GQChave.h"
#include "FHTAnderson801.h"
#include "FHTKey201.h"
#include "FHTKey101.h"
#include "FHTKey51.h"
#include "QWEKey.h"
#include "CubicSplineInterpolator.h"

#ifdef HAVE_BOOST_PROGRESS
#include "boost/progress.hpp"
#endif

namespace Lemma {

    enum TXRXMODE { TX, RX, TXRX, NOMODE };

    class WireAntenna;
    class PolygonalWireAntenna;
    class FieldPoints;
    class DipoleSource;
    class LayeredEarthEM;

    // =======================================================================
    //        Class:  EmEarth1D
    /// \ingroup  FDEM1D
    /// \brief    Implimentation of 1D EM solution.
    /// \details  We've done a lot of different things.
    // =======================================================================
    class EMEarth1D : public LemmaObject {

		friend std::ostream &operator<<(std::ostream &stream,
			const EMEarth1D &ob);

        public:

            //friend class KernelEm1D;

            // ====================  LIFECYCLE     ===========================

            /** Default protected constructor. */
            explicit EMEarth1D ( const ctor_key& );

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

            // TODO, add this
            //static EMEarth1D* DeSerialize(const YAML::Node& node);

            // ====================  OPERATORS     ===========================

            // ====================  OPERATIONS    ===========================

            /// Calculates the field(s) due to an ungrounded dipole source
            /// Calls FORTRAN library em1d (em1dnew.for)
#ifdef KIHALEE_EM1D
            void MakeCalc();
#endif

            /** C++ wrapper for em1dnew.for, serial */
            void MakeCalc3();

            /** Calculates the field(s) due to a wire antennae */
            void CalculateWireAntennaFields(bool progressbar=false);

            // ====================  ACCESS        ===========================

            /** Attaches an antennae */
            void AttachWireAntenna( std::shared_ptr<WireAntenna> antennae );

            /** Attaches a dipole for calculation */
            void AttachDipoleSource( std::shared_ptr<DipoleSource> dipole );

            /** Attaches a layered earth model for calculation */
            void AttachLayeredEarthEM( std::shared_ptr<LayeredEarthEM> Earth );

            /** Attaches a set of receiver points for calculation */
            void AttachFieldPoints( std::shared_ptr<FieldPoints> Receivers );

            /** Sets the fields that are calcultated, E,H or BOTH */
            void SetFieldsToCalculate( const FIELDCALCULATIONS &calc );

            /** Sets the method to use to evaluate the Hankel integral,
             */
            void SetHankelTransformMethod( const HANKELTRANSFORMTYPE &type );

            /**
             *   Accesor for field points
             */
            inline FieldPoints* GetFieldPoints() {
                return this->Receivers.get();
            }

            /**
             *  Sets the Mode enum tag.
             */
            void SetTxRxMode( const TXRXMODE& ModeSet ) {
                this->Mode = ModeSet;
            }

            // ====================  INQUIRY       ===========================

            /**
             *  Returns the name of the underlying class, similiar to Python's type
             *  @return string of class name
             */
            virtual std::string GetName() const;

            /**
             *  Returns the Mode enum tag.
             */
            inline TXRXMODE GetTxRxMode() const {
                return Mode;
            }

        protected:

            // ====================  OPERATIONS    ===========================

            /** Used internally, this is the innermost loop of the MakeCalc3,
             *  and CalculateWireAntennaField routines.
             */
            void SolveSingleTxRxPair(const int &irec,
                    HankelTransform* Hankel,
                    const Real &wavef, const int &ifreq,
                    DipoleSource* tDipole);

//             void SolveSingleTxRxPair(const int &irec,
//                     std::shared_ptr<HankelTransform> Hankel,
//                     const Real &wavef, const int &ifreq,
//                     std::shared_ptr<DipoleSource> tDipole);

            /** Used internally, this is the innermost loop of the MakeCalc3,
             *  and CalculateWireAntennaField routines.
             */
            void SolveLaggedTxRxPair(const int &irec, HankelTransform* Hankel,
                    const Real &wavef, const int &ifreq,
                    PolygonalWireAntenna* antenna);

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

            /**
             * Convenience tag that can be used for marking whether these
             *  fields are part of a transmit or receive array.
             */
            TXRXMODE       Mode = NOMODE;

            /** ASCII string representation of the class name */
            static constexpr auto CName = "EMEarth1D";

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
