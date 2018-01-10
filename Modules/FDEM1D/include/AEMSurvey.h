/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      09/24/2013 04:06:42 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2013, XRI Geophysics, LLC
 * @copyright Copyright (c) 2013, Trevor Irons
 */

#ifndef  AEMSURVEY_INC
#define  AEMSURVEY_INC

#include "LemmaObject.h"
#include "DipoleSource.h"

namespace Lemma {

/**
  \brief   Contains pertinant information about an AEM survey
  \details All important details about an AEM survey are stored here, but
           nothing about the underlying earth model.
 */
class AEMSurvey : public LemmaObject {

    friend std::ostream &operator<<(std::ostream &stream,
            const AEMSurvey &ob);

    /// AEMSurveyReader currently provides the only concrete way to use this class.
    /// @todo consider making this an abstract class, that can only then exist through
    ///       the reader. That is if you will not allow users to build their own.
    friend class AEMSurveyReader;

    public:

    // ====================  LIFECYCLE     =======================

    /**
     * @copybrief LemmaObject::New()
     * @copydetails LemmaObject::New()
     */
    static AEMSurvey* New();

    /**
     *  @copybrief   LemmaObject::Delete()
     *  @copydetails LemmaObject::Delete()
     */
    void Delete();

    // ====================  OPERATORS     =======================

    // ====================  OPERATIONS    =======================

    // ====================  ACCESS        =======================

    /** Returns a particular EM source. For now only dipole sources are supported,
     *  general loops int the new future.
     *  @param[in] isource is the source fiducial to return
     */
    DipoleSource* GetSource(const int& isource);

    /** @return the total number of sources
     */
    int GetNumberOfSources();

    /** Returns vector of all frequencies used in the survey
     *  @return a vector of the unique frequencies
     */
    VectorXr GetFrequencies();

    // ====================  INQUIRY       =======================

    protected:

    // ====================  LIFECYCLE     =======================

    /** Default protected constructor, use New */
    AEMSurvey (const std::string& name);

    /** Default protected destructor, use Delete */
    ~AEMSurvey ();

    /**
     *  @copybrief   LemmaObject::Release()
     *  @copydetails LemmaObject::Release()
     */
    void Release();

    private:

    // ====================  DATA MEMBERS  =========================

    std::vector<DipoleSource*>  Sources;

    VectorXr Freqs;


}; // -----  end of class  AEMSurvey  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef AEMSURVEY_INC  -----

