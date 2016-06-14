/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      03/03/2015 10:29:01
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2015, XRI Geophysics, LLC
 * @copyright Copyright (c) 2015, Trevor Irons
 */
#ifndef  TEMSURVEYLINERECORDDATA_INC
#define  TEMSURVEYLINERECORDDATA_INC

#include "LemmaObject.h"

namespace Lemma {

    /**
      \brief
      \details
     */
    class TEMSurveyLineRecordData : public LemmaObject {

        friend std::ostream &operator<<(std::ostream &stream,
                const TEMSurveyLineRecordData &ob);

        friend class TEMSurveyLineRecord;

        public:

        // ====================  LIFECYCLE     =======================

        /**
         * @copybrief LemmaObject::New()
         * @copydetails LemmaObject::New()
         */
        static TEMSurveyLineRecordData* New();


        /** Provides deep copy
         */
        TEMSurveyLineRecordData* Clone();

        /**
         *  @copybrief   LemmaObject::Delete()
         *  @copydetails LemmaObject::Delete()
         */
        void Delete();

        // ====================  OPERATORS     =======================

        /**
         * Records can be added
         */
        TEMSurveyLineRecordData* operator + (const TEMSurveyLineRecordData&);

        /**
         * Records can be subtracted
         */
        TEMSurveyLineRecordData* operator - (const TEMSurveyLineRecordData&);

        /**
         * Records can be added in place
         */
        void operator += (const TEMSurveyLineRecordData&);

        /**
         * Records can be subtracted in place
         */
        void operator -= (const TEMSurveyLineRecordData&);

        // ====================  OPERATIONS    =======================

        // ====================  ACCESS        =======================

        /** @return a VectorXr of the merged data (all moments)
         * @todo TODO this is actually a copy? is this intended?
         */
        VectorXr GetCombinedData();

        /** @return a VectorXr of the combined gate centres alligned with
         *          GetCombinedData();
         */
        VectorXr GetCombinedGateCentres();
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
        static TEMSurveyLineRecordData* DeSerialize(const YAML::Node& node);
#endif

        protected:

        // ====================  LIFECYCLE     =======================

        /** Default protected constructor, use New */
        TEMSurveyLineRecordData (const std::string& name);

#ifdef HAVE_YAMLCPP
        /** Protected DeDerializing constructor, use factory DeSerialize  method*/
        TEMSurveyLineRecordData (const YAML::Node& node);
#endif

        /** Default protected destructor, use Delete */
        ~TEMSurveyLineRecordData ();

        /**
         *  @copybrief   LemmaObject::Release()
         *  @copydetails LemmaObject::Release()
         */
        void Release();

        private:

        // ====================  DATA MEMBERS  =========================

        std::vector<VectorXr>           DataSeries;

        std::vector<VectorXr>           GateCentres;

    }; // -----  end of class  TEMSurveyLineRecordData  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef TEMSURVEYLINERECORDDATA_INC  -----
