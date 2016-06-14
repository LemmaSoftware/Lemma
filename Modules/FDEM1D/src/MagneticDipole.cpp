/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      09/25/2013 08:20:14 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2013, XRI Geophysics, LLC
 * @copyright Copyright (c) 2013, Trevor Irons
 */

#include "MagneticDipole.h"

namespace Lemma {

    // ====================  FRIEND METHODS  =====================

    std::ostream &operator<<(std::ostream &stream, const MagneticDipole &ob) {

        stream << *(DipoleSource*)(&ob);

        return stream;
    }
    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  MagneticDipole
    //      Method:  MagneticDipole
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    MagneticDipole::MagneticDipole (const std::string& name) : DipoleSource(name) {

    }  // -----  end of method MagneticDipole::MagneticDipole  (constructor)  -----


    //--------------------------------------------------------------------------------------
    //       Class:  MagneticDipole
    //      Method:  New()
    // Description:  public constructor
    //--------------------------------------------------------------------------------------
    MagneticDipole* MagneticDipole::New() {
        MagneticDipole*  Obj = new MagneticDipole("MagneticDipole");
        Obj->AttachTo(Obj);
        return Obj;
    }

    //--------------------------------------------------------------------------------------
    //       Class:  MagneticDipole
    //      Method:  ~MagneticDipole
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    MagneticDipole::~MagneticDipole () {

    }  // -----  end of method MagneticDipole::~MagneticDipole  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  MagneticDipole
    //      Method:  Delete
    // Description:  public destructor
    //--------------------------------------------------------------------------------------
    void MagneticDipole::Delete() {
        this->DetachFrom(this);
    }

    //--------------------------------------------------------------------------------------
    //       Class:  MagneticDipole
    //      Method:  Release
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    void MagneticDipole::Release() {
        delete this;
    }

} // namespace Lemma
