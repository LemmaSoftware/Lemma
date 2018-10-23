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
      \brief    Describes the location of an electrode used in a DC/IP survey
      \details  This class is used together with FEM4EllipticPDE to solve DC/IP
                problems.
     */
    class DCIPElectrode : public LemmaObject {

        friend std::ostream &operator<<(std::ostream &stream,
                const DCIPElectrode &ob);

        public:

        // ====================  LIFECYCLE     =======================

        /**
         * \brief   Returns new shared pointer to DCIP object.
         * \details Use this method, as the default constructor is locked.
         */
        static std::shared_ptr<DCIPElectrode> NewSP();

        /**
         *  Uses YAML to serialize this object.
         *  @return a YAML::Node
         */
        YAML::Node Serialize() const;

        /**
         *   Constructs an object from a YAML::Node.
         */
        static std::shared_ptr<DCIPElectrode> DeSerialize(const YAML::Node& node);

        /** Default protected constructor, use New */
        explicit DCIPElectrode (const ctor_key& key);

        /** Protected DeDerializing constructor, use factory DeSerialize  method*/
        DCIPElectrode (const YAML::Node& node, const ctor_key& key);

        /** Default protected destructor, use Delete */
        virtual ~DCIPElectrode ();

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

        /** Returns the name of the underlying class, similiar to Python's type */
        virtual std::string GetName() const {
            return this->CName;
        }

        protected:

        // ====================  LIFECYCLE     =======================

        private:

        // ====================  DATA MEMBERS  =========================

        /** ASCII string representation of the class name */
        static constexpr auto CName = "DCIPElectrode";

        /** no copy */
        DCIPElectrode ( const DCIPElectrode& ) = delete;

        /** The location of the electrode */
        Vector3r Location;

        /** Local Node_ID on a mesh */
        int Node_ID;

        /** String label for the electrode */
        std::string Label;

    }; // -----  end of class  DCIPElectrode  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef DCIPELECTRODE_INC  -----

