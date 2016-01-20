/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     02/05/2010
  @version  $Id: pulse.h 193 2014-11-10 23:51:41Z tirons $
 **/

#ifndef  _PULSE_H_INC
#define  _PULSE_H_INC

#include "LemmaObject.h"

namespace Lemma {

    // =======================================================================
    //        Class:  Pulse
    /// \brief Pure abstract class defining interace to some sort of pulse.
    /// \details
    // =======================================================================
    class Pulse : public LemmaObject {

        public:

            /** Recursively prints information about this object
             */
            friend std::ostream &operator<<(std::ostream &stream,
                    const Pulse &ob);

            // ====================  LIFECYCLE     ===========================

            // ====================  OPERATORS     ===========================

            // ====================  OPERATIONS    ===========================

            // ====================  ACCESS        ===========================

            // ====================  INQUIRY       ===========================

            // ====================  DATA MEMBERS  ===========================

        protected:

            // ====================  LIFECYCLE     ===========================

            /// Default protected constructor.
            Pulse (const std::string &name);

            /// Default protected constructor.
            ~Pulse ();

    }; // -----  end of class  Pulse  -----

}       // -----  end of Lemma  name  -----


#endif   // ----- #ifndef _PULSE_H_INC  -----
