/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     06/17/2010
  @version  $Id: receivercubes.h 193 2014-11-10 23:51:41Z tirons $
 **/

#ifndef  RECEIVERCUBES_INC
#define  RECEIVERCUBES_INC

#include "receiverpoints.h"

namespace Lemma {


    // ===================================================================
    //        Class:  ReceiverCubes
    /** This class builds on Reciver points, but supports a cube around
     * each point. It is a direct descendent and may be used wherever
     * ReceiverPoints is used
     */
    // ===================================================================
    class ReceiverCubes : public ReceiverPoints {

        friend std::ostream &operator<<(std::ostream &stream,
                const ReceiverCubes &ob);

        friend class EmDipEarth1D;

        public:

            // ====================  LIFECYCLE     =======================

            /**
             *  Returns pointer to new DipoleSource. Location is
             *  initialized to (0,0,0) type and polarization are
             *  initialized  to nonworking values that will throw
             *  exceptions if used.
             */
            static ReceiverCubes* New();

            /**
             * @copybrief LemmaObject::Delete()
             * @copydetails LemmaObject::Delete()
             */
            void Delete();

            // ====================  OPERATORS     =======================

            // ====================  OPERATIONS    =======================

            /** Sets the number of reciver cubes.
             *  This method also resizes all the dynamic vectors in the class
             *  to support this size.
             *  @param[in] nrec is the number of cubes in the class.
             */
            void SetNumberOfReceivers(const int &nrec);

            /** Sets the length of the x dimension of a single cube.
             * @param[in] i is the cube number.
             * @param[in] lx is the length of this cube, in this dimension.
             */
            void SetLengthX(const int &i, const Real &lx);

            /** Sets the length of the y dimension of a single cube.
             * @param[in] i is the cube number.
             * @param[in] ly is the length of this cube, in this dimension.
             */
            void SetLengthY(const int &i, const Real &ly);

            /** Sets the length of the z dimension of a single cube.
             * @param[in] i is the cube number.
             * @param[in] lz is the length of this cube, in this dimension.
             */
            void SetLengthZ(const int &i, const Real &lz);

            /** Sets the lengths of all three dimensions of a single cube.
             * @param[in] i is the cube number.
             * @param[in] lx is the length of this cube, in this dimension.
             * @param[in] ly is the length of this cube, in this dimension.
             * @param[in] lz is the length of this cube, in this dimension.
             */
            void SetLength(const int &i, const Real& lx, const Real& ly,
                            const Real &lz);

            /** Sets the length of the x dimension of a single cube.
             * @param[in] i is the cube number.
             * @param[in] V is a vector of lenths.
             */
            void SetLength(const int &i, const Vector3r &V);


            /** Gets the x dimension of cube i
              * @param[in] i is the cube in quesion
              * @return The legth of the x dimension of cube i
              */
            Real GetLengthX(const int &i);

            /** Gets the y dimension of cube i
              * @param[in] i is the cube in quesion
              * @return The length of the y dimension of cube i
              */
            Real GetLengthY(const int &i);

            /** Gets the z dimension of cube i
              * @param[in] i is the cube in quesion
              * @return The length of the z dimension of cube i
              */
            Real GetLengthZ(const int &i);

            /** Gets the z dimension of cube i
              * @param[in] i is the cube in quesion
              * @return A vector of the lengths of the cube
              */
            Vector3r GetLength(const int& i);

            /** Returns the volume of a cube
              * @param[in] i is the cube of interest
              * @return is the volume
              */
            Real GetVolume(const int&i);

            /** Returns the volume of all the receiver cubes.
              */
            Real GetVolumeSum();

            // ====================  ACCESS        =======================

            // ====================  INQUIRY       =======================

            // Returns the volume of a cube.
            //Real GetCubeVolume(const int&i);
            //VectorXr GetCubeVolumes();

        protected:

            // ====================  LIFECYCLE     =======================

            /// Default protected constructor.
            ReceiverCubes (const std::string &name);

            /// Default protected constructor.
            ~ReceiverCubes ();

            /**
             * @copybrief LemmaObject::Release()
             * @copydetails LemmaObject::Release()
             */
            void Release();

            // ====================  DATA MEMBERS  =========================

            /// Dimension of each cube stored here, first indice is x, second y, and
            /// third z
            Vector3Xr Lengths;

        private:

    }; // -----  end of class  ReceiverCubes  -----

}       // -----  end of Lemma  name  -----

#endif   // ----- #ifndef RECEIVERCUBES_INC  -----
