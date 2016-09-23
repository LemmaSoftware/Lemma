/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     03/23/2010
  @version  $Id: earthmodel.h 198 2014-12-27 06:14:05Z tirons $
 **/

#ifndef  EARTHMODEL_INC
#define  EARTHMODEL_INC

#include "LemmaObject.h"

namespace Lemma {

    // ===================================================================
    //        Class:  EarthModel
    /// \ingroup LemmaCore
    /// \brief abstract class for Earth models
    /// \details
    // ===================================================================
    class EarthModel : public LemmaObject {

        // ====================    FRIENDS     ===========================
        friend std::ostream &operator<<(std::ostream &stream,
                        const EarthModel &ob);

        public:

            // ====================  LIFECYCLE     =======================
            /** YAML Serializing method
             */
            YAML::Node Serialize() const;
            //static LayeredEarth* DeSerialize(const YAML::Node& node);

            // ====================  OPERATORS     =======================

            // ====================  OPERATIONS    =======================

            // ====================  ACCESS        =======================

            /** Sets the earth's magnetic field, in Tesla
                @param[in] bfield contains the x,y, and z components of
                the field. EarthModel::Inc, EarthModel::Dec, and
                EarthModel::BMag are computed:
                \f{eqnarray*}{ \mathrm{Inc} = &
                \cos^{-1}( \mathbf{B} \cdot \hat{\mathbf{z}} / B )
                \\ \mathrm{Dec} =& \cos^{-1}
                         (\mathbf{B} \cdot \hat{\mathbf{x}} / B )
                   \\           \mathrm{Mag} =& \Vert \mathbf{B} \Vert  \f}
               @param[in] unit specifies the units of bfield.
              */
            void SetMagneticFieldComponents(const Vector3r &bfield,
                            const MAGUNITS &unit);

            /// Sets the magnetic field using inclination, declination,
            /// and magnitude, units of Tesla
            /// @param[in] inc is the field's inclination
            /// @param[in] dec is the field's declination
            /// @param[in] Mag is the field's magnitude
            /// @param[in] unit specifies the the units used.
            /// The components are set according to
            /// \f{eqnarray*} { B_x =& B \cos(inc (\pi/180)) \cos(dec (\pi/180))
            ///            \\   B_y =& B \cos(inc (\pi/180)) \sin(dec (\pi/180))
            ///            \\   B_z =& B \sin(inc (\pi/180))
            /// \f}
            void SetMagneticFieldIncDecMag(const Real& inc, const Real&dec,
                            const Real& Mag, const MAGUNITS &unit);

            // ====================  INQUIRY       =======================

            /// Gets the magnetic field of the earth, in T
            Vector3r GetMagneticField( );

            /// Gets the magnetic field of the earth, in T
            Vector3r GetMagneticFieldInGauss(  );

            /// Gets the magnetic field unit vector of the earth
            Vector3r GetMagneticFieldUnitVector( );

            /// Returns the magnitude of the magnetic field \f$ \Vert
            /// \mathbf{B}_0 \Vert \f$
            Real GetMagneticFieldMagnitude();

            /// Returns the magnitude of the magnetic field \f$ \Vert
            /// \mathbf{B}_0 \Vert \f$ in Gauss units
            Real GetMagneticFieldMagnitudeInGauss( );

            /** Returns the name of the underlying class, similiar to Python's type */
            virtual inline std::string GetName() const {
                return this->CName;
            }

        protected:

            // ====================  LIFECYCLE     =======================

            /// Default protected constructor.
            EarthModel ( );

            /** Deserialize constructor */
            EarthModel (const YAML::Node& node);

            /// Default protected constructor.
            ~EarthModel ();

            // ====================  DATA MEMBERS  =========================

        private:

            EarthModel(const EarthModel& ) = delete;

            /** ASCII string representation of the class name */
            static constexpr auto CName = "EarthModel";

            /// Magnetic field, in units of nT
            Vector3r          BField;

            /// Magnetic field unit vector \f$  \hat{\mathbf{B}_0} \f$
            Vector3r          BFieldUnit;

            /// Inclination of the magnetic field
            Real              BInc;

            /// Declination of the magnetic field
            Real              BDec;

            /// Declination of the magnetic field
            Real              BMag;

    }; // -----  end of class  EarthModel  -----

    //////////////////////////////////////////////////////////////
    // Error Classes

    /** If the Earth class is NULL valued, throw this
     */
    class NullEarth : public std::runtime_error {
            /** Thrown when an Earth pointer is NULL
             */
            public: NullEarth ();
    };

}       // -----  end of Lemma  name  -----

#endif   // ----- #ifndef EARTHMODEL_INC  -----
