/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     01/27/2010
  @version  $Id: timer.h 87 2013-09-05 22:44:05Z tirons $
 **/

#ifndef  TIMER_H_INC
#define  TIMER_H_INC

#include <iostream>
#include <fstream>
#include <ctime>
#ifdef LEMMAUSEOMP
#include "omp.h"
#endif

#include "lemma.h"

namespace Lemma {

/** \brief Simple timer class.
  * \details Can be used either with multithreaded applications, using
  * OpenMP defined functions, or using system wall time. System wall time
  * is not good for parallel programs as it reports cpu core execution time.
  */
class jsw_timer {

    public:

        /**
          * Constructor that automatically starts the timer.
          */
        jsw_timer(): start ( std::clock() ), elapsed ( 0.0 ) {

        }

        /** begins the timer.
          */
        void begin() {
            #ifdef LEMMAUSEOMP
            start =  omp_get_wtime();
            #else
            start = std::clock();
            #endif
            elapsed = 0.0;
        }

        /** Ends the timer.
          * @return the time that the stopwatch ended at.
          */
        Real end() {

            #ifdef LEMMAUSEOMP
            elapsed = static_cast<Real> (omp_get_wtime()) - start;
            return elapsed;
            #else
            elapsed = static_cast<Real> ( std::clock() ) - start;
            return elapsed /= CLOCKS_PER_SEC;
            #endif

        }

        /**
          * @return the elapsed time between start and end.
          */
        Real last() const { return elapsed; }

    private:
        #ifdef LEMMAUSEOMP
        Real start;
        #else
        std::clock_t start;
        #endif
        Real    elapsed;
};

}       // -----  end of Lemma  name  -----

#endif   // ----- #ifndef TIMER_H_INC  -----
