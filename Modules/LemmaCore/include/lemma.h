/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     06/19/2009 09:12:20 AM  The Birth of Lemma!
  @version  $Id: lemma.h 203 2015-01-09 21:19:04Z tirons $
 **/

//  \image html lemma.png
/** \mainpage Lemma is an ElectroMagnetics Modelling API

    \authors Trevor Irons and M. Andrew Kass and others

    Originally Lemma was intended as a recursive acronym standing for
    <B>L</B>emma is an <B>E</B>lectro<B>M</B>agnetics <B>M</B>odelling <B>A</B>PI.
    As the breadth of the project has expanded, the name has remained
    appropriate in a more literal sense. Lemma is a flexible cross-platform
    library delivering an expressive API that can be used to easily create
    versatile programs. Lemma is not itself a program, instead it is a
    collection of building blocks to make geophysical applications.
    We retain this name because:

    - In mathematics a Lemma is a proven proposition which is used as a
    stepping stone to a larger result rather than as a statement in-and-of
    itself.
    - In addition to the electromagnetic modelling, some other facilities are
    provided such as numerical optimization and inversion capabilities. These
    tools are also considered stepping stones to final products.

    We feel that this is a particularly approprate name, as Lemma's
    API can be leveraged create powerful applications such as forward
    modelling and inverting frequency and time-domain
    surveys of arbitrary survey design, sNMR surveys, CSAMT and more.

    \section Motivation
    Why another Geophysical EM project? For starters, there aren't that many
    quality open source packages out there. Those that do exist are generally
    specialized to perform a single task and extending them is a major undertaking.
    Lemma's approach is much different, by providing a set of general tools users
    can easily assemble applications that suite their needs. Furthermore, most are
    written in either Fortran or MATLAB, and can be difficult to integrate into
    multiphysics applications. To our knowlege, Lemma is the only C++ EM simulation
    package freely available.

    \section Capabilities Capabilities
    In the long term, we have many goals for this software project. Due to its
    design, Lemma can be built upon and extended easily. The initial aim is to
    provide flexible 1D and 3D EM modelling in the time and frequency domains.
    The project is still in beta, but we have made a lot of progress already.
    We will release our first non-beta release as soon as the following are
    supported.

    \subsection FDM Frequency-domain forward modelling
    Lemma was initially called EMMODFD: Electromagnetic Modelling in the Frequency
    Domain. As such this is the most mature area of Lemma.

    \par 1D
    Frequency domain solutions to electrical and magnetic dipoles can be computed
    quasi-analytically in 1D. Calculations can be made in or above the layered
    media, and complex electrical conductivity and magnetic susceptibility are
    supported according to the Cole-Cole model. Sources may be embdedded in the
    media or in the resisitive air layer. Lemma can also can compute fields due
    to arbitrarily shaped ungrounded wire loops, topography of the loops is also
    supported. Two separate approaches to solving the Hankel transform, one
    based on Anderson's digitial filtering technique, and another based on Gaussian
    quadrature.

    \par 3D
    A fast 3D solver that can modify the 1D results based
    on arbitrary electrical conductivity model is nearing completion and is
    provided in a separate module.

    \par future work
    We are also planning on supporting grounded wires in the near future.

    \subsection TDM Time-domain forward modelling
	A 1D time-domain solution has been implemented that utilises both a
	dipole source as well as a wire loop.  Currently, only one receiver is
	modelled at a time, but will be generalised.  In addition, utilities
	to read in data files for modelling have been implemented.
    We would like to offer 3D time domain support, but this will not be
    provided before our first stable release.

    \subsection DataFormats Data Formats
    The EM community is plagued with myriad data formats. Often each equiptment
    manufacturer provides their own data format and interoperability is a
    constant struggle. We are working on a flexible data format based on the XML
    format that can be adapted to many types of data. The template for this
    format will be publically released and we hope it catches on in the community.
    At the least, it will provide a mechanism to compare datasets and datatypes
    within Lemma.

    \section Modules Modules
    Due to Lemma's design, it is easy to extend the platform. In some cases this
    extension results in adding functionality that is not directly related to
    ElectroMagnetics. The following modules utilise parts of Lemma to provide
    their functionality.

    \section Tutorials
        - \ref Tutorial - Basic intruduction to Lemma, including aquiring and
          compiling the code, class structure, and building your own
          applications.
        - \ref Extending Tutorial on how to extend Lemma.


    \section Development Development and design
    Ths package was initially developed by the Center for Gravity, Electrical, and
    Magnetic Studies (CGEM) at the Colorado School of Mines (CSM), the United
    States Geological Survey (USGS), and Broken Spoke Development, LLC. Where it drew
    on work by many others including Ki Ha Lee, and Walt Anderson. All new work and
    interfaces are written entirely in C++. Several small external projects are
    included, which are written in standard C, and FORTRAN 77. We adapt a
    modern, test driven, object oriented, C++ framework.
    More recent development has been undertaken at the University of Utah through the Energy
    and Geoscience Institute.

    \section Legalities

    \subsection Copyrights
    The following copyrights apply to the source.
    Most of the code was developed either by Trevor
    Copyright (C) 2008-2010 Trevor Irons <trevor.irons@lemmasoftware.org> or
    M. Andrew Kass Copyright (C) 2010 <mkass@usgs.gov>.

    The 1D EM solver was derived (but updated heavily) from a Fortran
    programme written by Ki Ha Lee in 1984. We have communicated with Ki Ha,
    and he assured us that this code is in the public domain.

    A Gaussian quadrature hankel transform originally written by Alan Chave was
    ported to C++. This code is in the public domain, and the source code was
    published in Geophysics.

    A digital filtering approach to the Hankel transform written by Walt
    Anderson was also rewritten for Lemma. The original Fortran code is also in
    the public domain.

    Please note that Ki Ha Lee and Walt Anderson had no part in this work, and
    the above should not be interpreted as any sort of endorsement by those
    parties.

    \subsection License

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    \section Contributing Suggestions and contributions
        We welcome contributions and suggestions. Feel free to email the development
        team at info@lemmasoftware.org.
        Under the terms of the MPL, if you modify a Lemma file, you are obligated to
        share those contributions back with the community.

    \section Useful Useful links
        - Home page https://lemmasoftware.org
        - Git repository https://git.lemmasoftware.org
        - Broken Spoke Develpment http://numericalgeo.com
        - CGEM at the Coloroado School of Mines http://geophysics.mines.edu/cgem/
        - EGI at the Eniversity of Utah http://egi.utah.edu/
**/

#pragma once

#ifndef __LEMMA_H
#define __LEMMA_H

    #include <LemmaConfig.h>

    // Include some basic stuff that will always be needed
    #include <iostream>
    #include <iomanip>
    #include <complex>
    #include <fstream>
    #include <string>
    #include <vector>
    #include <stdexcept>
    #include <sstream>

    #include <Eigen/Core>
    #include <cstddef>
    #include <Eigen/StdVector>
    #include <Eigen/Sparse>
    #include <unsupported/Eigen/FFT>
    //#include <unsupported/Eigen/SparseExtra>
    #include <Eigen/Geometry>

    /** \brief The only namespace used by Lemma
     *
     *  \details The rational behind this namespace is that built-in
     *  types should be used wherever possible, but not
     *  not built-in names. This allows for code that is better
     *  enacsulated and easier to modify. The typedefs and constants
     *  specified here are defined so that
     *  precision/inplimentation can easily be changed.
     *  All floating precision types should be typedefed in this file
     *  and should not be used natively within any code.
     *  Lemma uses
     *  the Eigen Matrix/Vector/Linear Algebra library.
     *  <http://eigen.tuxfamily.org> and a lot of the namespece typedefs
     *  are specifying Eigen types.
     */
    namespace Lemma {

        /// Real defines precision for the whole API, default is double
#ifdef  LEMMA_SINGLE_PRECISION
        typedef float Real;
#else      // -----      LEMMA_SINGLE_PRECISION  -----
        typedef double Real;
#endif     // -----  not LEMMA_SINGLE_PRECISION  -----

        /// Complex version of Real.
        typedef std::complex<Real> Complex;

        /// A 3 component Eigen vector of Reals
        typedef Eigen::Matrix<Real, 3, 1> Vector3r;

        /// A 3 X Dynamic Component Eigen matrix of Reals
        typedef Eigen::Matrix<Real, 3, Eigen::Dynamic> Vector3Xr;

        /// Variable length Eigen vector of Reals
        typedef Eigen::Matrix<Real, Eigen::Dynamic, 1> VectorXr;

        /// Variable length Eigen vector of integers (int)
        typedef Eigen::Matrix<int, Eigen::Dynamic, 1> VectorXi;

        /// Variable length Eigen vector of Complexes
        typedef Eigen::Matrix<Complex, Eigen::Dynamic, 1> VectorXcr;

        /// A 3 Component Eigen vector of Complexes
        typedef Eigen::Matrix<Complex, 3, 1> Vector3cr;

        /// A 3 X Dynamic Component Eigen matrix of Complexes
        typedef Eigen::Matrix<Complex, 3, Eigen::Dynamic> Vector3Xcr;

        /// Variable length Eigen Matrix of Reals
        typedef Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic> MatrixXr;

        /// Variable length Eigen Matrix of ints
        typedef Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> MatrixXi;

        /// Variable length Eigen vector of Complexes
        typedef Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic> MatrixXcr;

        ////////////////////////////////////////
        // Constants used across the programmes

        /// Restating the obvious, this is pi
        const Real PI = 4.0*atan(1.0);

        /// Permitivity of Free Space
        //const Real EPSILON0 = 8.854187817e-12;
        const Real EPSILON0 = 8.854187817e-12;

        /// Permeability of free space
        const Real MU0 = 4.*PI*1e-7;

        /// 1/4 of \f$ \pi\f$
        const Real QPI = .25/PI;

        /// Some functions will convert units from SI (standard) to Gauss
        /// This is because NMR calculations are much more natural in Gauss
        enum MAGUNITS  {TESLA, NANOTESLA, GAUSS};

        /// Unit of temperature entered
        enum TEMPUNITS {CELCIUS, KELVIN};

        /// Unit of time entered
        enum TIMEUNITS {SEC, MILLISEC, MICROSEC, NANOSEC, PICOSEC};

        /// Unit of time entered
        enum FREQUENCYUNITS {HZ, KHZ, MHZ, GHZ};

		/// FEM coil relative orientations
		enum FEMCOILORIENTATION {COAXIAL, COPLANAR};

		/// General orientation relative to coordinate system
		enum ORIENTATION {X, Y, Z, NX, NY, NZ};

        /// Type of field
        enum FIELDTYPE         {HFIELDREAL, HFIELDIMAG, EFIELDREAL, EFIELDIMAG};

        /// Compenent of vector field
        enum FIELDCOMPONENT    {XCOMPONENT=0, YCOMPONENT=1, ZCOMPONENT=2};

        /// Spatial component of vector
        enum SPATIALCOORDINANT  {XCOORD=0, YCOORD=1, ZCOORD=2};

        /** Evaluation method for Hankel integrals.
         *  ANDERSON801     Walt Anderson's 801 point filter
         *  CHAVE           Alan Chave's gaussian quadrature integration method
         *  FHTKEY201       Key's 201 point filter
         *  FHTKEY201       Key's 101 point filter
         *  FHTKEY51        Key's 51 point filter
         *  QWEKEY          Key's Gaussian quadrature integration method
         */
        enum HANKELTRANSFORMTYPE { ANDERSON801, CHAVE, FHTKEY201, FHTKEY101, FHTKEY51, QWEKEY,
                                    FHTKONG61, FHTKONG121, FHTKONG241, IRONS };

        /** Enum is OK because these are the only physically possible sources.
         @param NOSOURCETYPE is default.
         @param ELECTRICDIPOLE is an electric dipole
         @param MAGNETICDIPOLE is a magnetic dipole
        */
        enum DipoleSourceType {NOSOURCETYPE, GROUNDEDELECTRICDIPOLE, UNGROUNDEDELECTRICDIPOLE, MAGNETICDIPOLE};

        /// Only three polarizations are supported. They may be summed to
        /// approximate others
        /// @param NOPOLARISATION is uninitialized, default value
        /// @param XPOLARISATION is a dipole oriented in the x direction
        /// @param YPOLARISATION is a dipole oriented in the y direction
        /// @param ZPOLARISATION is a dipole oriented in the z direction
        enum DipoleSourcePolarisation {NOPOLARISATION, XPOLARISATION,
            YPOLARISATION,  ZPOLARISATION};

        /// The polarity may be either negative or positinve
        enum DipoleSourcePolarity {NEGATIVE, POSITIVE};

        /** The fields to make calculations on
        */
        enum FIELDCALCULATIONS {E, H, BOTH};

        /** Windowing function type
         */
        enum WINDOWTYPE { HAMMING, /*!< A hamming window */
                          HANNING, /*!< A hanning window */
                          RECTANGULAR /*!< Rectangular window */
        };


    }

#endif // __Lemma_H
