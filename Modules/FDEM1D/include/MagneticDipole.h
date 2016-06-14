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

#ifndef  MAGNETICDIPOLE_INC
#define  MAGNETICDIPOLE_INC

#include "dipolesource.h"

namespace Lemma {

/**
  @class
  \brief   Magnetic Dipole
  \details Used to model an idealised magnetic dipole of arbitrary
           polarisation.
 */
class MagneticDipole : public DipoleSource {

    friend std::ostream &operator<<(std::ostream &stream,
            const MagneticDipole &ob);

    public:

    // ====================  LIFECYCLE     =======================

    /**
     * @copybrief LemmaObject::New()
     * @copydetails LemmaObject::New()
     */
    static MagneticDipole* New();

    /**
     *  @copybrief   LemmaObject::Delete()
     *  @copydetails LemmaObject::Delete()
     */
    void Delete();

    // ====================  OPERATORS     =======================

    // ====================  OPERATIONS    =======================

    // ====================  ACCESS        =======================

    // ====================  INQUIRY       =======================

    protected:

    // ====================  LIFECYCLE     =======================

    /** Default protected constructor, use New */
    MagneticDipole (const std::string& name);

    /** Default protected destructor, use Delete */
    ~MagneticDipole ();

    /**
     *  @copybrief   LemmaObject::Release()
     *  @copydetails LemmaObject::Release()
     */
    void Release();

    private:

    // ====================  DATA MEMBERS  =========================

    KernelEM1DManager*          Kernels;

}; // -----  end of class  MagneticDipole  -----

} // namespace Lemma

#endif   // ----- #ifndef MAGNETICDIPOLE_INC  -----
