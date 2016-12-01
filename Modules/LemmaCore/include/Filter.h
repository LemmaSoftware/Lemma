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
      \ingroup LemmaCore
      \brief  Abstract class for filtering.
      \details Derived classes include fourier domain and time domain filters.
     */
    // ===================================================================

    class Filter : public LemmaObject {

        friend std::ostream &operator<<(std::ostream &stream, const Filter& ob);

        public:

            // ====================  LIFECYCLE     =======================
            /**
             *  Uses YAML to serialize this object.
             *  @return a YAML::Node
             */
            virtual YAML::Node Serialize() const;

            // ====================  OPERATORS     =======================

            // ====================  OPERATIONS    =======================


            // ====================  ACCESS        =======================

            // ====================  INQUIRY       =======================

            /** Returns the name of the underlying class, similiar to Python's type */
            virtual std::string GetName() const;

        protected:

            // ====================  LIFECYCLE     =======================

            /// Default protected constructor.
            Filter ( );

            /// Default protected constructor.
            Filter ( const YAML::Node& node );

            /// Default protected constructor.
            virtual ~Filter ();

            // ====================  DATA MEMBERS  =========================

        private:

            /** ASCII string representation of the class name */
            static constexpr auto CName = "Filter";

    }; // -----  end of class  Filter  -----


}		// -----  end of Lemma  name  -----

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */
