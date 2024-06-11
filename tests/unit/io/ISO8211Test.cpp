//
// Test Suite for geos::io::ISO8211 class.

#include <tut/tut.hpp>
#include <utility.h>
// geos
#include <geos/geom/Polygon.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/Dimension.h>
#include <geos/geom/Envelope.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/Point.h>
#include <geos/geom/PrecisionModel.h>
#include <geos/io/GeoJSONReader.h>
#include <geos/io/GeoJSONWriter.h>
#include <geos/util/IllegalArgumentException.h>
#include <geos/util.h>

#include <geos/io/iso8211/iso8211.h>

// std
// #include <dirent.h>
#include <cmath>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>

using geos::geom::CoordinateSequence;

namespace tut {
    //
    // Test Group
    //

    // Common data used by tests
    struct test_io_iso8211 {
        // Typedefs used as short names by test cases
        typedef std::unique_ptr<geos::geom::Geometry> GeometryAutoPtr;
        typedef std::unique_ptr<geos::geom::Polygon> PolygonAutoPtr;
        typedef geos::geom::GeometryFactory GeometryFactory;

        using Geometry = geos::geom::Geometry;
        using Envelope = geos::geom::Envelope;
        using Polygon = geos::geom::Polygon;

        geos::geom::PrecisionModel pm;
        GeometryFactory::Ptr factory;
        geos::io::GeoJSONReader geojsonreader;
        geos::io::GeoJSONWriter geojsonwriter;

        std::string logFile;
        std::ofstream destFile;

        test_io_iso8211()
            : pm(1.0)
            , factory(GeometryFactory::create(&pm, 0))
            , geojsonreader(*(factory.get()))
        {
            logFile = "D://geos//s57//log.txt";
            destFile.open(logFile, std::ios_base::app);
        }

        void writeLine(std::string line) {
            if (destFile.is_open()) {
                destFile << line;
            }
        }

        /* **********************************************************************/
        /*                          ViewRecordField()                           */
        /*                                                                      */
        /*      Dump the contents of a field instance in a record.              */
        /* **********************************************************************/

        void ViewRecordField(DDFField* poField)

        {
            DDFFieldDefn* poFieldDefn = poField->GetFieldDefn();

            // Get pointer to this fields raw data.  We will move through
            // it consuming data as we report subfield values.

            const char* pachFieldData = poField->GetData();
            int nBytesRemaining = poField->GetDataSize();

            /* -------------------------------------------------------- */
            /*      Loop over the repeat count for this fields          */
            /*      subfields.  The repeat count will almost            */
            /*      always be one.                                      */
            /* -------------------------------------------------------- */

            int nRepeatCount = poField->GetRepeatCount();

            writeLine("repeat count:" + std::to_string(nRepeatCount));
            writeLine("\n");

            for (int iRepeat = 0; iRepeat < nRepeatCount; iRepeat++)
            {

                /* -------------------------------------------------------- */
                /*   Loop over all the subfields of this field, advancing   */
                /*   the data pointer as we consume data.                   */
                /* -------------------------------------------------------- */
                int subfieldCount = poFieldDefn->GetSubfieldCount();
                for (int iSF = 0; iSF < subfieldCount; iSF++)
                {
                    DDFSubfieldDefn* poSFDefn = poFieldDefn->GetSubfield(iSF);

                    std::string fieldName = "sub-field-name:";
                    std::string line = "     sub-index:" + std::to_string(iSF) + fieldName + poSFDefn->GetName();
                    writeLine(line);

                    int nBytesConsumed =
                        ViewSubfield(poSFDefn, pachFieldData, nBytesRemaining);

                    nBytesRemaining -= nBytesConsumed;
                    pachFieldData += nBytesConsumed;
                }
                writeLine("\n");
            }
        }

        /* **********************************************************************/
        /*                            ViewSubfield()                            */
        /* **********************************************************************/

        int ViewSubfield(DDFSubfieldDefn* poSFDefn, const char* pachFieldData, int nBytesRemaining)

        {
            int nBytesConsumed = 0;
            int intResult;
            double doubleResult;
            std::string stringResult;

            switch (poSFDefn->GetType())
            {
            case DDFInt:
                intResult = poSFDefn->ExtractIntData(pachFieldData, nBytesRemaining, &nBytesConsumed);
                writeLine(std::to_string(intResult));
                break;

            case DDFFloat:
                doubleResult = poSFDefn->ExtractFloatData(pachFieldData, nBytesRemaining, &nBytesConsumed);
                writeLine(std::to_string(doubleResult));
                break;

            case DDFString:
                stringResult = poSFDefn->ExtractStringData(pachFieldData, nBytesRemaining, &nBytesConsumed);
                writeLine(stringResult);
                break;

            default:
                break;
            }

            return nBytesConsumed;
        }


    };

    typedef test_group<test_io_iso8211> group;
    typedef group::object object;

    group test_io_iso8211_group("geos::io::iso8211::DDFRecord");

    //
    // Test Cases
    //

    // Test of Aree Buffer
    template<>
    template<>
    void object::test<1>
        ()
    {
        std::string s57Path = "D://geos//s57//US2WC05M//US2WC05M.000";
        struct stat sb;

        if (stat(s57Path.c_str(), &sb) == 0) {
            std::fstream inputFile(s57Path);
            std::string geojson = std::string((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

            DDFModule oModule;
            oModule.Open(s57Path.c_str());

            DDFRecord* poRecord = nullptr;
            int nRecordCount = 0;
            int nFieldCount = 0;

            while ((poRecord = oModule.ReadRecord()) != nullptr)
            {
                /* ------------------------------------------------------------ */
                /*      Loop over each field in this particular record.         */
                /* ------------------------------------------------------------ */
                int count = poRecord->GetFieldCount();
                for (int iField = 0; iField < count; iField++)
                {
                    DDFField* poField = poRecord->GetField(iField);

                    std::string fieldName = "field-name:";
                    std::string line = "index   " + std::to_string(iField)  + fieldName + poField->GetFieldDefn()->GetName();
                    writeLine(line);
                    writeLine("\n");

                    ViewRecordField(poField);

                    nFieldCount++;
                }
                
                nRecordCount++;
            }

            oModule.Close();

            inputFile.clear();
            inputFile.close();
        }
    }

} // namespace tut
