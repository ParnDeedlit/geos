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

        enum GEOS_DLL GraphicAuxiliary {
            AuxiliaryDefault = 0,
            /* 区外环 缓冲 生成新的 区 */
            PolygonExteriorRingBuffer = 1,
            /* 区外环 缓冲 生成新的 区的外环线 */
            PolygonExteriorRingBufferExteriorRing = 2,
            /* 区内部最大距离线 */
            PolygonMaxLength = 3,
            /* 区内部最大距离线中心点 */
            PolygonMaxLengthCenter = 4,
            /* 区内外圈拆分成多个要素 */
            PolygonSplitExteriorRingAndInteriorRing = 5
        };

    } // namespace geos::geom
} // namespace geos


#ifdef _MSC_VER
#pragma warning(pop)
#endif


