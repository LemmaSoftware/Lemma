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

    std::ostream &operator << (std::ostream &stream, const TEMInductiveReceiver &ob) {
        stream << ob.Serialize()  << "\n";
        return stream;
    }

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  TEMInductiveReceiver
    //      Method:  TEMInductiveReceiver
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    TEMInductiveReceiver::TEMInductiveReceiver (const ctor_key& key) : TEMReceiver(key) {

    }  // -----  end of method TEMInductiveReceiver::TEMInductiveReceiver  (constructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  TEMInductiveReceiver
    //      Method:  TEMInductiveReceiver
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    TEMInductiveReceiver::TEMInductiveReceiver (const YAML::Node& node, const ctor_key& key) : TEMReceiver(node, key) {

    }  // -----  end of method TEMInductiveReceiver::TEMInductiveReceiver  (constructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  TEMInductiveReceiver
    //      Method:  New()
    // Description:  public constructor
    //--------------------------------------------------------------------------------------
    std::shared_ptr<TEMInductiveReceiver> TEMInductiveReceiver::NewSP() {
        return std::make_shared<TEMInductiveReceiver>( ctor_key() );
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
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node  TEMInductiveReceiver::Serialize (  ) const {
        YAML::Node node;
        node = TEMReceiver::Serialize();
        node.SetTag( this->GetName() ); // Set Tag after doing parents
        return node;
    }		// -----  end of method TEMInductiveReceiver::Serialize  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMInductiveReceiver
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    std::shared_ptr<TEMInductiveReceiver> TEMInductiveReceiver::DeSerialize ( const YAML::Node& node  ) {
        if (node.Tag() != CName) {
            throw  DeSerializeTypeMismatch( CName, node.Tag());
        }
        return std::make_shared<TEMInductiveReceiver>(node, ctor_key());
    }		// -----  end of method TEMInductiveReceiver::DeSerialize  -----

}		// -----  end of Lemma  name  -----

