/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      03/03/2015 09:54:25
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2015, XRI Geophysics, LLC
 * @copyright Copyright (c) 2015, Trevor Irons
 */

#ifndef  TEMSURVEYDATA_INC
#define  TEMSURVEYDATA_INC

#include "LemmaObject.h"
#include "TEMSurveyLineData.h"

namespace Lemma {

    /**
      \brief    Holds data from a TEMSurvey.
      \details  Any form of TEMSurvey may be represented here. The
                majority of the specification can be found in the associated
                TEMSurvey. The TEMSurveyData object is intended to be lightweight.
     */
    class TEMSurveyData : public LemmaObject {

        friend std::ostream &operator<<(std::ostream &stream,
                const TEMSurveyData &ob);

        friend class TEMSurvey;

        public:

        // ====================  LIFECYCLE     =======================

        /**
         * @copybrief LemmaObject::New()
         * @copydetails LemmaObject::New()
         */
        static TEMSurveyData* New();

        /**
         *  @return a deep copy of this, this is acomplished through Serialize
         *          and DeSerialize methods.
         */
        TEMSurveyData* Clone();

        /**
         *  @copybrief   LemmaObject::Delete()
         *  @copydetails LemmaObject::Delete()
         */
        void Delete();

        // ====================  OPERATORS     =======================

        /** Surveys can be added or subtracted from each other, throws a
         *  pointer that will need to be managed (Deleted)
         */
        TEMSurveyData* operator+(const TEMSurveyData& Survey);

        /** Surveys can be added or subtracted from each other, throws a
         *  pointer that will need to be managed (Deleted).
         */
        TEMSurveyData* operator-(const TEMSurveyData& Survey);

        /** Surveys can be added or subtracted from each other
         */
        void operator+=(const TEMSurveyData& Survey);

        /** Surveys can be added or subtracted from each other
         */
        void operator-=(const TEMSurveyData& Survey);


        /**  @param[in] idx the index to return
         *   @return the SurveyLine with index idx
         */
        TEMSurveyLineData* operator( ) ( const int& idx ) const;

        /**  @param[in] iline the line index to return
         *   @param[in] irec the line index to return
         *   @return the SurveyLine with index iline, irec
         */
        TEMSurveyLineRecordData* operator( ) ( const int& iline, const int& irec ) const;


        // ====================  OPERATIONS    =======================

        /** Sets the number of lines
         */
        void SetNumberOfLines( const int& nlines );

        // ====================  ACCESS        =======================

        /** @return the number of lines in the data
         */
        int GetNumberOfLines() const;

        /**
         *   @return pointer to line
         */
        TEMSurveyLineData* GetLine(const int& iline) const;

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
        static TEMSurveyData* DeSerialize(const YAML::Node& node);
#endif

        protected:

        // ====================  LIFECYCLE     =======================

        /** Default protected constructor, use New */
        TEMSurveyData (const std::string& name);

#ifdef HAVE_YAMLCPP
        /** Protected DeDerializing constructor, use factory DeSerialize  method*/
        TEMSurveyData (const YAML::Node& node);
#endif

        /** Default protected destructor, use Delete */
        ~TEMSurveyData ();

        /**
         *  @copybrief   LemmaObject::Release()
         *  @copydetails LemmaObject::Release()
         */
        void Release();

        private:

        // ====================  DATA MEMBERS  =========================

        std::vector<TEMSurveyLineData*>  LineData;

    }; // -----  end of class  TEMSurveyData  -----



}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef TEMSURVEYDATA_INC  -----
