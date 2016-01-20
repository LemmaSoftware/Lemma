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

        /**
         * @copybrief LemmaObject::New()
         * @copydetails LemmaObject::New()
         */
        static TEMInductiveReceiver* New();

        /**
         *  @copybrief   LemmaObject::Delete()
         *  @copydetails LemmaObject::Delete()
         */
        void Delete();

        // ====================  OPERATORS     =======================

        // ====================  OPERATIONS    =======================

        // ====================  ACCESS        =======================

        // ====================  INQUIRY       =======================

#ifdef HAVE_YAMLCPP
        /**
         *  Uses YAML to serialize this object.
         *  @return a YAML::Node
         */
        YAML::Node Serialize() const;

        /**
         *   Constructs an object from a YAML::Node.
         */
        static TEMInductiveReceiver* DeSerialize(const YAML::Node& node);
#endif

        protected:

        // ====================  LIFECYCLE     =======================

        /** Default protected constructor, use New */
        TEMInductiveReceiver (const std::string& name);

#ifdef HAVE_YAMLCPP
        /** Default protected constructor, use New */
        TEMInductiveReceiver (const YAML::Node& node);
#endif

        /** Default protected destructor, use Delete */
        ~TEMInductiveReceiver ();

        /**
         *  @copybrief   LemmaObject::Release()
         *  @copydetails LemmaObject::Release()
         */
        void Release();

        private:

        // ====================  DATA MEMBERS  =========================

    }; // -----  end of class  TEMInductiveReceiver  -----


}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef TEMINDUCTIVERECEIVER_INC  -----


