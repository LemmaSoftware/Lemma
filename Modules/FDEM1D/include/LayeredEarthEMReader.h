/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      09/27/2013 01:44:36 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@lemmasoftware.org
 * @copyright Copyright (c) 2013, 2018 Trevor Irons
 */

#ifndef  LAYEREDEARTHEMREADER_INC
#define  LAYEREDEARTHEMREADER_INC

#include "LemmaObject.h"
#include "ASCIIParser.h"
#include "LayeredEarthEM.h"

namespace Lemma {

    /**
      \brief
      \details
     */
    class LayeredEarthEMReader : public LemmaObject {

		friend std::ostream &operator<<(std::ostream &stream,
			const LayeredEarthEMReader &ob);

        friend std::ostream &operator<<(std::ostream &stream,
                const LayeredEarthEMReader &ob);

        struct ctor_key{};

        public:

        // ====================  LIFECYCLE     =======================

        /**
         * @copybrief LemmaObject::New()
         * @copydetails LemmaObject::New()
         */
        static LayeredEarthEMReader* New();

        /**
         *  @copybrief   LemmaObject::Delete()
         *  @copydetails LemmaObject::Delete()
         */
        void Delete();

        // ====================  OPERATORS     =======================

        // ====================  OPERATIONS    =======================

        /** Reads ASCII input file. The format is as follows
@code
4           // Number of NON-AIR layers
200   10    // resistivity, thickness layer 1 [Ohm-m] [m]
20    10    // resistivity, thickness layer 2 [Ohm-m] [m]
1120  10    // resistivity, thickness layer 3 [Ohm-m] [m]
200         // resistivity of bottom layer [Ohm-m]
@endcode
         * @param[in] name is the filename
         */
        void ReadASCIIInputFile( const std::string& name );

        // ====================  ACCESS        =======================

        /** @return the pointer address of the LayeredEarthEM
         */
        LayeredEarthEM* GetLayeredEarth();

        // ====================  INQUIRY       =======================

        protected:

        // ====================  LIFECYCLE     =======================

        /** Default protected constructor, use New */
        LayeredEarthEMReader (const std::string& name);

        /** Default protected destructor, use Delete */
        ~LayeredEarthEMReader ();

        /**
         *  @copybrief   LemmaObject::Release()
         *  @copydetails LemmaObject::Release()
         */
        void Release();

        private:

        // ====================  DATA MEMBERS  =========================

        LayeredEarthEM*         LayEarth;

    }; // -----  end of class  LayeredEarthEMReader  -----


}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef LAYEREDEARTHEMREADER_INC  -----
