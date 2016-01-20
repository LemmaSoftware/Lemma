/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass, Edited Trevor Irons 02/28/2014
  @date     02/10/2011
  @version  $Id: instrumenttem.cpp 266 2015-04-01 03:24:00Z tirons $
 **/
#include "instrumenttem.h"

namespace Lemma {

	std::ostream &operator<<(std::ostream &stream,
		const InstrumentTem &ob) {
	stream << *(LemmaObject*)(&ob);
	return stream;
	}

	// =======================  Lifecycle  ==================================

	InstrumentTem::InstrumentTem(const std::string &name) : Instrument(name),
		Antenna(NULL), Receiver(NULL), EarthMod(NULL), Dipole(NULL), bipolarWaveform(true),
        NPreviousPulse(0), RefTime(0.0), PulseT(.06), TxFreq(0.30), RType(INDUCTIVE) {
	}

	InstrumentTem::~InstrumentTem() {
		if (NumberOfReferences != 0) {
			throw DeleteObjectWithReferences(this);
		}

        if (this->Antenna != NULL) {
			this->Antenna->DetachFrom(this);
		}

		if (this->Dipole != NULL) {
			this->Dipole->DetachFrom(this);
		}

        if (this->EarthMod != NULL) {
			this->EarthMod->DetachFrom(this);
		}

        if (this->Receiver != NULL) {
			this->Receiver->DetachFrom(this);
		}

	}

	InstrumentTem* InstrumentTem::New() {
		InstrumentTem* Obj = new InstrumentTem("InstrumentTem");
		Obj->AttachTo(Obj);
		return Obj;
	}

	void InstrumentTem::Delete() {
		this->DetachFrom(this);
	}

	void InstrumentTem::Release() {
		delete this;
	}

	// =======================  Operations  =================================
	void InstrumentTem::MakeDirectCalculation( const HANKELTRANSFORMTYPE& hType) {

		//int temp;
		int NumTimes;
		Real answer;
		EMEarth1D *EMEarthLocal=EMEarth1D::New();
		EMEarthLocal->AttachLayeredEarthEM(this->EarthMod);

		//DigitalFilterCosTrans *DFIntegrate=DigitalFilterCosTrans::New();
		DigitalFilterSinTrans *DFIntegrate=DigitalFilterSinTrans::New();

		TemIntegrationKernel *TEMIntKern=TemIntegrationKernel::New();
		TEMIntKern->SetEMEarth1D(EMEarthLocal);
		//TEMIntKern->SetTransmitter(this->Antenna);
		TEMIntKern->SetReceiver(this->Receiver);
		DFIntegrate->AttachKernel(TEMIntKern);

		if (this->Dipole != NULL) {  //AND this->Antenna == NULL
			EMEarthLocal->AttachDipoleSource(this->Dipole);
			TEMIntKern->SetDipole(this->Dipole);
		}
		if (this->Antenna != NULL) { //AND this->Dipole == NULL
			EMEarthLocal->AttachWireAntenna(this->Antenna);
			TEMIntKern->SetTransmitter(this->Antenna);
		}
		EMEarthLocal->SetFieldsToCalculate(H);
		    //EMEarthLocal->SetHankelTransformMethod(DIGITALFILTERING);
		    //EMEarthLocal->SetHankelTransformMethod(FHTKEY201);
		    EMEarthLocal->SetHankelTransformMethod(hType);
		    //EMEarthLocal->SetHankelTransformMethod(GAUSSIANQUADRATURE);

		//EMEarthLocal->AttachReceiverPoints(this->Receiver);
		// For testing with one time only
		//DFIntegrate->Compute(1,1,1e-16);
		//std::cout<<DFIntegrate->GetAnswer()<<std::endl;
		//
		NumTimes = this->TimeGates.size();
		//Testing
		//std::cout << "NumTimes: " << NumTimes << std::endl;
		this->ModelledData.resize(NumTimes,2);
		for (int ii=0;ii<NumTimes;ii++) {
			//TESTING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//RELAXING THE VALUE!
			DFIntegrate->Compute(this->TimeGates(ii), 1, 1e-7);
			//Testing
			//std::cout << "DFIntegrate done computing" << std::endl;
			answer = DFIntegrate->GetAnswer()(0);
			answer = (2./PI)*answer;
			this->ModelledData(ii,0) = this->TimeGates(ii);
			this->ModelledData(ii,1) = answer;
			//std::cout << "   " << TimeGates(ii) << "  " <<
			//	answer << std::endl;
			//std::cout << "   " << this->ModelledData(ii,0) << "  " <<
			//	this->ModelledData(ii,1) << std::endl;
			// Call cosine transform which calls argument
		}
		//
		TEMIntKern->Delete();
		EMEarthLocal->Delete();
		DFIntegrate->Delete();

	}

    void InstrumentTem::MakeLaggedCalculation(const HANKELTRANSFORMTYPE& hType) {

        EMEarth1D *EMEarthLocal=EMEarth1D::New();
		    EMEarthLocal->AttachLayeredEarthEM(this->EarthMod);
		    EMEarthLocal->SetFieldsToCalculate(H);
		    EMEarthLocal->SetHankelTransformMethod(hType);
            EMEarthLocal->AttachReceiverPoints(this->Receiver);

		TemIntegrationKernel *TEMIntKern=TemIntegrationKernel::New();

		    TEMIntKern->SetReceiver(this->Receiver);
		    TEMIntKern->SetEMEarth1D(EMEarthLocal);


		if (this->Dipole != NULL) {  //AND this->Antenna == NULL
			EMEarthLocal->AttachDipoleSource(this->Dipole);
			TEMIntKern->SetDipole(this->Dipole);
            Dipole->SetNumberOfFrequencies(1);
		}
		if (this->Antenna != NULL) { //AND this->Dipole == NULL
			EMEarthLocal->AttachWireAntenna(this->Antenna);
            Antenna->SetCurrent(1);
			TEMIntKern->SetTransmitter(this->Antenna);
            Antenna->SetNumberOfFrequencies(1);
		}


        // TODO IMPORTANT!!
        // Compute number of lagged convolutions to do, TODO, need to make sure we do these
        // TO THE LATEST time needed for multiple pulse subtraction!!!
        int ilag = 1;
        Real sub = 2.1*PulseT + 2.*NPreviousPulse*PulseT;
        if (!bipolarWaveform) sub = NPreviousPulse*PulseT;

        //TimeGates.array() += RefTime;
        //RefTime = 0;

        // This is a critical line, for reasons I don't understand, it's important to calculate past the
        // theoretical GateWidths(0)/2.
        while ( (TimeGates.tail(1)(0)+sub)*std::pow(1./exp(.1), ilag-1) > TimeGates(0)-GateWidths(0) ) ++ ilag;

		//DigitalFilterSinTrans *DFIntegrate=DigitalFilterSinTrans::New(); // use with step response kernel
		DigitalFilterCosTrans *DFIntegrate=DigitalFilterCosTrans::New();   // use with impulse response kernel
		    DFIntegrate->AttachKernel(TEMIntKern);
            DFIntegrate->SetNumConv(ilag);
		    DFIntegrate->Compute( this->TimeGates.tail(1)(0)+sub, 1, 1e-7 ); // rho ntol, tol=1e-7 to 1e-13

        CubicSplineInterpolator *Spline = CubicSplineInterpolator::New();
            //Spline->SetKnots( DFIntegrate->GetAbscissaArguments().array(), (2./PI) * DFIntegrate->GetAnswer().col(0).array() );
            Spline->SetKnots( DFIntegrate->GetAbscissaArguments().array(), (.25/PI) * DFIntegrate->GetAnswer().col(0).array() );


		this->ModelledData.resize(TimeGates.size(), 2);
            this->ModelledData.col(0) = TimeGates;
            this->ModelledData.col(1).setZero();

        //  This is non-extrapolated times and values
        //this->ModelledData.resize(ilag, 2);
        //    this->ModelledData.col(0) = DFIntegrate->GetAbscissaArguments(); // this->TimeGates(ii);
		//	this->ModelledData.col(1) = (2./PI)*DFIntegrate->GetAnswer();
        //std::cout <<  (2./PI)*DFIntegrate->GetAnswer().transpose()  << std::endl;

        // Take into account receiver gate integration and tx waveform
        FoldAndConvolve(Spline);

        // Direct caclulation, step response to use need re reset temintegrationkernel back to step!
		//	this->ModelledData.col(1) = (2./PI)*Spline->InterpolateOrderedSet( TimeGates );

        Spline->Delete();
	    TEMIntKern->Delete();
		EMEarthLocal->Delete();
		DFIntegrate->Delete();

	}

	// =======================  Access  =====================================

	void InstrumentTem::SetPulse( const VectorXr& Amp, const VectorXr& times, bool bipolar ) {

        bipolarWaveform = bipolar;
        assert(Amp.size() == times.size());

        TxAmp = Amp;
        TxAbs = times;

        TxDiff = VectorXr::Zero(TxAmp.size());
        TxDelta = VectorXr::Zero(TxAmp.size());

        for (int ip=1; ip<TxAmp.size(); ++ ip) {
            TxDiff(ip)  = (TxAmp(ip-1)-TxAmp(ip  )) /
                          (TxAbs(ip  )-TxAbs(ip-1)) ;
            TxDelta(ip) = (TxAmp(ip-1)-TxAmp(ip  )) ;
        }

        /*
        std::cout << "diff "  << TxDiff.transpose() << std::endl;
        std::cout << "delta "  << TxDelta.transpose() << std::endl;
        std::cout << "amp "  << TxAmp.transpose() << std::endl;
        std::cout << "abs "  << TxAbs.transpose() << std::endl;
        */
	}

	void InstrumentTem::EMEarthModel(LayeredEarthEM* Earth) {
		if (this->EarthMod != NULL) {
			this->EarthMod->DetachFrom(this);
		}
		//std::cout << "InstrumentTem: EarthMod null test" << std::endl;
		Earth->AttachTo(this);
		//std::cout << "InstrumentTem: EarthMod attached" << std::endl;
		this->EarthMod = Earth;
		//std::cout << "InstrumentTem: EMEarthModel Attached" << std::endl;

	}

	void InstrumentTem::SetTransmitLoop(WireAntenna *antennae) {
		if (this->Antenna != NULL) {
			this->Antenna->DetachFrom(this);
		}
		antennae->AttachTo(this);
		this->Antenna = antennae;
		if (this->Dipole != NULL) {
			this->Dipole->DetachFrom(this);
		}
		//std::cout << "InstrumentTem: TransmitLoop Attached" << std::endl;

	}

	void InstrumentTem::SetDipoleSource(DipoleSource* dipolesource) {
		if (this->Dipole != NULL) {
			this->Dipole->DetachFrom(this);
		}
		dipolesource->AttachTo(this);
		this->Dipole = dipolesource;
		if (this->Antenna != NULL) {
			this->Antenna->DetachFrom(this);
		}
	}

	void InstrumentTem::SetReceiver(ReceiverPoints* Receivers) {
		if (this->Receiver != NULL) {
			this->Receiver->DetachFrom(this);
		}
		Receivers->AttachTo(this);
		this->Receiver = Receivers;
		//std::cout << "InstrumentTem: Receivers Attached" << std::endl;

	}

	void InstrumentTem::SetTimes(const VectorXr &times) {
		this->TimeGates = times;
	}


    //--------------------------------------------------------------------------------------
    //       Class:  InstrumentTem
    //      Method:  SetReceiverType
    //--------------------------------------------------------------------------------------
    void InstrumentTem::SetReceiverType ( const ReceiverType& rt ) {
        RType = rt;
        return ;
    }		// -----  end of method InstrumentTem::SetReceiverType  -----


    //--------------------------------------------------------------------------------------
    //       Class:  InstrumentTEM
    //      Method:  SetTimeGates
    //--------------------------------------------------------------------------------------
    void InstrumentTem::SetTimeGates ( const VectorXr &centres, const VectorXr& widths  ) {
        assert (centres.size() == widths.size());
        TimeGates = centres;
        GateWidths = widths;
        return ;
    }		// -----  end of method InstrumentTEM::SetTimeGates  -----




	int InstrumentTem::GetNumberOfTimes() {

		return this->TimeGates.size();
	}

	MatrixXr InstrumentTem::GetMeasurements() {

		return this->ModelledData;
	}



    //--------------------------------------------------------------------------------------
    //       Class:  InstrumentTem
    //      Method:  SetReferenceTime
    //--------------------------------------------------------------------------------------
    void InstrumentTem::SetReferenceTime ( const Real& rtime ) {
        RefTime = rtime;
        return ;
    }		// -----  end of method InstrumentTem::SetReferenceTime  -----



    //--------------------------------------------------------------------------------------
    //       Class:  InstrumentTem
    //      Method:  FoldAndConvolve
    //--------------------------------------------------------------------------------------
    void InstrumentTem::FoldAndConvolve ( CubicSplineInterpolator* Spline ) {

        const static Real GLW[3] = {.5555556, .8888889, .5555556};
        const static Real GLX[3] = {-.7745967, 0., .7745967};

        // TODO BUG check against LEROI!!!
        //SubtractPrevious(Spline);


        switch (RType) {
            case INDUCTIVE:
                // Differentiate to compute impulse response for step input
                // Iterate over time gate
                for (int ig=0; ig<TimeGates.size(); ++ig) {
                    this->ModelledData.col(1)[ig] =
                                  ( ( ConvolveWaveform(Spline, RefTime + TimeGates[ig] - GateWidths[ig]/2.) -
                                      ConvolveWaveform(Spline, RefTime + TimeGates[ig] + GateWidths[ig]/2.) ) / GateWidths[ig]);
                }
                break;

            case MAGNETOMETER:
                for (int ig=0; ig<TimeGates.size(); ++ig) {
                    /*
                    TC(2) = (TCLS(JT) + TOPN(JT)) / 2.
                    TC(1) = TC(2) + HWIDTH * GLX(1)
                    TC(3) = TC(2) + HWIDTH * GLX(3)
                    */
                    this->ModelledData.col(1)[ig] = 0;
                    for (int ii=0; ii<3; ++ii) {
                        //Real tt = RefTime + TimeGates[ig] + GLX[ii]*(GateWidths[ig]/2.);
                        Real tt = RefTime + TimeGates[ig] + GLX[ii]*(GateWidths[ig]/2.);
                        this->ModelledData.col(1)[ig] += ConvolveWaveform(Spline, tt) * GLW[ii]/2.;
                    }
                }
                break;

            case ELECTRIC:
                throw std::runtime_error("InstrumentTEM->FoldAndConvolve with Electic Receivers is not implimented");
                break;
        }

        return ;
    }		// -----  end of method InstrumentTem::FoldAndConvolve  -----


    //--------------------------------------------------------------------------------------
    //       Class:  InstrumentTem
    //      Method:  ConvolveWaveform
    //--------------------------------------------------------------------------------------
    Real InstrumentTem::ConvolveWaveform ( CubicSplineInterpolator* Spline, const Real& t ) {
        static const Real T0MIN = 1e-7;
        Real cnv(0);
        Real tf = t - Spline->GetKnotAbscissa()[0];
        bool der = false;
        Real tend(0);
        for (int ip=1; ip<TxAmp.size(); ++ip) {
            if (TxAbs(ip) < T0MIN) continue;
            if (TxAbs(ip-1) > tf)  break;

            Real tb = t - std::min(tf, TxAbs(ip));
            Real delt = TxAbs(ip) - TxAbs(ip-1);

            der = false;
            if (delt > T0MIN) {
                tend = t - TxAbs(ip-1);
                der = true;
            }

            if (der) {
                //std::cout.precision(16);
                // TODO Integrate seems to be kind of screwy!! Affecting early/on times
                cnv += TxDiff(ip) * Spline->Integrate(tb, tend, 41200); //CUBINT (TRP,YPLS,NTYPLS,TB,TEND)
                // TODO bug in direct spline integration, works sometimes
                //cnv += TxDiff(ip) * Spline->Integrate(tb, tend);
                //cnv += Spline->Integrate(tb, tend, 6264); //CUBINT (TRP,YPLS,NTYPLS,TB,TEND)
                //Real temp = Spline->Integrate(tb, tend, 10036); //CUBINT (TRP,YPLS,NTYPLS,TB,TEND)
                //Real temp2 = Spline->Integrate(tb, tend); //CUBINT (TRP,YPLS,NTYPLS,TB,TEND)
                //std::cout << "der " << TxDiff(ip) << "\t" << std::scientific << tb << "\t" << tend << "\t" << MU0*temp2 << "\t" << MU0*temp << "\n";
                //cnv += temp;
            } else {
                cnv += TxDelta(ip) * Spline->Interpolate(tb); //CUBVAL (TRP,YPLS,NTYPLS,TB)
            }
        }
        return cnv;

    }		// -----  end of method InstrumentTem::ConvolveWaveform  -----


    //--------------------------------------------------------------------------------------
    //       Class:  InstrumentTem
    //      Method:  SubtractPrevious
    //--------------------------------------------------------------------------------------
    void InstrumentTem::SubtractPrevious ( CubicSplineInterpolator *Spline ) {
        // Start by calculating impulse response of this pulse plus any arbitrary number of previous pulses at this
        // repetition frequency
        VectorXr aTimeGates = Spline->GetKnotAbscissa();
        Real pol (1.);
        if (bipolarWaveform) pol = -1.;
        VectorXr PrevPulse = aTimeGates.array() + PulseT;
		// TODO, if not bipolar what does convention say, is this added or should this be zero and handled only by nPreviousPulse?
        VectorXr Fold = Spline->GetKnotOrdinate( ); // +
        //VectorXr Fold =  pol*Spline->InterpolateOrderedSet( PrevPulse ) ;
        VectorXr PrevPulse1; // = PrevPulse.array(); // + PulseT;
        for (int ip=0; ip<NPreviousPulse; ++ip) {
            PrevPulse1.array() += PrevPulse.array() + PulseT;
            PrevPulse.array() += PrevPulse1.array() + PulseT;
            Fold += Spline->InterpolateOrderedSet( PrevPulse1 ) + pol*Spline->InterpolateOrderedSet( PrevPulse ) ;
        }
        Spline->ResetKnotOrdinate(Fold);
        return ;
    }		// -----  end of method InstrumentTem::SubtractPrevious  -----



} // namespace Lemma
