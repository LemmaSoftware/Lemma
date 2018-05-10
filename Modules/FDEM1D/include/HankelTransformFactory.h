/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      04/18/2018 11:59:00 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     tirons@egi.utah.edu
 * @copyright Copyright (c) 2018, University of Utah
 * @copyright Copyright (c) 2018, Lemma Software, LLC
 */


#pragma once
#include "LemmaObject.h"

#include "FHTAnderson801.h"
#include "FHT.h"

#include "FHTKey201.h"
#include "FHTKey101.h"
#include "FHTKey51.h"
#include "QWEKey.h"
#include "GQChave.h"

namespace Lemma {

    /**
     * \ingroup FDEM1D
     * \brief  Factory generator of HankelTranform types
     * \details
     */
    class HankelTransformFactory : public LemmaObject {

        friend std::ostream &operator<<(std::ostream &stream, const HankelTransformFactory &ob);

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
         * @see HankelTransformFactory::NewSP
         */
        explicit HankelTransformFactory ( const ctor_key& );

        /**
         * DeSerializing constructor.
         * @note This method is locked, and cannot be called directly.
         *       The reason that the method is public is to enable the use
         *       of make_shared whilst enforcing the use of shared_ptr,
         *       in c++-17, this curiosity may be resolved.
         * @see HankelTransformFactory::DeSerialize
         */
        HankelTransformFactory ( const YAML::Node& node, const ctor_key& );

        /**
         * Default destructor.
         * @note This method should never be called due to the mandated
         *       use of smart pointers. It is necessary to keep the method
         *       public in order to allow for the use of the more efficient
         *       make_shared constructor.
         */
        virtual ~HankelTransformFactory ();

        /**
         *  Uses YAML to serialize this object.
         *  @return a YAML::Node
         *  @see HankelTransformFactory::DeSerialize
         */
        virtual YAML::Node Serialize() const;

        /*
         *  Factory method for generating concrete class.
         *  @return a std::shared_ptr of type HankelTransformFactory
         */
        //static std::shared_ptr< HankelTransformFactory > NewSP();
        static std::shared_ptr< HankelTransform > NewSP( const HANKELTRANSFORMTYPE Type) {
            switch (Type) {
                case ANDERSON801:
                    return FHTAnderson801::NewSP();
                case FHTKEY201:
                    //return FHTKey201::NewSP();
                    return FHT<FHTKEY201>::NewSP();
                case FHTKEY101:
                    return FHT<FHTKEY101>::NewSP();
                    //return FHTKey101::NewSP();
                case FHTKEY51:
                    return FHT<FHTKEY51>::NewSP();
                case FHTKONG61:
                    return FHT<FHTKONG61>::NewSP();
                case FHTKONG121:
                    return FHT<FHTKONG121>::NewSP();
                case FHTKONG241:
                    return FHT<FHTKONG241>::NewSP();
                case CHAVE:
                    return GQChave::NewSP();
                case QWEKEY:
                    return QWEKey::NewSP();
                default:
                    std::cerr << "HankelTransformFactory only works with defined types\n";
            }

        }

        /**
         *   Constructs an HankelTransformFactory object from a YAML::Node.
         *   @see HankelTransformFactory::Serialize
         */
        static std::shared_ptr<HankelTransformFactory> DeSerialize(const YAML::Node& node);

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
        HankelTransformFactory( const HankelTransformFactory& ) = delete;

        // ====================  DATA MEMBERS  =========================

        private:

        /** ASCII string representation of the class name */
        static constexpr auto CName = "HankelTransformFactory";

    }; // -----  end of class  HankelTransformFactory  -----
}  // -----  end of namespace Lemma ----

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */


