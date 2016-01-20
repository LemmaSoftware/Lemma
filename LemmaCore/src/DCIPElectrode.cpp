/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      11/10/2014 10:53:53 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#include "DCIPElectrode.h"

namespace Lemma {



    // ====================  FRIEND METHODS  =====================
#ifdef HAVE_YAMLCPP
    std::ostream &operator << (std::ostream &stream, const DCIPElectrode &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc --- as a direct stream should encapulate thingy
        return stream;
    }
#else
    std::ostream &operator<<(std::ostream &stream, const DCIPElectrode& ob) {
        stream << *(LemmaObject*)(&ob);
        return stream;
    }
#endif

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  DCIPElectrode
    //      Method:  DCIPElectrode
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    DCIPElectrode::DCIPElectrode (const std::string& name) : LemmaObject(name), Node_ID(-1), Label(std::string("None")) {

    }  // -----  end of method DCIPElectrode::DCIPElectrode  (constructor)  -----

#ifdef HAVE_YAMLCPP
    //--------------------------------------------------------------------------------------
    //       Class:  DCIPElectrode
    //      Method:  DCIPElectrode
    // Description:  DeSerializing constructor (protected)
    //--------------------------------------------------------------------------------------
    DCIPElectrode::DCIPElectrode (const YAML::Node& node) : LemmaObject(node) {
        if (node.Tag() != this->Name) {
            throw std::runtime_error("In DCIPElectrode(node), node is of wrong type");
        }
        this->Location = node["Location"].as<Vector3r>();
        this->Node_ID = node["Node_ID"].as<int>();
        this->Label = node["Label"].as<std::string>();
    }  // -----  end of method DCIPElectrode::DCIPElectrode  (constructor)  -----
#endif

    //--------------------------------------------------------------------------------------
    //       Class:  DCIPElectrode
    //      Method:  New()
    // Description:  public constructor
    //--------------------------------------------------------------------------------------
    DCIPElectrode* DCIPElectrode::New() {
        DCIPElectrode*  Obj = new DCIPElectrode("DCIPElectrode");
        Obj->AttachTo(Obj);
        return Obj;
    }

    //--------------------------------------------------------------------------------------
    //       Class:  DCIPElectrode
    //      Method:  ~DCIPElectrode
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    DCIPElectrode::~DCIPElectrode () {

    }  // -----  end of method DCIPElectrode::~DCIPElectrode  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  DCIPElectrode
    //      Method:  Delete
    // Description:  public destructor
    //--------------------------------------------------------------------------------------
    void DCIPElectrode::Delete() {
        this->DetachFrom(this);
    }

    //--------------------------------------------------------------------------------------
    //       Class:  DCIPElectrode
    //      Method:  Release
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    void DCIPElectrode::Release() {
        delete this;
    }


#ifdef HAVE_YAMLCPP
    //--------------------------------------------------------------------------------------
    //       Class:  DCIPElectrode
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node  DCIPElectrode::Serialize (  ) const {
        YAML::Node node = LemmaObject::Serialize();
        node.SetTag( this->Name );
        // FILL IN CLASS SPECIFICS HERE
        node["Location"] = Location;
        node["Node_ID"] = Node_ID;
        node["Label"] = Label;
        return node;
    }		// -----  end of method DCIPElectrode::Serialize  -----


    //--------------------------------------------------------------------------------------
    //       Class:  DCIPElectrode
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    DCIPElectrode* DCIPElectrode::DeSerialize ( const YAML::Node& node  ) {
        DCIPElectrode* Object = new DCIPElectrode(node);
        Object->AttachTo(Object);
        DESERIALIZECHECK( node, Object )
        return Object ;
    }		// -----  end of method DCIPElectrode::DeSerialize  -----
#endif


    //--------------------------------------------------------------------------------------
    //       Class:  DCIPElectrode
    //      Method:  SetLocation
    //--------------------------------------------------------------------------------------
    void DCIPElectrode::SetLocation ( const Vector3r& loc ) {
        Location = loc;
        return ;
    }		// -----  end of method DCIPElectrode::SetLocation  -----



    //--------------------------------------------------------------------------------------
    //       Class:  DCIPElectrode
    //      Method:  SetTag
    //--------------------------------------------------------------------------------------
    void DCIPElectrode::SetLabel ( const std::string& inLabel  ) {
        Label = inLabel;
        return ;
    }		// -----  end of method DCIPElectrode::SetTag  -----


    //--------------------------------------------------------------------------------------
    //       Class:  DCIPElectrode
    //      Method:  get_Label
    //--------------------------------------------------------------------------------------
    std::string DCIPElectrode::GetLabel (  ) {
        return Label;
    }		// -----  end of method DCIPElectrode::get_Label  -----


}		// -----  end of Lemma  name  -----

