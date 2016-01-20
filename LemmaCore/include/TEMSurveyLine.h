/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      10/10/2014 11:14:39 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#ifndef  TEMSURVEYLINE_INC
#define  TEMSURVEYLINE_INC

#include "LemmaObject.h"
#include "TEMSurveyLineRecord.h"
#include "TEMSurveyLineData.h"

namespace Lemma {

    /**
      \brief    Represents a TEM survey line.
      \details  This class is managed by TEMSurvey. You cannot create a TEMSurvey line directly
                this should be done through a TEMSurvey.
                @todo neeed to impliment a BaseClass besides LemmaObject without pure virtual Delete.
     */
    class TEMSurveyLine : public LemmaObject {

        friend std::ostream &operator<<(std::ostream &stream,
                const TEMSurveyLine &ob);

        friend class TEMSurvey;

        public:

        // ====================  LIFECYCLE     =======================



        // ====================  OPERATORS     =======================

        // ====================  OPERATIONS    =======================

        // ====================  ACCESS        =======================

        /**
            @return returns pointer to a specific record along the line
         */
        TEMSurveyLineRecord*  GetRecord(const unsigned int& irec);

        /**
            @param[in] nrec is the number of records along the line
         */
        void SetNumberOfRecords( const int& nrec );

        // ====================  INQUIRY       =======================

#ifdef HAVE_YAMLCPP
        /**
         *  Uses YAML to serialize this object.
         *  @return a YAML::Node
         */
        YAML::Node Serialize() const;

        /**
         *   Constructs an object from a YAML::Node.
         */
        static TEMSurveyLine* DeSerialize(const YAML::Node& node);
#endif

        protected:

        TEMSurveyLineData* ForwardModel(LayeredEarthEM* model, bool additiveNoise = false);

        // ====================  LIFECYCLE     =======================

        /** Default protected constructor, use New */
        TEMSurveyLine (const std::string& name);

#ifdef HAVE_YAMLCPP
        /** Protected DeDerializing constructor, use factory DeSerialize  method*/
        TEMSurveyLine (const YAML::Node& node);
#endif

        /** Default protected destructor, use Delete */
        ~TEMSurveyLine ();

        /**
         *  @copybrief   LemmaObject::Release()
         *  @copydetails LemmaObject::Release()
         */
        void Release();

        private:

        /**
         *   Delete should not be called...
         */
        void Delete();

        // ====================  DATA MEMBERS  =========================

        std::vector<TEMSurveyLineRecord*>     Records;

    }; // -----  end of class  TEMSurveyLine  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef TEMSURVEYLINE_INC  -----
