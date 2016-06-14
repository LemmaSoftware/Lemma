/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      10/10/2014 11:19:38 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#include "TEMSurveyLine.h"

namespace Lemma {

    // ====================  FRIEND METHODS  =====================
#ifdef HAVE_YAMLCPP
    std::ostream &operator << (std::ostream &stream, const TEMSurveyLine &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc --- as a direct stream should encapulste thingy
        return stream;
    }
#else
    std::ostream &operator<<(std::ostream &stream, const TEMSurveyLine& ob) {
        stream << *(LemmaObject*)(&ob);
        return stream;
    }
#endif

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLine
    //      Method:  TEMSurveyLine
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    TEMSurveyLine::TEMSurveyLine (const std::string& name) : LemmaObject(name) {

    }  // -----  end of method TEMSurveyLine::TEMSurveyLine  (constructor)  -----

#ifdef HAVE_YAMLCPP
    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLine
    //      Method:  TEMSurveyLine
    // Description:  DeSerializing constructor (protected)
    //--------------------------------------------------------------------------------------
    TEMSurveyLine::TEMSurveyLine (const YAML::Node& node) : LemmaObject(node) {

    }  // -----  end of method TEMSurveyLine::TEMSurveyLine  (constructor)  -----
#endif

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLine
    //      Method:  ~TEMSurveyLine
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    TEMSurveyLine::~TEMSurveyLine () {

    }  // -----  end of method TEMSurveyLine::~TEMSurveyLine  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLine
    //      Method:  Delete
    // Description:  public destructor
    //--------------------------------------------------------------------------------------
    void TEMSurveyLine::Delete() {
        // THIS is a private member, this class must be used through a TEMSurvey
        this->DetachFrom(this);
    }

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLine
    //      Method:  Release
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    void TEMSurveyLine::Release() {
        for (std::vector<TEMSurveyLineRecord*>::iterator it = Records.begin() ; it != Records.end(); ++it) {
            (*it)->DetachFrom(this);
        }
        delete this;
    }


#ifdef HAVE_YAMLCPP
    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLine
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node  TEMSurveyLine::Serialize (  ) const {
        YAML::Node node;
        node.SetTag( this->Name );
        node["numberOfRecords"] = Records.size();
        for (unsigned int it = 0; it< Records.size();  ++it) {
            node[std::string("record_") +  to_string(it)] = Records[it]->Serialize();
        }
        return node;
    }		// -----  end of method TEMSurveyLine::Serialize  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLine
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    TEMSurveyLine* TEMSurveyLine::DeSerialize ( const YAML::Node& node  ) {
        TEMSurveyLine* Object = new TEMSurveyLine(node);
        Object->AttachTo(Object);
        DESERIALIZECHECK( node, Object )
            return Object ;
    }		// -----  end of method TEMSurveyLine::DeSerialize  -----
#endif

    TEMSurveyLineRecord* TEMSurveyLine::GetRecord( const unsigned int& irec )  {
        return Records[irec];
    }


    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLine
    //      Method:  SetNumberOfRecords
    //--------------------------------------------------------------------------------------
    void TEMSurveyLine::SetNumberOfRecords ( const int& nrec  ) {

        // clean up any old records
        for (std::vector<TEMSurveyLineRecord*>::iterator it = Records.begin() ; it != Records.end(); ++it) {
            (*it)->DetachFrom(this);
        }
        Records.clear();

        // Allocate these lines
        Records.reserve(nrec);
        for (int ir=0; ir<nrec; ++ir) {
            Records.push_back( new TEMSurveyLineRecord("TEMSurveyLineRecord") );
            Records[ir]->AttachTo(this);
        }

        return ;
    }		// -----  end of method TEMSurveyLine::SetNumberOfRecords  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLine
    //      Method:  ForwardModel
    //--------------------------------------------------------------------------------------
    TEMSurveyLineData* TEMSurveyLine::ForwardModel ( LayeredEarthEM* Model, bool additiveNoise ) {
        TEMSurveyLineData* Data = TEMSurveyLineData::New();
        for (std::vector<TEMSurveyLineRecord*>::iterator it = Records.begin() ; it != Records.end(); ++it) {
            TEMSurveyLineRecordData* RecData = (*it)->ForwardModel(Model, additiveNoise) ;
            RecData->AttachTo(Data);
            Data->RecordData.push_back( RecData );
        }
        return Data;
    }		// -----  end of method TEMSurveyLine::ForwardModel  -----


}		// -----  end of Lemma  name  -----
