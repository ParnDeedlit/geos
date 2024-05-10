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
        ->setInputLayer("/mnt/d/geos/MIPARE_A_Mercator.geojson")
        ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::Self)
        ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingBuffer)
        ->setScale(868003)
        ->excute()
        ->makeOutputLayer("/mnt/d/geos/output/MIPARE_A_Mercator.geojson")

        ->clear()

        ->setInputLayer("/mnt/d/geos/MIPARE_A_Mercator.geojson")
        ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::Self)
        ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingBufferExteriorRing)
        ->setScale(868003)
        ->excute()
        ->makeOutputLayer("/mnt/d/geos/output/MIPARE_A_L_Mercator.geojson");

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
        ->setInputLayer("/mnt/d/geos/LNDARE_A_Mercator.geojson")
        ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::Self)
        ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingBuffer)
        ->setScale(868003)
        ->excute()
        ->makeOutputLayer("/mnt/d/geos/output/LNDARE_A_Mercator.geojson");

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
        ->setInputLayer("/mnt/d/geos/TSEZNE_A_Mercator.geojson")
        ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::Self)
        ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingBuffer)
        ->setScale(868003)
        ->excute()
        ->makeOutputLayer("/mnt/d/geos/output/TSEZNE_A_Mercator.geojson");

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
        ->setInputLayer("/mnt/d/geos/MIPARE_A_Mercator.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/output/LNDARE_A_Mercator.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/TSEZNE_A_Mercator.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/TSSLPT_A_Mercator.geojson")
        ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::OtherDifference)
        ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingBuffer)
        ->setScale(868003)
        ->excute()
        ->makeOutputLayer("/mnt/d/geos/output/MIPARE_A_CLIP_Mercator.geojson");
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
        ->setInputLayer("/mnt/d/geos/MIPARE_A_Mercator.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/output/LNDARE_A_Mercator.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/TSEZNE_A_Mercator.geojson")
        ->setAlgorithmLayer("/mnt/d/geos/TSSLPT_A_Mercator.geojson")
        ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::OtherDifference)
        ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonExteriorRingBufferExteriorRing)
        ->setScale(868003)
        ->excute()
        ->makeOutputLayer("/mnt/d/geos/output/MIPARE_A_L_CLIP_Mercator.geojson");
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
        ->setInputLayer("/mnt/d/geos/SEAARE_A_Mercator.geojson")
        ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::Self)
        ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonMaxLength)
        ->setScale(868003)
        ->excute()
        ->makeOutputLayer("/mnt/d/geos/output/SEAARE_A_Mercator.geojson");
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
        ->setInputLayer("/mnt/d/geos/SEAARE_A_Mercator.geojson")
        ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::Self)
        ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonMaxLengthCenter)
        ->setScale(868003)
        ->excute()
        ->makeOutputLayer("/mnt/d/geos/output/SEAARE_A_LABEL_Mercator.geojson");
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
        ->setInputLayer("/mnt/d/geos/RESARE_A_Mercator.geojson")
        ->setGraphicAlgorithm(geos::geom::GraphicAlgorithm::SelfSplit)
        ->setGraphicAuxiliary(geos::geom::GraphicAuxiliary::PolygonSplitExteriorRingAndInteriorRing)
        ->setScale(868003)
        ->excute()
        ->makeOutputLayer("/mnt/d/geos/output/RESARE_A_Mercator.geojson");
    delete layer1;
}

} // namespace tut
