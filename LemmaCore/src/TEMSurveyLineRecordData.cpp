/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      03/03/2015 10:34:56
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2015, XRI Geophysics, LLC
 * @copyright Copyright (c) 2015, Trevor Irons
 */

#include "TEMSurveyLineRecordData.h"

namespace Lemma {


    // ====================  FRIEND METHODS  =====================
#ifdef HAVE_YAMLCPP
    std::ostream &operator << (std::ostream &stream, const TEMSurveyLineRecordData &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc --- as a direct stream should encapulste thingy
        return stream;
    }
#else
    std::ostream &operator<<(std::ostream &stream, const TEMSurveyLineRecordData& ob) {
        stream << *(LemmaObject*)(&ob);
        return stream;
    }
#endif

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecordData
    //      Method:  TEMSurveyLineRecordData
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    TEMSurveyLineRecordData::TEMSurveyLineRecordData (const std::string& name) : LemmaObject(name) {

    }  // -----  end of method TEMSurveyLineRecordData::TEMSurveyLineRecordData  (constructor)  -----

#ifdef HAVE_YAMLCPP
    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecordData
    //      Method:  TEMSurveyLineRecordData
    // Description:  DeSerializing constructor (protected)
    //--------------------------------------------------------------------------------------
    TEMSurveyLineRecordData::TEMSurveyLineRecordData (const YAML::Node& node) : LemmaObject(node) {
        for (int ii=0; ii< node["numberOfSeries"].as<int>( ); ++ii) {
            DataSeries.push_back( node["DataSeries_" + to_string(ii)].as<VectorXr>() );
            GateCentres.push_back( node["GateCentres_" + to_string(ii)].as<VectorXr>() );
        }
    }  // -----  end of method TEMSurveyLineRecordData::TEMSurveyLineRecordData  (constructor)  -----
#endif

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecordData
    //      Method:  New()
    // Description:  public constructor
    //--------------------------------------------------------------------------------------
    TEMSurveyLineRecordData* TEMSurveyLineRecordData::New() {
        TEMSurveyLineRecordData*  Obj = new TEMSurveyLineRecordData("TEMSurveyLineRecordData");
        Obj->AttachTo(Obj);
        return Obj;
    }


    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecordData
    //      Method:  Clone
    //--------------------------------------------------------------------------------------
    TEMSurveyLineRecordData* TEMSurveyLineRecordData::Clone (  ) {
        return DeSerialize( this->Serialize() )  ;
    }		// -----  end of method TEMSurveyLineRecordData::Clone  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecordData
    //      Method:  ~TEMSurveyLineRecordData
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    TEMSurveyLineRecordData::~TEMSurveyLineRecordData () {

    }  // -----  end of method TEMSurveyLineRecordData::~TEMSurveyLineRecordData  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecordData
    //      Method:  Delete
    // Description:  public destructor
    //--------------------------------------------------------------------------------------
    void TEMSurveyLineRecordData::Delete() {
        this->DetachFrom(this);
    }

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecordData
    //      Method:  Release
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    void TEMSurveyLineRecordData::Release() {
        delete this;
    }

        // ====================  OPERATORS     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecordData
    //      Method:  operator +
    //--------------------------------------------------------------------------------------
    TEMSurveyLineRecordData* TEMSurveyLineRecordData::operator + ( const TEMSurveyLineRecordData& Survey ) {
        TEMSurveyLineRecordData* clone = this->Clone();
        for (unsigned int ir=0; ir<DataSeries.size(); ++ir) {
            clone->DataSeries[ir] += Survey.DataSeries[ir];
        }
        return clone;
    }		// -----  end of method TEMSurveyLineRecordData::operator +  -----

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecordData
    //      Method:  operator -
    //--------------------------------------------------------------------------------------
    TEMSurveyLineRecordData* TEMSurveyLineRecordData::operator - ( const TEMSurveyLineRecordData& Survey ) {
        TEMSurveyLineRecordData* clone = this->Clone();
        for (unsigned int ir=0; ir<DataSeries.size(); ++ir) {
            clone->DataSeries[ir] -= Survey.DataSeries[ir];
        }
        return clone;
    }		// -----  end of method TEMSurveyLineRecordData::operator -  -----

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecordData
    //      Method:  operator +=
    //--------------------------------------------------------------------------------------
    void TEMSurveyLineRecordData::operator += ( const TEMSurveyLineRecordData& Survey ) {

        if (DataSeries.size() != Survey.DataSeries.size()) {
            throw std::runtime_error("size mismatch in TEMSurveyLineRecordData::operator +=");
        }

        for (unsigned int ii=0; ii<DataSeries.size(); ++ii) {
            this->DataSeries[ii] += Survey.DataSeries[ii];
        }

        return ;
    }		// -----  end of method TEMSurveyLineRecordData::operator +=  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecordData
    //      Method:  operator -=
    //--------------------------------------------------------------------------------------
    void TEMSurveyLineRecordData::operator -= ( const TEMSurveyLineRecordData& Survey ) {

        if (DataSeries.size() != Survey.DataSeries.size()) {
            throw std::runtime_error("size mismatch in TEMSurveyLineRecordData::operator +=");
        }

        for (unsigned int ii=0; ii<DataSeries.size(); ++ii) {
            this->DataSeries[ii] -= Survey.DataSeries[ii];
        }

        return ;
    }		// -----  end of method TEMSurveyLineRecordData::operator -=  -----


        // ====================  ACCESS        =======================

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecordData
    //      Method:  GetCombinedData
    //--------------------------------------------------------------------------------------
    VectorXr TEMSurveyLineRecordData::GetCombinedData (  ) {

        int nd = 0;
        for (unsigned int id=0; id<DataSeries.size(); ++id) {
            nd += DataSeries[id].size();
        }
        VectorXr combined = VectorXr(nd);

        nd = 0;
        for (unsigned int id=0; id<DataSeries.size(); ++id) {
            combined.segment(nd, DataSeries[id].size()) = DataSeries[id];
            nd += DataSeries[id].size();
        }

        return  combined;
    }		// -----  end of method TEMSurveyLineRecordData::GetCombinedData  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecordData
    //      Method:  GetCombinedGateCentres
    //--------------------------------------------------------------------------------------
    VectorXr TEMSurveyLineRecordData::GetCombinedGateCentres (  ) {
        int nd = 0;
        for (unsigned int id=0; id<GateCentres.size(); ++id) {
            nd += GateCentres[id].size();
        }
        VectorXr combined = VectorXr(nd);

        nd = 0;
        for (unsigned int id=0; id<GateCentres.size(); ++id) {
            combined.segment(nd, GateCentres[id].size()) = GateCentres[id];
            nd += GateCentres[id].size();
        }

        return  combined;
    }		// -----  end of method TEMSurveyLineRecordData::GetCombinedGateCentres  -----


#ifdef HAVE_YAMLCPP
    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecordData
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node  TEMSurveyLineRecordData::Serialize (  ) const {
        YAML::Node node = LemmaObject::Serialize();;
        node.SetTag( this->Name );
        node["numberOfSeries"] = DataSeries.size();
        for (unsigned int is=0; is<DataSeries.size(); ++is) {
            node[std::string("DataSeries_") + to_string(is)] = DataSeries[is];
            node[std::string("GateCentres_") + to_string(is)] = GateCentres[is];
        }
        return node;
    }		// -----  end of method TEMSurveyLineRecordData::Serialize  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecordData
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    TEMSurveyLineRecordData* TEMSurveyLineRecordData::DeSerialize ( const YAML::Node& node  ) {
        TEMSurveyLineRecordData* Object = new TEMSurveyLineRecordData(node);
        Object->AttachTo(Object);
        DESERIALIZECHECK( node, Object )
            return Object ;
    }		// -----  end of method TEMSurveyLineRecordData::DeSerialize  -----
#endif

}		// -----  end of Lemma  name  -----

