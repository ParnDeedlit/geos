#include <geos/geom/graphic/GraphicLabel.h>
#include <geos/geom/graphic/GraphicMeasure.h>

#include <geos/geom/Point.h>
#include <geos/geom/LineString.h>
#include <geos/geom/LinearRing.h>

#include <cmath>
#include <memory>
#include <string>
#include <numbers>
#include <iostream>
#include <fstream>

namespace geos {
    namespace geom { // geos::geom

        std::unique_ptr<GraphicLabelInfo> GraphicLabel::polygonMaxLengthLabel(const geos::geom::Polygon* polygon) {
            const double PI = 3.141592653589793238463;
            std::unique_ptr<GraphicLabelInfo> labelInfo = std::make_unique<GraphicLabelInfo>();
            std::unique_ptr<LineString> line = measure.measurePolygonMaxLength(polygon);
            if (line->getNumPoints() == 2) {
                double x = line->getPointN(1)->getX() - line->getPointN(0)->getX();
                double y = line->getPointN(1)->getY() - line->getPointN(0)->getY();

                double cx = (line->getPointN(1)->getX() + line->getPointN(0)->getX()) / 2;
                double cy = (line->getPointN(1)->getY() + line->getPointN(0)->getY()) / 2;

                double rotate = atan(y / x) * 180 / PI;
                rotate = rotate * (-1);
                labelInfo->fontRotate = rotate;
                labelInfo->fontGeometry = factory->createPoint(Coordinate(cx, cy));
                labelInfo->fontWidth = line->getLength();
            }
            else {

            }
            return labelInfo;
        }

        double GraphicLabel::calcPointsByWidth(double strWidth, int strLength, double scale) {
            double fontSize = 2;
            for (int i = 1; i < 1000; i++) {
                double allmm = i * strLength * mmPerPoint;
                double allWidth = allmm * scale;
                if (strWidth < allWidth) {
                    fontSize = i - 1;
                    break;
                }
            }
            return fontSize;
        }
    }
}
