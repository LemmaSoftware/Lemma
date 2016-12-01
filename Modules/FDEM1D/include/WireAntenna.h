/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     12/16/2009
  @version  $Id: WireAntenna.h 201 2015-01-03 00:07:47Z tirons $
 **/

#ifndef __WIREANTENNA_H
#define __WIREANTENNA_H

#include "DipoleSource.h"

#ifdef LEMMAUSEVTK
#include "vtkActor.h"
#endif

namespace Lemma {


    // ===================================================================
    /**      Class:  WireAntenna
      * \brief   Class representing a wire antennae.
      * \details This is an abstract class.
	  */
    // ===================================================================
    class WireAntenna : public LemmaObject {

        friend std::ostream &operator<<(std::ostream &stream, const WireAntenna &ob);

        protected:
        struct ctor_key {};

        public:

            // ====================  LIFECYCLE     =======================

            /** Locked default constructor */
            explicit WireAntenna ( const ctor_key& );

            /** Locked deserializing constructor */
            WireAntenna ( const YAML::Node& node, const ctor_key& );

            /** Destructor */
            virtual ~WireAntenna();

            /**
             * Initialises antenna to contain no points, with no current
             * and no frequency. NumberOfTurns set to 1
             */
            static std::shared_ptr<WireAntenna> NewSP();

            /**
             * Provides deep copy
             */
            virtual std::shared_ptr<WireAntenna> Clone() const ;

            /**
             *  Uses YAML to serialize this object.
             *  @return a YAML::Node
             */
            YAML::Node Serialize() const;

            /**
             *   Constructs an object from a YAML::Node.
             */
            static std::shared_ptr<WireAntenna> DeSerialize( const YAML::Node& node );

            // ====================  OPERATORS     =======================

            // ====================  OPERATIONS    =======================

            /**
              * Approximates with evenly spaced electric dipoles around loop
              * @param[in] delta is the spacing between moments
			  */
            virtual void ApproximateWithElectricDipoles(const Real &delta);

            // ====================  ACCESS        =======================

			/**
              * Sets the number of turns in the loop, default is 1
              * @param[in] nturns is the number of turns of the loop.
			  */
            void SetNumberOfTurns(const int &nturns);

            /**
              * Sets the number of points in the loop
			  */
            void SetNumberOfPoints(const int &np);

			/**
              * Sets a corner point of the loop. Counting starts at 0. It
              * is not required to have the loop be closed right now, but
              * its a good idea.
			  */
            void SetPoint(const int &p, const Vector3r &pos);

            /**
              * Sets a corner point of the loop. Counting starts at 0. It
              * is not required to have the loop be closed right now, but
              * its a good idea.
			  */
            void SetPoint(const int &p, const Real&x, const Real& y, const Real& z);

            /** Sets the frequency of the current in the loop, default is 0
			  * @param[in] ifreq is the frequency number to set
              * @param[in] freq is the frequency (Hz) of the current
			  */
            void SetFrequency(const int& ifreq, const Real &freq);
			/**
              * Sets the number of frequencies.
              * @param[in] nfreq is the number of frequencies that will be
              * computed.
			  */
            void SetNumberOfFrequencies(const int& nfreq);

			/**
              * Sets the current in the loop, default is 0
              * @param[in] amp is the current in the loop, in Amperes
			  */
            void SetCurrent(const Real &amp);

            // ====================  INQUIRY       =======================

			/**
              * Returns the number of turns
              * @return the number of turns of the loop.
			  */
            int GetNumberOfTurns( );

            /**
             * Returns all of the points
             */
            Vector3Xr GetPoints();

            /**
             * Returns all of the points in a general matrix, useful for python wrapper
             */
            MatrixXr GetPointsMat();

			/**
              * Returns the frequency in the loop.
              * @return is the frequency in the loop in Hz
			  */
            Real GetFrequency(const int& ifreq);


			/**
              * Returns the current in the loop.
              * @return is the Current
			  */
            Real GetCurrent( );


			/**
              * Returns pointer to a dipole source
			  */
            std::shared_ptr<DipoleSource> GetDipoleSource(const int &dip);

			/**
              * returns number of dipoles used to approximate this
              * loop
			  */
            int GetNumberOfDipoles();

			/**
              * @return the number of frequencies of this wire loop.
			  */
            int GetNumberOfFrequencies();

            #ifdef LEMMAUSEVTK
            /** Returns an actor that can be placed into a vtk scene easily
              * Note that this function throws a pointer, it is the
              * receivers job to manage this memory!
			  */
            vtkActor* GetVtkActor(const int &idip);
            #endif

            /** Returns true or false if loop is horizontally planar
             */
            bool IsHorizontallyPlanar();

            /** Returns the name of the underlying class, similiar to Python's type */
            virtual std::string GetName() const;

        protected:

            // ====================  DATA MEMBERS  =======================

			/**
              * Number of points that define the loop
			  */
            int NumberOfPoints;

			/**
              * Current in antennae (Amps)
			  */
            Real Current;

			/**
              * Number of turns
			  */
            int NumberOfTurns;

			/**
              * List of the dipoles
			  */
            std::vector< std::shared_ptr<DipoleSource> > Dipoles;

			/**
              * Points that define this loop
			  */
            Vector3Xr                            Points;

			/**
              * Frequencies of the loop.
			  */
            VectorXr                             Freqs;

        private:

            static constexpr auto CName = "WireAntenna";

    }; // -----  end of class  WireAntenna  -----

}
#endif // __WIREANTENNA_H
