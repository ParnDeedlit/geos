#pragma once

#include <geos/export.h>
#include <geos/geom/Geometry.h> // for inheritance
#include <geos/geom/CoordinateSequence.h> // for proper use of unique_ptr<>
#include <geos/geom/Envelope.h> // for proper use of unique_ptr<>
#include <geos/geom/Dimension.h> // for Dimension::DimensionType

#include <geos/io/GeoJSONReader.h>
#include <geos/io/GeoJSONWriter.h>

#include <geos/geom/graphic/GraphicFilter.h>
#include <geos/geom/graphic/GraphicAlgorithm.h>
#include <geos/geom/graphic/GraphicAuxiliary.h>
#include <geos/geom/graphic/GraphicMeasure.h>
#include <geos/geom/graphic/GraphicLabel.h>

#include <geos/algorithm/Orientation.h>

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

    namespace io {
        class GeoJSONReader;
        class GeoJSONWriter;
        class GeoJSONFeatureCollection;
        class GeoJSONFeature;
    }
}

namespace geos {
    namespace geom { // geos::geom

        /**
         * Implementation of GraphicLayer
         * 1. one layer for one GeoJSONFeatureCollection.
         *
         */
        class GEOS_DLL GraphicLayer {

        public:
            GraphicLayer() : pm(1.0), factory(GeometryFactory::create(&pm, 0)), geojsonreader(*(factory.get())) {
                filter = geos::geom::GraphicFilter::FilterAll;
                algorithmfilter = geos::geom::GraphicFilter::FilterAll;
            }

            /*
             * 设置被计算图层-geojson文件地址uri
             */
            GraphicLayer* setInputLayer(std::string geojsonPath);
            /*
             * 外部传入-设置被计算图层geojsoncollection集合
             */
            GraphicLayer* setInputLayer(const std::vector<geos::io::GeoJSONFeature>& f);
            /*
             * 内部创建-设置被计算图层geojsoncollection集合
             */
            GraphicLayer* setInputLayer(std::vector<geos::io::GeoJSONFeature>&& f);
            /*
             * 设置计算的图层地址uri
             */
            GraphicLayer* setAlgorithmLayer(std::string geojsonPath);
            /*
             * 外部传入-设置geojsoncollection集合
             */
            GraphicLayer* setAlgorithmLayer(const std::vector<geos::io::GeoJSONFeature>& f);
            /*
             * 内部创建-设置geojsoncollection集合
             */
            GraphicLayer* setAlgorithmLayer(std::vector<geos::io::GeoJSONFeature>&& f);

            /*
             * 设置过滤模式
             */
            GraphicLayer* setGraphicFilter(GraphicFilter fil);

            /*
             * 设置制图辅助图层过滤模式
             */
            GraphicLayer* setGraphicAlgorithmFilter(GraphicFilter fil);
            /*
             * 设置制图算法策略
             */
            GraphicLayer* setGraphicAlgorithm(GraphicAlgorithm algo);
            /*
             * 设置制图辅助图层模式
             */
            GraphicLayer* setGraphicAuxiliary(GraphicAuxiliary aux);

            /*
             * 执行
             * layer
             *     ->setInputLayer()
             *     ->setGraphicAlgorithm()
             *     ->setGraphicAuxiliary()
             *     ->excute()
             *     ->getOutputLayerGeoJSONCollection()
             */
            GraphicLayer* excute();

            /*
             * 清空
             * layer
             *     ->setInputLayer()
             *     ->setGraphicAlgorithm()
             *     ->setGraphicAuxiliary()
             *     ->excute()
             *     ->getOutputLayerGeoJSONCollection()
             *     ->clear()
             */
            GraphicLayer* clear();

            /*
             * 导出辅助图层geojson文件
             * layer
             *     ->setInputLayer()
             *     ->setGraphicAlgorithm()
             *     ->setGraphicAuxiliary()
             *     ->excute()
             *     ->makeOutputLayer()
             */
            GraphicLayer* makeOutputLayer(std::string geojsonPath);

            /*
             * 获取辅助图层geojson数据集
             */
            const std::vector<geos::io::GeoJSONFeature> getOutputLayerGeoJSONCollection();

            GraphicLayer* setScale(double s);

            GraphicLayer* setTranslate(double x, double y);

        private:
            GraphicLayer* excuteInputLayerBySelf();
            GraphicLayer* excuteInputLayerSplitSelf();
            GraphicLayer* excuteInputLayerByAlgorithm();
            GraphicLayer* addProperties(std::map<std::string, geos::io::GeoJSONValue> &properties, std::string key, geos::io::GeoJSONValue value);

            std::vector<geos::io::GeoJSONFeature> inputs;
            std::vector<geos::io::GeoJSONFeature> algorithms;
            std::vector<geos::io::GeoJSONFeature> outputs;

            GraphicFilter filter;
            GraphicFilter algorithmfilter;
            GraphicAlgorithm algorithm;
            GraphicAuxiliary auxiliary;
            GraphicMeasure measure;
            GraphicLabel graphiclabel;

            geos::geom::PrecisionModel pm;
            GeometryFactory::Ptr factory;
            geos::io::GeoJSONReader geojsonreader;
            geos::io::GeoJSONWriter geojsonwriter;
            std::string outputPath;
            double scale;
            double translatex;
            double translatey;
            geos::algorithm::Orientation orientation;
        };

    } // namespace geos::geom
} // namespace geos


#ifdef _MSC_VER
#pragma warning(pop)
#endif


