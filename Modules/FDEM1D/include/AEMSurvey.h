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
 * @email     Trevor.Irons@lemmasoftware.org
 * @copyright Copyright (c) 2013, XRI Geophysics, LLC
 * @copyright Copyright (c) 2013,2018 Trevor Irons
 */

#ifndef  AEMSURVEY_INC
#define  AEMSURVEY_INC

#include "LemmaObject.h"
#include "DipoleSource.h"

namespace Lemma {

/**
  \ingroup FDEM1D
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
    static std::shared_ptr<AEMSurvey> NewSP();

    /** Default protected constructor, use New */
    AEMSurvey (const ctor_key&);

    /** Default protected destructor, use Delete */
    ~AEMSurvey ();

    // ====================  OPERATORS     =======================

    // ====================  OPERATIONS    =======================

    // ====================  ACCESS        =======================

    /** Returns a particular EM source. For now only dipole sources are supported,
     *  general loops int the new future.
     *  @param[in] isource is the source fiducial to return
     */
    std::shared_ptr<DipoleSource> GetSource(const int& isource);

    /** @return the total number of sources
     */
    int GetNumberOfSources();

    /** Returns vector of all frequencies used in the survey
     *  @return a vector of the unique frequencies
     */
    VectorXr GetFrequencies();

    // ====================  INQUIRY       =======================

    /** Returns the name of the underlying class, similiar to Python's type */
    virtual std::string GetName() const {
        return this->CName;
    }

    protected:

    // ====================  LIFECYCLE     =======================

    private:

    // ====================  DATA MEMBERS  =========================

    std::vector< std::shared_ptr<DipoleSource> >  Sources;

    VectorXr Freqs;

    static constexpr auto CName = "AEMSurvey";

}; // -----  end of class  AEMSurvey  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef AEMSURVEY_INC  -----

