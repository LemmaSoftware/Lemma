/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      09/25/2013 08:20:14 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2013, XRI Geophysics, LLC
 * @copyright Copyright (c) 2013, Trevor Irons
 */

#ifndef  CUBICSPLINEINTERPOLATOR_INC
#define  CUBICSPLINEINTERPOLATOR_INC

#include "LemmaObject.h"

namespace Lemma {

// Simple struct to hold spline terms
struct SplineSet{
    VectorXr a;
    VectorXr b;
    VectorXr c;
    VectorXr d;
    VectorXr x;

    SplineSet( ) {
    }

    SplineSet(const int&n) {
        a = VectorXr::Zero(n+1);
        b = VectorXr::Zero(n);
        c = VectorXr::Zero(n+1);
        d = VectorXr::Zero(n);
        x = VectorXr::Zero(n+1);
    }
};

/**
  \brief   Real 1D Natural cubic spline interpolator.
  \details Splines are fit between knots \f$j\f$ according to the forulae
           \f[ S_j(x) =  a_j + b_j(x - x_j) + c_j(x-x_j)^2 + d_j(x-x_y)^3  \f]
           The spline must satisfy the following conditions
            \f{eqnarray} {
                S_i(x_i) & = & y_i = S_{i-1}(x_i), i = 1,..., n-1 \\
                S'_i(x_i) & = &  S'_{i-1}(x_i), i = 1,..., n-1 \\
                S''_i(x_i) & = & S''_{i-1}(x_i), i = 1,..., n-1 \\
                S''_0(x_0) & = & S''_{n-1}(x_n) = 0
            \f}
 */
class CubicSplineInterpolator : public LemmaObject {

    friend std::ostream &operator<<(std::ostream &stream,
            const CubicSplineInterpolator &ob);

    public:

    // ====================  LIFECYCLE     =======================

    /**
     * @copybrief LemmaObject::New()
     * @copydetails LemmaObject::New()
     */
    static CubicSplineInterpolator* New();

    /**
     *  @copybrief   LemmaObject::Delete()
     *  @copydetails LemmaObject::Delete()
     */
    void Delete();

    // ====================  OPERATORS     =======================

    // ====================  OPERATIONS    =======================

    /** Sets the knots to use for interpolation.
        @param[in] x are the absissa values
        @param[in] y are the ordinate values
     */
    void SetKnots(const VectorXr& x, const VectorXr& y);

    /** Resets the knots to use for interpolation, when abscissa values haven't changed.
        @param[in] y are the ordinate values
     */
    void ResetKnotOrdinate( const VectorXr& y );

    /** Interpolate a monotonically increasing ordered set.
        @param[in] x are the interpolation abscissa points
        @return the ordinate values at x
     */
    VectorXr InterpolateOrderedSet(const VectorXr& x);

    /** integrates the spline from x0 to x1. Uses composite Simpson's rule and n is the number of segments
     *  @param[in] x0 is left argument
     *  @param[in] x1 is right argument
     *  @param[in] n is the number of points, must be even
     */
    Real Integrate(const Real& x0, const Real& x1, const int& n);

    /** integrates using cubic spline values. Taken from AMRIRA P223F project code Leroi, which in turn was based on
        This is a modification of the FUNCTION PPVALU in the book
        "A PRACTICAL GUIDE TO SPLINES"  by C. DE BOOR
     */
    Real Integrate(const Real& x0, const Real& x1);

    /** @returns the know abscissa values
     */
    VectorXr GetKnotAbscissa();

    /** @returns the know abscissa values
     */
    VectorXr GetKnotOrdinate();

    /** Interpolation at a single point.
        @param[in] x is the interpolation abscissa point
        @param[in] i is an optional index to start searching at. Defaults to zero
        @return the ordinate value at x
     */
    Real Interpolate(const Real& x, int& i);

    /** Interpolation at a single point.
        @param[in] x is the interpolation abscissa point
        @return the ordinate value at x
     */
    Real Interpolate(const Real& x);

    // ====================  ACCESS        =======================

    // ====================  INQUIRY       =======================

    protected:

    // ====================  LIFECYCLE     =======================

    /** Default protected constructor, use New */
    CubicSplineInterpolator (const std::string& name);

    /** Default protected destructor, use Delete */
    ~CubicSplineInterpolator ();

    /**
     *  @copybrief   LemmaObject::Release()
     *  @copydetails LemmaObject::Release()
     */
    void Release();

    // ====================  OPERATIONS    =======================

    /**
        Performs spline fitting.
     */
    //void Spline() {
    // }

    /** Finds the interval of knots in spline to use for integration.
     */
    int Interval(const Real& x);

    private:

    SplineSet Spline;

    int ilo;

    int mflag;

    // ====================  DATA MEMBERS  =========================

}; // -----  end of class  CubicSplineInterpolator  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef CUBICSPLINEINTERPOLATOR_INC  -----
