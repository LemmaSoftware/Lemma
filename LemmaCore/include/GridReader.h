/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
  @file
  @date      09/23/2013 11:25:11 AM
  @version   $Id$
  @author    Trevor Irons (ti)
  @email     Trevor.Irons@xri-geo.com
  @copyright Copyright (c) 2013, XRI Geophysics, LLC
  @copyright Copyright (c) 2013, Trevor Irons
 **/

#ifndef  GRIDREADER_INC
#define  GRIDREADER_INC

#include "LemmaObject.h"
#include "grid.h"

namespace Lemma {

    /**
      @class
      \brief   Abstract class for grid readers.
      \details Provides a consistent interface for reading grid files in Lemma.
     */
    class GridReader : public LemmaObject {

        friend std::ostream &operator<<(std::ostream &stream,
                const GridReader &ob) {
            stream << *(LemmaObject*)(&ob);
            return stream;
        }

        public:

            // ====================  LIFECYCLE     =======================

            // ====================  OPERATORS     =======================

            // ====================  OPERATIONS    =======================

            virtual void ReadASCIIGridFile( const std::string& name )=0;

            virtual Grid* GetGrid( )=0;

            // ====================  ACCESS        =======================

            // ====================  INQUIRY       =======================

        protected:

            // ====================  LIFECYCLE     =======================

            /** Default protected constructor, use New */
            GridReader (const std::string& name) : LemmaObject(name) {
            }

            /** Default protected constructor, use Delete */
            ~GridReader () {
            }

        private:

            // ====================  DATA MEMBERS  =========================

    }; // -----  end of class  GridReader  -----


}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef GRIDREADER_INC  -----
