/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      04/03/2014 01:08:28 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#include "Lemma"
#ifdef TINYXMLSUPPORT
#include "tinyxml2.h"

#include <ctime>
using namespace tinyxml2;
using namespace Lemma;

void ParseTEM( XMLElement* element );

template <typename T>
std::vector<T> ReadLine( const int& nr, const std::string& line ) ;

int main(int argc, char **argv) {

    if ( argc > 1 ) {
		XMLDocument* doc = new XMLDocument();
		clock_t startTime = clock();
		doc->LoadFile( argv[1] );
		clock_t loadTime = clock();
		int errorID = doc->ErrorID();

        // Get some info
        //static const char* xml = "<element/>";
        //doc->Parse( xml );
        //XMLNode* titleElement = doc->FirstChild(); //Element()->FirstChildElement( "PLAY" )->FirstChildElement( "TITLE" );
        XMLElement* titleElement = doc->FirstChildElement( ); //->FirstChildElement( "TITLE" );
        //const char* title = titleElement->Value(); //GetText();
        const char* title = titleElement->Value(); //GetText();

        printf( "Test file '%s' loaded. ErrorID=%d\n", argv[1], errorID );
        printf( "Name of FirstChildElement: %s\n", title );

        if (strcmp ( titleElement->Value(), "NGXFile" ) != 0) {
            std::cout <<"NGX file was not detected!" << std::endl;
            exit(EXIT_FAILURE);
        } else {
            //std::cout <<
            //titleElement->FirstAttribute() ;//"type");// << std::endl;;
            if (strcmp(  titleElement->FindAttribute("type")->Value(),"tem") == 0) {
                // Parse TEM file
                ParseTEM( titleElement );
            } else {
                std::cout << "No parser for NGX filetype " << titleElement->FindAttribute("type")->Value() << std::endl;
                std::cout << "YOU should write one!\n";
                exit(EXIT_FAILURE);
            }

        }

		delete doc; doc = 0;

        clock_t deleteTime = clock();
		if ( !errorID ) {
			printf( "========================================================\n");
			printf( "Load time=%u\n",   (unsigned)(loadTime - startTime) );
			printf( "Delete time=%u\n", (unsigned)(deleteTime - loadTime) );
			printf( "Total time=%u\n",  (unsigned)(deleteTime - startTime) );
		}


		exit(0);
	} else {
        std::cout << "Enter NGX.x file to parse\n";
    }
}

void ParseTEM( XMLElement* element ) {

    std::cout << "This is a NGX.t file" << std::endl;


    if (strcmp( element->FindAttribute("version")->Value(),"0.1") == 0) {
        std::cout << "Version 0.1 detected" << std::endl;
    } else {
        std::cout << "Unsupported NGX.t version" << std::endl;
    }

    // system description
    std::cout << "System description" << std::endl;
    std::cout << element->FirstChildElement("system")->FirstChildElement("name")->GetText() << std::endl;
    XMLElement* system = element->FirstChildElement("system");
        int ntx = atoi(system->FirstChildElement("transmitters")->FindAttribute("num")->Value());
        std::cout << "numTx = " << ntx << std::endl;
        // OK Parse Transmitter(s)
        XMLElement* tx = system->FirstChildElement("transmitters")->FirstChildElement("transmitter");
        //    std::cout << "Tx id = " << tx->FindAttribute("id")->Value() << std::endl;
        //std::cout << tx->FirstChildElement("loop.coordinates")->FindAttribute("npoint")->Value();//->FirstChildElement("northingPoints")->Value() << std::endl;
        //std::cout << tx->FirstChildElement("loop.coordinates")->FirstChildElement("northing.points")->GetText();//->FirstChildElement("northingPoints")->Value() << std::endl;
        for (int it=0; it<ntx; ++it) {
            std::cout << "\nTransmitter " << it << std::endl;
            if (it > 0) tx = tx->NextSiblingElement();
            int npoints = atoi(tx->FirstChildElement("loop.coordinates")->FindAttribute("npoint")->Value());

            std::cout << "points= " << npoints; // tx->FirstChildElement("loop.coordinates")->FindAttribute("npoint")->Value();//->FirstChildElement("northingPoints")->Value() << std::endl;
            std::vector<Real> LP = ReadLine<Real>( npoints, std::string(tx->FirstChildElement("loop.coordinates")->FirstChildElement("northing.points")->GetText()) );
            for (int ii=0; ii<npoints; ++ii) std::cout << "\t"<< LP[ii] ;
            //std::cout << tx->FirstChildElement("loop.coordinates")->FirstChildElement("northing.points")->GetText();//->FirstChildElement("northingPoints")->Value() << std::endl;
            //std::cout << tx->FirstChildElement("loop.coordinates")->FirstChildElement("easting.points")->GetText();//->FirstChildElement("northingPoints")->Value() << std::endl;
        }
    std::cout << "\nEND system description\n";

}


template <typename T>
std::vector<T> ReadLine( const int& nr, const std::string& line ) {

    std::vector<T>   lineData;
    std::stringstream  lineStream(line);

    T value;
    while(lineStream >> value) {
        lineData.push_back(value);
    }
    return lineData;
}
#else

int main() {
    std::cout << "you have to compile lemma with external tinyxml library to use this" << std::endl;
}

#endif

    //--------------------------------------------------------------------------------------
    //       Class:  ASCIIParser
    //      Method:  ReadInts
    //--------------------------------------------------------------------------------------
//     std::vector<int> ReadInts ( const int& nr ) {
//         std::string buf;
//         char *dump = new char[BufferSize];
//         std::vector<int> vals(0);
//         while (input >> buf) {
//             if (buf.substr(0, CommentString.size()) == CommentString) {
//                 input.getline(dump, BufferSize);
//             } else {
//                 vals.push_back( atoi(buf.c_str() ));
//             }
//             if (static_cast<int>(vals.size()) == nr) {
//                 delete [] dump;
//                 return vals;
//             }
//
//         }
//         delete [] dump;
//         return vals;
//     }		// -----  end of method ASCIIParser::ReadInts  -----

