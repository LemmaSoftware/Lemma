/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      10/08/2014 03:17:52 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */


#ifndef  TEMINDUCTIVERECEIVER_INC
#define  TEMINDUCTIVERECEIVER_INC

#include "TEMReceiver.h"

namespace Lemma {

    /**
      \brief   Derived class of TEMReceiver.
      \details An inductive receiver, measures dB/dt
     */
    class TEMInductiveReceiver : public TEMReceiver {

        friend std::ostream &operator<<(std::ostream &stream,
                const TEMInductiveReceiver &ob);

        public:

        // ====================  LIFECYCLE     =======================

        /** Default protected constructor, use NewSP */
        explicit TEMInductiveReceiver (const ctor_key& key);

        /** Default protected constructor, use DeSerialize */
        TEMInductiveReceiver (const YAML::Node& node, const ctor_key& key);

        /** Default protected destructor */
        ~TEMInductiveReceiver ();

        /**
         * @copybrief LemmaObject::New()
         * @copydetails LemmaObject::New()
         */
        static std::shared_ptr<TEMInductiveReceiver> NewSP();

        /**
         *  Uses YAML to serialize this object.
         *  @return a YAML::Node
         */
        YAML::Node Serialize() const;

        /**
         *   Constructs an object from a YAML::Node.
         */
        static std::shared_ptr<TEMInductiveReceiver> DeSerialize(const YAML::Node& node);

        // ====================  OPERATORS     =======================

        // ====================  OPERATIONS    =======================

        // ====================  ACCESS        =======================

        // ====================  INQUIRY       =======================

        /** Returns the name of the underlying class, similiar to Python's type */
        virtual std::string GetName() const {
            return this->CName;
        }

        protected:

        // ====================  LIFECYCLE     =======================

        private:

        // ====================  DATA MEMBERS  =========================

        static constexpr auto CName = "PolygonalWireAntenna";

    }; // -----  end of class  TEMInductiveReceiver  -----


}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef TEMINDUCTIVERECEIVER_INC  -----


