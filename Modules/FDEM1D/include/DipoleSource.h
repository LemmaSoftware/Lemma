/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     12/02/2009
 **/

#ifndef __DIPOLESOURCE_H
#define __DIPOLESOURCE_H

#include <memory>
#include "LemmaObject.h"
#include "LayeredEarthEM.h"
//#include "PolygonalWireAntenna.h"

#ifdef LEMMAUSEVTK
#include "vtkActor.h"
#include "vtkLineSource.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkTubeFilter.h"
#include "vtkRegularPolygonSource.h"
#include "vtkProperty.h"
#endif

namespace Lemma {

    // Forward declarations
    class KernelEM1DManager;
    class FieldPoints;
    class HankelTransform;

    // ==========================================================================
    //        Class:  DipoleSource
    /// \ingroup  FDEM1D
    /// \brief    Dipole sources form the backbone of Lemma.
    /// \details  More complex sources are constructed from a superposition of
    ///           dipoles.
    // ==========================================================================
    // pybind11 struggles with deriving from enable_shared_from_this,
    //          instead we revert to raw pointers inside Lemma which also boosts performance
    //class DipoleSource : public LemmaObject, std::enable_shared_from_this<DipoleSource> {
    class DipoleSource : public LemmaObject {

        // ====================    FRIENDS     ======================

        friend std::ostream &operator<<(std::ostream &stream, const DipoleSource &ob);

        friend class EMEarth1D;
        friend class PolygonalWireAntenna;

        public:

            //bool operator==(DipoleSource& rhs)const;

            // ====================  LIFECYCLE     ======================

            /** Default locked constructor. */
            explicit DipoleSource ( const ctor_key& );

            /** Locked deserializing constructor */
            DipoleSource ( const YAML::Node& node, const ctor_key& );

            /** Default locked constructor. */
            ~DipoleSource ();

            /**
             *  Returns shared_ptr  to new DipoleSource. Location is
             *  initialized to (0,0,0) type and polarization are
             *  initialized  to nonworking values that will throw
             *  exceptions if used.
             */
            static std::shared_ptr< DipoleSource > NewSP();

            /**
             *   YAML Serializing method
             */
            YAML::Node Serialize() const;

            /**
             *   Constructs an object from a YAML::Node.
             */
            static std::shared_ptr< DipoleSource > DeSerialize(const YAML::Node& node);

            /**
             *   Constructs an object from a string representation of a YAML::Node. This is primarily
             *   used in Python wrapping
             */
            static std::shared_ptr<DipoleSource> DeSerialize( const std::string& node ) {
                return DipoleSource::DeSerialize(YAML::Load(node));
            }

            /** Returns a deep copy of the dipole. Used to make thread safe methods. Does not
                copy attachments.
             */
            std::shared_ptr< DipoleSource > Clone();

            // ====================  OPERATORS     ======================


            // ====================  ACCESS        ======================

            /** Sets the position.
             * @param [in] posin
             */
            void SetLocation(const Vector3r &posin);

            /** Sets the location using three Real coordinate arguments.
             *  @param[in] xp is the x coordinate of the dipole
             *  @param[in] yp is the y coordinate of the dipole
             *  @param[in] zp is the z coordinate of the dipole
             */
            void SetLocation(const Real &xp, const Real &yp, const Real &zp);

            /** Sets the dipole direction (polarisation). This method
             * replaced SetPolarisation(DipoleSourcePolarisation) and allows for general dipole
             * directionality.
             * @param[in] dir is the direction of the dipole. This will be normalised.
             */
            void SetPolarisation(const Vector3r &dir);

            /** Sets the polarisation of the dipole. Conveneince method that calls
             *  SetPolarisation(const Vector3r &dir), constructing the normalized Vector | <x, y, z> |
             */
            void SetPolarisation(const Real& x, const Real& y, const Real& z );

            /// Sets the dipole polarisation
            /// @param[in] pol is the enumerated polarisation
            void SetPolarisation(const DipoleSourcePolarisation &pol);

            /// Sets the dipole source type
            /// @param[in] stype is one of the enerated values taking either
            /// ELECTRICDIPOLE or MAGNETICDIPOLE
            void SetType(const DIPOLESOURCETYPE &stype);

            /// Sets the dipole moment
            void SetMoment(const Real &moment);

            /// Sets the dipole phse
            void SetPhase(const Real &phase);

            /// Sets the polarity
            void SetPolarity(const DipoleSourcePolarity& pol);

            /// Sets number of frequencies
            void SetNumberOfFrequencies(const int &nfreq);

            /// Sets a specific frequency.
            /// @param[in] ifreq is the frequency bin number
            /// @param[in] freq is the frequency to set, in Hz
            void SetFrequency(const int &ifreq, const Real &freq);

            /// Sets the frequencies of the dipole.
            /// @param[in] freqs is a vector of the frequencies. Also sets
            /// number of frequencies
            void SetFrequencies(const VectorXr& freqs);

            // ====================  INQUIRY       ======================

            /**  Accessor to polarisation vector.
                 @return returns the unit polarisation vector.
             */
            Vector3r GetPolarisation();

            /// Returns Vector3r position of the dipole
            Vector3r GetLocation();

            /// Returns a specific coordinate of the dipole
            /// @param coordinate 0=x, 1=y, 2=z
            Real  GetLocation(const int &coordinate);

            /// Returns enumerated of DIPOLESOURCETYPE
            DIPOLESOURCETYPE GetDipoleSourceType();

            /// Returns the dipole type
            DIPOLESOURCETYPE GetType();

            /// Returns pointer to KernelEM1DManager
            std::shared_ptr<KernelEM1DManager>  GetKernelManager();

            // Returns enumerated DipoleSourcePolarization
            //DipoleSourcePolarisation GetDipoleSourcePolarisation();

            /// Returns the dipole moment
            Real GetMoment();

            /// Returns the angular frequency of the dipole
            Real GetAngularFrequency(const int &ifreq);

            /// Returns the frequency of the dipole (Hz)
            Real GetFrequency(const int &ifreq);

            /// Returns the frequency of the dipole (Hz)
            VectorXr GetFrequencies( );

            /// Returns the phase offset of the dipole
            Real GetPhase();

            /// Returns the number of frequencies
            int GetNumberOfFrequencies();

            #ifdef LEMMAUSEVTK
            /// Returns an actor that can be placed into a vtk scene easily
            /// Note that this function throws a pointer, it is the receivers
            /// job to manage this memory!
            vtkActor* GetVtkActor();
            #endif

            /** Returns the name of the underlying class, similiar to Python's type */
            virtual std::string GetName() const ;

        protected:

            // ====================  OPERATIONS    ======================

            /** Determines if kernels have been loaded already, and if so if they can be reused
             */
            void SetKernels(const int& ifreq,  const FIELDCALCULATIONS&  Fields,
                    std::shared_ptr<FieldPoints> Receivers, const int& irec,
                    std::shared_ptr<LayeredEarthEM> Earth );

            /** For use in lagged calculations, this does the necessary parts to make field calculations
                for a segment.
             */
            void SetupLight(const int& ifreq,  const FIELDCALCULATIONS&  Fields, const int& irec);

            /** resets the kernels if they cannot be reused */
            virtual void ReSetKernels(const int& ifreq,  const FIELDCALCULATIONS&  Fields,
                    std::shared_ptr<FieldPoints> Receivers,
                    const int& irec, std::shared_ptr<LayeredEarthEM> Earth );

            /** Updates the receiver fields */
            virtual void UpdateFields(const int& ifreq, HankelTransform* Hankel, const Real& wavef);

        private:

            // ====================  DATA MEMBERS  ======================

            /// Defines the type of source (magnetic or electric)
            DIPOLESOURCETYPE             Type;

            // Polarization of the dipole, (x, y or z)
            //DipoleSourcePolarisation     Polarisation;

            // Dipole polarity
            //DipoleSourcePolarity         Polarity;

            /// Which receiver index should Kernels be configured for
            int                          irec;
            int                          lays;
            int                          layr;

            /// Phase offset of the dipole, referenced from 0
            Real                         Phase;

            /// Dipole Moment
            Real                         Moment;

            Real                         xxp;
            Real                         yyp;
            Real                         rho;
            Real                         sp;
            Real                         cp;
            Real                         scp;
            Real                         sps;
            Real                         cps;
            Real                         c2p;
            Real                         kernelFreq;

            FIELDCALCULATIONS            FieldsToCalculate = BOTH;

            VectorXcr                    f;
            VectorXi                     ik;

            /// Central location of the dipole
            Vector3r                                    Location;

            /// Unit vector defining directionality of the dipole
            Vector3r                                    Phat;

            /// Freqencies of the source, in Hz
            VectorXr                                    Freqs;

            /// Storage of the EM1D kernels used by this dipole
            std::shared_ptr<KernelEM1DManager>          KernelManager;

            /// Receiver points, keep track if these have changed
            std::shared_ptr<FieldPoints>                Receivers;

            /// Layered Earth used by Kernels
            std::shared_ptr<LayeredEarthEM>             Earth;

            /** ASCII string representation of the class name */
            static constexpr auto CName = "DipoleSource";

    }; // -----  end of class  DipoleSource  -----

    /** If no dipole source has been specified, throw this error.
    */
    class NullDipoleSource : public std::runtime_error {
        public:
        /** Thrown when a DipoleSource pointer is NULL
         */
        NullDipoleSource ( );
    };

    /** Error class for assignment of a dipole source that did not connect properly.
     */
    class NonValidDipoleTypeAssignment  : public std::runtime_error {
        public:
            NonValidDipoleTypeAssignment(  );
    };

    /** Error class for a non-valid dipole type. Generally thrown if this was not
     * set properly.
     */
    class NonValidDipoleType  : public std::runtime_error {
        public:
            /** Throws error. This is a deprecated function. Call the method with
             *  the pointer address instead.
             */
            NonValidDipoleType( );
            /** Throws error with information on the class throwing the error.
            */
            NonValidDipoleType( LemmaObject *ptr );
    };

    /** Error class for non valid dipole polarisation
     */
    class NonValidDipolePolarisation  : public std::runtime_error {
        public:
            NonValidDipolePolarisation( );
    };

    /** Error class for non valid dipole polarity
     */
    class NonValidDipolePolarity  : public std::runtime_error {
        public:
            NonValidDipolePolarity( );
    };

    /** Error class for non valid dipole polarisation
     */
    class NonValidDipolePolarisationAssignment  : public std::runtime_error {
        public:
            NonValidDipolePolarisationAssignment( );
    };

    /** Error class for non valid location coordinate.
     */
    class NonValidLocationCoordinate  : public std::runtime_error {
        public:
            NonValidLocationCoordinate( );
    };

}

#endif // __DIPOLESOURCE_H
