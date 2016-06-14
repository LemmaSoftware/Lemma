/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      01/29/2014 06:59:39 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */


#ifndef  GROUNDEDELECTRICDIPOLE_INC
#define  GROUNDEDELECTRICDIPOLE_INC

#include "dipolesource.h"

namespace Lemma {

    /**
      \brief   Grounded electric dipole
      \details Used to model an idealised grounded electrid dipole of arbitrary
               polarisation.
     */
    class GroundedElectricDipole : public DipoleSource {

        friend std::ostream &operator<<(std::ostream &stream,
                const GroundedElectricDipole &ob);

        public:

        // ====================  LIFECYCLE     =======================

        /**
         * @copybrief LemmaObject::New()
         * @copydetails LemmaObject::New()
         */
        static GroundedElectricDipole* New();

        /**
         *  @copybrief   LemmaObject::Delete()
         *  @copydetails LemmaObject::Delete()
         */
        void Delete();

        // ====================  OPERATORS     =======================

        // ====================  OPERATIONS    =======================

        // ====================  ACCESS        =======================

        // ====================  INQUIRY       =======================

        protected:

        // ====================  LIFECYCLE     =======================

        /** Default protected constructor, use New */
        GroundedElectricDipole (const std::string& name);

        /** Default protected destructor, use Delete */
        ~GroundedElectricDipole ();

        /**
         *  @copybrief   LemmaObject::Release()
         *  @copydetails LemmaObject::Release()
         */
        void Release();

        private:

        // ====================  DATA MEMBERS  =========================

    }; // -----  end of class  GroundedElectricDipole  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef GROUNDEDELECTRICDIPOLE_INC  -----

