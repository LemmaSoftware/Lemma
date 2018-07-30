/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      07/28/18 18:05:24
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@utah.edu
 * @copyright Copyright (c) 2018, University of Utah
 * @copyright Copyright (c) 2018, Lemma Software, LLC
 */

#include "RectilinearGridVTKImporter.h"

namespace Lemma {

    // ====================  FRIEND METHODS  =====================

    std::ostream &operator << (std::ostream &stream, const RectilinearGridVTKImporter &ob) {
        stream << ob.Serialize()  << "\n";
        return stream;
    }

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGridVTKImporter
    //      Method:  RectilinearGridVTKImporter
    // Description:  constructor (locked)
    //--------------------------------------------------------------------------------------
    RectilinearGridVTKImporter::RectilinearGridVTKImporter (const ctor_key&) : LemmaObject( LemmaObject::ctor_key()  ) {

    }  // -----  end of method RectilinearGridVTKImporter::RectilinearGridVTKImporter  (constructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGridVTKImporter
    //      Method:  RectilinearGridVTKImporter
    // Description:  DeSerializing constructor (locked)
    //--------------------------------------------------------------------------------------
    RectilinearGridVTKImporter::RectilinearGridVTKImporter (const YAML::Node& node, const ctor_key&) : LemmaObject(node, LemmaObject::ctor_key()) {

    }  // -----  end of method RectilinearGridVTKImporter::RectilinearGridVTKImporter  (constructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGridVTKImporter
    //      Method:  NewSP()
    // Description:  public constructor returing a shared_ptr
    //--------------------------------------------------------------------------------------
    std::shared_ptr< RectilinearGridVTKImporter >  RectilinearGridVTKImporter::NewSP() {
        return std::make_shared< RectilinearGridVTKImporter >( ctor_key() );
    }

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGridVTKImporter
    //      Method:  ~RectilinearGridVTKImporter
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    RectilinearGridVTKImporter::~RectilinearGridVTKImporter () {

    }  // -----  end of method RectilinearGridVTKImporter::~RectilinearGridVTKImporter  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGridVTKImporter
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node  RectilinearGridVTKImporter::Serialize (  ) const {
        YAML::Node node = LemmaObject::Serialize();
        node.SetTag( GetName() );
        // FILL IN CLASS SPECIFICS HERE
        return node;
    }		// -----  end of method RectilinearGridVTKImporter::Serialize  -----

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGridVTKImporter
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    std::shared_ptr<RectilinearGridVTKImporter> RectilinearGridVTKImporter::DeSerialize ( const YAML::Node& node  ) {
        if (node.Tag() !=  "RectilinearGridVTKImporter" ) {
            throw  DeSerializeTypeMismatch( "RectilinearGridVTKImporter", node.Tag());
        }
        return std::make_shared< RectilinearGridVTKImporter > ( node, ctor_key() );
    }		// -----  end of method RectilinearGridVTKImporter::DeSerialize  -----


    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGridVTKImporter
    //      Method:  set_SetVTKInput
    //--------------------------------------------------------------------------------------
    void RectilinearGridVTKImporter::SetVTKInput ( vtkSmartPointer<vtkRectilinearGrid> vgrid  ) {
        vtkGrid	= vgrid;

        return ;
    }		// -----  end of method RectilinearGridVTKImporter::set_SetVTKInput  -----


    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGridVTKImporter
    //      Method:  ConvertGrid
    //--------------------------------------------------------------------------------------
    void RectilinearGridVTKImporter::ConvertGrid ( const Real& shiftx, const Real& shifty, const Real& shiftz  ) {

        rGrid = RectilinearGrid::NewSP();

        int dims[3];
        vtkGrid->GetDimensions( dims );
        auto xcoords = vtkGrid->GetXCoordinates(  );
        auto ycoords = vtkGrid->GetYCoordinates(  );
        auto zcoords = vtkGrid->GetZCoordinates(  );

        VectorXr dx = VectorXr(dims[0]-1);
        VectorXr dy = VectorXr(dims[1]-1);
        VectorXr dz = VectorXr(dims[2]-1);

        for (int ix=1; ix<dims[0]; ++ix) {
            dx(ix-1) = xcoords->GetTuple1(ix) - xcoords->GetTuple1(ix-1);
        }
        for (int iy=1; iy<dims[1]; ++iy) {
            dy(iy-1) = ycoords->GetTuple1(iy) - ycoords->GetTuple1(iy-1);
        }
        for (int iz=1; iz<dims[2]; ++iz) {
            dz(iz-1) = zcoords->GetTuple1(iz) - zcoords->GetTuple1(iz-1);
        }

        rGrid->SetDimensions( dims[0], dims[1], dims[2] );
        rGrid->SetOffset( xcoords->GetTuple1(0)+shiftx, ycoords->GetTuple1(0)+shifty, zcoords->GetTuple1(0)+shiftz );
        rGrid->SetSpacing( dx, dy, dz  );

        return ;
    }		// -----  end of method RectilinearGridVTKImporter::ConvertGrid  -----


} // ----  end of namespace Lemma  ----

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */

