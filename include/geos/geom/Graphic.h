#pragma once

#include <geos/export.h>
#include <geos/geom/Geometry.h> // for inheritance
#include <geos/geom/CoordinateSequence.h> // for proper use of unique_ptr<>
#include <geos/geom/Envelope.h> // for proper use of unique_ptr<>
#include <geos/geom/Dimension.h> // for Dimension::DimensionType

#include <string>
#include <vector>
#include <memory> // for unique_ptr

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251) // warning C4251: needs to have dll-interface to be used by clients of class
#endif

// Forward declarations
namespace geos {
namespace geom { // geos::geom
class Coordinate;
class CoordinateFilter;
class CoordinateSequenceFilter;
class GeometryComponentFilter;
class GeometryFilter;
}
}

namespace geos {
namespace geom { // geos::geom

/**
 * Implementation of Point.
 *
 * A Point is valid iff:
 *
 * - the coordinate which defines it is a valid coordinate
 *   (i.e does not have an NaN X or Y ordinate)
 *
 */
class GEOS_DLL Graphic {


};

} // namespace geos::geom
} // namespace geos


#ifdef _MSC_VER
#pragma warning(pop)
#endif


