/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     01/29/2010
  @version  $Id: kernel.cpp 193 2014-11-10 23:51:41Z tirons $
 **/

#include "kernel.h"
namespace Lemma {

	std::ostream &operator<<(std::ostream &stream,
				const Kernel &ob) {
		stream << *(LemmaObject*)(&ob);
		return stream;
	}

	Kernel::Kernel(const std::string& name) :
			LemmaObject(name) {
	}

	Kernel::~Kernel() {
	}
}
