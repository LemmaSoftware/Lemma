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

PYBIND11_MODULE(LemmaCore, m) {

    py::add_ostream_redirect(m, "ostream_redirect");

    m.doc() = "Python binding of LemmaCore, additional details can be found at https://lemmasoftware.org";

    py::class_<Lemma::RectilinearGrid, std::shared_ptr<Lemma::RectilinearGrid> > RectilinearGrid(m, "RectilinearGrid");

        // lifecycle
        RectilinearGrid.def(py::init(&Lemma::RectilinearGrid::NewSP))
        .def_static("DeSerialize", py::overload_cast<const std::string&>(&Lemma::RectilinearGrid::DeSerialize), "Construct object from yaml representation")

        // print
        .def("__repr__", &Lemma::RectilinearGrid::Print)
        .def("Serialize", &Lemma::RectilinearGrid::Print, "YAML representation of the class")

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

        // modifiers
        .def("SetDimensions", &Lemma::RectilinearGrid::SetDimensions, "Sets the number of cells in x, y, and z")
        .def("SetOffset", &Lemma::RectilinearGrid::SetOffset, "Sets the origin offset in x, y, and z")
        .def("SetSpacing", &Lemma::RectilinearGrid::SetSpacing, "Sets the grid spacing in x, y, and z")
    ;

    py::class_<Lemma::ASCIIParser, std::shared_ptr<Lemma::ASCIIParser> > ASCIIParser(m, "ASCIIParser");

        // lifecycle
        ASCIIParser.def(py::init(&Lemma::ASCIIParser::NewSP))
        .def_static("DeSerialize", py::overload_cast<const std::string&>(&Lemma::ASCIIParser::DeSerialize), "Construct object from yaml representation")

        // print
        .def("__repr__", &Lemma::ASCIIParser::Print)
        .def("Serialize", &Lemma::ASCIIParser::Print, "YAML representation of the class")

        // accessors
        .def("GetName", &Lemma::ASCIIParser::GetName, "Returns the name of the class")
        .def("GetFileLocation", &Lemma::ASCIIParser::GetFileLocation, "Returns the current file location")

        // modifiers
        .def("SetCommentString", &Lemma::ASCIIParser::SetCommentString, "Sets the comment string after which all text is ignored")
        .def("SetBufferSize", &Lemma::ASCIIParser::SetBufferSize, "Sets the buffer size")

        // methods
        .def("Open", &Lemma::ASCIIParser::Open, "Opens file specified by argument")
        .def("Close", &Lemma::ASCIIParser::Close, "Closes current file object")
        .def("ReadReals", &Lemma::ASCIIParser::ReadReals, "Returns vector of nr reals")
        .def("ReadInts", &Lemma::ASCIIParser::ReadInts, "Returns vector of ni ints")
        .def("ReadStrings", &Lemma::ASCIIParser::ReadStrings, "Returns vector of ns strings")
        .def("JumpToLocation", &Lemma::ASCIIParser::JumpToLocation, "File object jumps to specified location")

    ;

    py::class_<Lemma::CubicSplineInterpolator, std::shared_ptr<Lemma::CubicSplineInterpolator> >(m, "CubicSplineInterpolator")

        // lifecycle
        .def(py::init(&Lemma::CubicSplineInterpolator::NewSP))
        .def_static("DeSerialize", py::overload_cast<const std::string&>(&Lemma::CubicSplineInterpolator::DeSerialize), "Construct object from yaml representation")

        // print
        .def("__repr__", &Lemma::CubicSplineInterpolator::Print)
        .def("Serialize", &Lemma::CubicSplineInterpolator::Print, "YAML representation of the class")

        // accessors
        .def("GetName", &Lemma::CubicSplineInterpolator::GetName, "Returns the name of the class")
        .def("GetKnotAbscissa", &Lemma::CubicSplineInterpolator::GetKnotAbscissa, "Returns the knot abscissa values")
        .def("GetKnotOrdinate", &Lemma::CubicSplineInterpolator::GetKnotOrdinate, "Returns the knot ordinate values")

        // modifiers
        .def("SetKnots", &Lemma::CubicSplineInterpolator::SetKnots, "Sets the knots to use for interpolation")
        .def("ResetKnotOrdinate", &Lemma::CubicSplineInterpolator::ResetKnotOrdinate, "Resets the knots to use for interpolation, when abscissa values haven't changed")

        // methods
        .def("InterpolateOrderedSet", &Lemma::CubicSplineInterpolator::InterpolateOrderedSet, "Interpolate a monotonically increasing ordered set.")
        .def("Integrate", py::overload_cast<const Lemma::Real&, const Lemma::Real& >(&Lemma::CubicSplineInterpolator::Integrate), "Integrates between the arguments using cubic spline values.")
        .def("IntegrateN", py::overload_cast<const Lemma::Real&, const Lemma::Real&, const int& >(&Lemma::CubicSplineInterpolator::Integrate), "Integrates the spline from x0 to x1. Uses composite Simpson's rule and n is the number of segments")
        .def("Interpolate", py::overload_cast<const Lemma::Real& >(&Lemma::CubicSplineInterpolator::Interpolate), "Interpolation at a single point, x is the interpolation abscissa point, returns the ordinate value at x")
        .def("InterpolateI", py::overload_cast<const Lemma::Real&, int& >(&Lemma::CubicSplineInterpolator::Interpolate), "Interpolation at a single point, x is the interpolation abscissa point and i is the knot to begin searchin at returns the ordinate value at x")

    ;

    // ABC
    //py::class_<Lemma::EarthModel, std::shared_ptr<Lemma::EarthModel> >(m, "EarthModel")

    /*
    py::class_<Lemma::LayeredEarth, std::shared_ptr<Lemma::LayeredEarth> >(m, "LayeredEarth")

        // lifecycle
        .def(py::init(&Lemma::LayeredEarth::NewSP))
        //.def_static("DeSerialize", py::overload_cast<const std::string&>(&Lemma::LayeredEarth::DeSerialize), "Construct object from yaml representation")

        // print
        .def("__repr__", &Lemma::LayeredEarth::Print)
        .def("Serialize", &Lemma::LayeredEarth::Print, "YAML representation of the class")

    ;
    */
}

