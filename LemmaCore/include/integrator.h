/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     02/07/2011
  @version  $Id: integrator.h 193 2014-11-10 23:51:41Z tirons $
 **/

#ifndef  INTEGRATOR_INC
#define  INTEGRATOR_INC

#include "LemmaObject.h"

namespace Lemma {

    // ===================================================================
    //  Class:  Integrator
    /**
      @class
      \brief
      \details
     */
    // ===================================================================
    class Integrator : public LemmaObject {

        /// Prints out basic info about the class
        friend std::ostream &operator<<(std::ostream &stream,
                        const Integrator &ob);

        public:

            // ====================  LIFECYCLE     =======================

            // ====================  OPERATORS     =======================

            // ====================  OPERATIONS    =======================

            // ====================  ACCESS        =======================

            // ====================  INQUIRY       =======================

        protected:

            // ====================  LIFECYCLE     =======================

            /// Default protected constructor.
            Integrator (const std::string& name);

            /// Default protected constructor.
            ~Integrator ();

            // ====================  DATA MEMBERS  =========================

        private:

    }; // -----  end of class  Integrator  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef INTEGRATOR_INC  -----

