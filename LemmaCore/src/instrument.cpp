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

#include "instrument.h"

namespace Lemma {

	std::ostream &operator<<(std::ostream &stream,
				const Instrument &ob) {

		stream << *(LemmaObject*)(&ob);
		return stream;
	}

	Instrument::Instrument(const std::string &name) :
			LemmaObject(name) {
	}

	Instrument::~Instrument() {
	}

}		// -----  end of Lemma  name  -----
