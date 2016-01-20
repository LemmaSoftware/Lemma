/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     02/07/2011
  @version  $Id: integrator.cpp 193 2014-11-10 23:51:41Z tirons $
 **/

#include "integrator.h"

namespace Lemma {

    std::ostream &operator<<(std::ostream &stream,
			const Integrator &ob) {
		stream << *(LemmaObject*)(&ob);
		return stream;
	}

    Integrator::Integrator(const std::string& name) :
        LemmaObject(name)  {
    }

    Integrator::~Integrator( ) {
    }

}		// -----  end of Lemma  name  -----

