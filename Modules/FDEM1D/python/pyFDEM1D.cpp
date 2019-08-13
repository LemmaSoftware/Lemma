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
#include "FDEM1D"

namespace py = pybind11;

PYBIND11_MODULE(pyFDEM1D, m) {

    py::add_ostream_redirect(m, "ostream_redirect");

    m.doc() = "Python binding of Lemma::FDEM1D, additional details can be found at https://lemmasoftware.org";


    py::class_<Lemma::WireAntenna, std::shared_ptr<Lemma::WireAntenna> > WireAntenna(m, "WireAntenna"); //, py::dynamic_attr());

        // lifecycle
        WireAntenna.def(py::init(&Lemma::WireAntenna::NewSP))

        // print
        .def("__repr__", &Lemma::WireAntenna::Print)
        .def("Serialize", &Lemma::WireAntenna::Print, "YAML representation of the class")
        .def_static("DeSerialize", py::overload_cast<const std::string&>(&Lemma::WireAntenna::DeSerialize), "Construct object from yaml representation")

        // modifiers
        .def("SetNumberOfPoints", &Lemma::WireAntenna::SetNumberOfPoints, "Sets the number of points comprising the antenna")
        .def("SetPoint", py::overload_cast<const int&, const Lemma::Real&, const Lemma::Real&, const Lemma::Real&>(&Lemma::WireAntenna::SetPoint), "Sets a point in the antenna")
        .def("SetPoint", py::overload_cast<const int&, const Lemma::Vector3r&>(&Lemma::WireAntenna::SetPoint), "Sets a point in the antenna")
        .def("SetNumberOfTurns", &Lemma::WireAntenna::SetNumberOfTurns, "Sets the number of turns of the antenna")
        .def("SetNumberOfFrequencies", &Lemma::WireAntenna::SetNumberOfFrequencies, "Sets the number of frequencies of the transmitter")
        .def("SetFrequency", &Lemma::WireAntenna::SetFrequency, "Sets a single frequency of the transmitter")
        .def("SetCurrent", &Lemma::WireAntenna::SetCurrent, "Sets the current of the transmitter in amps")

        // accessors
        .def("GetCurrent", &Lemma::WireAntenna::GetCurrent, "Returns the current of the transmitter in amps")
        .def("GetPoints", &Lemma::WireAntenna::GetPoints, "Returns the points defining the transmitter")
        .def("GetNumberOfDipoles", &Lemma::WireAntenna::GetNumberOfDipoles, "Returns the number of dipoles defining the transmitter")
        .def("GetNumberOfFrequencies", &Lemma::WireAntenna::GetNumberOfFrequencies, "Returns the number of frequencies for the transmitter")
        .def("IsHorizontallyPlanar", &Lemma::WireAntenna::IsHorizontallyPlanar, "Returns true if the transmitter is flat")
        .def("GetName", &Lemma::WireAntenna::GetName, "Returns the class name of the object")

        // operations
        .def("ApproximateWithElectricDipoles", &Lemma::WireAntenna::ApproximateWithElectricDipoles, "Approximates loop with electric dipoles")

    ;

    py::class_<Lemma::PolygonalWireAntenna, std::shared_ptr<Lemma::PolygonalWireAntenna> > PolygonalWireAntenna(m, "PolygonalWireAntenna", WireAntenna); //, py::dynamic_attr())

        // lifecycle
        PolygonalWireAntenna.def(py::init(&Lemma::PolygonalWireAntenna::NewSP))

        // print
        .def("__repr__", &Lemma::PolygonalWireAntenna::Print)
        .def("Serialize", &Lemma::PolygonalWireAntenna::Print, "YAML representation of the class")

        // accessors
        .def("GetName", &Lemma::PolygonalWireAntenna::GetName, "Returns the name of the class")

        // modifiers
        //.def("SetNumberOfPoints", &Lemma::WireAntenna::SetNumberOfPoints, "Sets the number of points comprising the antenna")

        // operations
        .def("ApproximateWithElectricDipoles", &Lemma::PolygonalWireAntenna::ApproximateWithElectricDipoles, "Approximates loop with series of electric dipoles around loop")

    ;

}


/*
BOOST_PYTHON_MODULE(pyLemmaCore)
{
    Py_Initialize();
    np::initialize();

 	bp::class_<Lemma::PolygonalWireAntenna, boost::noncopyable> ("PolygonalWireAntenna", boost::python::no_init)

        // print
        .def(boost::python::self_ns::str(bp::self))

        // Lifecycle
        .def("__init__", boost::python::make_constructor(&Lemma::PolygonalWireAntenna::NewSP))
        //.def("Serialize", &Lemma::PolygonalWireAntenna::Serialize)
        //.def("DeSerialize", &Lemma::PolygonalWireAntenna::DeSerialize)

        // Accessors
    ;
}
*/
