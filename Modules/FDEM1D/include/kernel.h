/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     01/29/2010
  @version  $Id: kernel.h 193 2014-11-10 23:51:41Z tirons $
 **/

#ifndef __kernel_h
#define __kernel_h

#ifdef LEMMAUSEVTK
#include "vtkImplicitFunction.h"
#endif

#include "LemmaObject.h"
#include "receivercubes.h"

namespace Lemma {

        // ==================================================================
        //        Class:  Kernel
        /// \brief Pure abstract class for some kind of kernel function.
        /// \details Provides a consistent interface to kernel functions.
        // ==================================================================
        class Kernel : public LemmaObject {

            /// Prints out basic info about the class
            friend std::ostream &operator<<(std::ostream &stream,
                        const Kernel &ob);

            public:

                // ====================  LIFECYCLE     ======================

                //static Kernel* New();

                virtual void Delete()=0;

                // ====================  OPERATORS     ======================

                // ====================  OPERATIONS    ======================

                // ====================  ACCESS        ======================

                // ====================  INQUIRY       ======================

                virtual VectorXcr ComputeSensitivity()=0;

                #ifdef LEMMAUSEVTK
                virtual VectorXcr ComputeSensitivity(vtkImplicitFunction* Impl)=0;
                #endif

                /** computes sensitivity for same value */
                virtual VectorXcr ComputeSensitivity(const Real& fval)=0;

                /** Computes sensitivity for array of values */
                virtual VectorXcr ComputeSensitivity(const VectorXr& fval)=0;

                virtual void SetFieldCubes(ReceiverCubes* pts)=0;

                // ====================  DATA MEMBERS  ======================

            protected:

                // ====================  LIFECYCLE     ======================

                /// Default protected constructor.
                Kernel (const std::string& name);

                /// Default protected constructor.
                ~Kernel ();

        private:

    }; // -----  end of class  Kernel  -----

}

#endif // __kernel_h
