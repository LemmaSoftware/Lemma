/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     02/05/2010
  @version  $Id: pulse.cpp 193 2014-11-10 23:51:41Z tirons $
 **/

#include "pulse.h"

namespace Lemma {

	std::ostream &operator<<(std::ostream &stream,
			const Pulse &ob) {
		stream << *(LemmaObject*)(&ob);
		return stream;
	}

	Pulse::Pulse(const std::string &name) : LemmaObject(name) {

	}

	Pulse::~Pulse() {
	}
}		// -----  end of Lemma  name  -----

