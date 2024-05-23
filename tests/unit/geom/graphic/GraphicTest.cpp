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
    geos::geom::GraphicLayer* layer = new geos::geom::GraphicLayer();
    layer
        ->setInputLayer("/mnt/d/geos/Polygon_MIPARE.geojson")
        ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::Self)
        ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingBuffer)
        ->setScale(868003)
        ->excute()
        ->makeOutputLayer("/mnt/d/geos/output/Polygon_MIPARE.geojson")

        ->clear()

        ->setInputLayer("/mnt/d/geos/Polygon_MIPARE.geojson")
        ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::Self)
        ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingBufferExteriorRing)
        ->setScale(868003)
        ->excute()
        ->makeOutputLayer("/mnt/d/geos/output/Polygon_Outline_MIPARE.geojson");

    delete layer;
}

// Test of Aree Buffer
template<>
template<>
void object::test<2>
()
{
    geos::geom::GraphicLayer* layer = new geos::geom::GraphicLayer();
    layer
        ->setInputLayer("/mnt/d/geos/Polygon_LNDARE.geojson")
        ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::Self)
        ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingBuffer)
        ->setScale(868003)
        ->excute()
        ->makeOutputLayer("/mnt/d/geos/output/Polygon_LNDARE.geojson");

     delete layer;
}

// Test of Line Buffer
template<>
template<>
void object::test<3>
()
{
    geos::geom::GraphicLayer* layer = new geos::geom::GraphicLayer();
    layer
        ->setInputLayer("/mnt/d/geos/Polygon_TSEZNE.geojson")
        ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::Self)
        ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingBuffer)
        ->setScale(868003)
        ->excute()
        ->makeOutputLayer("/mnt/d/geos/output/Polygon_TSEZNE.geojson");

    delete layer;
}

// Test of Aree Clip
template<>
template<>
void object::test<4>
()
{
    geos::geom::GraphicLayer* layer1 = new geos::geom::GraphicLayer();
    layer1
        ->setInputLayer("/mnt/d/geos/Polygon_MIPARE.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/Polygon_OBSTRN.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/output/Polygon_LNDARE.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/Polygon_TSEZNE.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/Polygon_TSSLPT.geojson")
        ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::OtherDifference)
        ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingBuffer)
        ->setScale(868003)
        ->excute()
        ->makeOutputLayer("/mnt/d/geos/output/Polygon_Clip_MIPARE.geojson");
    delete layer1;
}

// Test of Aree Clip
template<>
template<>
void object::test<5>
()
{
    geos::geom::GraphicLayer* layer1 = new geos::geom::GraphicLayer();
    layer1
        ->setInputLayer("/mnt/d/geos/Polygon_MIPARE.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/Polygon_OBSTRN.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/output/Polygon_LNDARE.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/Polygon_TSEZNE.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/Polygon_TSSLPT.geojson")
        ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::OtherDifference)
        ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingBufferExteriorRing)
        ->setScale(868003)
        ->excute()
        ->makeOutputLayer("/mnt/d/geos/output/Polygon_Outline_Clip_MIPARE.geojson");
    delete layer1;
}

// Test of Area Max Lenth Line
template<>
template<>
void object::test<6>
()
{
    geos::geom::GraphicLayer* layer1 = new geos::geom::GraphicLayer();
    layer1
        ->setInputLayer("/mnt/d/geos/Polygon_SEAARE.geojson")
        ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::Self)
        ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonMaxLength)
        ->setScale(868003)
        ->excute()
        ->makeOutputLayer("/mnt/d/geos/output/Polygon_SEAARE.geojson");
    delete layer1;
}

// Test of Area Max Lenth Line Center Point
template<>
template<>
void object::test<7>
()
{
    geos::geom::GraphicLayer* layer1 = new geos::geom::GraphicLayer();
    layer1
        ->setInputLayer("/mnt/d/geos/Polygon_SEAARE.geojson")
        ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::Self)
        ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonMaxLengthCenter)
        ->setScale(868003)
        ->excute()
        ->makeOutputLayer("/mnt/d/geos/output/Polygon_Label_SEAARE.geojson");
    delete layer1;
}

// Test of Split RESARE
template<>
template<>
void object::test<8>
()
{
    geos::geom::GraphicLayer* layer1 = new geos::geom::GraphicLayer();
    layer1
        ->setInputLayer("/mnt/d/geos/Polygon_RESARE.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/Polygon_LNDARE.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/Polygon_OBSTRN.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/Polygon_TSEZNE.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/Polygon_TSSLPT.geojson")
        ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::OtherDifference)
        ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingAndInteriorRingToExteriorRing)
        ->setScale(868003)
        ->excute()
        ->makeOutputLayer("/mnt/d/geos/output/LineString_RESARE.geojson");
    delete layer1;
}

// Test of Aree Clip
template<>
template<>
void object::test<9>
()
{
    geos::geom::GraphicLayer* layer1 = new geos::geom::GraphicLayer();
    layer1
        ->setInputLayer("/mnt/d/geos/Polygon_CTNARE.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/Polygon_LNDARE.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/Polygon_OBSTRN.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/Polygon_TSEZNE.geojson")
        ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::OtherDifference)
        ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingAndInteriorRingToExteriorRing)
        ->setScale(868003)
        ->excute()
        ->makeOutputLayer("/mnt/d/geos/output/Polygon_Outline_Clip_CTNARE.geojson");
    delete layer1;
}

// Test of Aree Clip
template<>
template<>
void object::test<10>
()
{
    geos::geom::GraphicLayer* layer1 = new geos::geom::GraphicLayer();
    layer1
        ->setInputLayer("/mnt/d/geos/Polygon_PRCARE.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/Polygon_LNDARE.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/Polygon_OBSTRN.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/Polygon_TSEZNE.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/Polygon_TSSLPT.geojson")
        ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::OtherDifference)
        ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingToExteriorRing)
        ->setScale(868003)
        ->excute()
        ->makeOutputLayer("/mnt/d/geos/output/Polygon_Outline_Clip_PRCARE.geojson");
    delete layer1;
}

// Test of Line Clip
template<>
template<>
void object::test<11>
()
{
    geos::geom::GraphicLayer* layer1 = new geos::geom::GraphicLayer();
    layer1
        ->setInputLayer("/mnt/d/geos/Point_RDOSTA.geojson")
        ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::Self)
        ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PointBuffer)
        ->setScale(2 * 868003)
        ->excute()
        ->makeOutputLayer("/mnt/d/geos/output/Point_RDOSTA.geojson");
    delete layer1;
}


// Test of Line Clip
template<>
template<>
void object::test<12>
()
{
    geos::geom::GraphicLayer* layer1 = new geos::geom::GraphicLayer();
    layer1
        ->setInputLayer("/mnt/d/geos/LineString_COALNE.geojson")
        ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::LineDefault)
        ->setAlgorithmLayer("/mnt/d/geos/Point_RDOSTA.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/Point_BUISGL.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/Point_UWTROC.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/Point_BCNLAT.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/Point_BCNSPP.geojson")
        ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::OtherBufferDifference)
        ->setScale(2 * 868003)
        ->excute()
        ->makeOutputLayer("/mnt/d/geos/output/LineString_Clip_COALNE.geojson");
    delete layer1;
}


// Test of Point Union
template<>
template<>
void object::test<13>
()
{
    geos::geom::GraphicLayer* layer1 = new geos::geom::GraphicLayer();
    layer1
        ->setInputLayer("/mnt/d/geos/Point_OBSTRN.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/Point_OBSTRN.geojson")
        ->setGraphicFilter(geos::geom::GraphicFilter::FilterPoint)
        ->setGraphicAlgorithmFilter(geos::geom::GraphicFilter::FilterPoint)
        ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::OtherBufferUnion)
        ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PointBuffer)
        ->setScale(868003)
        ->excute()
        ->makeOutputLayer("/mnt/d/geos/output/Point_Outline_Union_OBSTRN.geojson");
    delete layer1;
}

} // namespace tut
