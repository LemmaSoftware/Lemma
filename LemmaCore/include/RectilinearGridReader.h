/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
  @file
  @date      09/23/2013 11:05:11 AM
  @version   $Id$
  @author    Trevor Irons (ti)
  @email     Trevor.Irons@xri-geo.com
  @copyright Copyright (c) 2013, XRI Geophysics, LLC
  @copyright Copyright (c) 2013, Trevor Irons
 **/

#ifndef  RECTILINEARGRIDREADER_INC
#define  RECTILINEARGRIDREADER_INC

#include "GridReader.h"
#include "rectilineargrid.h"
#include "ASCIIParser.h"

namespace Lemma {

    /**
      @class
      \brief
      \details
     */
    class RectilinearGridReader : public GridReader {

        friend std::ostream &operator<<(std::ostream &stream,
                const RectilinearGridReader &ob);

        public:

            // ====================  LIFECYCLE     =======================

            /**
             * @copybrief LemmaObject::New()
             * @copydetails LemmaObject::New()
             */
            static RectilinearGridReader* New();

            /**
             *  @copybrief   LemmaObject::Delete()
             *  @copydetails LemmaObject::Delete()
             */
            void Delete();

            // ====================  OPERATORS     =======================

            // ====================  OPERATIONS    =======================

            /**
            Reads a .mod file, a simple representation of the grid. These files
            have the following format:
            @verbatim
            nx ny nz  // number of cells in x, y, and z
            ox oy oz  // grid offset of top SW corner

            hx[0] hx[1] ... hx[nx-1]  // grid cell spacing in x, from 0 to nx-1
            hy[0] hy[1] ... hy[ny-1]  // grid cell spacing in y, from 0 to ny-1
            hz[0] hz[1] ... hz[nz-1]  // grid cell spacing in z, from 0 to nz-1

            // c++ style (//) comments are allowed anywhere, c-style are not
            @endverbatim
            */
            void ReadASCIIGridFile( const std::string& name );

            // ====================  ACCESS        =======================
            /**
             * Accessor method for the underlying RectilinearGrid class constructed
             * from the input file.
             */
            RectilinearGrid* GetGrid();

            // ====================  INQUIRY       =======================

        protected:

            // ====================  LIFECYCLE     =======================

            /** Default protected constructor, use New */
            RectilinearGridReader (const std::string& name);

            /** Default protected constructor, use Delete */
            ~RectilinearGridReader ();

            /**
             *  @copybrief   LemmaObject::Release()
             *  @copydetails LemmaObject::Release()
             */
            void Release();

        private:

            // ====================  DATA MEMBERS  =========================

            /** Object holding constructed Grid */
            RectilinearGrid* Grid;

            /** Performs actual file parsing */
            ASCIIParser* Parser;

    }; // -----  end of class  RectilinearGridReader  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef RECTILINEARGRIDREADER_INC  -----
