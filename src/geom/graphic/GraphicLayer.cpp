#include <geos/geom/graphic/GraphicLayer.h>

#include <geos/operation/buffer/BufferOp.h>
#include <geos/util/IllegalArgumentException.h>
#include <geos/util.h>

#include <cmath>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

namespace geos {
    namespace geom { // geos::geom
        
        GraphicLayer* GraphicLayer::setInputLayer(std::string geojsonPath) {
           inputs.clear();
           struct stat sb;

           if (stat(geojsonPath.c_str(), &sb) == 0) {
               std::fstream inputFile(geojsonPath);
               std::string geojson = std::string((std::istreambuf_iterator<char>(inputFile)),
                   std::istreambuf_iterator<char>());
               inputs = geojsonreader.readFeatures(geojson).getFeatures();
               inputFile.clear();
               inputFile.close();
           }

           return this;
        }

        GraphicLayer* GraphicLayer::setInputLayer(const std::vector<geos::io::GeoJSONFeature>& f) {
            inputs.clear();
            inputs = f;
            return this;
        }

        GraphicLayer* GraphicLayer::setInputLayer(std::vector<geos::io::GeoJSONFeature>&& f) {
            return this;
        }

        GraphicLayer* GraphicLayer::setAlgorithmLayer(std::string geojsonPath) {
            struct stat sb;

            if (stat(geojsonPath.c_str(), &sb) == 0) {
                std::fstream inputFile(geojsonPath);
                std::string geojson = std::string((std::istreambuf_iterator<char>(inputFile)),
                    std::istreambuf_iterator<char>());
                std::vector<geos::io::GeoJSONFeature> temps = geojsonreader.readFeatures(geojson).getFeatures();

                for (int i = 0; i < temps.size(); i++) {
                    algorithms.push_back(temps[i]);
                }

                inputFile.clear();
                inputFile.close();
            }

            return this;
        }

        GraphicLayer* GraphicLayer::setAlgorithmLayer(const std::vector<geos::io::GeoJSONFeature>& f) {
            algorithms = f;
            return this;
        }

        GraphicLayer* GraphicLayer::setAlgorithmLayer(std::vector<geos::io::GeoJSONFeature>&& f) {
            return this;
        }

        GraphicLayer* GraphicLayer::setGraphicFilter(GraphicFilter fil) {
            filter = fil;
            return this;
        }

        GraphicLayer* GraphicLayer::setGraphicAlgorithmFilter(GraphicFilter fil) {
            algorithmfilter = fil;
            return this;
        }

        GraphicLayer* GraphicLayer::setGraphicAlgorithm(GraphicAlgorithm algo) {
            algorithm = algo;
            return this;
        }

        GraphicLayer* GraphicLayer::setGraphicAuxiliary(GraphicAuxiliary aux) {
            auxiliary = aux;
            return this;
        }

        GraphicLayer* GraphicLayer::excute() {
            switch (algorithm)
            {
            case geos::geom::Self:
                excuteInputLayerBySelf();
                break;
            case geos::geom::SelfSplit:
                excuteInputLayerSplitSelf();
                break;
            case geos::geom::OtherDifference:
            case geos::geom::OtherBufferDifference:
            case geos::geom::OtherUnion:
            case geos::geom::OtherBufferUnion:
                excuteInputLayerByAlgorithm();
                break;
            default:
                break;
            }
            return this;
        }

        GraphicLayer* GraphicLayer::clear() {
            outputs.clear();
            return this;
        }

        GraphicLayer* GraphicLayer::excuteInputLayerBySelf() {
            for (int i = 0; i < inputs.size(); i++) {
                const geos::geom::Geometry* geo = inputs[i].getGeometry();
                const geos::geom::Point* point;
                const geos::geom::Polygon* poly;
                const geos::geom::Polygon* ring;
                std::unique_ptr <geos::geom::GraphicLabelInfo> labelInfo;
                std::unique_ptr<geos::geom::Geometry> deal;

                // 几何部分
                switch (auxiliary)
                {
                case geos::geom::AuxiliaryDefault:
                    break;
                case geos::geom::PolygonExteriorRingBuffer:
                    if (geo->getGeometryTypeId() == geos::geom::GEOS_POLYGON) {
                        poly = dynamic_cast<const geos::geom::Polygon*>(geo);
                        deal = poly->getExteriorRing()->buffer(scale);
                    }
                    else continue;
                    break;
                case geos::geom::PolygonExteriorRingBufferExteriorRing:
                    if (geo->getGeometryTypeId() == geos::geom::GEOS_POLYGON) {
                        poly = dynamic_cast<const geos::geom::Polygon*>(geo);
                        deal = poly->getExteriorRing()->buffer(scale);
                        ring = dynamic_cast<const geos::geom::Polygon*>(deal.get());
                        deal = ring->getExteriorRing()->clone();
                    }
                    else continue;
                    break;
                case geos::geom::PolygonMaxLength:
                    if (geo->getGeometryTypeId() == geos::geom::GEOS_POLYGON) {
                        poly = dynamic_cast<const geos::geom::Polygon*>(geo);
                        deal = measure.measurePolygonMaxLength(poly);
                    }
                    else continue;
                    break;
                case geos::geom::PolygonMaxLengthCenter:
                    if (geo->getGeometryTypeId() == geos::geom::GEOS_POLYGON) {
                        poly = dynamic_cast<const geos::geom::Polygon*>(geo);
                        labelInfo = graphiclabel.polygonMaxLengthLabel(poly);
                        deal = labelInfo->fontGeometry->clone();
                    }
                    else continue;
                    break;
                case geos::geom::PointBuffer:
                    if (geo->getGeometryTypeId() == geos::geom::GEOS_POINT) {
                        point = dynamic_cast<const geos::geom::Point*>(geo);
                        deal = point->buffer(scale);
                        ring = dynamic_cast<const geos::geom::Polygon*>(deal.get());
                        deal = ring->clone();
                    }
                    else continue;
                    break;
                default:
                    break;
                }

                // 属性部分
                std::map<std::string, geos::io::GeoJSONValue> properties = inputs[i].getProperties();
                if (auxiliary == geos::geom::PolygonMaxLengthCenter) {
                    addProperties(properties, labelInfo->fontRotateField, labelInfo->fontRotate);
                    if (properties.find("OBJNAM") != properties.end()) {
                        int size = properties["OBJNAM"].getString().size();
                        double fontSize = graphiclabel.calcPointsByWidth(labelInfo->fontWidth, size, scale);
                        addProperties(properties, labelInfo->fontSizeField, fontSize);
                    }
                }
                if (deal->isValid()) {
                    geos::io::GeoJSONFeature feature(deal->clone(), properties);
                    outputs.push_back(feature);
                }
            }

            return this;
        }

        GraphicLayer* GraphicLayer::excuteInputLayerSplitSelf() {
            for (int i = 0; i < inputs.size(); i++) {
                const geos::geom::Geometry* geo = inputs[i].getGeometry();
                const geos::geom::Polygon* poly;
                const geos::geom::Polygon* ring;
                std::unique_ptr <geos::geom::GraphicLabelInfo> labelInfo;
                std::unique_ptr<geos::geom::Geometry> deal;

                // 属性部分
                std::map<std::string, geos::io::GeoJSONValue> properties = inputs[i].getProperties();

                // 几何部分
                switch (auxiliary)
                {
                case geos::geom::PolygonSplitExteriorRingAndInteriorRing:
                    if (geo->getGeometryTypeId() == geos::geom::GEOS_POLYGON) {
                        poly = dynamic_cast<const geos::geom::Polygon*>(geo);
                        deal = poly->getExteriorRing()->clone();
                        if (deal->isValid()) {
                            geos::io::GeoJSONFeature feature(deal->clone(), properties);
                            outputs.push_back(feature);
                        }
                        for (int i = 0; i < poly->getNumInteriorRing(); i++) {
                            deal = poly->getInteriorRingN(i)->clone();
                            if (deal->isValid()) {
                                addProperties(properties, "isLand", true);
                                geos::io::GeoJSONFeature feature(deal->clone(), properties);
                                outputs.push_back(feature);
                            }
                        }
                    }
                    else continue;
                    break;
                default:
                    break;
                }
            }
            return this;
        }

        GraphicLayer* GraphicLayer::excuteInputLayerByAlgorithm() {
            for (int i = 0; i < inputs.size(); i++) {
                const geos::geom::Geometry* geo = inputs[i].getGeometry();
                const geos::geom::Point* point;
                const geos::geom::LineString* line;
                const geos::geom::Polygon* poly;
                const geos::geom::Polygon* ring;
                std::unique_ptr < geos::geom::GraphicLabelInfo> labelInfo;
                std::unique_ptr<geos::geom::Geometry> deal;
                bool isMultiResult = false;

                if (filter == geos::geom::FilterPoint) {
                    if (geo->getGeometryTypeId() != geos::geom::GEOS_POINT) {
                        continue;
                    }
                } else if (filter == geos::geom::FilterLineString) {
                    if (geo->getGeometryTypeId() != geos::geom::GEOS_LINESTRING) {
                        continue;
                    }
                } else if (filter == geos::geom::FilterPolygon) {
                    if (geo->getGeometryTypeId() != geos::geom::GEOS_POLYGON) {
                        continue;
                    }
                }

                // 属性部分
                std::map<std::string, geos::io::GeoJSONValue> properties = inputs[i].getProperties();

                // 几何部分
                switch (auxiliary)
                {
                case geos::geom::AuxiliaryDefault:
                    break;
                case geos::geom::PolygonExteriorRingBuffer:
                    if (geo->getGeometryTypeId() == geos::geom::GEOS_POLYGON) {
                        poly = dynamic_cast<const geos::geom::Polygon*>(geo);
                        deal = poly->getExteriorRing()->buffer(scale, 0, geos::operation::buffer::BufferOp::CAP_SQUARE);
                    }
                    else continue;
                    break;
                case geos::geom::PolygonExteriorRingBufferExteriorRing:
                    if (geo->getGeometryTypeId() == geos::geom::GEOS_POLYGON) {
                        poly = dynamic_cast<const geos::geom::Polygon*>(geo);
                        deal = poly->getExteriorRing()->buffer(scale, 0, geos::operation::buffer::BufferOp::CAP_SQUARE);
                        ring = dynamic_cast<const geos::geom::Polygon*>(deal.get());
                        if (orientation.isCCW(ring->getExteriorRing()->getCoordinates().get())) {
                            deal = ring->getExteriorRing()->reverse()->clone();
                        }
                        else {
                            deal = ring->getExteriorRing()->clone();
                        }
                    }
                    else continue;
                    break;
                case geos::geom::PolygonExteriorRing:
                    if (geo->getGeometryTypeId() == geos::geom::GEOS_POLYGON) {
                        poly = dynamic_cast<const geos::geom::Polygon*>(geo);
                        deal = poly->getExteriorRing()->buffer(1, 0, geos::operation::buffer::BufferOp::CAP_SQUARE);
                    }
                    else continue;
                    break;
                case geos::geom::PolygonExteriorRingToExteriorRing:
                    if (geo->getGeometryTypeId() == geos::geom::GEOS_POLYGON) {
                        poly = dynamic_cast<const geos::geom::Polygon*>(geo);
                        deal = poly->getExteriorRing()->buffer(1, 0, geos::operation::buffer::BufferOp::CAP_SQUARE);
                        ring = dynamic_cast<const geos::geom::Polygon*>(deal.get());
                        if (orientation.isCCW(ring->getExteriorRing()->getCoordinates().get())) {
                            deal = ring->getExteriorRing()->reverse()->clone();
                        }
                        else {
                            deal = ring->getExteriorRing()->clone();
                        }
                    }
                    else continue;
                    break;
                case geos::geom::PolygonExteriorRingAndInteriorRingToExteriorRing:
                    if (geo->getGeometryTypeId() == geos::geom::GEOS_POLYGON) {
                        isMultiResult = true;
                        poly = dynamic_cast<const geos::geom::Polygon*>(geo);
                        deal = poly->getExteriorRing()->clone();
                        if (deal->isValid()) {
                            if (algorithm == geos::geom::OtherDifference) {
                                for (int j = 0; j < algorithms.size(); j++) {
                                    deal = deal->difference(algorithms[j].getGeometry());
                                }
                            }
                            geos::io::GeoJSONFeature feature(deal->clone(), properties);
                            outputs.push_back(feature);
                        }
                        for (int i = 0; i < poly->getNumInteriorRing(); i++) {
                            deal = poly->getInteriorRingN(i)->clone();
                            if (deal->isValid()) {
                                if (algorithm == geos::geom::OtherDifference) {
                                    for (int j = 0; j < algorithms.size(); j++) {
                                        deal = deal->difference(algorithms[j].getGeometry());
                                    }
                                }
                                addProperties(properties, "isTopoLink", true);
                                geos::io::GeoJSONFeature feature(deal->clone(), properties);
                                outputs.push_back(feature);
                            }
                        }
                    }
                    else continue;
                    break;
                case geos::geom::PolygonMaxLength:
                    if (geo->getGeometryTypeId() == geos::geom::GEOS_POLYGON) {
                        poly = dynamic_cast<const geos::geom::Polygon*>(geo);
                        deal = measure.measurePolygonMaxLength(poly);
                    }
                    else continue;
                    break;
                case geos::geom::PolygonMaxLengthCenter:
                    if (geo->getGeometryTypeId() == geos::geom::GEOS_POLYGON) {
                        poly = dynamic_cast<const geos::geom::Polygon*>(geo);
                        labelInfo = graphiclabel.polygonMaxLengthLabel(poly);
                        deal = labelInfo->fontGeometry->clone();
                    }
                    else continue;
                    break;
                case geos::geom::LineDefault:
                    if (geo->getGeometryTypeId() == geos::geom::GEOS_LINESTRING) {
                        line = dynamic_cast<const geos::geom::LineString*>(geo);
                        deal = line->clone();
                    }
                    else continue;
                    break;
                case geos::geom::PointDefault:
                    if (geo->getGeometryTypeId() == geos::geom::GEOS_POINT) {
                        point = dynamic_cast<const geos::geom::Point*>(geo);
                        deal = point->clone();
                    }
                    else continue;
                    break;
                case geos::geom::PointBuffer:
                    if (geo->getGeometryTypeId() == geos::geom::GEOS_POINT) {
                        point = dynamic_cast<const geos::geom::Point*>(geo);
                        deal = point->buffer(scale);
                    }
                    else continue;
                    break;
                default:
                    break;
                }

                if (deal == nullptr || !deal->isValid()) {
                    continue;
                }

                if (!isMultiResult) {
                    for (int j = 0; j < algorithms.size(); j++) {
                        if (algorithmfilter == geos::geom::FilterPoint) {
                            if (algorithms[j].getGeometry()->getGeometryTypeId() != geos::geom::GEOS_POINT) {
                                continue;
                            }
                        }
                        else if (algorithmfilter == geos::geom::FilterLineString) {
                            if (algorithms[j].getGeometry()->getGeometryTypeId() != geos::geom::GEOS_LINESTRING) {
                                continue;
                            }
                        }
                        else if (algorithmfilter == geos::geom::FilterPolygon) {
                            if (algorithms[j].getGeometry()->getGeometryTypeId() != geos::geom::GEOS_POLYGON) {
                                continue;
                            }
                        }

                        if (algorithm == geos::geom::OtherDifference) {
                            deal = deal->difference(algorithms[j].getGeometry());
                        }
                        else if (algorithm == geos::geom::OtherBufferDifference) {
                            deal = deal->difference(algorithms[j].getGeometry()->buffer(scale)->clone().get());
                        }
                        else if (algorithm == geos::geom::OtherUnion) {
                            deal = deal->Union(algorithms[j].getGeometry());
                        }
                        else if (algorithm == geos::geom::OtherBufferUnion) {
                            deal = deal->Union(algorithms[j].getGeometry()->buffer(scale)->clone().get());
                        }
                    }

                    if (deal != nullptr && deal->isValid()) {
                        geos::io::GeoJSONFeature feature(deal->clone(), properties);
                        outputs.push_back(feature);
                    }
                }
            }

            return this;
        }

        GraphicLayer* GraphicLayer::makeOutputLayer(std::string geojsonPath) {
            geos::io::GeoJSONFeatureCollection featurebuffers(outputs);
            std::string result = geojsonwriter.write(featurebuffers);
            std::ofstream file(geojsonPath);
            if (!file.is_open()) {
                printf("file is open %s \r\n", geojsonPath.c_str());
            }
            else {
                file.write(result.c_str(), result.size());
            }
            file.close();
            return this;
        }

        const std::vector<geos::io::GeoJSONFeature> GraphicLayer::getOutputLayerGeoJSONCollection() {
            return outputs;
        }

        GraphicLayer* GraphicLayer::setScale(double s) {
            scale = s / 1000;
            return this;
        }

        GraphicLayer* GraphicLayer::addProperties(std::map<std::string, geos::io::GeoJSONValue> &properties, std::string key, geos::io::GeoJSONValue value) {
            if (properties.find(key) != properties.end()) {
                properties.emplace(key, value);
            }
            else {
                properties.insert(std::make_pair(key, value));
            }
            return this;
        }

    }
}
