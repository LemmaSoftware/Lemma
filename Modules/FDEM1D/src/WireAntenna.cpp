/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     12/16/2009
  @version  $Id: wireantenna.cpp 199 2014-12-29 19:25:20Z tirons $
 **/

#include "WireAntenna.h"

namespace Lemma {

    #ifdef HAVE_YAMLCPP
    std::ostream &operator << (std::ostream &stream, const WireAntenna &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc --- as a direct stream should encapulste thingy
        return stream;
    }
    #else
    std::ostream &operator<<(std::ostream &stream,
                const WireAntenna &ob) {

        stream << *(LemmaObject*)(&ob);
        stream << "Current: " << ob.Current << " [A]\n";
        stream << "Frequencies: " << ob.Freqs.transpose() << " [Hz]\n";
        stream << "Number of points " << ob.NumberOfPoints << "\n";
        stream << "Number of turns " << ob.NumberOfTurns << "\n";

        if (ob.NumberOfPoints) {
            stream << "Points:\n" << ob.Points.transpose() << "\n";
            //stream << "Dipoles used to approximate " << ob.Dipoles.size() << "\n";
        }
        return stream;
    }
    #endif
    // ====================  LIFECYCLE     =======================

    WireAntenna::WireAntenna(const std::string &name) :
        LemmaObject(name),
        NumberOfPoints(0), Current(1), NumberOfTurns(1) {
    }

#ifdef HAVE_YAMLCPP
    WireAntenna::WireAntenna(const YAML::Node &node) :
        LemmaObject(node) {
        Points =  node["Points"].as<Vector3Xr>();
        Freqs = node["Freqs"].as<VectorXr>();
        NumberOfPoints = node["NumberOfPoints"].as<int>();
        NumberOfTurns = node["NumberOfTurns"].as<int>();
        Current = node["Current"].as<Real>();
    }
#endif

    WireAntenna::~WireAntenna() {
        for (unsigned int id=0; id<Dipoles.size(); ++id) {
            Dipoles[id]->Delete();
        }
        Dipoles.clear();
        if (this->NumberOfReferences != 0) {
            throw DeleteObjectWithReferences(this);
        }
    }

    WireAntenna* WireAntenna::New() {
        WireAntenna* Obj = new WireAntenna("WireAntenna");
        Obj->AttachTo(Obj);
        return Obj;
    }

    WireAntenna* WireAntenna::Clone() {
        WireAntenna* copy = WireAntenna::New();
		//copy->AttachTo(copy); // NO! Attached above!
		copy->NumberOfPoints = this->NumberOfPoints;
		copy->Freqs = this->Freqs;
		copy->Current = this->Current;
		copy->NumberOfTurns = this->NumberOfTurns;
		copy->Points = this->Points;
		//copy->Dipoles = this->Dipoles; // no, disaster
		return copy;
    }

    void WireAntenna::Delete() {
        this->DetachFrom(this);
    }

    void WireAntenna::Release() {
        delete this;
    }


    // ====================  ACCESS        =======================

    void WireAntenna::SetNumberOfTurns(const int &nturns) {
        this->NumberOfTurns = nturns;
    }

    int WireAntenna::GetNumberOfTurns( ) {
        return this->NumberOfTurns;
    }

    void WireAntenna::SetNumberOfPoints(const int &np) {
        Points.resize( Eigen::NoChange, np);
        NumberOfPoints = np;
    }

    void WireAntenna::SetNumberOfFrequencies(const int &nfreq){
        Freqs.resize(nfreq);
        Freqs.setZero();
    }

    void WireAntenna::SetFrequency(const int& ifreq, const Real & freq) {
        assert(ifreq < Freqs.size());
        this->Freqs[ifreq] = freq;
    }

    Real WireAntenna::GetFrequency(const int& ifreq) {
        return this->Freqs[ifreq];
    }

    int WireAntenna::GetNumberOfFrequencies() {
        return Freqs.size();
    }

    Vector3Xr WireAntenna::GetPoints() {
        return Points;
    }

    MatrixXr WireAntenna::GetPointsMat() {
        return MatrixXr(Points.transpose());
    }

    void WireAntenna::SetCurrent(const Real &amps) {
        this->Current = amps;
    }

    Real WireAntenna::GetCurrent( ) {
        return this->Current;
    }

    void WireAntenna::SetPoint(const int &p, const Vector3r& pos) {

        if (p >= 0 && p<NumberOfPoints )    {
            Points.col(p) = pos;
        } else {
            throw  7;
        }

    }

    void WireAntenna::SetPoint(const int &p, const Real& x, const Real& y, const Real& z) {

        if (p >= 0 && p<NumberOfPoints )    {
            Points.col(p) = Vector3r(x,y,z);
        } else {
            throw  7;
        }

    }


    void WireAntenna::ApproximateWithElectricDipoles(const Real &deltai) {

        // Get rid of any dipoles
        for (unsigned int id=0; id<Dipoles.size(); ++id) {
            Dipoles[id]->Delete();
        }
        Dipoles.clear();

        Real Dist(0);
        Vector3r r;
        Vector3r p;

        for (int ip=0; ip<NumberOfPoints-1; ++ip) {

            Dist = (Points.col(ip+1) - Points.col(ip)).norm();
            r = (Points.col(ip+1) - Points.col(ip))/Dist;

            int nd = (int)(Dist/deltai);
            Real add = (Dist - (Real)(nd)*deltai) / (Real)(nd);
            Real delta = deltai + add;
            Real scale = (Real)(NumberOfTurns)*Current;

            p = Points.col(ip) + .5*delta*r;

            for (Real id=0.; id<Dist-delta/2; id+=delta) {

                // X dipoles
                if (std::abs(r[0]) > 1e-6) {
                    DipoleSource *tx = DipoleSource::New();
                    tx->SetLocation(p);
                    tx->SetType(GROUNDEDELECTRICDIPOLE);
                    tx->SetPolarisation(XPOLARISATION);
                    tx->SetFrequencies(Freqs);
                    tx->SetMoment(scale*delta*r[0]);
                    Dipoles.push_back(tx);
                }

                // Y dipoles
                if (std::abs(r[1]) > 1e-6) {
                    DipoleSource *ty = DipoleSource::New();
                    ty->SetLocation(p);
                    ty->SetType(GROUNDEDELECTRICDIPOLE);
                    ty->SetPolarisation(YPOLARISATION);
                    ty->SetFrequencies(Freqs);
                    ty->SetMoment(scale*delta*r[1]);
                    Dipoles.push_back(ty);
                }

                // Z dipoles
                if (std::abs(r[2]) > 1e-6) {
                    DipoleSource *tz = DipoleSource::New();
                    tz->SetLocation(p);
                    tz->SetType(GROUNDEDELECTRICDIPOLE);
                    tz->SetPolarisation(ZPOLARISATION);
                    tz->SetFrequencies(Freqs);
                    tz->SetMoment(scale*delta*r[2]);
                    Dipoles.push_back(tz);
                }

                p += delta*r;

            }
        }
    }

    // ====================  INQUIRY       =======================

    int WireAntenna::GetNumberOfDipoles() {
        return Dipoles.size();
    }

    DipoleSource* WireAntenna::GetDipoleSource(const int &dip) {
        return this->Dipoles[dip];
    }

    #ifdef LEMMAUSEVTK
    vtkActor* WireAntenna::GetVtkActor(const int &idip) {
        return Dipoles[idip]->GetVtkActor();
    }
    #endif

    #ifdef HAVE_YAMLCPP
    //--------------------------------------------------------------------------------------
    //       Class:  WireAntenna
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node WireAntenna::Serialize (  ) const {
        YAML::Node node = LemmaObject::Serialize();
        node.SetTag( this->Name );
        node["NumberOfPoints"] = NumberOfPoints;
        node["NumberOfTurns"] = NumberOfTurns;
        node["Current"] = Current;
        node["Points"] = Points;
        node["Freqs"] = Freqs;
        return node;
    }		// -----  end of method WireAntenna::Serialize  -----


    //--------------------------------------------------------------------------------------
    //       Class:  WireAntenna
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    WireAntenna* WireAntenna::DeSerialize ( const YAML::Node& node ) {
        WireAntenna* Object = new WireAntenna(node);
        Object->AttachTo(Object);
        DESERIALIZECHECK( node, Object )
        return Object ;
    }		// -----  end of method WireAntenna::DeSerialize  -----

    #endif


    //--------------------------------------------------------------------------------------
    //       Class:  WireAntenna
    //      Method:  IsPlanar
    //--------------------------------------------------------------------------------------
    bool WireAntenna::IsHorizontallyPlanar (  ) {
        //for (int ip=0; ip<NumberOfPoints; ++ip) {
        //   std::cout << Points(2,ip) << std::endl;
        //}
        if ( std::abs(Points.row(2).sum() - (NumberOfPoints*Points(2,0))) < 1e-5 ) {
            return true ;
        } else {
            return false;
        }
    }		// -----  end of method WireAntenna::IsPlanar  -----


}
