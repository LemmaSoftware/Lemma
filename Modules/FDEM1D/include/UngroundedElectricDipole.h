/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      01/29/2014 08:52:43 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */


#ifndef  UNGROUNDEDELECTRICDIPOLE_INC
#define  UNGROUNDEDELECTRICDIPOLE_INC

#include "dipolesource.h"

namespace Lemma {

    /**
      \brief  Ungrounded electric dipole
      \details Used to model an idealised ungrounded electric dipole. Used primarily in integrating
                around current sources.
     */
    class UngroundedElectricDipole : public DipoleSource {

        friend std::ostream &operator<<(std::ostream &stream,
                const UngroundedElectricDipole &ob);

        public:

        // ====================  LIFECYCLE     =======================

        /**
         * @copybrief LemmaObject::New()
         * @copydetails LemmaObject::New()
         */
        static UngroundedElectricDipole* New();

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
        UngroundedElectricDipole (const std::string& name);

        /** Default protected destructor, use Delete */
        ~UngroundedElectricDipole ();

        /**
         *  @copybrief   LemmaObject::Release()
         *  @copydetails LemmaObject::Release()
         */
        void Release();

        private:

        // ====================  DATA MEMBERS  =========================

    }; // -----  end of class  UngroundedElectricDipole  -----


}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef UNGROUNDEDELECTRICDIPOLE_INC  -----

