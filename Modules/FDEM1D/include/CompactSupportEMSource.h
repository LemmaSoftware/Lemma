/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      02/01/2018 08:26:44 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     tirons@egi.utah.edu
 * @copyright Copyright (c) 2018, University of Utah
 * @copyright Copyright (c) 2018, Lemma Software, LLC
 */

#ifndef  COMPACTSUPPORTEMSOURCE_INC
#define  COMPACTSUPPORTEMSOURCE_INC

#pragma once
#include "LemmaObject.h"

namespace Lemma {

    /**
     * \ingroup FDEM1D
     * \brief   Abstract base class representing an EM source.
     * \details Derived types include simple dipoles, as well as line sources or wire loops.
     */
    class CompactSupportEMSource : public LemmaObject {

        friend std::ostream &operator<<(std::ostream &stream, const CompactSupportEMSource &ob);

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
         * @see CompactSupportEMSource::NewSP
         */
        explicit CompactSupportEMSource ( const ctor_key& );

        /**
         * DeSerializing constructor.
         * @note This method is locked, and cannot be called directly.
         *       The reason that the method is public is to enable the use
         *       of make_shared whilst enforcing the use of shared_ptr,
         *       in c++-17, this curiosity may be resolved.
         * @see CompactSupportEMSource::DeSerialize
         */
        CompactSupportEMSource ( const YAML::Node& node, const ctor_key& );

        /**
         * Default destructor.
         * @note This method should never be called due to the mandated
         *       use of smart pointers. It is necessary to keep the method
         *       public in order to allow for the use of the more efficient
         *       make_shared constructor.
         */
        virtual ~CompactSupportEMSource ();

        /**
         *  Uses YAML to serialize this object.
         *  @return a YAML::Node
         *  @see CompactSupportEMSource::DeSerialize
         */
        virtual YAML::Node Serialize() const;

        /*
         *  Factory method for generating concrete class.
         *  @return a std::shared_ptr of type CompactSupportEMSource
         */
        static std::shared_ptr< CompactSupportEMSource > NewSP();

        /**
         *   Constructs an CompactSupportEMSource object from a YAML::Node.
         *   @see CompactSupportEMSource::Serialize
         */
        static std::shared_ptr<CompactSupportEMSource> DeSerialize(const YAML::Node& node);

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
        CompactSupportEMSource( const CompactSupportEMSource& ) = delete;

        // ====================  DATA MEMBERS  =========================

        private:

        /** ASCII string representation of the class name */
        static constexpr auto CName = "CompactSupportEMSource";

    }; // -----  end of class  CompactSupportEMSource  -----
}  // -----  end of namespace Lemma ----

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */


#endif   // ----- #ifndef COMPACTSUPPORTEMSOURCE_INC  -----

