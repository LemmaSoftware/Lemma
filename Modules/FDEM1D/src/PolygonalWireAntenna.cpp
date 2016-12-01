/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     05/18/2010
  @version  $Id: PolygonalWireAntenna.cpp 211 2015-02-27 05:43:26Z tirons $
 **/

#include "PolygonalWireAntenna.h"

namespace Lemma {

    std::ostream &operator << (std::ostream &stream, const PolygonalWireAntenna &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc --- as a direct stream should encapulste thingy
        return stream;
    }

	// ====================  LIFECYCLE     =======================

	PolygonalWireAntenna::PolygonalWireAntenna( const ctor_key& key ) : WireAntenna( WireAntenna::ctor_key() ), minDipoleRatio(.15),
   		minDipoleMoment(1e-6), maxDipoleMoment(1e1), rRepeat(1e10,1e10,1e10) {
		Points.setZero();
        //rRepeat.setOnes();
	}

    PolygonalWireAntenna::PolygonalWireAntenna( const YAML::Node& node, const ctor_key& ) : WireAntenna(node, WireAntenna::ctor_key() ) {
        minDipoleRatio   = node["minDipoleRatio"].as<Real>();
        maxDipoleMoment  = node["maxDipoleMoment"].as<Real>();
        minDipoleMoment  = node["minDipoleMoment"].as<Real>();
	}

	PolygonalWireAntenna::~PolygonalWireAntenna() {
	}

    //--------------------------------------------------------------------------------------
    //       Class:  PolygonalWireAntenna
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node PolygonalWireAntenna::Serialize (  ) const {
        YAML::Node node = WireAntenna::Serialize();
        node.SetTag( this->GetName() );
        node["minDipoleRatio"]  = minDipoleRatio;
        node["maxDipoleMoment"] = maxDipoleMoment;
        node["minDipoleMoment"] = minDipoleMoment;
        return node;
    }		// -----  end of method PolygonalWireAntenna::Serialize  -----

    //--------------------------------------------------------------------------------------
    //       Class:  WireAntenna
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    std::shared_ptr<PolygonalWireAntenna> PolygonalWireAntenna::DeSerialize ( const YAML::Node& node ) {
        if (node.Tag() != "PolygonalWireAntenna") {
            throw  DeSerializeTypeMismatch( "PolygonalWireAntenna", node.Tag());
        }
        return std::make_shared<PolygonalWireAntenna> ( node, ctor_key() );
    }		// -----  end of method WireAntenna::DeSerialize  -----

	std::shared_ptr<PolygonalWireAntenna> PolygonalWireAntenna::NewSP() {
        return std::make_shared<PolygonalWireAntenna>( ctor_key() );
	}

	std::shared_ptr<WireAntenna> PolygonalWireAntenna::Clone() const {
		auto copy = PolygonalWireAntenna::NewSP();
        copy->minDipoleRatio = this->minDipoleRatio;
		copy->minDipoleMoment = this->minDipoleMoment;
		copy->maxDipoleMoment = this->maxDipoleMoment;
		copy->NumberOfPoints = this->NumberOfPoints;
		copy->Freqs = this->Freqs;
		copy->Current = this->Current;
		copy->NumberOfTurns = this->NumberOfTurns;
		copy->Points = this->Points;
		//copy->Dipoles = this->Dipoles; // no, disaster
		return copy;
	}

    std::shared_ptr<PolygonalWireAntenna> PolygonalWireAntenna::ClonePA() const {
		auto copy = PolygonalWireAntenna::NewSP();
        copy->minDipoleRatio = this->minDipoleRatio;
		copy->minDipoleMoment = this->minDipoleMoment;
		copy->maxDipoleMoment = this->maxDipoleMoment;
		copy->NumberOfPoints = this->NumberOfPoints;
		copy->Freqs = this->Freqs;
		copy->Current = this->Current;
		copy->NumberOfTurns = this->NumberOfTurns;
		copy->Points = this->Points;
		//copy->Dipoles = this->Dipoles; // no, disaster
		return copy;
	}

    //--------------------------------------------------------------------------------------
    //       Class:  PolygonalWireAntenna
    //      Method:  GetName
    // Description:  Class identifier
    //--------------------------------------------------------------------------------------
    inline std::string PolygonalWireAntenna::GetName (  ) const {
        return CName;
    }		// -----  end of method PolygonalWireAntenna::GetName  -----

    void PolygonalWireAntenna::SetMinDipoleRatio (const Real& ratio) {
        minDipoleRatio = ratio;
    }

    void PolygonalWireAntenna::SetMinDipoleMoment (const Real& m) {
        minDipoleMoment = m;
    }

    void PolygonalWireAntenna::SetMaxDipoleMoment (const Real& m) {
        maxDipoleMoment = m;
    }

	// ====================  OPERATIONS    =======================

	void PolygonalWireAntenna::ApproximateWithElectricDipoles(const Vector3r &rp) {
        // Only resplit if necessary. Save a few cycles if repeated
        if ( (rRepeat-rp).norm() > 1e-16 ) {
		    Dipoles.clear();

		    // loop over all segments
		    for (int iseg=0; iseg<NumberOfPoints-1; ++iseg) {
			    InterpolateLineSegment(Points.col(iseg), Points.col(iseg+1), rp);
		    }
            rRepeat = rp;

        } else {
            for (unsigned int id=0; id<Dipoles.size(); ++id) {
                Dipoles[id]->SetFrequencies(Freqs);
            }
        }
	}

	Vector3r PolygonalWireAntenna::ClosestPointOnLine(const Vector3r &p1,
					const Vector3r &p2, const Vector3r &tp) {

		Vector3r v1 = p2 - p1;
		Vector3r v2 = p1 - tp;
		Vector3r v3 = p1 - p2;
		Vector3r v4 = p2 - tp;

		Real dot1 = v2.dot(v1);
		Real dot2 = v1.dot(v1);
		Real dot3 = v4.dot(v3);
		Real dot4 = v3.dot(v3);
		Real t1 = -1.*dot1/dot2;
		Real t2 = -1.*dot3/dot4;

		Vector3r pos =  p1+v1*t1 ;

		// check if on line
		//	else give back the closest end point
		if ( t1>=0 && t2>=0. ) {
			return pos;
		} else if (t1<0) {
			return p1;
		} else {
			return p2;
		}
	}

	void PolygonalWireAntenna::PushXYZDipoles(const Vector3r &step,
				    const Vector3r &cp, const Vector3r &dir,
					std::vector< std::shared_ptr<DipoleSource> > &xDipoles) {

		Real scale = (Real)(NumberOfTurns)*Current;

        auto tx = DipoleSource::NewSP();
		    tx->SetLocation(cp);
		    tx->SetType(UNGROUNDEDELECTRICDIPOLE);
			tx->SetPolarisation(dir);
			tx->SetFrequencies(Freqs);
			tx->SetMoment(scale*step.norm());
			xDipoles.push_back(tx);
	}

	void PolygonalWireAntenna::CorrectOverstepXYZDipoles(const Vector3r &step,
				    const Vector3r &cp,	const Vector3r &dir,
					std::vector< std::shared_ptr<DipoleSource> > &xDipoles ) {

		Real scale = (Real)(NumberOfTurns)*Current;

		// X oriented dipoles
		if (step.norm() > minDipoleMoment) {
			xDipoles[xDipoles.size()-1]->SetLocation(cp);
			xDipoles[xDipoles.size()-1]->SetMoment(scale*step.norm());
		}
	}

	void PolygonalWireAntenna::InterpolateLineSegment(const Vector3r &p1,
					const Vector3r &p2, const Vector3r & tp) {


		Vector3r phat = (p1-p2).array() / (p1-p2).norm();
		Vector3r c    = this->ClosestPointOnLine(p1, p2, tp);
		Real dtp = (tp-c).norm();    // distance to point at c
		Real dc1 = (p1-c).norm();    // distance to c from p1
		Real dc2 = (p2-c).norm();    // distance to c from p1

        // unit vector
		Vector3r cdir = (p2-p1).array() / (p2-p1).norm();

		///////////////////
		// dipoles for this segment
		std::vector< std::shared_ptr<DipoleSource> >       xDipoles;

		// go towards p1
		if ( ((c-p1).array().abs() > minDipoleMoment).any() ) {

			// cp = current pos, lp = last pos
			Vector3r cp = c + phat*(dtp*minDipoleRatio)*.5;
			Vector3r lp = c;
 			Real dist = (cp-p1).norm();
 			Real dist_old = dist+1.;

			// check that we didn't run past the end, or that we aren't starting at
			// the end, or that initial step runs over!
			Vector3r dir = (p1-cp).array() / (p1-cp).norm();   // direction of movement
			Vector3r step = phat*(dtp*minDipoleRatio);
 			Vector3r stepold = Vector3r::Zero();

			// (dir-phat) just shows if we are stepping towards or away  from p1
			while (dist < dist_old && (dir-phat).norm() < 1e-8) {

				PushXYZDipoles(step, cp, cdir, xDipoles);

				// Make 1/2 of previous step, 1/2 of this step, store this step
				stepold  = step;
				step     = phat*( (cp-tp).norm()*minDipoleRatio );
			    while ( (step.array().abs() > maxDipoleMoment).any() ) {
				    step *= .5;
			    }
				lp       = cp;
				cp      += .5*stepold + .5*step;
				dist     = (cp-p1).norm();
				dir      = (p1-cp).array() / (p1-cp).norm();
			}

			// cp now points to end last of dipole moments
			cp -= .5*step;

			// Fix last dipole position, so that entire wire is represented,
			// and no more
			Real distLastSeg = (c - cp).norm();

			if (distLastSeg + minDipoleMoment < dc1) {
				// case 1: understep, add dipole
				step  = (p1-cp).array();
				cp   += .5*step;
				PushXYZDipoles(step, cp, cdir, xDipoles);
			} else if (distLastSeg > dc1 + minDipoleMoment) {
				// case 2: overstep, reposition dipole and size
				step  = (p1 - (lp-.5*stepold));
				cp =  (lp-.5*stepold) + (.5*step);
				CorrectOverstepXYZDipoles(step, cp, cdir, xDipoles);
			}
			// else case 0: nearly 'perfect' fit do nothing
		}

		// go towards p2
		if ( ( (c-p2).array().abs() > minDipoleMoment).any() ) {

 			// cp = current pos, lp = last pos
 			Vector3r step = -phat*(dtp*minDipoleRatio);
			while ( (step.array().abs() > maxDipoleMoment).any() ) {
				step *= .5;
			}

 			Vector3r cp = c + step*.5;
 			Vector3r lp = c;
  			Real dist = (p2-cp).norm();
  			Real dist_old = dist+1e3;

			// check that we didn't run past the end, or that we aren't starting at
			// the end, or that initial step runs over!
			Vector3r dir = (p2-cp).array() / (p2-cp).norm();   // direction of movement

 			Vector3r stepold = Vector3r::Zero();

 			// (dir-phat) just shows if we are stepping towards or away  from p1
 			while (dist < dist_old && (dir+phat).norm() < 1e-8) {

 				PushXYZDipoles(step, cp, cdir, xDipoles);

				// Make 1/2 of previous step, 1/2 of this step, store this step
				stepold  = step;
				step     = -phat*( (cp-tp).norm()*minDipoleRatio );
			    while ( (step.array().abs() > maxDipoleMoment).any() ) {
				    step *= .5;
			    }
				lp       = cp;
				cp      += .5*stepold + .5*step;
				dist     = (cp-p2).norm();
				dir      = (p2-cp).array() / (p2-cp).norm();
			}

			// cp now points to end last of dipole moments
			cp -= .5*step;

			// Fix last dipole position, so that entire wire is represented,
			// and no more
			Real distLastSeg = (c - cp).norm();
			if (distLastSeg + minDipoleMoment < dc2) {
				// case 1: understep, add dipole
				step  = (p2-cp).array();
				cp   += .5*step;
				PushXYZDipoles(step, cp, cdir, xDipoles);

			} else if (distLastSeg > dc2 + minDipoleMoment) {

				// case 2: overstep, reposition dipole and size
				step  = (p2 - (lp-.5*stepold));
				cp =  (lp-.5*stepold) + (.5*step);
				CorrectOverstepXYZDipoles(step, cp, cdir, xDipoles);

			}
			// else case 0: nearly 'perfect' fit do nothing
		}
		Dipoles.insert(Dipoles.end(), xDipoles.begin(), xDipoles.end());
	}


}
