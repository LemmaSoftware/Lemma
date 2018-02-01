/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      02/01/2018 10:24:02 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     tirons@egi.utah.edu
 * @copyright Copyright (c) 2018, University of Utah
 * @copyright Copyright (c) 2018, Lemma Software, LLC
 */

#ifndef  CIRCULARLOOP_INC
#define  CIRCULARLOOP_INC

#pragma once
#include "CompactSupportEMSource.h"

namespace Lemma {

    /**
     * \ingroup FDEM1D
     * \brief   Analytic solution of a circular loop.
     * \details This specialized class provides the EM response of a circular loop on the surface of the earth.
     *          More general use cases can use PolygonalWireAntenna.
     */
    class CircularLoop : public CompactSupportEMSource {

        friend std::ostream &operator<<(std::ostream &stream, const CircularLoop &ob);

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
         * @see CircularLoop::NewSP
         */
        explicit CircularLoop ( const ctor_key& );

        /**
         * DeSerializing constructor.
         * @note This method is locked, and cannot be called directly.
         *       The reason that the method is public is to enable the use
         *       of make_shared whilst enforcing the use of shared_ptr,
         *       in c++-17, this curiosity may be resolved.
         * @see CircularLoop::DeSerialize
         */
        CircularLoop ( const YAML::Node& node, const ctor_key& );

        /**
         * Default destructor.
         * @note This method should never be called due to the mandated
         *       use of smart pointers. It is necessary to keep the method
         *       public in order to allow for the use of the more efficient
         *       make_shared constructor.
         */
        virtual ~CircularLoop ();

        /**
         *  Uses YAML to serialize this object.
         *  @return a YAML::Node
         *  @see CircularLoop::DeSerialize
         */
        virtual YAML::Node Serialize() const;

        /*
         *  Factory method for generating concrete class.
         *  @return a std::shared_ptr of type CircularLoop
         */
        static std::shared_ptr< CircularLoop > NewSP();

        /**
         *   Constructs an CircularLoop object from a YAML::Node.
         *   @see CircularLoop::Serialize
         */
        static std::shared_ptr<CircularLoop> DeSerialize(const YAML::Node& node);

        // ====================  OPERATORS     =======================

        // ====================  OPERATIONS    =======================

        // ====================  ACCESS        =======================

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
        CircularLoop( const CircularLoop& ) = delete;

        // ====================  DATA MEMBERS  =========================

        private:

        /** ASCII string representation of the class name */
        static constexpr auto CName = "CircularLoop";

    }; // -----  end of class  CircularLoop  -----
}  // -----  end of namespace Lemma ----

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */


#endif   // ----- #ifndef CIRCULARLOOP_INC  -----

