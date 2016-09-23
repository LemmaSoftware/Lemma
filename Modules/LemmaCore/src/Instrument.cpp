/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     02/05/2010
  @version  $Id: instrument.cpp 193 2014-11-10 23:51:41Z tirons $
 **/

#include "Instrument.h"

namespace Lemma {

    std::ostream &operator << (std::ostream &stream, const Instrument &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc --- as a direct stream should encapulste thingy
        return stream;
    }

	Instrument::Instrument( ) : LemmaObject() {
	}

    Instrument::Instrument( const YAML::Node& node ) : LemmaObject( node ) {
	    // Fill in class specifics
    }

	Instrument::~Instrument() {
	}

    //--------------------------------------------------------------------------------------
    //       Class:  Instrument
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node  Instrument::Serialize (  ) const {
        YAML::Node node = LemmaObject::Serialize();;
        node.SetTag( GetName() );
        // Fill in class specifics
        return node;
    }		// -----  end of method Instrument::Serialize  -----

}		// -----  end of Lemma  name  -----
