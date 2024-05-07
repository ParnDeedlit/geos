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

// Test of user's constructor to create non-empty valid Polygon
template<>
template<>
void object::test<1>
()
{
    using geos::geom::Coordinate;
    const double scale = 8680.0;
    std::fstream input_file("/mnt/d/geos/MIPARE_A_Mercator.geojson");
    if (!input_file.is_open()) {
    } else {
      std::string geojson = std::string((std::istreambuf_iterator<char>(input_file)),
					std::istreambuf_iterator<char>());
      geos::io::GeoJSONFeatureCollection features(geojsonreader.readFeatures(geojson));

      const geos::geom::Geometry* geo = features.getFeatures()[0].getGeometry();
      const geos::geom::Polygon* poly = dynamic_cast<const geos::geom::Polygon*>(geo);

      const geos::geom::LinearRing* linearRing = poly->getExteriorRing();
      std::unique_ptr<geos::geom::Geometry> buffer =  linearRing->buffer(scale * 0.25);
      std::string result = geojsonwriter.write(buffer.get());

      std::ofstream file("/mnt/d/geos/output/MIPARE_A_Mercator.geojson");
      if (!file.is_open()) {
      } else {
	    file.write(result.c_str(), result.size());
      }
    }
}

} // namespace tut
