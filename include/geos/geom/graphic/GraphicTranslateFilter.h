#include <geos/export.h>

#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateFilter.h>

namespace geos {
    namespace geom { // geos::geom


        class GEOS_DLL GraphicTranslateFilter : public CoordinateFilter {
        public:
            GraphicTranslateFilter(double dx, double dy) : dx(dx), dy(dy) {}

            void filter_ro(const CoordinateXY* coord) override {
            }

            void filter_ro(const Coordinate* c) override {
            }

            void filter_ro(const CoordinateXYM* c) override {
            }

            void filter_ro(const CoordinateXYZM* c) override {
            }

            void filter_rw(CoordinateXY* c) const override {
                c->x += dx;
                c->y += dy;
            }

            void filter_rw(Coordinate* c) const override {
                c->x += dx;
                c->y += dy;
            }

            void filter_rw(CoordinateXYM* c) const override {
                c->x += dx;
                c->y += dy;
            }

            void filter_rw(CoordinateXYZM* c) const override {
                c->x += dx;
                c->y += dy;
            }

        private:
            double dx, dy;
        };


    }
}
