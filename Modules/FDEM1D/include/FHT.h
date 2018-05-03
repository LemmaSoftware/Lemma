/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      05/02/2018 09:46:38 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     tirons@egi.utah.edu
 * @copyright Copyright (c) 2018, University of Utah
 * @copyright Copyright (c) 2018, Trevor Irons & Lemma Software, LLC
 */


#pragma once
#include "HankelTransform.h"

namespace Lemma {

    /**
      \ingroup FDEM1D
      \brief   Impliments lagged and related fast Hankel transform through
               digital filtering.
      \details A general Fast Hankel Transform routine which uses the digital
               filter apporach. Both lagged and related kernels are supported in
               order to minimize kernel function calls.
               This approach performs a complete sweep of the
               coefficients , for a variant that uses a longer filter which may
               be truncated, see FHTAnderson801.
               @see FHTAnderson801
               @see GQChave
               @see QWEKey
     */
    class FHT : public HankelTransform {

        friend std::ostream &operator<<(std::ostream &stream, const FHT &ob);

        public:

        // ====================  LIFECYCLE     =======================

        /**
         * Default protected constructor, use NewSP methods to construct
         * @see FHT::NewSP
         */
        FHT (const ctor_key& key ) : HankelTransform( key ) {
        }

        /**
         * Protected DeDerializing constructor, use factory DeSerialize  method.
         * @see FHT::DeSerialize
         */
        FHT (const YAML::Node& node, const ctor_key& key) : HankelTransform(node, key) {
        }

        /** Default protected destructor, use smart pointers (std::shared_ptr) */
        ~FHT () {
        }

        /**
         *  Factory method for generating concrete class.
         *  @return a std::shared_ptr of type FHT
         */
        static std::shared_ptr< FHT > NewSP() {
            return std::make_shared< FHT >( ctor_key() );
        }

        /**
         *  Uses YAML to serialize this object.
         *  @return a YAML::Node
         *   @see FHT::DeSerialize
         */
        YAML::Node Serialize() const;

        /**
         *   Constructs an FHT object from a YAML::Node.
         *   @see FHT::Serialize
         */
        static std::shared_ptr<FHT> DeSerialize(const YAML::Node& node);

        // ====================  OPERATORS     =======================

        // ====================  OPERATIONS    =======================

        Complex Zgauss(const int&, const Lemma::EMMODE&, const int&, const Real&, const Real&, Lemma::KernelEM1DBase*) {
            return 0;
        }

        // ====================  ACCESS        =======================

        // ====================  INQUIRY       =======================

        /** Returns the name of the underlying class, similiar to Python's type */
        virtual std::string GetName() const {
            return this->CName;
        }


        protected:

        // ====================  LIFECYCLE     =======================

        // ====================  DATA MEMBERS  =========================

        private:

        static constexpr auto CName = "FHT";

    }; // -----  end of class  FHT  -----
}  // -----  end of namespace Lemma ----

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */


