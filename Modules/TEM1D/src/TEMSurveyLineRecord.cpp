/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      10/10/2014 12:23:07 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#include "TEMSurveyLineRecord.h"

namespace Lemma {


    // ====================  FRIEND METHODS  =====================
#ifdef HAVE_YAMLCPP
    std::ostream &operator << (std::ostream &stream, const TEMSurveyLineRecord &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc --- as a direct stream should encapulste thingy
        return stream;
    }
#else
    std::ostream &operator<<(std::ostream &stream, const TEMSurveyLineRecord& ob) {
        stream << *(LemmaObject*)(&ob);
        return stream;
    }
#endif

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecord
    //      Method:  TEMSurveyLineRecord
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    TEMSurveyLineRecord::TEMSurveyLineRecord (const std::string& name) : LemmaObject(name),
        numberOfPulseSequences(0)
    {

    }  // -----  end of method TEMSurveyLineRecord::TEMSurveyLineRecord  (constructor)  -----

#ifdef HAVE_YAMLCPP
    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecord
    //      Method:  TEMSurveyLineRecord
    // Description:  DeSerializing constructor (protected)
    //--------------------------------------------------------------------------------------
    TEMSurveyLineRecord::TEMSurveyLineRecord (const YAML::Node& node) : LemmaObject(node),
        numberOfPulseSequences(0)
    {

    }  // -----  end of method TEMSurveyLineRecord::TEMSurveyLineRecord  (constructor)  -----
#endif

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecord
    //      Method:  ~TEMSurveyLineRecord
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    TEMSurveyLineRecord::~TEMSurveyLineRecord () {

        for (unsigned int ip=0; ip<Transmitters.size(); ++ip) {
            if (Transmitters[ip] != NULL) {
                Transmitters[ip]->DetachFrom(this);
            }
        }
        for (unsigned int ir=0; ir<Receivers.size(); ++ir) {
            if (Receivers[ir] != NULL) {
                Receivers[ir]->DetachFrom(this);
            }
        }

    }  // -----  end of method TEMSurveyLineRecord::~TEMSurveyLineRecord  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecord
    //      Method:  Delete
    // Description:  public destructor
    //--------------------------------------------------------------------------------------
    void TEMSurveyLineRecord::Delete() {
        this->DetachFrom(this);
    }

    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecord
    //      Method:  Release
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    void TEMSurveyLineRecord::Release() {
        delete this;
    }


    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecord
    //      Method:  SetPulseSequenceReceiverPair
    //--------------------------------------------------------------------------------------
    void TEMSurveyLineRecord::SetTransmitterReceiverPair ( const int& ii, TEMTransmitter* Tx, TEMReceiver* Rx  ) {
        Receivers[ii] = Rx;
        Rx->AttachTo(this);

        Transmitters[ii] = Tx;
        Tx->AttachTo(this);
        return ;
    }		// -----  end of method TEMSurveyLineRecord::SetPulseSequenceReceiverPair  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecord
    //      Method:  GetReceiver
    //--------------------------------------------------------------------------------------
    TEMReceiver* TEMSurveyLineRecord::GetReceiver ( const int& irec  ) {
        if ( irec >= (int)(Receivers.size()) ) {
            throw std::runtime_error( "TEMSurveyLineRecord::GetReceiver(const int& irec)--array bounds error" );
        }
        return Receivers[irec] ;
    }		// -----  end of method TEMSurveyLineRecord::GetReceiver  -----


#ifdef HAVE_YAMLCPP
    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecord
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node  TEMSurveyLineRecord::Serialize (  ) const {
        YAML::Node node;
        node.SetTag( this->Name );
        node["numberOfPulseSequences"] = numberOfPulseSequences;

        // FILL IN CLASS SPECIFICS HERE

        for( int is=0; is<numberOfPulseSequences; ++is) {
            node[std::string("pulse_") + to_string(is)]    = Transmitters[is]->Serialize();
            node[std::string("receiver_") + to_string(is)] = Receivers[is]->Serialize();
        }


        return node;
    }		// -----  end of method TEMSurveyLineRecord::Serialize  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecord
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    TEMSurveyLineRecord* TEMSurveyLineRecord::DeSerialize ( const YAML::Node& node  ) {
        TEMSurveyLineRecord* Object = new TEMSurveyLineRecord(node);
        Object->AttachTo(Object);
        DESERIALIZECHECK( node, Object )
            return Object ;
    }		// -----  end of method TEMSurveyLineRecord::DeSerialize  -----
#endif


    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecord
    //      Method:  SetNumberOfPulseSequences
    //--------------------------------------------------------------------------------------
    void TEMSurveyLineRecord::SetNumberOfPulseSequences ( const int& nseq ) {

        for (unsigned int ip=0; ip<Transmitters.size(); ++ip) {
            Transmitters[ip]->DetachFrom(this);
        }
        for (unsigned int ir=0; ir<Receivers.size(); ++ir) {
            Receivers[ir]->DetachFrom(this);
        }
        Transmitters.clear();
        Receivers.clear();

        numberOfPulseSequences = nseq;

        Receivers.reserve(nseq);
        Transmitters.reserve(nseq);
        for (int ir=0; ir<nseq; ++ir) {
            Transmitters.push_back( NULL );
            Receivers.push_back( NULL );
        }

        return ;
    }		// -----  end of method TEMSurveyLineRecord::SetNumberOfPulseSequences  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecord
    //      Method:  GetNumberOfPulseSequences
    //--------------------------------------------------------------------------------------
    int TEMSurveyLineRecord::GetNumberOfPulseSequences (  ) {
        return numberOfPulseSequences ;
    }		// -----  end of method TEMSurveyLineRecord::GetNumberOfPulseSequences  -----


    //--------------------------------------------------------------------------------------
    //       Class:  TEMSurveyLineRecord
    //      Method:  ForwardModel
    //--------------------------------------------------------------------------------------
    TEMSurveyLineRecordData* TEMSurveyLineRecord::ForwardModel ( LayeredEarthEM* model, bool additiveNoise ) {

        TEMSurveyLineRecordData* Data = TEMSurveyLineRecordData::New();

        // DO I want to work with InstrumentTem here? Or just roll it myself at this point?
        for (unsigned int ip=0; ip<Transmitters.size(); ++ip) {

            InstrumentTem *instrument = InstrumentTem::New();

            // For thread safety...
            TEMTransmitter*  TC =  Transmitters[ip]->Clone();
            TEMReceiver* RC = Receivers[ip]->Clone();

            // TODO some of this needs to be moved logic checked or use overriden functions, etc.
            instrument->EMEarthModel(model);
	        instrument->SetTransmitLoop( TC );
            instrument->SetReferenceTime( RC->GetReferenceTime() );
	        instrument->SetTimeGates( RC->GetWindowCentres(), RC->GetWindowWidths() );
	        instrument->SetReceiver( RC );   // Clone??
            instrument->SetReceiverType(INDUCTIVE);
            instrument->SetPulse( TC->GetWfmAmps(), TC->GetWfmTimes() );

            // TODO pass this
            //instrument->MakeLaggedCalculation( FHTKEY51 );
            instrument->MakeLaggedCalculation( ANDERSON801 );

            Data->DataSeries.push_back( MU0*instrument->GetMeasurements().col(1) );
            Data->GateCentres.push_back( RC->GetWindowCentres() );

            if (additiveNoise) {
                Data->DataSeries.back() += RC->SampleNoise();
            }

//                 int nlag = instrument->GetMeasurements().rows();
//                 std::cout.precision(12);
//                 std::cout << "// time [s]  dB/dt [T/s] \n";
//                 for (int ii=0; ii<nlag; ii++) {
//  		            std::cout<<"  "<<instrument->GetMeasurements()(ii,0)<<"  "<< MU0*instrument->GetMeasurements()(ii,1)<<std::endl;
//  	            }

            instrument->Delete();
            TC->Delete();
            RC->Delete();
        }

        return Data;
    }		// -----  end of method TEMSurveyLineRecord::ForwardModel  -----


}		// -----  end of Lemma  name  -----
