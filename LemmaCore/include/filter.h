/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     07/20/2010
  @version  $Id: filter.h 193 2014-11-10 23:51:41Z tirons $
 **/

#include "LemmaObject.h"

namespace Lemma {


    // ===================================================================
    //  Class:  Filter
    /**
      @class  Filter
      \brief  Abstract class for filtering.
      \details Derived classes include fourier domain and time domain filters.
     */
    // ===================================================================

    class Filter : public LemmaObject {

        public:

            // ====================  LIFECYCLE     =======================

            // ====================  OPERATORS     =======================

            // ====================  OPERATIONS    =======================

            // ====================  ACCESS        =======================

            // ====================  INQUIRY       =======================

        protected:

            // ====================  LIFECYCLE     =======================

            /// Default protected constructor.
            Filter (const std::string& name);

            /// Default protected constructor.
            ~Filter ();

            // ====================  DATA MEMBERS  =========================

        private:

    }; // -----  end of class  Filter  -----


}		// -----  end of Lemma  name  -----

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */
