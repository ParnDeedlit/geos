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
            /* 默认区 */
            PolygonDefault = 1,
            /* 区外环 缓冲 生成新的 区 */
            PolygonExteriorRingBuffer = 2,
            /* 区外环 缓冲 生成新的 区的外环线 */
            PolygonExteriorRingBufferExteriorRing = 3,
            /* 区外环 不缓冲 生成新的 区 */
            PolygonExteriorRing = 4,
            /* 区外环 不缓冲 生成新的 区的外环线 */
            PolygonExteriorRingToExteriorRing = 5,
            /* 区外环内环 不缓冲 生成新的 区的外环线 */
            PolygonExteriorRingAndInteriorRingToExteriorRing = 6,
            /* 区内部最大距离线 */
            PolygonMaxLength = 7,
            /* 区内部最大距离线中心点 */
            PolygonMaxLengthCenter = 8,
            /* 区内外圈拆分成多个要素 */
            PolygonSplitExteriorRingAndInteriorRing = 9,
            /* 默认区 */
            PolygonTranslate = 10,
            LineDefault = 11,

            PointDefault = 12,
            /* 点缓冲生成 新区 */
            PointBuffer = 13,
        };

    } // namespace geos::geom
} // namespace geos


#ifdef _MSC_VER
#pragma warning(pop)
#endif


