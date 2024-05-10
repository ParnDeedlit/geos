#pragma once

#include <geos/export.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/Point.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/graphic/GraphicMeasure.h>

#include <string>
#include <vector>
#include <memory> // for unique_ptr

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251) // warning C4251: needs to have dll-interface to be used by clients of class
#endif


namespace geos {
    namespace geom { // geos::geom
        class GEOS_DLL GraphicLabelInfo {
        public:
            double fontWidth = 0;
            double fontRotate = 0;
            double fontSize = 12;
            std::string fontFamily = "宋体";
            std::unique_ptr<geos::geom::Geometry> fontGeometry;

            std::string fontRotateField = "fontRotate";
            std::string fontSizeField = "fontSize";
            std::string fontFamilyField = "fontFamily";
        };

        class GEOS_DLL GraphicLabel {
        public:
            GraphicLabel() : pm(1000), factory(GeometryFactory::create(&pm, 0)) {}

            std::unique_ptr<GraphicLabelInfo> polygonMaxLengthLabel(const geos::geom::Polygon* polygon);
            /*
            * 根据字符的数字的数量 以及 字符对应的投影坐标的字符的外包矩形宽度  以及当前比例尺 计算出字体的大小
             * strWidth 生成的投影坐标的字符的外包矩形宽度
             * strLength 字符串的数量  中国 = 2  中华人民共和国 = 7
             * scale 比例尺
             */
            double calcPointsByWidth(double strWidth, int strLength, double scale);
        private:
            double mmPerInch = 25.4;
            double pointsPerInch = 72;
            double mmPerPoint = 25.4 / 72;
            GraphicMeasure measure;

            geos::geom::PrecisionModel pm;
            GeometryFactory::Ptr factory;
        };

    } // namespace geos::geom
} // namespace geos


#ifdef _MSC_VER
#pragma warning(pop)
#endif

