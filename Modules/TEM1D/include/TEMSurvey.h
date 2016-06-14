/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      10/08/2014 01:51:50 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#ifndef  TEMSURVEY_INC
#define  TEMSURVEY_INC

#include "LemmaObject.h"
#include "TEMSurveyLine.h"
#include "TEMSurveyData.h"

#include "layeredearthem.h"

#ifdef HAVE_YAMLCPP
#include "yaml-cpp/yaml.h"
#endif

namespace Lemma {


    /**
      \brief   Describes a TEM survey.
      \details This class aims to encapulate any type of TEM survey.
     */
    class TEMSurvey : public LemmaObject {

        friend std::ostream &operator<<(std::ostream &stream,
                const TEMSurvey &ob);

        public:

        // ====================  LIFECYCLE     =======================

        /**
         * @copybrief LemmaObject::New()
         * @copydetails LemmaObject::New()
         */
        static TEMSurvey* New();

        /**
         *  @copybrief   LemmaObject::Delete()
         *  @copydetails LemmaObject::Delete()
         */
        void Delete();

        // ====================  OPERATORS     =======================

        /**  @param[in] idx the index to return
         *   @return the SurveyLine with index idx
         */
        TEMSurveyLine* operator( ) ( const int& idx ) const ;

        /**  @param[in] iline the line index to return
         *   @param[in] irec the line index to return
         *   @return the SurveyLine with index iline, irec
         */
        TEMSurveyLineRecord* operator( ) ( const int& iline, const int& irec ) const;

        // ====================  OPERATIONS    =======================

        /**
         *  @param[in] nlines sets the number of lines (TEMSurveyLine) in the survey. A line is any kind of
         *             related set of measurements. In Airborne EM, this is a flight line, but
         *             the idea extends to ground based profile lines.
         */
        void SetNumberOfLines( const int& nlines );

        /**
         *  Most basic form of forward modelling. Uses input model for ALL TEMSurveyLines and
         *  TEMSurveyLineRecords (atomic). Most useful in the
         *  case that there is only 1 line and 1 record. But even in that case it can be called directly from
         *  the record. This method will likely be replaced  soon.
         */
        TEMSurveyData* ForwardModel(LayeredEarthEM* model, bool additiveNoise=false);

        // ====================  ACCESS        =======================

        /**
         *   @return pointer to requested TEMSurveyLine
         */
        TEMSurveyLine*  GetLine(const unsigned int& iline);

        // ====================  INQUIRY       =======================

        #ifdef HAVE_YAMLCPP
        YAML::Node Serialize() const;
        static TEMSurvey* DeSerialize(const YAML::Node& node);
        #endif

        protected:

        // ====================  LIFECYCLE     =======================

        /** Default protected constructor, use New */
        TEMSurvey (const std::string& name);

        #ifdef HAVE_YAMLCPP
        /** Default protected constructor, use New */
        TEMSurvey (const YAML::Node& node);
        #endif

        /** Default protected destructor, use Delete */
        ~TEMSurvey ();

        /**
         *  @copybrief   LemmaObject::Release()
         *  @copydetails LemmaObject::Release()
         */
        void Release();

        private:

        // ====================  DATA MEMBERS  =========================

        std::vector<TEMSurveyLine*>     Lines;

    }; // -----  end of class  TEMSurvey  -----


}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef TEMSURVEY_INC  -----
