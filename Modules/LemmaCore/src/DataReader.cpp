/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     03/01/2010
  @version  $Id: datareader.cpp 193 2014-11-10 23:51:41Z tirons $
 **/

#include "DataReader.h"

namespace Lemma {

    std::ostream &operator << (std::ostream &stream, const DataReader &ob) {
        stream << ob.Serialize()  << "\n";
        return stream;
    }

	DataReader::DataReader( const ctor_key& key ) : LemmaObject( key ) {

	}

    DataReader::DataReader( const YAML::Node& node, const ctor_key& key ) : LemmaObject( node, key ) {

	}

	DataReader::~DataReader() {

	}

}		// -----  end of Lemma  name  -----
