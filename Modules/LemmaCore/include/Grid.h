/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     10/25/2009
  @version  $Id: grid.h 193 2014-11-10 23:51:41Z tirons $
 **/

#ifndef __GRID_H
#define __GRID_H

#include "LemmaObject.h"

namespace Lemma {

// ===============================================================================
//        Class:  Grid
/// \ingroup LemmaCore
/// \brief Abstract base class for grids.
/// \details Grids define values in space, they may either be an integral part
/// of models, or simply the output of a forward modelling procedure.
// ===============================================================================

class Grid : public LemmaObject {

    friend std::ostream &operator<<(std::ostream &stream,
            const Grid &ob);

    public:

        // ====================  LIFECYCLE     ===================================

        virtual YAML::Node Serialize() const;

        // ====================  OPERATORS     ===================================

        // ====================  OPERATIONS    ===================================

        // ====================  ACCESS        ===================================

        // ====================  INQUIRY       ===================================

        /** Returns the name of the underlying class, similiar to Python's type */
        virtual std::string GetName() const;

    protected:

        // ====================  LIFECYCLE     ===================================

        /** Protected DeDerializing constructor, use factory DeSerialize  method on non abstract classes*/
        Grid (const YAML::Node& node, const ctor_key& );

        /// Default protected constructor.
        Grid ( const ctor_key& );

        /// Default protected constructor.
        virtual ~Grid ( );

        // ====================  DATA MEMBERS  ===================================

    private:

        /** ASCII string representation of the class name */
        static constexpr auto CName = "Grid";

}; // -----  end of class  Grid  -----

} // namespace Lemma

#endif // __GRID_H

