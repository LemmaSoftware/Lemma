/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      10/02/2014 03:21:07 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#include "helper.h"

namespace Lemma {

std::string enum2String(const FREQUENCYUNITS& FreqUnits) {
    std::string t;
    switch (FreqUnits) {
        case HZ:
            t = std::string("HZ");
            break;
        case KHZ:
            t = std::string("KHZ");
            break;
        case MHZ:
            t = std::string("MHZ");
            break;
        case GHZ:
            t = std::string("GHZ");
            break;
    }
    return t;
}

std::string enum2String(const TIMEUNITS& Units) {
    std::string t;
    switch (Units) {
        case SEC:
            t = std::string("SEC");
            break;
        case MILLISEC:
            t = std::string("MILLISEC");
            break;
        case MICROSEC:
            t = std::string("MICROSEC");
            break;
        case NANOSEC:
            t = std::string("NANOSEC");
            break;
        case PICOSEC:
            t = std::string("PICOSEC");
            break;
    }
    return t;
}


std::string enum2String(const MAGUNITS& Units) {
    std::string t;
    switch (Units) {
        case TESLA:
            t = std::string("TESLA");
            break;
        case NANOTESLA:
            t = std::string("NANOTESLA");
            break;
        case GAUSS:
            t = std::string("GAUSS");
            break;
    }
    return t;
}

std::string enum2String(const FIELDCALCULATIONS& Field) {
    std::string t;
    switch (Field) {
        case E:
            t = std::string("E");
            break;
        case H:
            t = std::string("H");
            break;
        case BOTH:
            t = std::string("BOTH");
            break;
    }
    return t;
}


std::string enum2String(const TEMPUNITS& Units) {
    std::string t;
    switch (Units) {
        case CELCIUS:
            t = std::string("CELCIUS");
            break;
        case KELVIN:
            t = std::string("KELVIN");
            break;
    }
    return t;
}

std::string enum2String(const FIELDCOMPONENT& Comp) {
    std::string t;
    switch (Comp) {
        case XCOMPONENT:
            t = std::string("XCOMPONENT");
            break;
        case YCOMPONENT:
            t = std::string("YCOMPONENT");
            break;
        case ZCOMPONENT:
            t = std::string("ZCOMPONENT");
            break;
    }
    return t;
}

std::string enum2String(const HANKELTRANSFORMTYPE& Type) {
    std::string t;
    switch (Type) {
        case ANDERSON801:
            t = std::string("ANDERSON801");
            break;
        case CHAVE:
            t = std::string("CHAVE");
            break;
        case QWEKEY:
            t = std::string("QWEKEY");
            break;
        case FHTKEY201:
            t = std::string("FHTKEY201");
            break;
        case FHTKEY101:
            t = std::string("FHTKEY101");
            break;
        case FHTKEY51:
            t = std::string("FHTKEY51");
            break;
        case FHTKONG61:
            t = std::string("FHTKONG61");
            break;
        case FHTKONG121:
            t = std::string("FHTKONG121");
            break;
        case FHTKONG241:
            t = std::string("FHTKONG241");
            break;
        case IRONS:
            t = std::string("IRONS");
            break;

    }
    return t;
}

std::string enum2String( const WINDOWTYPE& Type ) {
    std::string t;
    switch (Type) {
        case HAMMING:
            return std::string("HAMMING");
        case HANNING:
            return  std::string("HANNING");
        case RECTANGULAR:
            return std::string("RECTANGULAR");
        default:
            throw( std::runtime_error( "In enum2String WINDOWTYPE, type not identified" ) );
    }
}

template<>
FREQUENCYUNITS string2Enum<FREQUENCYUNITS>( const std::string& str ) {
    if       (str ==  "HZ") return   HZ;
    else if  (str == "KHZ") return  KHZ;
    else if  (str == "MHZ") return  MHZ;
    else if  (str == "GHZ") return  GHZ;
    else {
        throw std::runtime_error("string not recognized as FREUENCYUNIT");
    }
}

template<>
HANKELTRANSFORMTYPE string2Enum<HANKELTRANSFORMTYPE>( const std::string& str ) {
    if       (str ==  "ANDERSON801") return   ANDERSON801;
    else if  (str == "CHAVE") return  CHAVE;
    else if  (str == "QWEKEY") return  QWEKEY;
    else if  (str == "FHTKEY201") return  FHTKEY201;
    else if  (str == "FHTKEY51") return  FHTKEY51;
    else if  (str == "FHTKEY101") return  FHTKEY101;
    else if  (str == "FHTKONG61") return  FHTKONG61;
    else if  (str == "FHTKONG121") return  FHTKONG121;
    else if  (str == "FHTKONG241") return  FHTKONG241;
    else if  (str == "IRONS") return  IRONS;
    else {
        throw std::runtime_error("string not recognized as HANKELTRANSFORMTYPE");
    }
}


template<>
TIMEUNITS string2Enum<TIMEUNITS>( const std::string& str ) {
    if       (str == "SEC")      return      SEC;
    else if  (str == "MILLISEC") return MILLISEC;
    else if  (str == "MICROSEC") return MICROSEC;
    else if  (str == "NANOSEC")  return  NANOSEC;
    else if  (str == "PICOSEC")  return  PICOSEC;
    else {
        throw std::runtime_error("string not recognized as TIMEUNIT");
    }
}

template<>
FIELDCOMPONENT string2Enum<FIELDCOMPONENT>( const std::string& str) {
    if       (str == "XCOMPONENT")      return XCOMPONENT;
    else if  (str == "YCOMPONENT")      return YCOMPONENT;
    else if  (str == "ZCOMPONENT")      return ZCOMPONENT;
    else {
        throw std::runtime_error("string not recognized as FieldComponent");
    }
}

template<>
WINDOWTYPE string2Enum<WINDOWTYPE>( const std::string& str ) {
    if      (str == "HAMMING")      return HAMMING;
    if      (str == "HANNING")      return HANNING;
    if      (str == "RECTANGULAR")  return RECTANGULAR;
    else {
        throw std::runtime_error("string not recognized as WindowType");
    }
}

}		// -----  end of Lemma  name  -----

