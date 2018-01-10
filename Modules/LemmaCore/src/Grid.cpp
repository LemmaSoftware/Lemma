/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     10/25/2009
  @version  $Id: grid.cpp 193 2014-11-10 23:51:41Z tirons $
 **/

#include "Grid.h"

namespace Lemma {

    std::ostream &operator << (std::ostream &stream, const Grid &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc --- as a direct stream should encapulste thingy
        return stream;
    }

    // ====================  LIFECYCLE     ===================================

	Grid::Grid( const ctor_key& key ) : LemmaObject( key ) {
	}

    Grid::Grid( const YAML::Node& node, const ctor_key& key ) : LemmaObject(node, key) {

    }

	Grid::~Grid  ( ) {
	}

    YAML::Node Grid::Serialize() const {
        YAML::Node node = LemmaObject::Serialize();
        node.SetTag( this->GetName() );
        return node;
    }

    //--------------------------------------------------------------------------------------
    //       Class:  Grid
    //      Method:  GetName
    // Description:  Class identifier
    //--------------------------------------------------------------------------------------
    inline std::string Grid::GetName (  ) const {
        return CName;
    }		// -----  end of method Grid::get_GetName  -----

}
