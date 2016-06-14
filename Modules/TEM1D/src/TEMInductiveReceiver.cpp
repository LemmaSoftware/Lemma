/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      10/08/2014 03:19:58 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#include "TEMInductiveReceiver.h"

namespace Lemma {



    // ====================  FRIEND METHODS  =====================
#ifdef HAVE_YAMLCPP
    std::ostream &operator << (std::ostream &stream, const TEMInductiveReceiver &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc --- as a direct stream should encapulste thingy
        return stream;
    }
#else
    std::ostream &operator<<(std::ostream &stream, const TEMInductiveReceiver& ob) {
        stream << *(TEMReceiver*)(&ob);
        return stream;
    }
#endif

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  TEMInductiveReceiver
    //      Method:  TEMInductiveReceiver
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    TEMInductiveReceiver::TEMInductiveReceiver (const std::string& name) : TEMReceiver(name) {

    }  // -----  end of method TEMInductiveReceiver::TEMInductiveReceiver  (constructor)  -----

#ifdef HAVE_YAMLCPP
    //--------------------------------------------------------------------------------------
    //       Class:  TEMInductiveReceiver
    //      Method:  TEMInductiveReceiver
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    TEMInductiveReceiver::TEMInductiveReceiver (const YAML::Node& node) : TEMReceiver(node) {

    }  // -----  end of method TEMInductiveReceiver::TEMInductiveReceiver  (constructor)  -----
#endif

    //--------------------------------------------------------------------------------------
    //       Class:  TEMInductiveReceiver
    //      Method:  New()
    // Description:  public constructor
    //--------------------------------------------------------------------------------------
    TEMInductiveReceiver* TEMInductiveReceiver::New() {
        TEMInductiveReceiver*  Obj = new TEMInductiveReceiver("TEMInductiveReceiver");
        Obj->AttachTo(Obj);
        return Obj;
    }

    //--------------------------------------------------------------------------------------
    //       Class:  TEMInductiveReceiver
    //      Method:  ~TEMInductiveReceiver
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    TEMInductiveReceiver::~TEMInductiveReceiver () {

    }  // -----  end of method TEMInductiveReceiver::~TEMInductiveReceiver  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  TEMInductiveReceiver
    //      Method:  Delete
    // Description:  public destructor
    //--------------------------------------------------------------------------------------
    void TEMInductiveReceiver::Delete() {
        this->DetachFrom(this);
    }

    //--------------------------------------------------------------------------------------
    //       Class:  TEMInductiveReceiver
    //      Method:  Release
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    void TEMInductiveReceiver::Release() {
        delete this;
    }


#ifdef HAVE_YAMLCPP
    //--------------------------------------------------------------------------------------
    //       Class:  TEMInductiveReceiver
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node  TEMInductiveReceiver::Serialize (  ) const {
        YAML::Node node;
        node = TEMReceiver::Serialize();
        node.SetTag( this->Name ); // Set Tag after doing parents
        return node;
    }		// -----  end of method TEMInductiveReceiver::Serialize  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMInductiveReceiver
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    TEMInductiveReceiver* TEMInductiveReceiver::DeSerialize ( const YAML::Node& node  ) {
        TEMInductiveReceiver* Object = new TEMInductiveReceiver(node);
        Object->AttachTo(Object);
        DESERIALIZECHECK( node, Object )
        return Object ;
    }		// -----  end of method TEMInductiveReceiver::DeSerialize  -----
#endif


}		// -----  end of Lemma  name  -----

