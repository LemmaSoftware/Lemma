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
#include "RectilinearGrid.h"
#include "ASCIIParser.h"

namespace Lemma {

    /**
      @class
      \ingroup LemmaCore
      \brief   Base Class for rectilinear grid reader data files
      \details
     \todo this set of classes is deprecated, use serialize methods!
     */
    class RectilinearGridReader : public GridReader {

        friend std::ostream &operator<<(std::ostream &stream,
                const RectilinearGridReader &ob);

        /*
         *  This key is used to lock the constructors
         */
        struct ctor_key {};

        public:

            // ====================  LIFECYCLE     =======================

            /** Default constructor
             * @note This method is locked
             * @see NewSP
             */
            explicit RectilinearGridReader ( const ctor_key& );

            /**
             * DeSerializing constructor.
             * @note This method is locked, and cannot be called directly.
             *       The reason that the method is public is to enable the use
             *       of make_shared whilst enforcing the use of shared_ptr,
             *       in c++-17, this curiosity may be resolved.
             * @see DeSerialize
             */
            explicit RectilinearGridReader ( const YAML::Node& node, const ctor_key& );

            /** Default destructor.
             *  @note This should never be called explicitly, use NewSP
             */
            ~RectilinearGridReader ( );

            /**
             *  Factory method for generating concrete class.
             *  @return a std::shared_ptr of type RectilinearGridReader
             */
            static std::shared_ptr< RectilinearGridReader > NewSP();

            /**
             *  Constructs an object from a YAML serialization
             *  @return a std::shared_ptr of type RectilinearGridReader
             */
            static std::shared_ptr< RectilinearGridReader >  DeSerialize( const YAML::Node& node );

            /**
             *  Uses YAML to serialize this object.
             *  @return a YAML::Node
             */
            virtual YAML::Node Serialize() const;

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
            std::shared_ptr<Grid> GetGrid();

            // ====================  INQUIRY       =======================

            /** Returns the name of the underlying class, similiar to Python's type */
            virtual inline std::string GetName() const {
                return this->CName;
            }

        protected:

            // ====================  LIFECYCLE     =======================

        private:

            // ====================  DATA MEMBERS  =========================

            /** ASCII string representation of the class name */
            static constexpr auto CName = "RectilinearGridReader";

            /** Object holding constructed Grid */
            std::shared_ptr<RectilinearGrid> rGrid;

            /** Performs actual file parsing */
            std::shared_ptr<ASCIIParser> Parser;

    }; // -----  end of class  RectilinearGridReader  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef RECTILINEARGRIDREADER_INC  -----


/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp syntax=cpp.doxygen: */
