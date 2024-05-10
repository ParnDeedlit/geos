#pragma once

#include <geos/export.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/MultiPolygon.h>

#include <string>
#include <vector>
#include <memory> // for unique_ptr

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251) // warning C4251: needs to have dll-interface to be used by clients of class
#endif


namespace geos {
    namespace geom { // geos::geom

        class GEOS_DLL GraphicMeasure {
        public:
            GraphicMeasure() : pm(1000), factory(GeometryFactory::create(&pm, 0)) {}

            /*
             * 计算区内部最长距离
             * @see https://gis.stackexchange.com/questions/132422/getting-max-length-of-polygon-and-average-width-using-postgis
             */
            std::unique_ptr <geos::geom::LineString> measurePolygonMaxLength(const geos::geom::Polygon* polygon);
            /*
             * 计算多区内部最长距离
             * @see https://gis.stackexchange.com/questions/132422/getting-max-length-of-polygon-and-average-width-using-postgis
             */
            std::unique_ptr <geos::geom::LineString> measureMultiPolygonMaxLength(const geos::geom::MultiPolygon* multipolygon);
            /*
             * 计算区内部平均距离
             * @see https://gis.stackexchange.com/questions/132422/getting-max-length-of-polygon-and-average-width-using-postgis
             */
            std::unique_ptr <geos::geom::LineString> measurePolygonAverageLength(const geos::geom::Polygon* polygon);
            /*
             * 计算多区内部平均距离
             * @see https://gis.stackexchange.com/questions/132422/getting-max-length-of-polygon-and-average-width-using-postgis
             */
            std::unique_ptr <geos::geom::LineString> measureMultiPolygonAverageLength(const geos::geom::MultiPolygon* multipolygon);

            /*
             * 计算两点之间距离
             */
            double measureCoordinate(Coordinate &a, Coordinate &b);
        private:
            geos::geom::PrecisionModel pm;
            GeometryFactory::Ptr factory;
        };

    } // namespace geos::geom
} // namespace geos


#ifdef _MSC_VER
#pragma warning(pop)
#endif

