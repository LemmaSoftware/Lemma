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
 * @email     Trevor.Irons@lemmasoftware.org
 * @copyright Copyright (c) 2013,2018 Trevor Irons
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

    SplineSet(const Index& n) {
        a = VectorXr::Zero(n+1);
        b = VectorXr::Zero(n);
        c = VectorXr::Zero(n+1);
        d = VectorXr::Zero(n);
        x = VectorXr::Zero(n+1);
    }
};

/**
 * \ingroup LemmaCore
 * \brief   Real 1D Natural cubic spline interpolator.
 * \details Splines are fit between knots \f$j\f$ according to the forulae
 *          \f[ S_j(x) =  a_j + b_j(x - x_j) + c_j(x-x_j)^2 + d_j(x-x_y)^3  \f]
 *          The spline must satisfy the following conditions
 *           \f{eqnarray} {
 *               S_i(x_i) & = & y_i = S_{i-1}(x_i), i = 1,..., n-1 \\
 *               S'_i(x_i) & = &  S'_{i-1}(x_i), i = 1,..., n-1 \\
 *               S''_i(x_i) & = & S''_{i-1}(x_i), i = 1,..., n-1 \\
 *               S''_0(x_0) & = & S''_{n-1}(x_n) = 0
 *           \f}
 */
class CubicSplineInterpolator : public LemmaObject {

    friend std::ostream &operator<<(std::ostream &stream,
            const CubicSplineInterpolator& ob);

    //struct ctor_key {};

    public:

    // ====================  LIFECYCLE     =======================

    /** Default constructor */
    explicit CubicSplineInterpolator ( const ctor_key& );

    /** DeSerializing constructor, locked use factory DeSerialize  method*/
    CubicSplineInterpolator ( const YAML::Node& node, const ctor_key& );

    /** Destructor use smart pointers to auto delete */
    virtual ~CubicSplineInterpolator ();

    /**
     *  Factory method for generating concrete class.
     *  @return a std::shared_ptr of type CubicSplineInterpolator
     */
    static std::shared_ptr<CubicSplineInterpolator> NewSP();

    /**
     *  Uses YAML to serialize this object.
     *  @return a YAML::Node
     */
    virtual YAML::Node Serialize() const;

    /**
     *   Constructs an object from a YAML::Node.
     */
    static std::shared_ptr< CubicSplineInterpolator > DeSerialize(const YAML::Node& node);

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

    /** Returns the name of the underlying class, similiar to Python's type */
    virtual inline std::string GetName() const {
        return CName;
    }

    protected:

    // ====================  OPERATIONS    =======================

    /** Finds the interval of knots in spline to use for integration.
     */
    Index Interval(const Real& x);

    private:

    /** Copy */
    CubicSplineInterpolator( const CubicSplineInterpolator& ) = delete;

    /** ASCII string representation of the class name */
    static constexpr auto CName = "CubicSplineInterpolator";

    SplineSet Spline;

    Index ilo;

    int mflag;

    // ====================  DATA MEMBERS  =========================

}; // -----  end of class  CubicSplineInterpolator  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef CUBICSPLINEINTERPOLATOR_INC  -----

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp syntax=cpp.doxygen: */
