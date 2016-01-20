/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     11/01/2010
  @version  $Id: emearth3d.cpp 193 2014-11-10 23:51:41Z tirons $
 **/

#include "emearth3d.h"

namespace Lemma {

    // ====================  LIFECYCLE     =======================

    EMEarth3D::EMEarth3D(const std::string &name) : LemmaObject(name)
    {
    }

    EMEarth3D* EMEarth3D::New() {
        EMEarth3D* Obj = new EMEarth3D("EMEarth3D");
        Obj->AttachTo(Obj);
        return Obj;
    }

    EMEarth3D::~EMEarth3D ( ) {
    }

    void EMEarth3D::Delete() {
        this->DetachFrom(this);
    }

    void EMEarth3D::Release() {
        delete this;
    }

}		// -----  end of Lemma  name  -----
