/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      03/03/2015 10:16:47
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2015, XRI Geophysics, LLC
 * @copyright Copyright (c) 2015, Trevor Irons
 */

#ifndef  TEMSURVEYLINEDATA_INC
#define  TEMSURVEYLINEDATA_INC

#include "LemmaObject.h"
#include "TEMSurveyLineRecordData.h"

namespace Lemma {

/**
  \brief   Holds Data for a TEM survey line
  \details An entire flight line worth of data is stored here.
 */
class TEMSurveyLineData : public LemmaObject {

    friend std::ostream &operator<<(std::ostream &stream,
            const TEMSurveyLineData &ob);

    //friend class TEMSurveyData;
    friend class TEMSurveyLine;

    public:

    // ====================  LIFECYCLE     =======================

    /**
     * @copybrief LemmaObject::New()
     * @copydetails LemmaObject::New()
     */
    static TEMSurveyLineData* New();

    /**
     *  @return a deep copy of this, this is acomplished through Serialize
     *          and DeSerialize methods.
     */
    TEMSurveyLineData* Clone();

    /**
     *  @copybrief   LemmaObject::Delete()
     *  @copydetails LemmaObject::Delete()
     */
    void Delete();

    // ====================  OPERATORS     =======================

    /** Survey lines can be added or subtracted from each other
     */
    TEMSurveyLineData* operator+(const TEMSurveyLineData& Survey);

    /** Survey lines can be added or subtracted from each other
     */
    TEMSurveyLineData* operator-(const TEMSurveyLineData& Survey);

    /** Survey lines can be added or subtracted from each other
     */
    void operator+=(const TEMSurveyLineData& Survey);

    /** Survey lines can be added or subtracted from each other
     */
    void operator-=(const TEMSurveyLineData& Survey);

    /**  @param[in] ir the index to return
     *   @return the SurveyLineRecord with index ir
     */
    TEMSurveyLineRecordData* operator( ) ( const int& ir ) const ;

    // ====================  OPERATIONS    =======================

    // ====================  ACCESS        =======================

    /**
     *    @return pointer to specified record
     */
    TEMSurveyLineRecordData*  GetRecord(const int& ir) const ;

    /**
     * @return the number of records
     */
    int GetNumberOfRecords() const;

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
    static TEMSurveyLineData* DeSerialize(const YAML::Node& node);
#endif

    protected:

    // ====================  LIFECYCLE     =======================

    /** Default protected constructor, use New */
    TEMSurveyLineData (const std::string& name);

#ifdef HAVE_YAMLCPP
    /** Protected DeDerializing constructor, use factory DeSerialize  method*/
    TEMSurveyLineData (const YAML::Node& node);
#endif

    /** Default protected destructor, use Delete */
    ~TEMSurveyLineData ();

    /**
     *  @copybrief   LemmaObject::Release()
     *  @copydetails LemmaObject::Release()
     */
    void Release();

    private:

    // ====================  DATA MEMBERS  =========================

    std::vector<TEMSurveyLineRecordData*>   RecordData;

}; // -----  end of class  TEMSurveyLineData  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef TEMSURVEYLINEDATA_INC  -----

