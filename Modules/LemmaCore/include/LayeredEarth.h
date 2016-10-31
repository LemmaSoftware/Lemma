/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     06/24/2009
  @version   0.0
 **/

#ifndef __LAYEREDEARTH_H
#define __LAYEREDEARTH_H

#include "EarthModel.h"

namespace Lemma {

	const int MAXLAYERS	= 10;

	// =======================================================================
	//        Class:  LayeredEarth
    /// \ingroup LemmaCore
	/// \brief Abstract 1D layered earth.
	// =======================================================================
	class LayeredEarth : public EarthModel {

        friend class LayeredEarthEM;

		public:

			// ====================    FRIENDS     ===========================
			/** Recursively prints out information about this class
             */
            friend std::ostream &operator<<(std::ostream &stream,
						const LayeredEarth &ob);

			// ====================  LIFECYCLE     ===========================

			// ====================  OPERATORS     ===========================

			// ====================  OPERATIONS    ===========================

			// ====================  ACCESS        ===========================

			/** Sets the number of layers and resizes all model parameters to
			 *  be the correct size, finally initialises all values to free
			 *  space.
             */
			virtual void SetNumberOfLayers(const int& nlay)=0;

			/** Sets the thickness of the layers
             */
			void SetLayerThickness(const VectorXr &thick);

			// ====================  INQUIRY       ===========================

			/** Returns the number of layers in the model */
			int GetNumberOfLayers( );

			/** Returns the number of layers in the model, excluding the air
			 * layer
             */
			int GetNumberOfNonAirLayers( );

			/** Returns the layer that a depth is in.
			 * @param[in] depth is the depth you desire to know the layer of
			 * @return the indice of the layer that depth is in.
             */
			int GetLayerAtThisDepth(const Real& depth);

			/** Returns the thickness of a layer
             *  @param[in] lay is the layer indice
             *  @return the thickness of a layer
             */
			Real GetLayerThickness(const int & lay);

            /** Returns the thickness of a layer
             *  @param[in] lay is the layer indice
             *  @return the thickness of all intermediary (non-infinite) layers
             */
			VectorXr GetLayerThickness( );

			/** Returns the depth of the bottom interface of a layer
             *  @param[in] lay is the layer indice
             *  @return the depth of the bottom interface of a layer
             */
			Real GetLayerDepth(const int & lay);

            /** YAML Serializing method
             */
            YAML::Node Serialize() const;

            /** Returns the name of the underlying class, similiar to Python's type */
            virtual inline std::string GetName() const {
                return this->CName;
            }

		protected:

			// ====================  LIFECYCLE     ===========================

			/** Default protected constructor. */
			LayeredEarth ( );

            /** Default protected constructor. */
			LayeredEarth (const YAML::Node& node);

			/** Default protected constructor. */
			~LayeredEarth ();

			// ====================  DATA MEMBERS  ===========================

        private:

            /** ASCII string representation of the class name */
            static constexpr auto CName = "LayeredEarth";

            /** no copy */
            LayeredEarth ( const LayeredEarth& ) = delete;

			/** Number of layers in the model, including the air layer,
			 * and counting from 0
             */
			int               NumberOfLayers;

			/** Number of interfaces in the model. Equal to NumberOfLayers - 1
			 */
            int               NumberOfInterfaces;

			/** Vector of layer thicknesses */
			VectorXr          LayerThickness;

	}; // -----  end of class  LayeredEarth  -----


    ///////////////////////////////////////////////////////////////////
    // Error classes

    /** If a model with less than two layers is specified, throw error. Not set up
     * for whole space solutions.
     */
    class EarthModelWithLessThanTwoLayers : public std::runtime_error {
        public:
            EarthModelWithLessThanTwoLayers( );
    };

    /** If a solver has a max number of layers, and this is exceeded, throw this
     * error.
     */
    class EarthModelWithMoreThanMaxLayers : public std::runtime_error {
        public:
            /** Thrown when an Earth model with more than the maximum number of
             *  Layers is given
             */
            EarthModelWithMoreThanMaxLayers( );
    };

    /** If the model parameters have different lengths than the earth model, throw
     * this error.
     */
    class EarthModelParametersDoNotMatchNumberOfLayers : public std::runtime_error {
        public:
            /** Thrown when the parameters do not match the number of Layers
             */
            EarthModelParametersDoNotMatchNumberOfLayers ( );
    };

    /** If a request if made for a non-valid earth model parameter, throw this
     * error.
     */
    class RequestForNonValidEarthModelParameter : public std::runtime_error {
        public:
            /** Thrown when a request is made for a non-valid earth parameter
             */
            RequestForNonValidEarthModelParameter ( );
    };

} // namespace Lemma

#endif // __LAYEREDEARTH_H
