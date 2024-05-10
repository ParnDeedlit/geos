#include <geos/geom/graphic/GraphicMeasure.h>
#include <geos/operation/distance/DistanceOp.h>

#include <geos/geom/LineString.h>
#include <geos/geom/LinearRing.h>

#include <cmath>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>

namespace geos {
    namespace geom { // geos::geom

        std::unique_ptr <geos::geom::LineString> GraphicMeasure::measurePolygonMaxLength(const geos::geom::Polygon* polygon) {
            double maxLength = 0.0;
            double calcLength = 0.0;
            const geos::geom::LinearRing* linearRing = polygon->getExteriorRing();

            std::unique_ptr<geos::geom::Point> start;
            std::unique_ptr<geos::geom::Point> end;
            
            for (int i = 0; i < linearRing->getNumPoints() - 2; i++) {
                for (int j = i + 1; j < linearRing->getNumPoints() - 1; j++) {
                    calcLength = geos::operation::distance::DistanceOp::distance(linearRing->getPointN(i).get(), linearRing->getPointN(j).get());
                    if (calcLength > maxLength) {
                        maxLength = calcLength;
                        start = linearRing->getPointN(i);
                        end = linearRing->getPointN(j);
                    }
                }
            }

            CoordinateSequence coordinateSequence(2);
            std::vector<Coordinate> lines;
            if (start->getCoordinate()->x <= end->getCoordinate()->x) {
                lines.push_back(Coordinate(start->getCoordinate()->x, start->getCoordinate()->y));
                lines.push_back(Coordinate(end->getCoordinate()->x, end->getCoordinate()->y));
            }
            else {
                lines.push_back(Coordinate(end->getCoordinate()->x, end->getCoordinate()->y));
                lines.push_back(Coordinate(start->getCoordinate()->x, start->getCoordinate()->y));
            }

            coordinateSequence.setPoints(lines);
            std::unique_ptr<LineString> lineString = factory->createLineString(coordinateSequence);
            return lineString;
        }

        std::unique_ptr <geos::geom::LineString> GraphicMeasure::measureMultiPolygonMaxLength(const geos::geom::MultiPolygon* multipolygon) {
            double maxLength = 0.0;
            double calcLength = 0.0;
            Coordinate start;
            Coordinate end;

            std::vector<geos::geom::Coordinate> points;
            for (int i = 0; i < multipolygon->getNumGeometries(); i++) {
                const geos::geom::LinearRing* linearRing = multipolygon->getGeometryN(i)->getExteriorRing();
                for (int j = 0; j < linearRing->getNumPoints(); j++) {
                    points.push_back(linearRing->getCoordinateN(j));
                }
            }

            for (int i = 0; i < points.size() - 2; i++) {
                for (int j = i + 1; j < points.size() - 1; j++) {
                    calcLength = measureCoordinate(points[i], points[j]);
                    if (calcLength > maxLength) {
                        maxLength = calcLength;
                        start = points[i];
                        end = points[j];
                    }
                }
            }

            CoordinateSequence coordinateSequence(2);
            std::vector<Coordinate> lines;
            if (start.x <= end.x) {
                lines.push_back(Coordinate(start.x, start.y));
                lines.push_back(Coordinate(end.x, end.y));
            }
            else {
                lines.push_back(Coordinate(end.x, end.y));
                lines.push_back(Coordinate(start.x, start.y));
            }

            coordinateSequence.setPoints(lines);
            std::unique_ptr<LineString> lineString = factory->createLineString(coordinateSequence);
            return lineString;
        }

        std::unique_ptr <geos::geom::LineString> GraphicMeasure::measurePolygonAverageLength(const geos::geom::Polygon* polygon) {
            std::unique_ptr<LineString> lineString;
            return lineString;
        }

        std::unique_ptr <geos::geom::LineString> GraphicMeasure::measureMultiPolygonAverageLength(const geos::geom::MultiPolygon* multipolygon) {
            std::unique_ptr<LineString> lineString;
            return lineString;
        }

        double GraphicMeasure::measureCoordinate(Coordinate& a, Coordinate& b) {
            return sqrt(abs(a.x - b.x) * abs(a.y - b.y));
        }

    }
}
