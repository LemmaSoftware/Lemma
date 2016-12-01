/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      09/25/2013 08:20:01 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2013, XRI Geophysics, LLC
 * @copyright Copyright (c) 2013, Trevor Irons
 */


#ifndef  RECTILINEARGRIDVTKEXPORTER_INC
#define  RECTILINEARGRIDVTKEXPORTER_INC

#ifdef  LEMMAUSEVTK

#include "LemmaObject.h"
#include "RectilinearGrid.h"

#include <vtkSmartPointer.h>
#include "vtkXMLRectilinearGridWriter.h"
#include "vtkRectilinearGrid.h"
#include "vtkDoubleArray.h"
#include "vtkFieldData.h"
#include "vtkCellData.h"

namespace Lemma {

/**
  \ingroup LemmaCore
  \brief   Exports a LemmaRectilinearGrid into a VTKRectilinearGrid.
  \details Results can then be dumped to a VTK file or used directly.
 */
class RectilinearGridVTKExporter : public LemmaObject {

    friend std::ostream &operator<<(std::ostream &stream,
            const RectilinearGridVTKExporter &ob);

    struct ctor_key {};

    public:

    // ====================  LIFECYCLE     =======================

    /**
     *  Factory method for generating concrete class.
     *  @return a std::shared_ptr of type RectilinearGridVTKExporter
     */
    static std::shared_ptr< RectilinearGridVTKExporter > NewSP();

    /** Default constructor */
    explicit RectilinearGridVTKExporter ( const ctor_key& );

    /** Deserializing constructor */
    explicit RectilinearGridVTKExporter ( const YAML::Node& node, const ctor_key& );

    /** Default protected destructor, use Delete */
    virtual ~RectilinearGridVTKExporter ();

    /**
     *  Uses YAML to serialize this object.
     *  @return a YAML::Node
     */
    virtual YAML::Node Serialize() const;

    /**
     *   Constructs an object from a YAML::Node.
     */
    static std::shared_ptr< RectilinearGridVTKExporter > DeSerialize(const YAML::Node& node);

    // ====================  OPERATORS     =======================

    // ====================  OPERATIONS    =======================

    // ====================  ACCESS        =======================
    /**
     * Sets the Grid to be exported
     */
    void SetGrid( std::shared_ptr<RectilinearGrid> Grid);

    // ====================  INQUIRY       =======================

    /**
     *  @return Returns a pointer to a vtkRectilinearGrid containing same information as the Lemma Grid.
     */
    vtkSmartPointer<vtkRectilinearGrid>  GetVTKGrid();

    /**
     *  Writes the exported VTKGrid out to a .vtr XML file
     *  @param[in] name is the filename to write to. The .vtr extension will be added
     */
    void WriteVTKGrid( const std::string& name );

    /** Returns the name of the underlying class, similiar to Python's type */
    virtual inline std::string GetName() const;

    protected:

    // ====================  LIFECYCLE     =======================

    private:

    /** ASCII string representation of the class name */
    static constexpr auto CName = "RectilinearGridVTKExporter";

    void BuildVTKRectilinearGrid();

    // ====================  DATA MEMBERS  =========================

    /** The Grid to convert from */
    std::shared_ptr<RectilinearGrid>    Grid;

    /** The Grid to convert to */
    vtkSmartPointer<vtkRectilinearGrid> VTKGrid;

}; // -----  end of class  RectilinearGridVTKExporter  -----

}		// -----  end of Lemma  name  -----

#endif     // -----  not LEMMAUSEVTK  -----

#endif   // ----- #ifndef RECTILINEARGRIDVTKEXPORTER_INC  -----

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp syntax=cpp.doxygen: */
