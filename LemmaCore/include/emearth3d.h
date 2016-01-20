/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     10/28/2010
  @version  $Id: emearth3d.h 193 2014-11-10 23:51:41Z tirons $
 **/


#ifndef  EMEARTH3D_INC
#define  EMEARTH3D_INC

#include "LemmaObject.h"

namespace Lemma {


    // ===================================================================
    //  Class:  EMEarth3D
    /**
      @class
      \brief   Impliments a 3D EM solution to a general conductivity
               structure.
      \details Calculations are made on a rectilinear grid. General 3D
               conductivity is supported, but not magnetic permeability.
     */
    // ===================================================================
    class EMEarth3D : public LemmaObject {

        public:

            // ====================  LIFECYCLE     =======================

            /** Returns a pointer to a new object of type EMEarth3D.
             * It allocates all necessary memory.
             */
            static EMEarth3D* New();

            /** Deletes this object. Delete also disconnects any
             * attachments to this object.
             */
            void Delete();

            // ====================  OPERATORS     =======================

            // ====================  OPERATIONS    =======================

            // ====================  ACCESS        =======================

            // ====================  INQUIRY       =======================

        protected:

            // ====================  LIFECYCLE     =======================

            /// Default protected constructor.
            EMEarth3D (const std::string& name);

            /// Default protected constructor.
            ~EMEarth3D ();

            /// Frees all memory
            void Release();

            // ====================  DATA MEMBERS  =========================

        private:

    }; // -----  end of class  EMEarth3D  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef EMEARTH3D_INC  -----
