/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      11/10/2014 10:53:34 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */


#ifndef  DCIPELECTRODE_INC
#define  DCIPELECTRODE_INC

#include "LemmaObject.h"

namespace Lemma {

    /**
      \brief
      \details
     */
    class DCIPElectrode : public LemmaObject {

        friend std::ostream &operator<<(std::ostream &stream,
                const DCIPElectrode &ob);

        public:

        // ====================  LIFECYCLE     =======================

        /**
         * @copybrief LemmaObject::New()
         * @copydetails LemmaObject::New()
         */
        static DCIPElectrode* New();

        /**
         *  @copybrief   LemmaObject::Delete()
         *  @copydetails LemmaObject::Delete()
         */
        void Delete();

        // ====================  OPERATORS     =======================

        // ====================  OPERATIONS    =======================

        // ====================  ACCESS        =======================

        /** Sets the physical location of the electrode
         *  @param[in] loc is the location
         */
        void SetLocation(const Vector3r& loc);

        /** Sets the tag (label) for the electrode
         */
        void SetLabel(const std::string& tag);

        /** Sets the tag (label) for the electrode
         */
        std::string GetLabel( );

        int GetNodeID() {return Node_ID;}
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
        static DCIPElectrode* DeSerialize(const YAML::Node& node);
#endif

        protected:

        // ====================  LIFECYCLE     =======================

        /** Default protected constructor, use New */
        DCIPElectrode (const std::string& name);

#ifdef HAVE_YAMLCPP
        /** Protected DeDerializing constructor, use factory DeSerialize  method*/
        DCIPElectrode (const YAML::Node& node);
#endif

        /** Default protected destructor, use Delete */
        ~DCIPElectrode ();

        /**
         *  @copybrief   LemmaObject::Release()
         *  @copydetails LemmaObject::Release()
         */
        void Release();

        private:

        // ====================  DATA MEMBERS  =========================

        /** The location of the electrode */
        Vector3r Location;

        /** Local Node_ID on a mesh */
        int Node_ID;

        /** String label for the electrode */
        std::string Label;

    }; // -----  end of class  DCIPElectrode  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef DCIPELECTRODE_INC  -----

