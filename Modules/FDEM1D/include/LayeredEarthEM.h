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

#ifndef __LAYEREDEARTHEM_H
#define __LAYEREDEARTHEM_H

#include "LayeredEarth.h"

namespace Lemma {

    // =======================================================================
    //        Class:  LayeredEarthEM
    /// \ingroup FDEM1D
    /// \brief  1D layered earth. Attributes include all aspects of
    /// Cole-Cole model.
    /// \details   Primarily used for EM calculations.
    /** @todo Layer 0 can be set right now, but all logic ignores and assumes
              air layer. This is surprising to users and constitutes a bug.
     */
    // =======================================================================
    class LayeredEarthEM : public LayeredEarth {

        // ====================    FRIENDS     ===========================
        /** Recursively streams information about this class */
        friend std::ostream &operator<<(std::ostream &stream, const LayeredEarthEM &ob);

        public:

            // ====================  LIFECYCLE     ===========================

            /** Default locked constructor. */
            explicit LayeredEarthEM ( const ctor_key& );

            /** Locked deserializing constructor. */
			LayeredEarthEM ( const YAML::Node& node, const ctor_key& );

            /** Default destructor */
            virtual ~LayeredEarthEM ();

            /**
             *  Factory method for generating concrete class.
             *  @return a std::shared_ptr of type LayeredEarthEM
             */
            static std::shared_ptr<LayeredEarthEM> NewSP();

            /** YAML Serializing method
             */
            YAML::Node Serialize() const;

            /**
             *   Constructs an object from a YAML::Node.
             */
            static std::shared_ptr< LayeredEarthEM > DeSerialize(const YAML::Node& node);

            /**
             *   Constructs an object from a string representation of a YAML::Node. This is primarily
             *   used in Python wrapping
             */
            static std::shared_ptr< LayeredEarthEM > DeSerialize( const std::string& node ) {
                return LayeredEarthEM::DeSerialize(YAML::LoadFile(node));
            }

            /** @return a deep copy
             */
			std::shared_ptr<LayeredEarthEM> Clone();

            // ====================  OPERATORS     ===========================

            // ====================  OPERATIONS    ===========================

            /** Evaluates the cole-cole model for suceptability and
             * permeability.
             * @param[in] omega is the frequency in Hz
             */
            void EvaluateColeColeModel(const Real& omega);

            // ====================  ACCESS        ===========================

            /** Sets the number of layers and resizes all model parameters to
             *  be the correct size, finally initialises all values to free
             *  space.
             *  @param[in] nlay is the number of layers
             */
            virtual void SetNumberOfLayers(const int& nlay);

            /** Sets Complex Conductivity of the layers
             *  @param[in] sigma is a vector of the conducivity of the layers
             */
            void SetLayerConductivity(const VectorXcr &sigma);

            /** Sets Complex Conductivity of a single layer
             *  @param[in] sigma is  the conducivity of the layer
             *  @param[in] ilay is  the layer index
             */
            void SetLayerConductivity(const int& ilay, const Complex& sigma);

            /* Sets Complex resitivity of the layers
             *  @param[in] ohm is a vector of the resistivity of the layers
                Singularity at 0 resistivity in air. Need to reformaulat things a bit
             */
            //void SetLayerResistivity(const VectorXcr &ohm);

            /** Sets the thickness of the layers
             *  @param[in] thick is a vector of the thicknesses of the layers.
             *  Must be two less than the total number of Layers as the top and
             *  bottom layers are assumed infinite.
             */
            void SetLayerThickness(const VectorXr &thick);

            /** Sets the High frequency susceptibility
             */
            void SetLayerHighFreqSusceptibility(const VectorXr& sushi);

            /** Sets the Low frequency susceptibility
             */
            void SetLayerLowFreqSusceptibility(const VectorXr& sushi);

            /** Sets the Layer breath susceptibility
             */
            void SetLayerBreathSusceptibility(const VectorXr& susbr);

            /** Sets the Layer tau susceptibility
             */
            void SetLayerTauSusceptibility(const VectorXr& susta);

            /** Sets the High frequency permitivity
             */
            void SetLayerHighFreqPermitivity(const VectorXr& sushi);

            /** Sets the Low frequency permitivity
             */
            void SetLayerLowFreqPermitivity(const VectorXr& sushi);

            /** Sets the Layer breath permitivity
             */
            void SetLayerBreathPermitivity(const VectorXr& oerbr);

            /** Sets the Layer breath permitivity
             */
            void SetLayerTauPermitivity(const VectorXr& oerbr);

            // ====================  INQUIRY       ===========================

            /** Returns the thickness of a layer
                @return a VectorXcr of the layer conductivities.
             */
            VectorXcr GetLayerConductivity( );

            /** Returns the conductivity of a layer
             *  @param[in] ilay is the layer indice of interest
             *  @return the complex valued conductivity of the layer of
             *  interest.
             */
            Complex GetLayerConductivity(const int &ilay);

            /** Returns the susceptibility of a layer
             *  @param[in] ilay is the layer indice of interest.
             *  @return a Complex value of the electrical susceptibility of the
             *  probes layer.
             */
            Complex GetLayerSusceptibility(const int &ilay);

            /** Returns the Susceptibility of Cole-Cole model
             *  @return a VectorXcr of the suceptibilities of the model.
             */
            VectorXcr GetLayerSusceptibility( );

            /** Returns the low freq susceptibility of a layer
             */
            Real GetLayerLowFreqSusceptibility(const int &ilay);

            /** Returns the low freq susceptibility of a layer
             */
            VectorXr GetLayerLowFreqSusceptibility( );

            /** Returns the low freq susceptibility of a layer
             */
            Real GetLayerHighFreqSusceptibility(const int &ilay);

            /** Returns the low freq susceptibility of a layer
             */
            VectorXr GetLayerHighFreqSusceptibility( );

            /** Returns the low freq susceptibility of a layer
             */
            Real GetLayerTauSusceptibility(const int &ilay);

            /** Returns the low freq susceptibility of a layer
             */
            VectorXr GetLayerTauSusceptibility( );

            /** Returns the low freq susceptibility of a layer
             */
            Real GetLayerBreathSusceptibility(const int &ilay);

            /** Returns the low freq susceptibility of a layer
             */
            VectorXr GetLayerBreathSusceptibility( );

            /** Returns the Relative Permitivity of the cole cole model
             */
            VectorXcr GetLayerPermitivity( );

            /** Returns the conductivity of a layer
             *  @param[in] ilay is the layer indice of interest.
             *  @return a Complex value of the electrical permitivity of the
             *  probed layer.
             */
            Complex GetLayerPermitivity(const int &ilay);

            /** Low Freq permivitity
             */
            VectorXr  GetLayerLowFreqPermitivity( );

            /** Returns the low freq susceptibility of a layer
             */
            Real GetLayerLowFreqPermitivity(const int &ilay);

            /** Returns the low freq susceptibility of a layer
             */
            Real GetLayerHighFreqPermitivity(const int &ilay);

            /** Returns the low freq susceptibility of a layer
             */
            VectorXr GetLayerHighFreqPermitivity( );

            /** Returns the low freq susceptibility of a layer
             */
            Real GetLayerTauPermitivity(const int &ilay);

            /** Returns the low freq susceptibility of a layer
             */
            VectorXr GetLayerTauPermitivity( );

            /** Returns the low freq susceptibility of a layer
             */
            Real GetLayerBreathPermitivity(const int &ilay);

            /** Returns the low freq susceptibility of a layer
             */
            VectorXr GetLayerBreathPermitivity( );

            /** Returns the name of the underlying class, similiar to Python's type */
            virtual std::string GetName() const ;

        protected:


        private:
            // ====================  DATA MEMBERS  ===========================

            /** Vector of layer Conductivity */
            VectorXcr         LayerConductivity;

            /** Vector of layer Susceptibility (chi)
             *  This is calculated using the Cole-Cole model
             *  \f[ sus(i) = sush(i)
             *   + \frac{(susl(i)-sush(i))}  {(1+(jw * sustau(i))^{susalp(i)})} \f]
             */
            VectorXcr         LayerSusceptibility;

            /** Vector of low frequency susceptibility, for use in Cole-Cole
              model */
            VectorXr          LayerLowFreqSusceptibility;

            /** Vector of high frequency susceptibility, for use in Cole-Cole
             model */
            VectorXr          LayerHighFreqSusceptibility;

            /** Vector of relaxation time constants, for use in Cole-Cole model
             */
            VectorXr          LayerTauSusceptibility;

            /** Vector relaxation breath, for use in Cole-Cole model
             */
            VectorXr          LayerBreathSusceptibility;

            /** Vector of Layer Permitivity (epsilon)
             *  Calculated using the Cole-Cole model
             *  \f[ epr(i) = eprh(i)
             *     + \frac{ (eprl(i)-eprh(i))}  { (1+(jw*eprtau(i))^{epralp(i)})} \f]
             */
            VectorXcr         LayerPermitivity;

            /** Low frequency permitivity for each layer */
            VectorXr          LayerLowFreqPermitivity;

            /** High frequency permitivity for each layer */
            VectorXr          LayerHighFreqPermitivity;

            /** Decay time constant for each layer */
            VectorXr          LayerTauPermitivity;

            /** Relaxation breath for each layer */
            VectorXr          LayerBreathPermitivity;

            /** ASCII string representation of the class name */
            static constexpr auto CName = "LayeredEarthEM";

    }; // -----  end of class  LayeredEarthEM  -----


} // namespace Lemma

#endif // __LAYEREDEARTHEM_H

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */
