//
// Test Suite for geos::geom::Graphic class.

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

#include <geos/geom/Graphic.h>
#include <geos/geom/graphic/GraphicLayer.h>

// std
#include <dirent.h>
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
struct test_geom_graphic {
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

    test_geom_graphic()
        : pm(1.0)
        , factory(GeometryFactory::create(&pm, 0))
        , geojsonreader(*(factory.get()))
    {
    
    }

    void dealMIPARE(const std::string& input, const std::string& output, double scale)
    {
        geos::geom::GraphicLayer* layer1 = new geos::geom::GraphicLayer();
        layer1
            ->setInputLayer(input + "Polygon_MIPARE.geojson")
            ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::Self)
            ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingBuffer)
            ->setScale(scale)
            ->excute()
            ->makeOutputLayer(output + "Polygon_MIPARE.geojson")
            ->clear()
            ->setInputLayer(input + "Polygon_MIPARE.geojson")
            ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::Self)
            ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingBufferExteriorRing)
            ->setScale(scale)
            ->excute()
            ->makeOutputLayer(output + "Polygon_Outline_MIPARE.geojson");

        delete layer1;
    }

    void dealLNDARE(const std::string& input, const std::string& output, double scale)
    {
        geos::geom::GraphicLayer* layer = new geos::geom::GraphicLayer();
        layer
            ->setInputLayer(input + "Polygon_LNDARE.geojson")
            ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::Self)
            ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingBuffer)
            ->setScale(scale)
            ->excute()
            ->makeOutputLayer(output + "Polygon_LNDARE.geojson");

        delete layer;
    }

    void dealTSEZNE(const std::string& input, const std::string& output, double scale)
    {
        geos::geom::GraphicLayer* layer = new geos::geom::GraphicLayer();
        layer
            ->setInputLayer(input + "/Polygon_TSEZNE.geojson")
            ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::Self)
            ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingBuffer)
            ->setScale(scale)
            ->excute()
            ->makeOutputLayer(output + "Polygon_TSEZNE.geojson");

        delete layer;
    }

    void dealClip_MIPARE(const std::string& input, const std::string& output, double scale)
    {
        geos::geom::GraphicLayer* layer1 = new geos::geom::GraphicLayer();
        layer1
            ->setInputLayer(input + "Polygon_MIPARE.geojson")
            ->setAlgorithmLayer(input + "Polygon_OBSTRN.geojson")
            ->setAlgorithmLayer(input + "output/Polygon_LNDARE.geojson")
            ->setAlgorithmLayer(input + "Polygon_TSEZNE.geojson")
            ->setAlgorithmLayer(input + "Polygon_TSSLPT.geojson")
            ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::OtherDifference)
            ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingBuffer)
            ->setScale(scale)
            ->excute()
            ->makeOutputLayer(output + "Polygon_Clip_MIPARE.geojson");
        delete layer1;

    }

    void dealOutline_Clip_MIPARE(const std::string& input, const std::string& output, double scale)
    {
        geos::geom::GraphicLayer* layer1 = new geos::geom::GraphicLayer();
        layer1
            ->setInputLayer(input + "Polygon_MIPARE.geojson")
            ->setAlgorithmLayer(input + "Polygon_OBSTRN.geojson")
            ->setAlgorithmLayer(output + "Polygon_LNDARE.geojson")
            ->setAlgorithmLayer(input + "Polygon_TSEZNE.geojson")
            ->setAlgorithmLayer(input + "Polygon_TSSLPT.geojson")
            ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::OtherDifference)
            ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingBufferExteriorRing)
            ->setScale(scale)
            ->excute()
            ->makeOutputLayer(output + "Polygon_Outline_Clip_MIPARE.geojson");
        delete layer1;
    }

    void dealSEAARE(const std::string& input, const std::string& output, double scale)
    {
        geos::geom::GraphicLayer* layer1 = new geos::geom::GraphicLayer();
        layer1
            ->setInputLayer(input + "Polygon_SEAARE.geojson")
            ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::Self)
            ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonMaxLength)
            ->setScale(scale)
            ->excute()
            ->makeOutputLayer(output + "Polygon_SEAARE.geojson");
        delete layer1;

    }

    void dealLabel_SEAARE(const std::string& input, const std::string& output, double scale)
    {
        geos::geom::GraphicLayer* layer1 = new geos::geom::GraphicLayer();
        layer1
            ->setInputLayer(input + "Polygon_SEAARE.geojson")
            ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::Self)
            ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonMaxLengthCenter)
            ->setScale(scale)
            ->excute()
            ->makeOutputLayer(output + "Polygon_Label_SEAARE.geojson");
        delete layer1;

    }

    void dealLineString_RESARE(const std::string& input, const std::string& output, double scale)
    {
        geos::geom::GraphicLayer* layer1 = new geos::geom::GraphicLayer();
        layer1
            ->setInputLayer(input + "Polygon_RESARE.geojson")
            ->setAlgorithmLayer(input + "Polygon_LNDARE.geojson")
            ->setAlgorithmLayer(input + "Polygon_OBSTRN.geojson")
            ->setAlgorithmLayer(input + "Polygon_TSEZNE.geojson")
            ->setAlgorithmLayer(input + "Polygon_TSSLPT.geojson")
            ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::OtherDifference)
            ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingAndInteriorRingToExteriorRing)
            ->setScale(scale)
            ->excute()
            ->makeOutputLayer(output + "LineString_RESARE.geojson");
        delete layer1;

    }

    void dealOutline_Clip_CTNARE(const std::string& input, const std::string& output, double scale)
    {
        geos::geom::GraphicLayer* layer1 = new geos::geom::GraphicLayer();
        layer1
            ->setInputLayer(input + "Polygon_CTNARE.geojson")
            ->setAlgorithmLayer(input + "Polygon_LNDARE.geojson")
            ->setAlgorithmLayer(input + "Polygon_OBSTRN.geojson")
            ->setAlgorithmLayer(input + "Polygon_TSEZNE.geojson")
            ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::OtherDifference)
            ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingAndInteriorRingToExteriorRing)
            ->setScale(scale)
            ->excute()
            ->makeOutputLayer(output + "Polygon_Outline_Clip_CTNARE.geojson");
        delete layer1;

    }

    void dealOutline_Clip_PRCARE(const std::string& input, const std::string& output, double scale)
    {
        geos::geom::GraphicLayer* layer1 = new geos::geom::GraphicLayer();
        layer1
            ->setInputLayer(input + "Polygon_PRCARE.geojson")
            ->setAlgorithmLayer(input + "Polygon_LNDARE.geojson")
            ->setAlgorithmLayer(input + "Polygon_OBSTRN.geojson")
            ->setAlgorithmLayer(input + "Polygon_TSEZNE.geojson")
            ->setAlgorithmLayer(input + "Polygon_TSSLPT.geojson")
            ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::OtherDifference)
            ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingToExteriorRing)
            ->setScale(scale)
            ->excute()
            ->makeOutputLayer(output + "Polygon_Outline_Clip_PRCARE.geojson");
        delete layer1;
    }

    void dealClip_COALNE(const std::string& input, const std::string& output, double scale)
    {
        geos::geom::GraphicLayer* layer1 = new geos::geom::GraphicLayer();
        layer1
            ->setInputLayer(input + "LineString_COALNE.geojson")
            ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::LineDefault)
            ->setAlgorithmLayer(input + "Point_RDOSTA.geojson")
            ->setAlgorithmLayer(input + "Point_BUISGL.geojson")
            ->setAlgorithmLayer(input + "Point_UWTROC.geojson")
            ->setAlgorithmLayer(input + "Point_BCNLAT.geojson")
            ->setAlgorithmLayer(input + "Point_BCNSPP.geojson")
            ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::OtherBufferDifference)
            ->setScale(2 * scale)
            ->excute()
            ->makeOutputLayer(output + "LineString_Clip_COALNE.geojson");
        delete layer1;
    }

    void dealOutline_Union_OBSTRN(const std::string& input, const std::string& output, double scale)
    {
        geos::geom::GraphicLayer* layer1 = new geos::geom::GraphicLayer();
        layer1
            ->setInputLayer(input + "Point_OBSTRN.geojson")
            ->setAlgorithmLayer(input + "Point_OBSTRN.geojson")
            ->setGraphicFilter(geos::geom::GraphicFilter::FilterPoint)
            ->setGraphicAlgorithmFilter(geos::geom::GraphicFilter::FilterPoint)
            ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::OtherBufferUnion)
            ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PointBuffer)
            ->setScale(scale)
            ->excute()
            ->makeOutputLayer(output + "Point_Outline_Union_OBSTRN.geojson");
        delete layer1;
    }
};

typedef test_group<test_geom_graphic> group;
typedef group::object object;

group test_geom_graphic_group("geos::geom::graphic::Graphic");

//
// Test Cases
//

// Test of Aree Buffer
template<>
template<>
void object::test<1>
()
{
    dealMIPARE("/mnt/d/geos/", "/mnt/d/geos/output/", 234270);
}

// Test of Aree Buffer
template<>
template<>
void object::test<2>
()
{
    dealLNDARE("/mnt/d/geos/", "/mnt/d/geos/output/", 234270);
}

// Test of Line Buffer
template<>
template<>
void object::test<3>
()
{
    dealTSEZNE("/mnt/d/geos/", "/mnt/d/geos/output/", 234270);
}

// Test of Aree Clip
template<>
template<>
void object::test<4>
()
{
    dealClip_MIPARE("/mnt/d/geos/", "/mnt/d/geos/output/", 234270);
}

// Test of Aree Clip
template<>
template<>
void object::test<5>
()
{
    dealOutline_Clip_MIPARE("/mnt/d/geos/", "/mnt/d/geos/output/", 234270);
}

// Test of Area Max Lenth Line
template<>
template<>
void object::test<6>
()
{
    dealSEAARE("/mnt/d/geos/", "/mnt/d/geos/output/", 234270);
}

// Test of Area Max Lenth Line Center Point
template<>
template<>
void object::test<7>
()
{
    dealLabel_SEAARE("/mnt/d/geos/", "/mnt/d/geos/output/", 234270);
}

// Test of Split RESARE
template<>
template<>
void object::test<8>
()
{
    dealLineString_RESARE("/mnt/d/geos/", "/mnt/d/geos/output/", 234270);
}

// Test of Aree Clip
template<>
template<>
void object::test<9>
()
{
    dealOutline_Clip_CTNARE("/mnt/d/geos/", "/mnt/d/geos/output/", 234270);
}

// Test of Aree Clip
template<>
template<>
void object::test<10>
()
{
    dealOutline_Clip_PRCARE("/mnt/d/geos/", "/mnt/d/geos/output/", 234270);
}


// Test of Line Clip
template<>
template<>
void object::test<11>
()
{
    dealClip_COALNE("/mnt/d/geos/", "/mnt/d/geos/output/", 234270);
}


// Test of Point Union
template<>
template<>
void object::test<12>
()
{
    dealOutline_Union_OBSTRN("/mnt/d/geos/", "/mnt/d/geos/output/", 234270);
}

//// Test of Aree Buffer
//template<>
//template<>
//void object::test<13>
//()
//{
//    std::string baseRoot("/mnt/d/GIS/gis/s57/data/graphic");
//    struct dirent* entry;
//    DIR* dp = opendir(baseRoot.c_str());
//
//    if (dp == NULL) {
//        std::cerr << "Error opening directory\n";
//        return;
//    }
//
//    while ((entry = readdir(dp))) {
//        if (strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0) {
//            std::string itemInputFolder = baseRoot + "/" + entry->d_name + "/Origin_3857/";
//            std::string itemOutputFolder = baseRoot + "/" + entry->d_name + "/Graphic_3857/";
//
//            dealMIPARE(itemInputFolder, itemOutputFolder, 234270);
//            dealLNDARE(itemInputFolder, itemOutputFolder, 234270);
//            dealTSEZNE(itemInputFolder, itemOutputFolder, 234270);
//            dealClip_MIPARE(itemInputFolder, itemOutputFolder, 234270);
//            dealOutline_Clip_MIPARE(itemInputFolder, itemOutputFolder, 234270);
//            dealSEAARE(itemInputFolder, itemOutputFolder, 234270);
//            dealLabel_SEAARE(itemInputFolder, itemOutputFolder, 234270);
//            dealLineString_RESARE(itemInputFolder, itemOutputFolder, 234270);
//            dealOutline_Clip_CTNARE(itemInputFolder, itemOutputFolder, 234270);
//            dealOutline_Clip_PRCARE(itemInputFolder, itemOutputFolder, 234270);
//            dealClip_COALNE(itemInputFolder, itemOutputFolder, 234270);
//            dealOutline_Union_OBSTRN(itemInputFolder, itemOutputFolder, 234270);
//        }
//
//    }
//
//    closedir(dp);
//}

} // namespace tut
