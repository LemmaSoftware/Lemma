/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      09/24/2013 04:29:27 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@lemmasoftware.org
 * @copyright Copyright (c) 2013, XRI Geophysics, LLC
 * @copyright Copyright (c) 2013, Trevor Irons
 */

#ifndef  AEMSURVEYREADER_INC
#define  AEMSURVEYREADER_INC

#include "LemmaObject.h"
#include "AEMSurvey.h"
#include "ASCIIParser.h"

namespace Lemma {

    /**
      \ingroup FDEM1D
      \brief   Reads an ASCII description of an AEM survey
      \details The file format is described as follows.
     */
    class AEMSurveyReader : public LemmaObject {

        friend std::ostream &operator<<(std::ostream &stream,
                const AEMSurveyReader &ob);

        public:

        // ====================  LIFECYCLE     =======================

        /**
         * @copybrief LemmaObject::NewSP()
         * @copydetails LemmaObject::NewSP()
         */
        static std::shared_ptr<AEMSurveyReader> NewSP();

        /** Default protected constructor, use New */
        AEMSurveyReader (const ctor_key&);

        /** Default protected destructor, use Delete */
        ~AEMSurveyReader ();

        // ====================  OPERATORS     =======================

        // ====================  OPERATIONS    =======================

        /**
         *
@code
// AEM Survey Specification file

     5    // N sources

400       // Source 1 frequency Hz
MD        // Source 1 type
-7  0 -10 // Source 1 relative position front/back, left/right, up/down
1         // Source 1 moment

1800      // Source 2  frequency Hz
VMD       // Source 2 type
-7  0 -10 // Source 2 relative position
1         // Source 2 moment

8200      // Source 3  freqeuncy Hz
MD        // Source 3 type
-7  0 -10 // Source 3 relative position
1         // Source 3 moment

40000     // Source 4  frequency Hz
MD        // Source 4 type
-7  0 -10 // Source 4 relative position
1         // Source 4 moment

140000    // Source 5  frequency Hz
MD        // Source 5 type
-7  0 -10 // Source 5 relative position
1         // Source 5 moment

// Flight Plan (in same coordinates as grid)
2     // Number of points

// location in x, y, z, polarisation [x,y,z]

-200 -200 -30  0  0  1
-190 -200 -30  0  0  1
@endcode
         */
        void ReadASCIIAEMFile( const std::string& name );

        // ====================  ACCESS        =======================

        /**
         * @return the AEMSurvey based on the file read
         */
        std::shared_ptr<AEMSurvey> GetSurvey();

        // ====================  INQUIRY       =======================
        /** Returns the name of the underlying class, similiar to Python's type */
        virtual std::string GetName() const {
            return this->CName;
        }

        protected:

        // ====================  LIFECYCLE     =======================

        private:

        // ====================  DATA MEMBERS  =========================

        std::shared_ptr<AEMSurvey>  Survey;

        static constexpr auto CName = "AEMSurveyReader";

    }; // -----  end of class  AEMSurveyReader  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef AEMSURVEYREADER_INC  -----

