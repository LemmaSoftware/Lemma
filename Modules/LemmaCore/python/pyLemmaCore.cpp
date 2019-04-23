/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      22/04/19 14:06:32
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@utah.edu
 * @copyright Copyright (c) 2019, University of Utah
 * @copyright Copyright (c) 2019, Lemma Software, LLC
 */

#include <pybind11/pybind11.h>
#include <pybind11/iostream.h>
#include <pybind11/eigen.h>
#include "LemmaCore"

namespace py = pybind11;

PYBIND11_MODULE(pyLemmaCore, m) {

    py::add_ostream_redirect(m, "ostream_redirect");

    m.doc() = "Python binding of LemmaCore, additional details can be found at https://lemmasoftware.org";

    py::class_<Lemma::RectilinearGrid, std::shared_ptr<Lemma::RectilinearGrid> >(m, "RectilinearGrid")

        // lifecycle
        .def(py::init(&Lemma::RectilinearGrid::NewSP))

        // print
        .def("__repr__", &Lemma::RectilinearGrid::Print)
        .def("Serialize", &Lemma::RectilinearGrid::Print)

        // accessors
        .def("GetName", &Lemma::RectilinearGrid::GetName, "Returns the name of the class")
        .def("GetNx", &Lemma::RectilinearGrid::GetNx, "Returns the number of cells in the x direction")
        .def("GetNy", &Lemma::RectilinearGrid::GetNy, "Returns the number of cells in the y direction")
        .def("GetNz", &Lemma::RectilinearGrid::GetNz, "Returns the number of cells in the z direction")
        .def("GetOx", &Lemma::RectilinearGrid::GetOx, "Returns the grid origin offset in the x direction")
        .def("GetOy", &Lemma::RectilinearGrid::GetOy, "Returns the grid origin offset in the y direction")
        .def("GetOz", &Lemma::RectilinearGrid::GetOz, "Returns the grid origin offset in the z direction")
        .def("GetDx", &Lemma::RectilinearGrid::GetDx, "Returns the grid spacing in the x direction")
        .def("GetDy", &Lemma::RectilinearGrid::GetDy, "Returns the grid spacing in the y direction")
        .def("GetDz", &Lemma::RectilinearGrid::GetDz, "Returns the grid spacing in the z direction")

        .def("SetDimensions", &Lemma::RectilinearGrid::SetDimensions, "Sets the number of cells in x, y, and z")
        .def("SetOffset", &Lemma::RectilinearGrid::SetOffset, "Sets the origin offset in x, y, and z")
        .def("SetSpacing", &Lemma::RectilinearGrid::SetSpacing, "Sets the grid spacing in x, y, and z");
}


/*
BOOST_PYTHON_MODULE(pyLemmaCore)
{
    Py_Initialize();
    np::initialize();

 	bp::class_<Lemma::RectilinearGrid, boost::noncopyable> ("RectilinearGrid", boost::python::no_init)

        // print
        .def(boost::python::self_ns::str(bp::self))

        // Lifecycle
        .def("__init__", boost::python::make_constructor(&Lemma::RectilinearGrid::NewSP))
        //.def("Serialize", &Lemma::RectilinearGrid::Serialize)
        //.def("DeSerialize", &Lemma::RectilinearGrid::DeSerialize)

        // Accessors



    ;

}
*/
