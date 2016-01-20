/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      03/03/2015 10:00:22
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2015, XRI Geophysics, LLC
 * @copyright Copyright (c) 2015, Trevor Irons
 */

#include "TEMSurveyData.h"

namespace Lemma {

// ====================  FRIEND METHODS  =====================
#ifdef HAVE_YAMLCPP
std::ostream &operator << (std::ostream &stream, const TEMSurveyData &ob) {
    stream << ob.Serialize()  << "\n---\n"; // End of doc --- as a direct stream should encapulste thingy
    return stream;
}
#else
std::ostream &operator<<(std::ostream &stream, const TEMSurveyData& ob) {
    stream << *(LemmaObject*)(&ob);
    return stream;
}
#endif

// =================== OPERATORS ===============================

TEMSurveyData* TEMSurveyData::operator + (const TEMSurveyData& d) {
    TEMSurveyData* clone = Clone();
    *clone += d;
    return clone;
}

void TEMSurveyData::operator += (const TEMSurveyData& rhs) {

    if ( this->GetNumberOfLines() != rhs.GetNumberOfLines() ) {
        throw std::runtime_error( "Line mismatch in TEMSurveyData +=" );
    }

    for (int iline=0; iline<this->GetNumberOfLines(); ++iline ) {
        *(*this)(iline) += *rhs(iline);
        //*LineData[iline] += *rhs(iline);
        //*LineData[iline] += *rhs.GetLine(iline);
    }
}

void TEMSurveyData::operator -= (const TEMSurveyData& rhs) {

    if ( this->GetNumberOfLines() != rhs.GetNumberOfLines() ) {
        throw std::runtime_error( "Line mismatch in TEMSurveyData +=" );
    }

    for (int iline=0; iline<this->GetNumberOfLines(); ++iline ) {
        *(*this)(iline) -= *rhs(iline);
        //*LineData[iline] -= *rhs(iline);
        //*LineData[iline] -= *rhs.GetLine(iline);
    }

}

TEMSurveyData* TEMSurveyData::operator - (const TEMSurveyData& d) {
    TEMSurveyData* clone = Clone();
    *clone -= d;
    return clone;
}

TEMSurveyLineData* TEMSurveyData::operator( ) ( const int& idx ) const {
    return LineData[ idx ];
}

TEMSurveyLineRecordData* TEMSurveyData::operator( ) ( const int& iline, const int& irec ) const {
    return LineData[ iline ]->GetRecord( irec );
}

// ====================  LIFECYCLE     =======================

//--------------------------------------------------------------------------------------
//       Class:  TEMSurveyData
//      Method:  TEMSurveyData
// Description:  constructor (protected)
//--------------------------------------------------------------------------------------
TEMSurveyData::TEMSurveyData (const std::string& name) : LemmaObject(name) {

}  // -----  end of method TEMSurveyData::TEMSurveyData  (constructor)  -----

#ifdef HAVE_YAMLCPP
//--------------------------------------------------------------------------------------
//       Class:  TEMSurveyData
//      Method:  TEMSurveyData
// Description:  DeSerializing constructor (protected)
//--------------------------------------------------------------------------------------
TEMSurveyData::TEMSurveyData (const YAML::Node& node) : LemmaObject(node) {

    //SetNumberOfLines( node["numberOfLines"].as<int>( ) );

    for (int ii=0; ii< node["numberOfLines"].as<int>( ); ++ii) {
        //LineData.push_back( new TEMSurveyLineData("TEMSurveyLineData") );
        LineData.push_back( TEMSurveyLineData::DeSerialize( node[ std::string("line_")  + to_string(ii)] ) );
        LineData[ii]->AttachTo(this);
    }


}  // -----  end of method TEMSurveyData::TEMSurveyData  (constructor)  -----
#endif

//--------------------------------------------------------------------------------------
//       Class:  TEMSurveyData
//      Method:  New()
// Description:  public constructor
//--------------------------------------------------------------------------------------
TEMSurveyData* TEMSurveyData::New() {
    TEMSurveyData*  Obj = new TEMSurveyData("TEMSurveyData");
    Obj->AttachTo(Obj);
    return Obj;
}


//--------------------------------------------------------------------------------------
//       Class:  TEMSurveyData
//      Method:  Clone
//--------------------------------------------------------------------------------------
TEMSurveyData* TEMSurveyData::Clone (  ) {
    return TEMSurveyData::DeSerialize( this->Serialize() ) ;
}		// -----  end of method TEMSurveyData::Clone  -----

//--------------------------------------------------------------------------------------
//       Class:  TEMSurveyData
//      Method:  ~TEMSurveyData
// Description:  destructor (protected)
//--------------------------------------------------------------------------------------
TEMSurveyData::~TEMSurveyData () {

}  // -----  end of method TEMSurveyData::~TEMSurveyData  (destructor)  -----

//--------------------------------------------------------------------------------------
//       Class:  TEMSurveyData
//      Method:  Delete
// Description:  public destructor
//--------------------------------------------------------------------------------------
void TEMSurveyData::Delete() {
    this->DetachFrom(this);
}

//--------------------------------------------------------------------------------------
//       Class:  TEMSurveyData
//      Method:  Release
// Description:  destructor (protected)
//--------------------------------------------------------------------------------------
void TEMSurveyData::Release() {

    // clean up any old lines
    for (std::vector<TEMSurveyLineData*>::iterator it = LineData.begin() ; it != LineData.end(); ++it) {
        (*it)->DetachFrom(this);
        (*it)->Delete();
    }

    delete this;
}

//--------------------------------------------------------------------------------------
//       Class:  TEMSurveyData
//      Method:  SetNumberOfLines
//--------------------------------------------------------------------------------------
void TEMSurveyData::SetNumberOfLines ( const int& nlines ) {

        // clean up any old lines
        for (std::vector<TEMSurveyLineData*>::iterator it = LineData.begin() ; it != LineData.end(); ++it) {
            (*it)->DetachFrom(this);
        }
        LineData.clear();

        // Allocate these lines
        LineData.reserve(nlines);
        for (int il=0; il<nlines; ++il) {
            LineData.push_back( TEMSurveyLineData::New() );
            LineData[il]->AttachTo(this);
        }

    return ;
}		// -----  end of method TEMSurveyData::SetNumberOfLines  -----


//--------------------------------------------------------------------------------------
//       Class:  TEMSurveyData
//      Method:  GetLine
//--------------------------------------------------------------------------------------
TEMSurveyLineData* TEMSurveyData::GetLine ( const int& iline) const {
    return LineData[iline] ;
}		// -----  end of method TEMSurveyData::GetLine  -----


//--------------------------------------------------------------------------------------
//       Class:  TEMSurveyData
//      Method:  GetNumberOfLines
//--------------------------------------------------------------------------------------
int TEMSurveyData::GetNumberOfLines (  ) const {
    return  LineData.size() ;
}		// -----  end of method TEMSurveyData::GetNumberOfLines  -----



#ifdef HAVE_YAMLCPP
//--------------------------------------------------------------------------------------
//       Class:  TEMSurveyData
//      Method:  Serialize
//--------------------------------------------------------------------------------------
YAML::Node  TEMSurveyData::Serialize (  ) const {
    YAML::Node node = LemmaObject::Serialize();;
    node.SetTag( this->Name );
    node["numberOfLines"] = LineData.size();

    for (unsigned int it = 0; it<LineData.size();  ++it) {
        node[std::string("line_") +  to_string(it)] = LineData[it]->Serialize();
    }

    return node;
}		// -----  end of method TEMSurveyData::Serialize  -----


//--------------------------------------------------------------------------------------
//       Class:  TEMSurveyData
//      Method:  DeSerialize
//--------------------------------------------------------------------------------------
TEMSurveyData* TEMSurveyData::DeSerialize ( const YAML::Node& node  ) {
    TEMSurveyData* Object = new TEMSurveyData(node);
    Object->AttachTo(Object);
    DESERIALIZECHECK( node, Object )
    return Object ;
}		// -----  end of method TEMSurveyData::DeSerialize  -----
#endif

}		// -----  end of Lemma  name  -----

