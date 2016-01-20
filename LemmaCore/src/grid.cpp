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

#include "grid.h"

namespace Lemma {

	std::ostream &operator<<(std::ostream &stream,
				const Grid &ob) {

		stream << *(LemmaObject*)(&ob);
		return stream;
	}

	Grid::Grid  ( const std::string& name ) : LemmaObject(name) {
	}

	Grid::~Grid  ( ) {
	}

}
