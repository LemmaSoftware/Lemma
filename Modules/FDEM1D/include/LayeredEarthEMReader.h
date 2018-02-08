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
      \ingroup FDEM1D
      \brief Reads ASCII representation of LayeredEarth, similiar to UBC format.
      \details Largely superceded by YAML serialisation, this class remains for legacy purposes.
     */
    class LayeredEarthEMReader : public LemmaObject {

		friend std::ostream &operator<<(std::ostream &stream,
			const LayeredEarthEMReader &ob);

        public:

        // ====================  LIFECYCLE     =======================

        /**
         * @copybrief LemmaObject::NewSP()
         * @copydetails LemmaObject::NewSP()
         */
        static std::shared_ptr<LayeredEarthEMReader> NewSP();

        /** Default locked constructor, use New */
        explicit LayeredEarthEMReader ( const ctor_key& );

        /** Locked deserializing constructor. */
			LayeredEarthEMReader ( const YAML::Node& node, const ctor_key& );

        /** Default destructor */
        virtual ~LayeredEarthEMReader ();

        /* YAML Serializing method
         */
        //YAML::Node Serialize() const;

        /*
         *   Constructs an object from a YAML::Node.
         */
        //static std::shared_ptr< LayeredEarthEMReader > DeSerialize(const YAML::Node& node);

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
        std::shared_ptr<LayeredEarthEM> GetLayeredEarth();

        // ====================  INQUIRY       =======================

        /** Returns the name of the underlying class, similiar to Python's type */
        virtual std::string GetName() const {
            return this->CName;
        }

        protected:

        // ====================  LIFECYCLE     =======================

        private:

        // ====================  DATA MEMBERS  =========================

        std::shared_ptr<LayeredEarthEM>         LayEarth;

        /** ASCII string representation of the class name */
        static constexpr auto CName = "LayeredEarthEMReader";

    }; // -----  end of class  LayeredEarthEMReader  -----


}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef LAYEREDEARTHEMREADER_INC  -----
