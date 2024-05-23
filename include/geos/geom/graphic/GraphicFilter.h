#pragma once

#include <geos/export.h>

#include <string>
#include <vector>
#include <memory> // for unique_ptr

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251) // warning C4251: needs to have dll-interface to be used by clients of class
#endif


namespace geos {
    namespace geom { // geos::geom

        enum GEOS_DLL GraphicFilter {
            /* 全计算 */
            FilterAll = 0,
            /* 过滤点 */
            FilterPoint = 1,
            /* 过滤线 */
            FilterLineString = 2,
            /* 过滤区 */
            FilterPolygon = 3,
        };

    } // namespace geos::geom
} // namespace geos


#ifdef _MSC_VER
#pragma warning(pop)
#endif


