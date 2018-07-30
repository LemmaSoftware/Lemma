/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      07/28/18 18:04:54
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@LemmaSoftware.org
 * @copyright Copyright (c) 2018, University of Utah
 * @copyright Copyright (c) 2018, Lemma Software, LLC
 */

#pragma once

#ifndef  RECTILINEARGRIDVTKIMPORTER_INC
#define  RECTILINEARGRIDVTKIMPORTER_INC

#ifdef  LEMMAUSEVTK

#pragma once
#include "LemmaObject.h"
#include "RectilinearGrid.h"

#include "vtkSmartPointer.h"
#include "vtkRectilinearGrid.h"
#include "vtkDataArray.h"

namespace Lemma {

    /**
     * \ingroup LemmaCore
     * \brief
     * \details
     */
    class RectilinearGridVTKImporter : public LemmaObject {

        friend std::ostream &operator<<(std::ostream &stream, const RectilinearGridVTKImporter &ob);

        protected:
        /*
         *  This key is used to lock the constructor. It is protected so that inhereted
         *  classes also have the key to contruct their base class.
         */
        struct ctor_key {};

        public:

        // ====================  LIFECYCLE     =======================

        /**
         * Default constructor.
         * @note This method is locked, and cannot be called directly.
         *       The reason that the method is public is to enable the use
         *       of make_shared whilst enforcing the use of shared_ptr,
         *       in c++-17, this curiosity may be resolved.
         * @see RectilinearGridVTKImporter::NewSP
         */
        explicit RectilinearGridVTKImporter ( const ctor_key& );

        /**
         * DeSerializing constructor.
         * @note This method is locked, and cannot be called directly.
         *       The reason that the method is public is to enable the use
         *       of make_shared whilst enforcing the use of shared_ptr,
         *       in c++-17, this curiosity may be resolved.
         * @see RectilinearGridVTKImporter::DeSerialize
         */
        RectilinearGridVTKImporter ( const YAML::Node& node, const ctor_key& );

        /**
         * Default destructor.
         * @note This method should never be called due to the mandated
         *       use of smart pointers. It is necessary to keep the method
         *       public in order to allow for the use of the more efficient
         *       make_shared constructor.
         */
        virtual ~RectilinearGridVTKImporter ();

        /**
         *  Uses YAML to serialize this object.
         *  @return a YAML::Node
         *  @see RectilinearGridVTKImporter::DeSerialize
         */
        virtual YAML::Node Serialize() const;

        /*
         *  Factory method for generating concrete class.
         *  @return a std::shared_ptr of type RectilinearGridVTKImporter
         */
        static std::shared_ptr< RectilinearGridVTKImporter > NewSP();

        /**
         *   Constructs an RectilinearGridVTKImporter object from a YAML::Node.
         *   @see RectilinearGridVTKImporter::Serialize
         */
        static std::shared_ptr<RectilinearGridVTKImporter> DeSerialize(const YAML::Node& node);

        // ====================  OPERATORS     =======================

        // ====================  OPERATIONS    =======================

        // ====================  ACCESS        =======================

        /**
         *  Sets the name of the VTK file to convert
         */
        void SetVTKInput( vtkSmartPointer<vtkRectilinearGrid> vgrid );

        /**
         *  Performs the actual grid conversion
         *  @param[in] xshift is additional offset to apply in x direction.
         *  @param[in] yshift is additional offset to apply in y direction.
         *  @param[in] zshift is additional offset to apply in z direction.
         */
        void ConvertGrid( const Real& xshift, const Real& yshift, const Real& zshift );

        /**
         *  @return smart pointer to RectilinearGrid class
         */
        std::shared_ptr<RectilinearGrid> GetGrid() {
            return this->rGrid;
        }

        // ====================  INQUIRY       =======================
        /**
         *  Returns the name of the underlying class, similiar to Python's type
         *  @return string of class name
         */
        virtual inline std::string GetName() const {
            return CName;
        }

        protected:

        // ====================  LIFECYCLE     =======================

        /** Copy is disabled */
        RectilinearGridVTKImporter( const RectilinearGridVTKImporter& ) = delete;

        // ====================  DATA MEMBERS  =========================

        private:

        /** ASCII string representation of the class name */
        static constexpr auto CName = "RectilinearGridVTKImporter";

        /** VTK file to import */
        vtkSmartPointer<vtkRectilinearGrid> vtkGrid;

        /** container to hold imported grid */
        std::shared_ptr<RectilinearGrid>  rGrid = nullptr;

    }; // -----  end of class  RectilinearGridVTKImporter  -----

}  // -----  end of namespace Lemma ----

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */


#endif      // -----  LEMMAUSEVTK  -----

#endif   // ----- #ifndef RECTILINEARGRIDVTKIMPORTER_INC  -----

