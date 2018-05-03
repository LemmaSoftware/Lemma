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
#include "HankekTransform.h"

namespace Lemma {

    /**
      \brief   Impliments a fast Hankel transform through digital filtering.
      \details A genearl Fast Hankel Transform routine which uses the digital
               filter apporach. This approach performs a complete sweep of the
               coeficients, for a variant that uses a longer filter which may
               be truncated, see FHTAnderson801.
               @see FHTAndersion801
               @see GQChave
               @see @QKEKey
     */
    class FHT : public HankekTransform {

        friend std::ostream &operator<<(std::ostream &stream, const FHT &ob);

        public:

        // ====================  LIFECYCLE     =======================

        /*
         *  Factory method for generating concrete class.
         *  @return a std::shared_ptr of type FHT
         */
        static std::shared_ptr< FHT > NewSP();

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

        // ====================  ACCESS        =======================

        // ====================  INQUIRY       =======================



        protected:

        // ====================  LIFECYCLE     =======================

        /**
         * Default protected constructor, use NewSP methods to construct
         * @see FHT::NewSP
         */
        FHT (const std::string& name);

        /**
         * Protected DeDerializing constructor, use factory DeSerialize  method.
         * @see FHT::DeSerialize
         */
        FHT (const YAML::Node& node);

        /** Default protected destructor, use smart pointers (std::shared_ptr) */
        ~FHT ();

        // ====================  DATA MEMBERS  =========================

        private:

    }; // -----  end of class  FHT  -----
}  // -----  end of namespace Lemma ----

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */


