/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      02/07/2014 12:50:52 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@lemmasoftware.org
 * @copyright Copyright (c) 2014,2018 Trevor Irons
 */

#include "CubicSplineInterpolator.h"

#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<cmath>

namespace Lemma {

    // ====================  FRIEND METHODS  =====================

    std::ostream &operator << (std::ostream &stream, const CubicSplineInterpolator &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc ---
        return stream;
    }

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  CubicSplineInterpolator
    //      Method:  CubicSplineInterpolator
    // Description:  constructor (locked with ctor_key)
    //--------------------------------------------------------------------------------------
    CubicSplineInterpolator::CubicSplineInterpolator ( const ctor_key& key ) : LemmaObject( key ) {

    }  // -----  end of method CubicSplineInterpolator::CubicSplineInterpolator  (constructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  CubicSplineInterpolator
    //      Method:  CubicSplineInterpolator
    // Description:  DeSerializing constructor (locked with ctor_key)
    //--------------------------------------------------------------------------------------
    CubicSplineInterpolator::CubicSplineInterpolator (const YAML::Node& node, const ctor_key& key ) : LemmaObject(node, key) {
    }  // -----  end of method CubicSplineInterpolator::CubicSplineInterpolator  (constructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  CubicSplineInterpolator
    //      Method:  NewSP()
    // Description:  public constructor
    //--------------------------------------------------------------------------------------
    std::shared_ptr<CubicSplineInterpolator> CubicSplineInterpolator::NewSP() {
        return std::make_shared<CubicSplineInterpolator>( ctor_key() );
    }

    //--------------------------------------------------------------------------------------
    //       Class:  CubicSplineInterpolator
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node  CubicSplineInterpolator::Serialize (  ) const {
        YAML::Node node = LemmaObject::Serialize();;
        node.SetTag( GetName() );
        // FILL IN CLASS SPECIFICS HERE
        return node;
    }		// -----  end of method CubicSplineInterpolator::Serialize  -----

    //--------------------------------------------------------------------------------------
    //       Class:  CubicSplineInterpolator
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    std::shared_ptr<CubicSplineInterpolator> CubicSplineInterpolator::DeSerialize ( const YAML::Node& node  ) {
        if (node.Tag() != "CubicSplineInterpolator") {
            throw  DeSerializeTypeMismatch( "CubicSplineInterpolator", node.Tag());
        }
        return std::make_shared<CubicSplineInterpolator>( node, ctor_key() );
    }

    //--------------------------------------------------------------------------------------
    //       Class:  CubicSplineInterpolator
    //      Method:  ~CubicSplineInterpolator
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    CubicSplineInterpolator::~CubicSplineInterpolator () {

    }  // -----  end of method CubicSplineInterpolator::~CubicSplineInterpolator  (destructor)  -----


    // ====================  OPERATIONS    =======================

    //--------------------------------------------------------------------------------------
    //       Class:  CubicSplineInterpolator
    //      Method:  SetKnots
    //--------------------------------------------------------------------------------------
    void CubicSplineInterpolator::SetKnots ( const VectorXr& x, const VectorXr& y ) {

        int n = x.size()-1;

        Spline = SplineSet(n);

        Spline.a = y;
        Spline.x = x;
        VectorXr h = VectorXr::Zero(n);
        for(int i=0; i<n; ++i)
            h(i) = Spline.x(i+1)-Spline.x(i);

        VectorXr alpha(n-1);
        for(int i=1; i<n-1; ++i)
            alpha(i) =  3.*(Spline.a[i+1]-Spline.a[i])/h[i] - 3.*(Spline.a[i]-Spline.a[i-1])/h[i-1] ;

        VectorXr l = VectorXr::Zero(n+1);
        VectorXr mu  = VectorXr::Zero(n+1);
        VectorXr z = VectorXr::Zero(n+1);
        l[0] = 1;
        mu[0] = 0;
        z[0] = 0;

        for(int i = 1; i < n-1; ++i) {
            l[i] = 2 *(Spline.x[i+1]-Spline.x[i-1])-h[i-1]*mu[i-1];
            mu[i] = h[i]/l[i];
            z[i] = (alpha[i]-h[i-1]*z[i-1])/l[i];
        }
        l[n] = 1;
        z[n] = 0;

        for(int j = n-1; j >= 0; --j) {
            Spline.c[j] = z[j] - mu[j] * Spline.c[j+1];
            Spline.b[j] = (Spline.a[j+1]-Spline.a[j])/h[j]-h[j]*(Spline.c[j+1]+2*Spline.c[j])/3;
            Spline.d[j] = (Spline.c[j+1]-Spline.c[j])/3/h[j];
        }
        // On OSX, this causes a strange bug 'sometimes', alignment?
        //Spline.c = Spline.c.head(n);

        return;
    }		// -----  end of method CubicSplineInterpolator::SetKnots  -----


    //--------------------------------------------------------------------------------------
    //       Class:  CubicSplineInterplator
    //      Method:  ResetKnotOrdinate
    //--------------------------------------------------------------------------------------
    void CubicSplineInterpolator::ResetKnotOrdinate ( const VectorXr& y ) {
        VectorXr x = Spline.x;
        SetKnots(x, y);
        return ;
    }		// -----  end of method CubicSplineInterplator::ResetKnotOrdinate  -----


    //--------------------------------------------------------------------------------------
    //       Class:  CubicSplineInterpolator
    //      Method:  InterpolateOrderedSet
    //--------------------------------------------------------------------------------------
    VectorXr CubicSplineInterpolator::InterpolateOrderedSet ( const VectorXr& x ) {
        VectorXr y = VectorXr::Zero(x.size());
        int ii = 0;
        for (int iy=0; iy<y.size(); ++iy) {
            y[iy] = Interpolate(x[iy], ii);
        }
        return y;
    }		// -----  end of method CubicSplineInterpolator::InterpolateOrderedSet  -----


    //--------------------------------------------------------------------------------------
    //       Class:  CubicSplineInterpolator
    //      Method:  Interpolate
    //--------------------------------------------------------------------------------------
    Real CubicSplineInterpolator::Interpolate ( const Real& x, int& i) {
        // O(n) search, could do bisection, but if these are sorted, then this is quick
        while(Spline.x[i] < x && i<Spline.x.size()) {
            ++i;
        }
        --i;

        //if ( x > Spline.x[i] ) {
        //    throw std::runtime_error("CubicSplineInterpolator::Interpolate ATTEMPT TO INTERPOLATE PAST LAST KNOT");
        //}

        return Spline.a[i] + Spline.b[i]*(x-Spline.x[i]) + Spline.c[i]*((x-Spline.x[i])*(x-Spline.x[i])) +
               Spline.d[i]*((x-Spline.x[i])*(x-Spline.x[i])*(x-Spline.x[i]) );
    }		// -----  end of method CubicSplineInterpolator::Interpolate  -----

    //--------------------------------------------------------------------------------------
    //       Class:  CubicSplineInterpolator
    //      Method:  Interpolate
    //--------------------------------------------------------------------------------------
    Real CubicSplineInterpolator::Interpolate ( const Real& x ) {
        int ii(0);
        return Interpolate(x, ii);
    }


    //--------------------------------------------------------------------------------------
    //       Class:  CubicSplineInterpolator
    //      Method:  Integrate
    //--------------------------------------------------------------------------------------
    Real CubicSplineInterpolator::Integrate ( const Real& x0, const Real& x1, const int& n ) {

        assert(n > 0);
        // force n to be even?
        //n += (n % 2);

        Real S = Interpolate(x0) + Interpolate(x1);
        Real h = (x1 - x0) / Real(n);

        int ik = 0;
        for (int i=1; i<n; i+=2) {
            S += 4. * Interpolate(x0 + (Real)(i)*h, ik);
        }

        ik = 0;
        for (int i=2; i<n-1; i+=2) {
            S += 2. * Interpolate(x0 + (Real)(i)*h, ik);
        }

        return  S * h / 3.;
    }


    //--------------------------------------------------------------------------------------
    //       Class:  CubicSplineInterpolator
    //      Method:  Integrate
    //--------------------------------------------------------------------------------------
    Real CubicSplineInterpolator::Integrate ( const Real& x0, const Real& x1 ) {


        int i0 = Interval(x0);
        int i1 = Interval(x1);

        Real h0 = x0 - Spline.x(i0);
        if (mflag == -1) h0 = 0;
        Real h1 = x1 - Spline.x(i1);

        Real cubint = (((Spline.d(i1)*h1/4.0 + Spline.c(i1) )*h1/3.0 +
                         Spline.b(i1) )*h1/2.0 + Spline.a(i1) )*h1
                    - (((Spline.d(i0)*h0/4.0 + Spline.c(i0) )*h0/3.0 +
                         Spline.b(i0) )*h0/2.0 + Spline.a(i0) )*h0;

        // Include integrals over intervening intervals.
        if (i1 > i0) {
            for (int i=i0; i<i1-1; ++i) {
                Real h = Spline.x(i+1) - Spline.x(i);
                cubint +=  (((Spline.d(i)*h/4.0 + Spline.c(i) )*h/3.0 +
                              Spline.b(i))*h/2.0 + Spline.a(i) )*h;
            }
        }
        return cubint;
    }		// -----  end of method CubicSplineInterpolator::Integrate  -----



    //--------------------------------------------------------------------------------------
    //       Class:  CubicSplineInterpolator
    //      Method:  Interval
    //--------------------------------------------------------------------------------------
    int CubicSplineInterpolator::Interval ( const Real& x ) {

        std::cerr << "ENTERING CubicSplineInterpolator::Inverval. Known bugs here"  << std::endl;
        int nx = Spline.x.size() - 2; // TODO check if this is correct or just -1
        // when x not in range
        if (x <= Spline.x(0) || nx <= 1 ) {
            mflag = -1;
            return 1;
        }

        if (x >= Spline.x(nx)) {
            mflag = 1;
            return nx;
        }

        mflag = 0;
        if (ilo >= nx) ilo = nx-1;
        int ihi = ilo+1;

        // if x is already in the interval
        if ( x<Spline.x(ihi) && x >= Spline.x(ilo) ) {
            //std::cout << "TRIVIAL INTERVAL " << Spline.x(ilo) << "\t" << x << "\t" << Spline.x(ihi) << std::endl;
            return ilo;
        }

        if (x <= Spline.x(ilo)) { // decrease ilo to capture
            int istep = 1;
            for (int ix=1; ix<nx; ++ix) {
                ihi = ilo;
                ilo = ihi - istep;
                ilo = std::max(1, ilo);
                if (x >= Spline.x(ilo) || ilo == 1) break;
                istep *= 2;
            }
        } else if (x >= Spline.x(ihi)) { // increase ihi to capture
            int istep = 1;
            for (int ix=1; ix<nx; ++ix) {
                ilo = ihi;
                ihi = ilo + istep;
                ihi = std::min(ihi, nx);
                if (x <= Spline.x(ihi) || ihi == nx) break;
                istep *= 2;
            }
        }

        // Now Spline.x(ilo) <= x < Spline.x(ihi) --> Narrow the interval.
        //std::cout << "WIDE INTERVAL " << Spline.x(ilo) << "\t" << x << "\t" << Spline.x(ihi) << std::endl;
        for (int ix=1; ix<nx; ++ix) {
            int middle = (ilo+ihi) / 2;
            if (middle == ilo) break;
            if (x < Spline.x(middle)) {
                ihi = middle;
            } else {
                ilo = middle;
            }
        }
        assert ( Spline.x(ilo) < x && x < Spline.x(ihi) );
        return ilo;
    }		// -----  end of method CubicSplineInterpolator::Inverval  -----



    //--------------------------------------------------------------------------------------
    //       Class:  CubicSplineInterplator
    //      Method:  GetAbscissa
    //--------------------------------------------------------------------------------------
    VectorXr CubicSplineInterpolator::GetKnotAbscissa (  ) {
        return Spline.x;
    }		// -----  end of method CubicSplineInterplator::get_GetAbscissa  -----


    //--------------------------------------------------------------------------------------
    //       Class:  CubicSplineInterpolator
    //      Method:  GetKnotOrdinate
    //--------------------------------------------------------------------------------------
    VectorXr CubicSplineInterpolator::GetKnotOrdinate (  ) {
        return Spline.a;
    }		// -----  end of method CubicSplineInterpolator::GetKnotOrdinate  -----


}		// -----  end of Lemma  name  -----

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */
