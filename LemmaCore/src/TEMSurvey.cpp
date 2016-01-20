/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      10/08/2014 01:52:04 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#include	"TEMSurvey.h"

namespace Lemma {


    // ====================  FRIEND METHODS  =====================

    #ifdef HAVE_YAMLCPP
    std::ostream &operator << (std::ostream &stream, const TEMSurvey &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc --- a direct stream should encapulste object
        return stream;
    }
    #else
    std::ostream &operator<<(std::ostream &stream, const TEMSurvey &ob) {
        stream << *(LemmaObject*)(&ob);
        return stream;
    }
    #endif

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurvey
    //      Method:  TEMSurvey
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    TEMSurvey::TEMSurvey (const std::string& name) : LemmaObject(name) {

    }  // -----  end of method TEMSurvey::TEMSurvey  (constructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurvey
    //      Method:  TEMSurvey
    // Description:  DeSerializing constructor (protected)
    //--------------------------------------------------------------------------------------
    #ifdef HAVE_YAMLCPP
    TEMSurvey::TEMSurvey (const YAML::Node& node) : LemmaObject(node) {

        Lines.resize( node["numberOfLines"].as<int>( ) );

    }  // -----  end of method TEMSurvey::TEMSurvey  (constructor)  -----
    #endif

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurvey
    //      Method:  New()
    // Description:  public constructor
    //--------------------------------------------------------------------------------------
    TEMSurvey* TEMSurvey::New() {
        TEMSurvey*  Obj = new TEMSurvey("TEMSurvey");
        Obj->AttachTo(Obj);
        return Obj;
    }

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurvey
    //      Method:  ~TEMSurvey
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    TEMSurvey::~TEMSurvey () {

    }  // -----  end of method TEMSurvey::~TEMSurvey  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurvey
    //      Method:  Delete
    // Description:  public destructor
    //--------------------------------------------------------------------------------------
    void TEMSurvey::Delete() {
        this->DetachFrom(this);
    }

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurvey
    //      Method:  Release
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    void TEMSurvey::Release() {
        for (std::vector<TEMSurveyLine*>::iterator it = Lines.begin() ; it != Lines.end(); ++it) {
            (*it)->DetachFrom(this);
        }
        delete this;
    }

    #ifdef HAVE_YAMLCPP
    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurvey
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node  TEMSurvey::Serialize (  ) const {
        YAML::Node node;
        node.SetTag( this->Name );
        node["numberOfLines"] = Lines.size();
        for (unsigned int it = 0; it< Lines.size();  ++it) {
            node[std::string("line_") +  to_string(it)] = Lines[it]->Serialize();
        }
        return node;
    }		// -----  end of method TEMSurvey::Serialize  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurvey
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    TEMSurvey* TEMSurvey::DeSerialize ( const YAML::Node& node  ) {
        TEMSurvey* Object = new TEMSurvey(node);
        Object->AttachTo(Object);
        DESERIALIZECHECK( node, Object )
        return Object ;
    }		// -----  end of method TEMSurvey::DeSerialize  -----
    #endif



        // ====================  OPERATORS    =======================

    TEMSurveyLine* TEMSurvey::operator( ) ( const int& idx ) const {
        return this->Lines[ idx ];
    }

    TEMSurveyLineRecord* TEMSurvey::operator( ) ( const int& iline, const int& irec ) const {
        return Lines[ iline ]->GetRecord( irec );
    }

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurvey
    //      Method:  SetNumberOfLines
    //--------------------------------------------------------------------------------------
    void TEMSurvey::SetNumberOfLines ( const int& nlines  ) {

        // clean up any old lines
        for (std::vector<TEMSurveyLine*>::iterator it = Lines.begin() ; it != Lines.end(); ++it) {
            (*it)->DetachFrom(this);
        }
        Lines.clear();

        // Allocate these lines
        Lines.reserve(nlines);
        for (int il=0; il<nlines; ++il) {
            Lines.push_back( new TEMSurveyLine("TEMSurveyLine") );
            Lines[il]->AttachTo(this);
        }

        return ;
    }		// -----  end of method TEMSurvey::SetNumberOfLines  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurvey
    //      Method:  GetLine
    //--------------------------------------------------------------------------------------
    TEMSurveyLine* TEMSurvey::GetLine ( const unsigned int&  iline ) {
        return Lines[iline];
    }		// -----  end of method TEMSurvey::GetLine  -----



    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurvey
    //      Method:  ForwardModel
    //--------------------------------------------------------------------------------------
    TEMSurveyData* TEMSurvey::ForwardModel ( LayeredEarthEM* model, bool additiveNoise ) {

        TEMSurveyData* ForwardData = TEMSurveyData::New();
            //ForwardData->SetNumberOfLines(Lines.size());

        for (std::vector<TEMSurveyLine*>::iterator it = Lines.begin() ; it != Lines.end(); ++it) {

            TEMSurveyLineData* LineData = ((*it)->ForwardModel( model, additiveNoise ) );
            LineData->AttachTo(ForwardData);
            ForwardData->LineData.push_back( LineData );
        }

        //for (int it=0; it<Lines.size(); ++it) {
        //    Lines[it]->ForwardModel( model );
        //}

        return  ForwardData;
    }		// -----  end of method TEMSurvey::ForwardModel  -----


}		// -----  end of Lemma  name  -----

