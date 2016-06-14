/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      10/08/2014 03:04:56 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#include "TEMReceiver.h"

namespace Lemma {


    // ====================  FRIEND METHODS  =====================
#ifdef HAVE_YAMLCPP
    std::ostream &operator << (std::ostream &stream, const TEMReceiver &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc --- as a direct stream should encapulste thingy
        return stream;
    }
#else
    std::ostream &operator<<(std::ostream &stream, const TEMReceiver& ob) {
        stream << *(ReceiverPoints*)(&ob);
        return stream;
    }
#endif

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  TEMReceiver
    //      Method:  TEMReceiver
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    TEMReceiver::TEMReceiver (const std::string& name) : ReceiverPoints(name), moment(1), referenceTime(0) {

    }  // -----  end of method TEMReceiver::TEMReceiver  (constructor)  -----

#ifdef HAVE_YAMLCPP
    //--------------------------------------------------------------------------------------
    //       Class:  TEMReceiver
    //      Method:  TEMReceiver
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    TEMReceiver::TEMReceiver (const YAML::Node& node) : ReceiverPoints(node) {

        moment = node["moment"].as<Real>();
        referenceTime = node["referenceTime"].as<Real>();
        component = string2Enum<FIELDCOMPONENT>( node["component"].as<std::string>() );

        windowCentres = node["windowCentres"].as<VectorXr>();
        windowWidths  = node["windowWidths"].as<VectorXr>();
        noiseSTD      = node["noiseSTD"].as<VectorXr>();
        //location = node["location"].as<Vector3r>();
    }  // -----  end of method TEMReceiver::TEMReceiver  (constructor)  -----
#endif

    //--------------------------------------------------------------------------------------
    //       Class:  TEMReceiver
    //      Method:  New()
    // Description:  public constructor
    //--------------------------------------------------------------------------------------
    TEMReceiver* TEMReceiver::New() {
        TEMReceiver*  Obj = new TEMReceiver("TEMReceiver");
        Obj->AttachTo(Obj);
        return Obj;
    }


    //--------------------------------------------------------------------------------------
    //       Class:  TEMReceiver
    //      Method:  Clone
    //--------------------------------------------------------------------------------------
    TEMReceiver* TEMReceiver::Clone() {
        TEMReceiver* Copy = TEMReceiver::New();
            Copy->SetNumberOfReceivers( this->NumberOfReceivers );
            Copy->Mask = this->Mask;
            Copy->Locations = this->Locations;
            // TEM stuff
            Copy->moment = this->moment;
            Copy->referenceTime = this->referenceTime;
            Copy->nHat = this->nHat;
            Copy->component = this->component;
            Copy->windowCentres = this->windowCentres;
            Copy->windowWidths = this->windowWidths;
            Copy->noiseSTD = this->noiseSTD;
        return Copy;
    }		// -----  end of method TEMReceiver::Clone  -----

    //--------------------------------------------------------------------------------------
    //       Class:  TEMReceiver
    //      Method:  ~TEMReceiver
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    TEMReceiver::~TEMReceiver () {

    }  // -----  end of method TEMReceiver::~TEMReceiver  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  TEMReceiver
    //      Method:  Delete
    // Description:  public destructor
    //--------------------------------------------------------------------------------------
    void TEMReceiver::Delete() {
        this->DetachFrom(this);
    }

    //--------------------------------------------------------------------------------------
    //       Class:  TEMReceiver
    //      Method:  Release
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    void TEMReceiver::Release() {
        delete this;
    }


    //--------------------------------------------------------------------------------------
    //       Class:  TEMReceiver
    //      Method:  SetWindows
    //--------------------------------------------------------------------------------------
    void TEMReceiver::SetWindows ( const VectorXr& centres, const VectorXr& widths, const TIMEUNITS& Units ) {

        Real sc(0);
        switch (Units) {
            case SEC:
                sc = 1;
                break;
            case MILLISEC:
                sc = 1e-3;
                break;
            case MICROSEC:
                sc = 1e-6;
                break;
            case NANOSEC:
                sc = 1e-9;
                break;
            case PICOSEC:
                sc = 1e-12;
                break;
        };
        windowCentres = sc*centres;
        windowWidths = sc*widths;
        noiseSTD = VectorXr::Zero(windowCentres.size());
        return ;
    }		// -----  end of method TEMReceiver::SetWindows  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMReceiver
    //      Method:  SetNoiseSTD
    //--------------------------------------------------------------------------------------
    void TEMReceiver::SetNoiseSTD ( const VectorXr& noiseIn ) {
        if ( noiseIn.size() == windowCentres.size() ) {
            noiseSTD = noiseIn;
        } else {
            throw std::runtime_error("TEMReceiver::SetNoiseSTD not aligned");
        }
        return ;

    }		// -----  end of method TEMReceiver::SetNoiseSTD  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMReceiver
    //      Method:  GetNoiseSTD
    //--------------------------------------------------------------------------------------
    VectorXr TEMReceiver::GetNoiseSTD (  ) {
        return noiseSTD ;
    }		// -----  end of method TEMReceiver::GetNoiseSTD  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMReceiver
    //      Method:  SampleNoise
    //--------------------------------------------------------------------------------------
    VectorXr TEMReceiver::SampleNoise (  ) {
        
	/* we have C++-11 now! No Boost! 
        boost::mt19937 rng(time(0));
        boost::normal_distribution<> nd(0.0, 1.0);
        boost::variate_generator<boost::mt19937&, boost::normal_distribution<> > var_nor(rng, nd);
	*/

        std::random_device rd;
        std::mt19937 gen(rd()); 
        std::normal_distribution<> d(0.0, 1.00);

        VectorXr noise = VectorXr::Zero( windowCentres.size() );
        for (int ii=0; ii<windowCentres.size(); ++ii) {
            //noise(ii) = var_nor(); // old boost way
            noise(ii) = d(gen);
        }
        return noise.array() * noiseSTD.array();
    }		// -----  end of method TEMReceiver::SampleNoise  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMReceiver
    //      Method:  SetReferenceTime
    //--------------------------------------------------------------------------------------
    void TEMReceiver::SetReferenceTime ( const Real& refTime, const TIMEUNITS& units ) {
        Real sc(0);
        switch (units) {
            case SEC:
                sc = 1;
                break;
            case MILLISEC:
                sc = 1e-3;
                break;
            case MICROSEC:
                sc = 1e-6;
                break;
            case NANOSEC:
                sc = 1e-9;
                break;
            case PICOSEC:
                sc = 1e-12;
                break;
        };
        referenceTime = sc*refTime;

        return ;
    }		// -----  end of method TEMReceiver::SetReferenceTime  -----



    //--------------------------------------------------------------------------------------
    //       Class:  TEMReceiver
    //      Method:  SetMoment
    //--------------------------------------------------------------------------------------
    void TEMReceiver::SetMoment ( const Real& mom ) {
        moment = mom;
        return ;
    }		// -----  end of method TEMReceiver::SetMoment  -----



    //--------------------------------------------------------------------------------------
    //       Class:  TEMReceiver
    //      Method:  SetLocation
    //--------------------------------------------------------------------------------------
    void TEMReceiver::SetRxLocation ( const Vector3r& loc ) {
        this->SetNumberOfReceivers(1); // Valgrind doesn't like??
        this->SetLocation(0, loc);
        //location = loc;
        return ;
    }		// -----  end of method TEMReceiver::SetLocation  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMReceiver
    //      Method:  SetComponent
    //--------------------------------------------------------------------------------------
    void TEMReceiver::SetComponent ( const FIELDCOMPONENT& comp  ) {
        component = comp;
        return ;
    }		// -----  end of method TEMReceiver::SetComponent  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMReceiver
    //      Method:  get_WindowWidths
    //--------------------------------------------------------------------------------------
    VectorXr TEMReceiver::GetWindowWidths (  ) {
        return windowWidths;
    }		// -----  end of method TEMReceiver::get_WindowWidths  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMReceiver
    //      Method:  get_WindowCentres
    //--------------------------------------------------------------------------------------
    VectorXr TEMReceiver::GetWindowCentres (  ) {
        return windowCentres;
    }		// -----  end of method TEMReceiver::get_WindowCentres  -----



    //--------------------------------------------------------------------------------------
    //       Class:  TEMReceiver
    //      Method:  GetReferenceTime
    //--------------------------------------------------------------------------------------
    Real TEMReceiver::GetReferenceTime (  ) {
        return referenceTime;
    }		// -----  end of method TEMReceiver::GetReferenceTime  -----


#ifdef HAVE_YAMLCPP
    //--------------------------------------------------------------------------------------
    //       Class:  TEMReceiver
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node  TEMReceiver::Serialize (  ) const {
        YAML::Node node = ReceiverPoints::Serialize();
        node.SetTag( this->Name );
        node["moment"] = moment;
        node["referenceTime"] = referenceTime;
        node["component"] = enum2String(component);

        node["windowCentres"] = windowCentres;
        node["windowWidths"] = windowWidths;
        node["noiseSTD"] = noiseSTD;
        //node["location"] = location;
        return node;
    }		// -----  end of method TEMReceiver::Serialize  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMReceiver
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    TEMReceiver* TEMReceiver::DeSerialize ( const YAML::Node& node  ) {
        TEMReceiver* Object = new TEMReceiver(node);
        Object->AttachTo(Object);
        DESERIALIZECHECK( node, Object )
        return Object ;

    }		// -----  end of method TEMReceiver::DeSerialize  -----
#endif

}		// -----  end of Lemma  name  -----

