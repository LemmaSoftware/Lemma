/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @version  $Id: cg.h 87 2013-09-05 22:44:05Z tirons $
 **/

#ifndef  CG_INC
#define  CG_INC


#include <iostream>
#include <fstream>
#include "lemma.h"

namespace Lemma {

    /** Port of netlib.org http://www.netlib.org/templates/cpp//cg.h
     *  Solves the symmetric postive definite system Ax=b.
     *  No preconditioner is used.
     *  Iterative template routine -- CG
     *
     *  CG solves the symmetric positive definite linear
     *  system Ax=b using the Conjugate Gradient method.
     *
     *  CG follows the algorithm described on p. 15 in the
     *  SIAM Templates book.
     *
     *  The return value indicates convergence within max_iter (input)
     *  iterations (0), or no convergence within max_iter iterations (1).
     *
     *  Upon successful return, output arguments have the following values:
     *
     *         x  --  approximate solution to Ax = b
     *  max_iter  --  the number of iterations performed before the
     *                tolerance was reached
     *       tol  --  the residual after the final iteration
     *  @param[in] A is a Real matrix A to be solved.
     *  @param[in] x0 is the starting model.
     *  @param[in] b is the right hand side.
     */
    template < typename Scalar >
    VectorXr CG(const MatrixXr &A, const VectorXr &x0, const VectorXr &b,
        int &max_iter, Scalar &tol) {

        VectorXr p, q;
        Scalar alpha, beta, rho, rho_1(0);
        VectorXr x = x0;
        Scalar normb = b.norm( );
        VectorXr r = b - A*x;

        if (normb == 0.0) {
            normb = 1;
        }

        Scalar resid = r.norm() / normb;
        if (resid <= tol) {
            tol = resid;
            max_iter = 0;
            return x;
        }

        for (int i = 1; i <= max_iter; i++) {

            rho = r.transpose()*r;

            if (i == 1) {
                p = r;
            }
            else {
                beta = rho / rho_1;
                p = r + beta * p;
            }

            q = A*p;
            alpha = rho / p.dot(q);

            x += alpha * p;
            r -= alpha * q;

            if ((resid = r.norm() / normb) <= tol) {
                tol = resid;
                max_iter = i;
                return x;
            }
            rho_1 = rho;
        }
        tol = resid;
        std::cerr << "CG FAILED TO REACH CONVERGENCE\n";
        return x;
    }

    // Specialised routine that appliex mod(Ax) so that b and x are in real, but
    // A is complex.
    template < typename Scalar >
    VectorXr CG_ModulusAx(const MatrixXcr &A, const VectorXr &x0, const VectorXr &b,
        int &max_iter, Scalar &tol) {

        VectorXr p, q;
        Scalar beta, rho, rho_1(0);
        Scalar alpha;
        VectorXr x = x0;
        Scalar normb = b.norm( );
        VectorXr r = b.array() - (A*x).array().abs();

        if (normb == 0.0) {
            normb = 1;
        }

        Scalar resid = r.norm() / normb;
        if (resid <= tol) {
            tol = resid;
            max_iter = 0;
            return x;
        }

        for (int i = 1; i <= max_iter; i++) {

            rho = r.dot(r); //conjugate().transpose()*r;

            if (i == 1) {
                p = r;
            }
            else {
                beta = rho / rho_1;
                p = r + beta * p;
            }

            q = (A*p).array().abs();
            alpha = rho / p.dot(q);

            x += alpha * p;
            r -= alpha * q;

            if ((resid = r.norm() / normb) <= tol) {
                tol = resid;
                max_iter = i;
                return x;
            }
            rho_1 = rho;
        }

        tol = resid;
        std::cerr << "CG FAILED TO REACH CONVERGENCE\n";
        return x;
    }

// Preconditioned version of above
//*****************************************************************
// Iterative template routine -- CG
//
// CG solves the symmetric positive definite linear
// system Ax=b using the Conjugate Gradient method.
//
// CG follows the algorithm described on p. 15 in the
// SIAM Templates book.
//
// The return value indicates convergence within max_iter (input)
// iterations (0), or no convergence within max_iter iterations (1).
//
// Upon successful return, output arguments have the following values:
//
//        x  --  approximate solution to Ax = b
// max_iter  --  the number of iterations performed before the
//               tolerance was reached
//      tol  --  the residual after the final iteration
//
//*****************************************************************

//template < class Matrix, class Vector, class Preconditioner, class Real >
//int
//CG(const Matrix &A, Vector &x, const Vector &b,
//   const Preconditioner &M, int &max_iter, Real &tol)
//{
    #include <limits>
    template <typename Preconditioner>
    VectorXr CG(const MatrixXr &A, const VectorXr &x0, const VectorXr &b,
        const Preconditioner &M, int &max_iter, Real &tol) {
        //const Eigen::SparseMatrix<Real> &M, int &max_iter, Real &tol) {

        VectorXr p, z, q;
        VectorXr x = x0;
        Real alpha(0), beta(0), rho(0), rho_1(0);
        Real normb;
        VectorXr r;
        #ifdef LEMMAUSEOMP
        #pragma omp parallel sections
        {
        #endif
            #ifdef LEMMAUSEOMP
            #pragma omp section
            #endif
            {
                normb = b.norm();
            }
            #ifdef LEMMAUSEOMP
            #pragma omp section
            #endif
            {
                r = b - A*x;
            }
        #ifdef LEMMAUSEOMP
        }
        #endif
        if (normb <= std::numeric_limits<double>::epsilon() ) {
            normb = 1;
        }
        Real resid = r.norm() / normb;
        if (resid <= tol) {
            tol = resid;
            max_iter = 0;
            return x;
        }
        // todo do 0th loop manually, gets rid of if statement
        for (int i = 1; i <= max_iter; i++) {
            z = M.solve(r);
            //z = M*r;
            rho = r.transpose()*z;
            if (i == 1) {
                p = z;
            } else {
                beta = rho / rho_1;
                p = z + beta * p;
            }
            q = A*p;
            alpha = rho / p.dot(q);
            #ifdef LEMMAUSEOMP
            #pragma omp parallel sections
            {
            #endif
                #ifdef LEMMAUSEOMP
                #pragma omp section
                #endif
                {
                    x += alpha * p;
                }
                #ifdef LEMMAUSEOMP
                #pragma omp section
                #endif
                {
                    r -= alpha * q;
                }
            #ifdef LEMMAUSEOMP
            }
            #endif
            if ((resid = r.norm() / normb) <= tol) {
               tol = resid;
               max_iter = i;
               return x;
            }
            rho_1 = rho;
        }
        tol = resid;
        std::cerr << "Preconditioned CG failed to converge\n";
        return x;
    }

    template < typename Scalar >
    VectorXr CGJ(const MatrixXr &A, const VectorXr &x0, const VectorXr &b,
        const Eigen::SparseMatrix<Scalar> &M, int &max_iter, Scalar &tol) {

        VectorXr p, z, q;
        VectorXr x = x0;
        Scalar alpha(0), beta(0), rho(0), rho_1(0);
        Scalar normb;
        VectorXr r;
        #ifdef LEMMAUSEOMP
        #pragma omp parallel sections
        {
        #endif
            #ifdef LEMMAUSEOMP
            #pragma omp section
            #endif
            {
                normb = b.norm();
            }
            #ifdef LEMMAUSEOMP
            #pragma omp section
            #endif
            {
                r = b - A*x;
            }
        #ifdef LEMMAUSEOMP
        }
        #endif
        if (normb <= std::numeric_limits<double>::epsilon() ) {
            normb = 1;
        }
        Scalar resid = r.norm() / normb;
        if (resid <= tol) {
            tol = resid;
            max_iter = 0;
            return x;
        }
        // todo do 0th loop manually, gets rid of if statement
        for (int i = 1; i <= max_iter; i++) {
            //z = M.solve(r);
            z = M*r;
            rho = r.transpose()*z;
            if (i == 1) {
                p = z;
            } else {
                beta = rho / rho_1;
                p = z + beta * p;
            }
            q = A*p;
            alpha = rho / p.dot(q);
            #ifdef LEMMAUSEOMP
            #pragma omp parallel sections
            {
            #endif
                #ifdef LEMMAUSEOMP
                #pragma omp section
                #endif
                {
                    x += alpha * p;
                }
                #ifdef LEMMAUSEOMP
                #pragma omp section
                #endif
                {
                    r -= alpha * q;
                }
            #ifdef LEMMAUSEOMP
            }
            #endif
            if ((resid = r.norm() / normb) <= tol) {
               tol = resid;
               max_iter = i;
               return x;
            }
            rho_1 = rho;
        }
        tol = resid;
        std::cerr << "Preconditioned CG failed to converge\n";
        return x;
    }

    ///////////////////////////////////////////////////////
    // Log Barrier
    /** Solves one iteration, using implicit multiplication of At*b, and ATA.
     *  @param[in] G is the sensitivity matrix to be inverted.
     *  @param[in] WdTWd is the data weighting matrix, usually sparse.
     *  @param[in] WmTWm is the model weighting matrix, usually sparse.
     *  @param[in] X1 is a vector of the inverse of the current model X. Used in
     *  log barrier. Should be computed as X1 = VectorXr( 1. / (x.array() - minVal ))
     *  @param[in] Y1 is a vector of the inverse of the current model X. Used in
     *  log barrier. Should be computed as X1 = VectorXr( 1. / (maxVal - x.array() ))
     *  @param[in] X2 is the analaogous inverse of X^2 diagonal matrix, stored
     *  as a Vector.
     *  @param[in] Y2 is the analaogous inverse of (maxVal-X)^2 diagonal matrix, stored
     *  as a Vector.
     *  @param[in] D2 is the datamisfit vector, formally (d_predicted - d_observed).
     *  @param[in,out] Mk is the input / output solution vector. The input value is
     *  an itital guess, and output is the solution.
     *  @param[in] Mr is the reference model.
     *  @param[in] BETA is the regularisation (Tikhonov parameter) to apply
     *  @param[in] P is a preconditioner of G. The product Pb ~ x
     *  @param[in,out] max_iter is the number of iterations.
     *  @param[in,out] tol is the desired tolerance on input, and achieved on
     *  output.
     */
    template < typename  Scalar >
    int implicit_log_CG(const MatrixXr& G, const MatrixXr& WdTWd, const VectorXr& WmTWm,
				const VectorXr& X1, const VectorXr& X2,
				const VectorXr& Y1, const VectorXr& Y2,
				const VectorXr& D2, VectorXr& Mk, const VectorXr& Mr,
				const Scalar& BETA, const MatrixXr& P,
                int& max_iter, Scalar& tol) {

        // TODO add WdTWD to this!

        Scalar resid;
        VectorXr p, z, q;
        Scalar alpha(0), beta(0), rho(0), rho_1(0);

        // Calculate 'B'
        VectorXr delM = Mk - Mr;
        VectorXr B = (-G.transpose()*D2).array() - BETA*((WmTWm.asDiagonal()*delM).array())
                     + X1.array() + Y1.array();

        Scalar normb = B.norm();

        // Implicit calc of A*x
        VectorXr Y = BETA*(WmTWm.asDiagonal()*Mk).array() +
                (X2.asDiagonal()*Mk).array() + (Y2.asDiagonal()*Mk).array();
        VectorXr Z = G*Mk;
        VectorXr U = G.transpose()*Z;

        VectorXr r = B - (Y + U);

        if (normb == 0.0) normb = 1;

        if ((resid = r.norm() / normb) <= tol) {
            tol = resid;
            max_iter = 0;
            return 0;
        }

        for (int i = 1; i <= max_iter; i++) {
            //z = M.solve(r); // we can solve directly z = P*r
            z = P*r;
            rho = r.dot(z);
            if (i == 1)  p = z;
            else {
                beta = rho / rho_1;
                p = beta * p;
 	            p = p+z;
            }
            Y = BETA*(WmTWm.array()*p.array()) + X2.array()*p.array() + Y2.array()*p.array();
            Z = G*p;
            U = G.transpose()*Z;
            q = Y+U;

            alpha = rho / p.dot(q);

            Mk = Mk + alpha * p;
            r  = r - (alpha * q);

            if ((resid = r.norm() / normb) <= tol) {
                tol = resid;
                max_iter = i;
                return 0;
            }
            rho_1 = rho;
        }
        tol = resid;
        return 1;
    }
} // end of namespace Lemma

#endif   // ----- #ifndef CG_INC  -----
