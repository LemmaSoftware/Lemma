/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      14/04/19 14:25:07
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@utah.edu
 * @copyright Copyright (c) 2019, University of Utah
 * @copyright Copyright (c) 2019, Lemma Software, LLC
 */


#pragma once
#include "LemmaObject.h"

namespace Lemma {

    /**
     * \ingroup FDEM1D
     * \brief
     * \details
     */
    class ChargedWellCasing : public LemmaObject {

        friend std::ostream &operator<<(std::ostream &stream, const ChargedWellCasing &ob);

        protected:
        /*
         *  This key is used to lock the constructor. It is protected so that inhereted
         *  classes also have the key to contruct their base class.
         */
        struct ctor_key {};

        public:

        // ====================  LIFECYCLE     =======================

        /**
         * Default constructor.
         * @note This method is locked, and cannot be called directly.
         *       The reason that the method is public is to enable the use
         *       of make_shared whilst enforcing the use of shared_ptr,
         *       in c++-17, this curiosity may be resolved.
         * @see ChargedWellCasing::NewSP
         */
        explicit ChargedWellCasing ( const ctor_key& );

        /**
         * DeSerializing constructor.
         * @note This method is locked, and cannot be called directly.
         *       The reason that the method is public is to enable the use
         *       of make_shared whilst enforcing the use of shared_ptr,
         *       in c++-17, this curiosity may be resolved.
         * @see ChargedWellCasing::DeSerialize
         */
        ChargedWellCasing ( const YAML::Node& node, const ctor_key& );

        /**
         * Default destructor.
         * @note This method should never be called due to the mandated
         *       use of smart pointers. It is necessary to keep the method
         *       public in order to allow for the use of the more efficient
         *       make_shared constructor.
         */
        virtual ~ChargedWellCasing ();

        /**
         *  Uses YAML to serialize this object.
         *  @return a YAML::Node
         *  @see ChargedWellCasing::DeSerialize
         */
        virtual YAML::Node Serialize() const;

        /*
         *  Factory method for generating concrete class.
         *  @return a std::shared_ptr of type ChargedWellCasing
         */
        static std::shared_ptr< ChargedWellCasing > NewSP();

        /**
         *   Constructs an ChargedWellCasing object from a YAML::Node.
         *   @see ChargedWellCasing::Serialize
         */
        static std::shared_ptr<ChargedWellCasing> DeSerialize(const YAML::Node& node);

        // ====================  OPERATORS     =======================

        // ====================  OPERATIONS    =======================

        // ====================  ACCESS        =======================

        /**
         *  Many boreholes have numerous segments of varying thickness.
         *  @param[in] The number of segments that the borehole contains.
         */
        void SetNumberOfSegments( const int& ns ) {
            NumberOfSegments = ns;
        }

        /**
         * @return The number of segments
         */
        int GetNumberOfSegments( void );

        // ====================  INQUIRY       =======================
        /**
         *  Returns the name of the underlying class, similiar to Python's type
         *  @return string of class name
         */
        virtual inline std::string GetName() const {
            return CName;
        }

        protected:

        // ====================  LIFECYCLE     =======================

        /** Copy is disabled */
        ChargedWellCasing( const ChargedWellCasing& ) = delete;

        // ====================  DATA MEMBERS  =========================

        private:

        /** ASCII string representation of the class name */
        static constexpr auto CName = "ChargedWellCasing";

        /** Total number of casing segments */
        int NumberOfSegments;

        VectorXr OutsideDiameter;

        VectorXr InsideDiameter;

        VectorXr SegmentConductivity;

    }; // -----  end of class  ChargedWellCasing  -----
}  // -----  end of namespace Lemma ----

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */


