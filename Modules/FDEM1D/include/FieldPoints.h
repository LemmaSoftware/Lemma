/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     12/02/2009
  @version  $Id: receiverpoints.h 199 2014-12-29 19:25:20Z tirons $
 **/

#ifndef __FIELDPOINTS_H
#define __FIELDPOINTS_H

#ifdef LEMMAUSEVTK
#include "vtkPointData.h"
#include "vtkFieldData.h"
#include "vtkGlyph3D.h"
#include "vtkArrowSource.h"
#include "vtkActor.h"
#include "vtkPolyDataMapper.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkDoubleArray.h"
#include "vtkDataObject.h"
#endif

#include "LemmaObject.h"
#include "dipolesource.h"

#ifdef HAVE_YAMLCPP
#include "yaml-cpp/yaml.h"
#endif

namespace Lemma {

    // =======================================================================
    //        Class:  FieldPoints
    /**
     *  \brief  Points in the subsurface where 1D EM calculations are made
     */ \details
    // =======================================================================
    class FieldPoints : public LemmaObject {

        public:

            // ====================    FRIENDS     ===========================

            /**
             *  Stream operator printing out information about this class.
             */
            friend std::ostream &operator<<(std::ostream &stream,
                        const FieldPoints &ob);

            friend class EMEarth1D;
            friend class DipoleSource;

            // ====================  LIFECYCLE     ===========================

            /**
             *  Factory method for generating concrete class.
             *  @return a std::shared_ptr of type FieldPoints
             */
            static std::shared_ptr<FieldPoints*> NewSP();

        	/**
             *  Uses YAML to serialize this object.
             *  @return a YAML::Node
             */
            YAML::Node Serialize() const;

            /**
             *   Constructs an object from a YAML::Node.
             *   @param[in] node is a YAML node containing the serialized class information
             *   @return a std::shared_ptr object of FieldPoints
             */
            static std::shared_ptr< FieldPoints* > DeSerialize(const YAML::Node& node);

            // ====================  OPERATORS     ===========================

            // ====================  OPERATIONS    ===========================

            // ====================  ACCESS        ===========================

            /** Sets the number of receivers */
            virtual void SetNumberOfReceivers(const int &nrec);

            /** Returns the location of a single receiver as an Eigen Vector */
            void SetLocation(const int& nrec, const Vector3r& loc);

            /// Returns the location of a single receiver as an Eigen Vector
            void SetLocation(const int& nrec, const Real& xp, const Real& yp,
                            const Real& zp);

            // ====================  INQUIRY       ===========================

            /// Returns the number of receiverpoints.
            int GetNumberOfReceivers();

            /// Returns all the receiver locations as a 3 X matrix
            Vector3Xr GetLocations();

            /// Returns all the receiver locations as a general matrix, useful for python wrapper
            MatrixXr GetLocationsMat();

            /// Returns the E field for all locations
            /// nfreq is the freqency desired
            Vector3Xcr GetEfield(const int &nfreq);

            /// Returns the E field for all locations
            /// nfreq is the freqency desired, cast to general dynamic matrix, for python interoperability
            MatrixXcr GetEfieldMat(const int &nfreq);

            /// Returns the H field for all locations
            /// nfreq is the freqency desired, cast to general dynamic matrix, for python interoperability
            MatrixXcr GetHfieldMat(const int &nfreq);

            /// Returns the H field for all locations
            /// nfreq is the freqency desired
            Vector3Xcr GetHfield(const int &nfreq);

            /// Returns all of the computed H fields. Every frequency
            std::vector<Vector3Xcr> GetHfield( );

            /// Returns all of the computed E fields. Every frequency
            std::vector<Vector3Xcr> GetEfield( );

            /// Returns the E field of a single receiver as an Eigen Vector
            /// nfreq is the freqency desired
            Vector3cr GetEfield(const int &nfreq, const int& loc);

            /// Returns the H field of a single receiver as an Eigen Vector
            /// nfreq is the freqency desired
            Vector3cr GetHfield(const int &nfreq, const int& loc);

            /// Returns the B field of a single receiver as an Eigen Vector
            /// nfreq is the freqency desired
            Vector3cr GetBfield(const int &nfreq, const int& loc);

            #ifdef LEMMAUSEVTK
            /// Returns vtk Glyph actor that can be placed into scenes
            vtkActor* GetVtkGlyphActor(const FIELDTYPE &ftype,
                const Real& clip, const Real &scale,
                const int &nfreq);

            /// Returns a vtk Data Object that can easily be plotted
            vtkDataObject * GetVtkDataObject(const FIELDTYPE &ftype,
                const int& nbin,
                const int& start, const int& end,
                const FIELDCOMPONENT& fcomp,
                const SPATIALCOORDINANT& scord);

            /// Returns a vtk Data Object that can easily be plotted
            vtkDataObject * GetVtkDataObjectFreq(const FIELDTYPE &ftype,
                const int& nrec,
                const int& fstart, const int& fend,
                const FIELDCOMPONENT& fcomp,
                const VectorXr& Freqs);
            #endif

            /// Returns the location of a single receiver as an Eigen Vector
            Vector3r GetLocation(const int& loc);

            /// Returns the x component of the location
            Real GetLocationX(const int& loc);

            /// Returns the y component of the location
            Real GetLocationY(const int& loc);

            /// Returns the z component of the location
            Real GetLocationZ(const int& loc);

            /// Resets fields
            void ClearFields();

            /// Sets the mask variable to true for this point.
            void MaskPoint(const int& i);

            /// Turns the mask off for this point.
            void UnMaskPoint(const int& i);

            /// Removes making on all points
            void UnMaskAllPoints();

            /// Returns the mask for this point
            int GetMask(const int& i);


        protected:

            // ====================  LIFECYCLE     ===========================

            /** Default protected constructor. */
            FieldPoints ( );

            #ifdef HAVE_YAMLCPP
            /** Default protected constructor. */
            FieldPoints (const YAML::Node& node);
            #endif

            /** Default protected constructor. */
            ~FieldPoints ();

            /**
             * @copybrief LemmaObject::Release()
             * @copydetails LemmaObject::Release()
             */
            void Release();

            // ====================  OPERATIONS    ===========================

            /// Sets the number of H bins. These bins are often frequencies.
            void SetNumberOfBinsH(const int& nbins);

            /// Sets the number of E bins. These bins are often frequencies.
            void SetNumberOfBinsE(const int& nbins);

            /** Internal function that resizes the EField data structure */
            void ResizeEField();

            /** Internal function that resizes the HField data structure */
            void ResizeHField();

            /// Sets the value of the E field
            void SetEfield(const int &nfreq, const int& loc,
                const Complex &ex, const Complex &ey, const Complex &ez);

            /// Sets the value of the H field
            void SetHfield(const int &nfreq, const int& loc,
                const Complex &hx, const Complex &hy, const Complex &hz);

            /// Appends the value of the E field. This method is not
            /// thread safe.
            void AppendEfield(const int&nfreq, const int& loc,
                const Complex &ex, const Complex &ey, const Complex &ez);

            /// Appends the value of the H field. This method is not
            /// thread safe.
            void AppendHfield(const int &nfreq, const int& loc,
                const Complex &hx, const Complex &hy, const Complex &hz);

            // ====================  DATA MEMBERS  ===========================

            /// Number of receivers
            int                         NumberOfReceivers;

            /// Number of fields
            int                         NumberOfBinsE;

            /// Number of fields
            int                         NumberOfBinsH;

            /// Used to mask this point so no computation is made at
            /// this point.
            VectorXi                    Mask;

            /// Locations of receivers
            Vector3Xr                   Locations;

        private:

            // NOTE, these are not serialized in output!

            /// Electric field at receiver locations
            std::vector<Vector3Xcr>     Efield;

            /// H field at receiver locations
            std::vector<Vector3Xcr>     Hfield;


            /** ASCII string representation of the class name */
            static constexpr auto CName = "FieldPoints";

    }; // -----  end of class  FieldPoints  -----
}

#endif // __FIELDPOINTS
