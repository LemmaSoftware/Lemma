/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     07/20/2010
  @version  $Id: filter.cpp 193 2014-11-10 23:51:41Z tirons $
 **/

#include "Filter.h"

namespace Lemma {

    std::ostream &operator << (std::ostream &stream, const Filter &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc --- as a direct stream should encapulste thingy
        return stream;
    }

    //--------------------------------------------------------------------------------------
    //       Class:  Filter
    //      Method:  Filter
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    Filter::Filter( const ctor_key& key ) : LemmaObject( key ) { }

    //--------------------------------------------------------------------------------------
    //       Class:  Filter
    //      Method:  ~Filter
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    Filter::~Filter( ) { }

    //--------------------------------------------------------------------------------------
    //       Class:  Filter
    //      Method:  Filter
    // Description:  DeSerializing constructor (protected)
    //--------------------------------------------------------------------------------------
    Filter::Filter ( const YAML::Node& node, const ctor_key& key ) : LemmaObject(node, key) {
    }

    //--------------------------------------------------------------------------------------
    //       Class:  Filter
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node  Filter::Serialize (  ) const {
        YAML::Node node = LemmaObject::Serialize();;
        node.SetTag( GetName() );
        // FILL IN CLASS SPECIFICS HERE
        return node;
    }		// -----  end of method Filter::Serialize  -----

    //--------------------------------------------------------------------------------------
    //       Class:  Filter
    //      Method:  GetName
    // Description:  Class identifier
    //--------------------------------------------------------------------------------------
    inline std::string Filter::GetName (  ) const {
        return CName;
    }		// -----  end of method Filter::get_GetName  -----

}		// -----  end of Lemma  name  -----
