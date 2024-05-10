#include <geos/geom/graphic/GraphicLayer.h>

#include <cmath>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>

namespace geos {
    namespace geom { // geos::geom
        
        GraphicLayer* GraphicLayer::setInputLayer(std::string geojsonPath) {
           inputs.clear();
           std::fstream inputFile(geojsonPath);
           std::string geojson = std::string((std::istreambuf_iterator<char>(inputFile)),
                std::istreambuf_iterator<char>());
           inputs = geojsonreader.readFeatures(geojson).getFeatures();
           inputFile.clear();
           inputFile.close();
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
            std::fstream inputFile(geojsonPath);
            std::string geojson = std::string((std::istreambuf_iterator<char>(inputFile)),
                std::istreambuf_iterator<char>());
            std::vector<geos::io::GeoJSONFeature> temps = geojsonreader.readFeatures(geojson).getFeatures();

            for (int i = 0; i < temps.size(); i++) {
                algorithms.push_back(temps[i]);
            }
            
            inputFile.clear();
            inputFile.close();
            return this;
        }

        GraphicLayer* GraphicLayer::setAlgorithmLayer(const std::vector<geos::io::GeoJSONFeature>& f) {
            algorithms = f;
            return this;
        }

        GraphicLayer* GraphicLayer::setAlgorithmLayer(std::vector<geos::io::GeoJSONFeature>&& f) {
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
                    poly = dynamic_cast<const geos::geom::Polygon*>(geo);
                    deal = poly->getExteriorRing()->buffer(scale);
                    break;
                case geos::geom::PolygonExteriorRingBufferExteriorRing:
                    poly = dynamic_cast<const geos::geom::Polygon*>(geo);
                    deal = poly->getExteriorRing()->buffer(scale);
                    ring = dynamic_cast<const geos::geom::Polygon*>(deal.get());
                    deal = ring->getExteriorRing()->clone();
                    break;
                case geos::geom::PolygonMaxLength:
                    poly = dynamic_cast<const geos::geom::Polygon*>(geo);
                    deal = measure.measurePolygonMaxLength(poly);
                    break;
                case geos::geom::PolygonMaxLengthCenter:
                    poly = dynamic_cast<const geos::geom::Polygon*>(geo);
                    labelInfo = graphiclabel.polygonMaxLengthLabel(poly);
                    deal = labelInfo->fontGeometry->clone();
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
                const geos::geom::Polygon* poly;
                const geos::geom::Polygon* ring;
                std::unique_ptr < geos::geom::GraphicLabelInfo> labelInfo;
                std::unique_ptr<geos::geom::Geometry> deal;

                switch (auxiliary)
                {
                case geos::geom::AuxiliaryDefault:
                    break;
                case geos::geom::PolygonExteriorRingBuffer:
                    poly = dynamic_cast<const geos::geom::Polygon*>(geo);
                    deal = poly->getExteriorRing()->buffer(scale);
                    break;
                case geos::geom::PolygonExteriorRingBufferExteriorRing:
                    poly = dynamic_cast<const geos::geom::Polygon*>(geo);
                    deal = poly->getExteriorRing()->buffer(scale);
                    ring = dynamic_cast<const geos::geom::Polygon*>(deal.get());
                    deal = ring->getExteriorRing()->clone();
                    break;
                case geos::geom::PolygonMaxLength:
                    poly = dynamic_cast<const geos::geom::Polygon*>(geo);
                    deal = measure.measurePolygonMaxLength(poly);
                    break;
                case geos::geom::PolygonMaxLengthCenter:
                    poly = dynamic_cast<const geos::geom::Polygon*>(geo);
                    labelInfo = graphiclabel.polygonMaxLengthLabel(poly);
                    deal = labelInfo->fontGeometry->clone();
                    break;
                default:
                    break;
                }

                for (int j = 0; j < algorithms.size(); j++) {
                    switch (algorithm)
                    {
                    case geos::geom::OtherDifference:
                        deal = deal->difference(algorithms[j].getGeometry());
                        break;
                    default:
                        break;
                    }
                }

                std::map<std::string, geos::io::GeoJSONValue> properties = inputs[i].getProperties();
                geos::io::GeoJSONFeature feature(deal->clone(), properties);
                outputs.push_back(feature);
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
