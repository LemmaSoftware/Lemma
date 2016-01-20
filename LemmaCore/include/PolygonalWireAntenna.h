/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     05/18/2010
  @version  $Id: PolygonalWireAntenna.h 211 2015-02-27 05:43:26Z tirons $
 **/

#include "WireAntenna.h"

#ifndef  POLYGONALWIREANTENNA_INC
#define  POLYGONALWIREANTENNA_INC

namespace Lemma {

    // ===================================================================
    //        Class:  PolygonalWireAntenna
    /// \brief   Class representing polygonal wire antennae.
    /// \details For EM calculations, dipoles representing this loop are
    ///    created dynamically, depending on receiver location.
    /// @todo enforce minimum dipole moment.
    // ===================================================================
    class PolygonalWireAntenna : public WireAntenna {

        friend std::ostream &operator<<(std::ostream &stream,
                const PolygonalWireAntenna &ob);

        public:

            // ====================  LIFECYCLE     =======================

            /**
             * Declares all memory and returns a new instance.
             */
            static PolygonalWireAntenna* New();

            /**
             * @copybrief LemmaObject::Delete()
             * @copydetails LemmaObject::Delete()
             */
            void Delete();

            /// Makes a deep copy of this antenna with all connections except
            /// the dipole approximation.
            PolygonalWireAntenna* Clone();

            // ====================  OPERATORS     =======================

            // ====================  OPERATIONS    =======================

            /// Approximates with ungrounded electrical dipoles, such that
            /// minDipoleRatio is satisfied.
            virtual void ApproximateWithElectricDipoles(const Vector3r &rp);

            /** Sets the minimum ratio for dipole moments to be used to
             * approximate the loop. A smaller ratio yields a more accurate
             * result, but is more expensive. Default is (1/5).
             */
            void SetMinDipoleRatio(const Real& ratio);

            /** Sets the minimum moment for dipole moments to be used to
             * approximate the loop.
             */
            void SetMinDipoleMoment(const Real& m);

            /** Sets the minimum moment for dipole moments to be used to
             * approximate the loop.
             */
            void SetMaxDipoleMoment(const Real& m);

            // ====================  ACCESS        =======================

            // ====================  INQUIRY       =======================

            #ifdef HAVE_YAMLCPP
            /**
             *  Uses YAML to serialize this object.
             *  @return a YAML::Node
             */
            YAML::Node Serialize() const;

            /**
             *   Constructs an object from a YAML::Node.
             */
            static PolygonalWireAntenna* DeSerialize(const YAML::Node& node);
            #endif

        protected:

            // ====================  LIFECYCLE     =======================

            /// Default protected constructor.
            PolygonalWireAntenna (const std::string& name);

#ifdef HAVE_YAMLCPP
            /// Default protected constructor.
            PolygonalWireAntenna (const YAML::Node& node);
#endif

            /// Default protected constructor.
            ~PolygonalWireAntenna ();

            /**
             * @copybrief LemmaObject::Release()
             * @copydetails LemmaObject::Release()
             */
            void Release();

            // ====================  DATA MEMBERS  =======================

            /// minimum ratio of dipole moment to distance to receiver point
            Real minDipoleRatio;

            /// Maximum dipole moment allowed
            Real minDipoleMoment;

            /// Maximum dipole moment allowed
            Real maxDipoleMoment;

            /// appends
            void PushXYZDipoles(const Vector3r &step, const Vector3r &cp,
                            const Vector3r &dir,
                            std::vector<DipoleSource*> &Dipoles) ;

            /// corrects for overstep
            void CorrectOverstepXYZDipoles(const Vector3r &step,
                            const Vector3r &cp,
                            const Vector3r &dir,
                            std::vector<DipoleSource*> &Dipoles );

            // ====================  OPERATIONS    =======================


            /// Returns the nearest point on a line segment to another point.
            /// if the point is not on the line-segment, return the
            /// nearest end-point
            /// @param[in] p0, p1 define the line segement
            /// @param[in] rp is a point in space. The function returns the
            /// closest point on the line to this point.
            /// @return The point (Lemma::Vector3r) on the line defined by p0,
            /// and p1, closest to rp
            Vector3r ClosestPointOnLine(const Vector3r &p0, const Vector3r &p1,
                            const Vector3r &rp);

            /// Interpolates dipoles along line segment defined by p0 and p1.
            void InterpolateLineSegment(const Vector3r &p0, const Vector3r &p1,
                            const Vector3r &rp);


            /// List of the dipoles
            //std::vector<DipoleSource*>           Dipoles;

            /// Points that define this loop
            //Vector3Xr                            Points;

        private:

            Vector3r                               rRepeat;

    }; // -----  end of class  PolygonalWireAntenna  -----

}       // -----  end of Lemma  name  -----

#endif   // ----- #ifndef POLYGONALWIREANTENNA_INC  -----
