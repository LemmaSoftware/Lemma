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

#include "LemmaCore"
#include <string>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
// Include the headers of MyLib

//using namespace boost::python;

namespace bp = boost::python;
namespace np = boost::python::numpy;

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
        .def("GetName", &Lemma::RectilinearGrid::GetName)
        .def("GetNx", &Lemma::RectilinearGrid::GetNx)
        .def("GetNy", &Lemma::RectilinearGrid::GetNy)
        .def("GetNz", &Lemma::RectilinearGrid::GetNz)
        .def("GetOx", &Lemma::RectilinearGrid::GetOx)
        .def("GetOy", &Lemma::RectilinearGrid::GetOy)
        .def("GetOz", &Lemma::RectilinearGrid::GetOz)
        //.def("GetDx", &Lemma::RectilinearGrid::GetDx)
        //.def("GetDy", &Lemma::RectilinearGrid::GetDy)
        //.def("GetDz", &Lemma::RectilinearGrid::GetDz)

        .def("SetDimensions", &Lemma::RectilinearGrid::SetDimensions)
        .def("SetOffset", &Lemma::RectilinearGrid::SetOffset)
        .def("SetSpacing", &Lemma::RectilinearGrid::SetSpacing)

    ;

}
