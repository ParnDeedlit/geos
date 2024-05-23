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
            /* 区外环 不缓冲 生成新的 区 */
            PolygonExteriorRing = 3,
            /* 区外环 不缓冲 生成新的 区的外环线 */
            PolygonExteriorRingToExteriorRing = 4,
            /* 区外环内环 不缓冲 生成新的 区的外环线 */
            PolygonExteriorRingAndInteriorRingToExteriorRing = 5,
            /* 区内部最大距离线 */
            PolygonMaxLength = 6,
            /* 区内部最大距离线中心点 */
            PolygonMaxLengthCenter = 7,
            /* 区内外圈拆分成多个要素 */
            PolygonSplitExteriorRingAndInteriorRing = 8,

            LineDefault = 9,

            PointDefault = 10,
            /* 点缓冲生成 新区 */
            PointBuffer = 11,
        };

    } // namespace geos::geom
} // namespace geos


#ifdef _MSC_VER
#pragma warning(pop)
#endif


