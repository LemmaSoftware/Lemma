/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     10/28/2010
  @version  $Id: rectilineargrid.h 193 2014-11-10 23:51:41Z tirons $
 **/

#ifndef  RECTILINEARGRID_INC
#define  RECTILINEARGRID_INC

#include "Grid.h"

namespace Lemma {

    // ===================================================================
    //  Class:  RectilinearGrid
    /**
      @class
      \ingroup LemmaCore
      \brief   Impliments a rectilinear grid.
      \details A rectilinear grid can vary regularily in space but must be
               constant variation across each dimension. In this way three
               vectors can define the entire grid, and each cell is right
               aligned with all its neighbours.
     */
    // ===================================================================
    class RectilinearGrid : public Grid {

        friend std::ostream &operator<<(std::ostream &stream,
			const RectilinearGrid &ob);

        public:

            // ====================  LIFECYCLE     =======================

            /**
             *  Factory method for generating concrete class.
             *  @return a std::shared_ptr of type RectilinearGrid
             */
            static std::shared_ptr<RectilinearGrid> NewSP();


            /**
             *  Uses YAML to serialize this object.
             *  @return a YAML::Node
             */
            virtual YAML::Node Serialize() const;

            /**
             *   Constructs an object from a YAML::Node.
             */
            static std::shared_ptr< RectilinearGrid > DeSerialize(const YAML::Node& node);

            // ====================  OPERATORS     =======================

            // ====================  OPERATIONS    =======================

            /**
             *  Sets the dimensions in x, y and z
             *  @param[in] nx is the number of cells in x
             *  @param[in] ny is the number of cells in y
             *  @param[in] nz is the number of cells in z
             */
            void SetDimensions (const int &nx, const int &ny, const int &nz);

            /**
             *  Sets the offset in x, y and z. This sets the corner position of the top SW corner.
             *  @param[in] ox is the number of cells in x
             *  @param[in] oy is the number of cells in y
             *  @param[in] oz is the number of cells in z
             */
            void SetOffset (const Real &ox, const Real &oy, const Real &oz);

            /**
             *  Sets the spacing in x, y and z
             *  @param[in] dx is a Vector of cell spacing in x, must be same
             *  size ad nx
             *  @param[in] dy is a Vector of cell spacing in x, must be same
             *  size ad ny
             *  @param[in] dz is a Vector of cell spacing in x, must be same
             *  size ad nz
             */
            void SetSpacing (const VectorXr &dx, const VectorXr &dy,
                const VectorXr &dz);


            // ====================  ACCESS        =======================

            /** Returns the number of cells in x
             *  @return number of cells in x
             */
            int GetNx();

            /** Returns the number of cells in y
             *  @return number of cells in y
             */
            int GetNy();

            /** Returns the number of cells in z
             *  @return number of cells in z
             */
            int GetNz();

            /** Returns the offset of cells in x
             *  @return offset of cells in x
             */
            Real GetOx();

            /** Returns the number of cells in y
             *  @return number of cells in y
             */
            Real GetOy();

            /** Returns the offset of cells in z
             *  @return offset of cells in z
             */
            Real GetOz();

            /** Returns Spacing Vector
             *  @return dx
             */
            VectorXr GetDx();

            /** Returns a spacing
             *  @return dx[ix]
             */
            Real GetDx(const int& ix);

            /** Returns Spacing Vector
             *  @return dy
             */
            VectorXr GetDy();

            /** Returns a spacing
             *  @return dy[iy]
             */
            Real GetDy(const int& iy);

            /** Returns Spacing Vector
             *  @return dz
             */
            VectorXr GetDz();

            /** Returns Spacing Vector
             *  @return dz[iz]
             */
            Real GetDz(const int& iz);

            // ====================  INQUIRY       =======================

            /** Returns the name of the underlying class, similiar to Python's type */
            virtual inline std::string GetName() const {
                return this->CName;
            }

        protected:

            // ====================  LIFECYCLE     =======================

            /** Default protected constructor. */
            RectilinearGrid ( );

            /** Protected DeDerializing constructor, use factory DeSerialize  method*/
            RectilinearGrid (const YAML::Node& node);

            /** Default protected constructor. */
            ~RectilinearGrid ();

            // ====================  DATA MEMBERS  =========================

            /// Number of cells in the x dimension
            int nx;

            /// Number of cells in the y dimension
            int ny;

            /// Number of cells in the z dimension
            int nz;

            /// Grid offset in x dimension
            Real ox;

            /// Grid offset in y dimension
            Real oy;

            /// Grid offset in z dimension
            Real oz;

            /// Cell spacing in the x dimension
            VectorXr dx;

            /// Cell spacing in the y dimension
            VectorXr dy;

            /// Cell spacing in the z dimension
            VectorXr dz;

        private:

            /** ASCII string representation of the class name */
            static constexpr auto CName = "RectilinearGrid";

    }; // -----  end of class  RectilinearGrid  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef RECTILINEARGRID_INC  -----

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */
