/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      03/03/2015 10:23:22
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2015, XRI Geophysics, LLC
 * @copyright Copyright (c) 2015, Trevor Irons
 */

#include "TEMSurveyLineData.h"

namespace Lemma {

    // ====================  FRIEND METHODS  =====================
#ifdef HAVE_YAMLCPP
    std::ostream &operator << (std::ostream &stream, const TEMSurveyLineData &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc --- as a direct stream should encapulste thingy
        return stream;
    }
#else
    std::ostream &operator<<(std::ostream &stream, const TEMSurveyLineData& ob) {
        stream << *(LemmaObject*)(&ob);
        return stream;
    }
#endif

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineData
    //      Method:  TEMSurveyLineData
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    TEMSurveyLineData::TEMSurveyLineData (const std::string& name) : LemmaObject(name) {

    }  // -----  end of method TEMSurveyLineData::TEMSurveyLineData  (constructor)  -----

#ifdef HAVE_YAMLCPP
    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineData
    //      Method:  TEMSurveyLineData
    // Description:  DeSerializing constructor (protected)
    //--------------------------------------------------------------------------------------
    TEMSurveyLineData::TEMSurveyLineData (const YAML::Node& node) : LemmaObject(node) {
        for (int ii=0; ii< node["numberOfRecords"].as<int>( ); ++ii) {
            RecordData.push_back( TEMSurveyLineRecordData::DeSerialize( node[ std::string("record_")  + to_string(ii)] ) );
            RecordData[ii]->AttachTo(this);
        }
    }  // -----  end of method TEMSurveyLineData::TEMSurveyLineData  (constructor)  -----
#endif

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineData
    //      Method:  New()
    // Description:  public constructor
    //--------------------------------------------------------------------------------------
    TEMSurveyLineData* TEMSurveyLineData::New() {
        TEMSurveyLineData*  Obj = new TEMSurveyLineData("TEMSurveyLineData");
        Obj->AttachTo(Obj);
        return Obj;
    }

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecordData
    //      Method:  Clone
    //--------------------------------------------------------------------------------------
    TEMSurveyLineData* TEMSurveyLineData::Clone (  ) {
        return TEMSurveyLineData::DeSerialize( this->Serialize() )  ;
    }		// -----  end of method TEMSurveyLineRecordData::Clone  -----

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineData
    //      Method:  ~TEMSurveyLineData
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    TEMSurveyLineData::~TEMSurveyLineData () {

    }  // -----  end of method TEMSurveyLineData::~TEMSurveyLineData  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineData
    //      Method:  Delete
    // Description:  public destructor
    //--------------------------------------------------------------------------------------
    void TEMSurveyLineData::Delete() {
        this->DetachFrom(this);
    }

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineData
    //      Method:  Release
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    void TEMSurveyLineData::Release() {
        for (std::vector<TEMSurveyLineRecordData*>::iterator it = RecordData.begin() ; it != RecordData.end(); ++it) {
            (*it)->DetachFrom(this);
            (*it)->Delete();
        }
        delete this;
    }

    // =================== OPERATORS ===============================

    TEMSurveyLineRecordData* TEMSurveyLineData::operator( ) ( const int& idx ) const {
        return RecordData[ idx ];
    }


    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineData
    //      Method:  operator +=
    //--------------------------------------------------------------------------------------
    void TEMSurveyLineData::operator += ( const TEMSurveyLineData& rhs) {

        if ( this->GetNumberOfRecords() != rhs.GetNumberOfRecords() ) {
            throw std::runtime_error( "Record mismatch in TEMSurveyLineData +=" );
        }

        for (int irec=0; irec<this->GetNumberOfRecords(); ++irec ) {
            *(*this)(irec) += *rhs(irec);
            //*LineData[irec] += *rhs(irec);
            //*LineData[irec] += *rhs.GetLine(irec);
        }

        return ;
    }		// -----  end of method TEMSurveyLineData::+=  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineData
    //      Method:  operator +
    //--------------------------------------------------------------------------------------
    TEMSurveyLineData* TEMSurveyLineData::operator + ( const TEMSurveyLineData& rhs) {
        TEMSurveyLineData* clone = Clone();
        *clone += rhs;
        return clone;
    }		// -----  end of method TEMSurveyLineData::operator +  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineData
    //      Method:  operator -=
    //--------------------------------------------------------------------------------------
    void TEMSurveyLineData::operator -= ( const TEMSurveyLineData& rhs ) {

        if ( this->GetNumberOfRecords() != rhs.GetNumberOfRecords() ) {
            throw std::runtime_error( "Record mismatch in TEMSurveyLineData +=" );
        }

        for (int irec=0; irec<this->GetNumberOfRecords(); ++irec ) {
            *(*this)(irec) -= *rhs(irec);
            //*LineData[irec] -= *rhs(irec);
            //*LineData[irec] -= *rhs.GetLine(irec);
        }
        return ;
    }		// -----  end of method TEMSurveyLineData::operator -=  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineData
    //      Method:  operator -
    //--------------------------------------------------------------------------------------
    TEMSurveyLineData* TEMSurveyLineData::operator - ( const TEMSurveyLineData& rhs) {
        TEMSurveyLineData* clone = Clone();
        *clone -= rhs;
        return clone;
    }		// -----  end of method TEMSurveyLineData::operator -  -----


    // ====================  ACCESS        =======================

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineData
    //      Method:  GetRecord
    //--------------------------------------------------------------------------------------
    TEMSurveyLineRecordData*    TEMSurveyLineData::GetRecord(const int& ir) const {
        return RecordData[ir];
    }		// -----  end of method TEMSurveyLineData::GetRecord  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecordData
    //      Method:  GetNumberOfRecords
    //--------------------------------------------------------------------------------------
    int TEMSurveyLineData::GetNumberOfRecords (  ) const {
        return RecordData.size();
    }		// -----  end of method TEMSurveyLineRecordData::GetNumberOfRecords  -----


#ifdef HAVE_YAMLCPP
    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineData
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node  TEMSurveyLineData::Serialize (  ) const {
        YAML::Node node = LemmaObject::Serialize();;
        node.SetTag( this->Name );
        node["numberOfRecords"] = RecordData.size();
        // FILL IN CLASS SPECIFICS HERE
        for (unsigned int it = 0; it< RecordData.size();  ++it) {
            node[std::string("record_") +  to_string(it)] = RecordData[it]->Serialize();
        }
        return node;
    }		// -----  end of method TEMSurveyLineData::Serialize  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineData
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    TEMSurveyLineData* TEMSurveyLineData::DeSerialize ( const YAML::Node& node  ) {
        TEMSurveyLineData* Object = new TEMSurveyLineData(node);
        Object->AttachTo(Object);
        DESERIALIZECHECK( node, Object )
            return Object ;
    }		// -----  end of method TEMSurveyLineData::DeSerialize  -----
#endif

}		// -----  end of Lemma  name  -----

