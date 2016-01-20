/*
 modifed from http://www.netlib.org/templates/cpp//
 Iterative template routine -- BiCGSTAB

 BiCGSTAB solves the unsymmetric linear system Ax = b
 using the Preconditioned BiConjugate Gradient Stabilized method

 BiCGSTAB follows the algorithm described on p. 27 of the
 SIAM Templates book.

 The return value indicates convergence within max_iter (input)
 iterations (0), or no convergence within max_iter iterations (1).

 Upon successful return, output arguments have the following values:

        x  --  approximate solution to Ax = b
 max_iter  --  the number of iterations performed before the
               tolerance was reached
      tol  --  the residual after the final iteration
*/

#include <iostream>
#include <fstream>
#include "lemma.h"

namespace Lemma {

    template <typename Scalar>
    VectorXr BiCGSTAB(const MatrixXr &A, const VectorXr &x0, const VectorXr &b,
          const MatrixXr &M, int &max_iter, Scalar& tol) {
        Scalar resid;
        Scalar rho_1(0), rho_2(0), alpha(0), beta(0), omega(0);
        VectorXr p, phat, s, shat, t, v;
        VectorXr x = x0;
        Scalar normb = b.norm();
        VectorXr r = b - A * x;
        VectorXr rtilde = r;

        if (normb == 0.0)
          normb = 1;

        if ((resid = r.norm() / normb) <= tol) {
          tol = resid;
          max_iter = 0;
          //return 0;
          return x;
        }

        for (int i = 1; i <= max_iter; i++) {
          rho_1 = rtilde.dot(r);
          if (rho_1 == 0) {
            tol = r.norm() / normb;
            //return 2;
            return x;
          }
          if (i == 1)
            p = r;
          else {
            beta = (rho_1/rho_2) * (alpha/omega);
            p = r + beta * (p - omega * v);
          }
          phat = M*p; //M.solve(p);
          v = A * phat;
          alpha = rho_1 / rtilde.dot(v);
          s = r - alpha * v;
          if ((resid = s.norm()/normb) < tol) {
            x += alpha * phat;
            tol = resid;
            //return 0;
            return x;
          }
          shat = M*s;//M.solve(s);
          t = A * shat;
          omega = t.dot(s) / t.dot(t);
          x += alpha * phat + omega * shat;
          r = s - omega * t;

          rho_2 = rho_1;
          if ((resid = r.norm() / normb) < tol) {
            tol = resid;
            max_iter = i;
            //return 0;
            return x;
          }
          if (omega == 0) {
            tol = r.norm() / normb;
            //return 3;
            return x;
          }
        }

        tol = resid;
        return x;
    }
}

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */
