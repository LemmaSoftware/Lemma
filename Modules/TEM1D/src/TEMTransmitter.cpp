/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      09/29/2014 11:43:28 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#include "TEMTransmitter.h"

namespace Lemma {

// ====================  FRIEND METHODS  =====================

#ifdef HAVE_YAMLCPP
std::ostream &operator << (std::ostream &stream, const TEMTransmitter &ob) {
    stream << ob.Serialize()  << "\n---\n"; // End of doc --- as a direct stream should encapulste thingy
    return stream;
}
// TODO CONSIDER INPUT OPERATOR TOO!
//std::ostream &operator << (std::ostream &stream, const TEMTransmitter &ob) {
//    stream << ob.Serialize()  << "\n---\n"; // End of doc --- as a direct stream should encapulste thingy
//    return stream;
//}
#else
std::ostream &operator << (std::ostream &stream, const TEMTransmitter &ob) {
    stream << *(PolygonalWireAntenna*)(&ob);
    stream << "YOU SHOULD COMPILE WITH YAML-CPP\n";
    return stream;
}
#endif


// ====================  LIFECYCLE     =======================

//--------------------------------------------------------------------------------------
//       Class:  TEMTransmitter
//      Method:  TEMTransmitter
// Description:  constructor (protected)
//--------------------------------------------------------------------------------------
TEMTransmitter::TEMTransmitter (const std::string& name) : PolygonalWireAntenna(name),
    repFreq(0), repFreqUnits(HZ) {

}  // -----  end of method TEMTransmitter::TEMTransmitter  (constructor)  -----

#ifdef HAVE_YAMLCPP
//--------------------------------------------------------------------------------------
//       Class:  TEMTransmitter
//      Method:  TEMTransmitter
// Description:  Deserializing constructor (protected)
//--------------------------------------------------------------------------------------
TEMTransmitter::TEMTransmitter (const YAML::Node& node) : PolygonalWireAntenna(node) {

    repFreq =  node["repFreq"].as<Real>();
    repFreqUnits =  string2Enum<FREQUENCYUNITS>( node["repFreqUnits"].as<std::string>() );
    wfmTimes = node["wfmTimes"].as<VectorXr>();
    wfmTimes = node["wfmAmps"].as<VectorXr>();

}  // -----  end of method TEMTransmitter::TEMTransmitter  (constructor)  -----
#endif

//--------------------------------------------------------------------------------------
//       Class:  TEMTransmitter
//      Method:  New()
// Description:  public constructor
//--------------------------------------------------------------------------------------
TEMTransmitter* TEMTransmitter::New() {
    TEMTransmitter*  Obj = new TEMTransmitter("TEMTransmitter");
    Obj->AttachTo(Obj);
    return Obj;
}


//--------------------------------------------------------------------------------------
//       Class:  TEMTransmitter
//      Method:  Clone
//--------------------------------------------------------------------------------------
TEMTransmitter* TEMTransmitter::Clone (  ) {
    TEMTransmitter* copy = TEMTransmitter::New();
    //copy->AttachTo(copy); // NO! Attached above!
	copy->NumberOfPoints = this->NumberOfPoints;
	copy->Freqs = this->Freqs;
	copy->Current = this->Current;
	copy->NumberOfTurns = this->NumberOfTurns;
	copy->Points = this->Points;
	//copy->Dipoles = this->Dipoles; // no, disaster

    copy->repFreq = this->repFreq;
    copy->repFreqUnits = this->repFreqUnits;
    copy->wfmTimes = this->wfmTimes;
    copy->wfmAmps = this->wfmAmps;

	return copy;
}		// -----  end of method TEMTransmitter::Clone  -----


//--------------------------------------------------------------------------------------
//       Class:  TEMTransmitter
//      Method:  ~TEMTransmitter
// Description:  destructor (protected)
//--------------------------------------------------------------------------------------
TEMTransmitter::~TEMTransmitter () {

}  // -----  end of method TEMTransmitter::~TEMTransmitter  (destructor)  -----

//--------------------------------------------------------------------------------------
//       Class:  TEMTransmitter
//      Method:  Delete
// Description:  public destructor
//--------------------------------------------------------------------------------------
void TEMTransmitter::Delete() {
    this->DetachFrom(this);
}

//--------------------------------------------------------------------------------------
//       Class:  TEMTransmitter
//      Method:  Release
// Description:  destructor (protected)
//--------------------------------------------------------------------------------------
void TEMTransmitter::Release() {
    delete this;
}

//--------------------------------------------------------------------------------------
//       Class:  TEMTransmitter
//      Method:  Serialize
//--------------------------------------------------------------------------------------
#ifdef HAVE_YAMLCPP
YAML::Node TEMTransmitter::Serialize (  ) const {

    YAML::Node node = PolygonalWireAntenna::Serialize();
        // or we could do just like below. Same basic uglyness
        node.SetTag( this->Name) ; //((PolygonalWireAntenna*)(&ob))->GetName() );
        //node["address"] =  ss.str();

        node["repFreq"]      = this->repFreq;
        node["repFreqUnits"] = enum2String(repFreqUnits);
        node["wfmTimes"] = wfmTimes;
        node["wfmAmps"] = wfmAmps;

    return node;
}		// -----  end of method TEMTransmitter::Serialize  -----

//--------------------------------------------------------------------------------------
//       Class:  TEMTransmitter
//      Method:  DeSerialize
//--------------------------------------------------------------------------------------
TEMTransmitter* TEMTransmitter::DeSerialize ( const  YAML::Node& node  ) {
    TEMTransmitter* Object = new TEMTransmitter(node);
    Object->AttachTo(Object);
    DESERIALIZECHECK( node, Object )
    return  Object;
}		// -----  end of method TEMTransmitter::DeSerialize  -----
#endif

    // ====================  INQUIRY       =======================

//--------------------------------------------------------------------------------------
//       Class:  TEMTransmitter
//      Method:  SetRepFrequency
//--------------------------------------------------------------------------------------
void TEMTransmitter::SetRepFrequency ( const Real& fr, const FREQUENCYUNITS& units ) {
    repFreq = fr;
    repFreqUnits = units;
    return ;
}		// -----  end of method TEMTransmitter::SetRepFrequency  -----


//--------------------------------------------------------------------------------------
//       Class:  TEMTransmitter
//      Method:  SetWaveform
//--------------------------------------------------------------------------------------
void TEMTransmitter::SetWaveform (  const VectorXr& times, const VectorXr& amps, const TIMEUNITS& units ) {
    // switch units ?
    switch (units) {
        case SEC:
            wfmTimes = times;
            break;
        case MILLISEC:
            wfmTimes = times*1e-3 ;
            break;
        case MICROSEC:
            wfmTimes = times*1e-6 ;
            break;
        case NANOSEC:
            wfmTimes = times*1e-9 ;
            break;
        case PICOSEC:
            wfmTimes = times*1e-12 ;
            break;
    };
    wfmAmps = amps;
    return ;
}		// -----  end of method TEMTransmitter::SetWaveform  -----


//--------------------------------------------------------------------------------------
//       Class:  TEMTransmitter
//      Method:  GetWfmAmps
//--------------------------------------------------------------------------------------
VectorXr TEMTransmitter::GetWfmAmps (  ) {
    return wfmAmps;
}		// -----  end of method TEMTransmitter::GetWfmAmps  -----



//--------------------------------------------------------------------------------------
//       Class:  TEMTransmitter
//      Method:  GetWfmTimes
//--------------------------------------------------------------------------------------
VectorXr TEMTransmitter::GetWfmTimes (  ) {
    return wfmTimes;
}		// -----  end of method TEMTransmitter::GetWfmTimes  -----


} // namespace Lemma
