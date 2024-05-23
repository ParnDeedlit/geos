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

        enum GEOS_DLL GraphicAlgorithm {
            /* 当前图层 基于自身 就可以生成新的图层，要素1对1映射  */
            Self = 0,
            /* 当前图层 基于自身 就可以生成新的图层，要素1对多映射  */
            SelfSplit = 1,
            /* 当前图层 基于 其他图层 生成新的图层 */
            /* 区别部分 = 当前几何部分 - 求交部分 */
            OtherDifference = 2,
            /* 区别部分 = 当前几何部分 - 其他图层缓冲后求交部分 */
            OtherBufferDifference = 3,
            /* 求并部分 = 当前几何部分 + 其他图层部分 */
            OtherUnion = 4,
            /* 求并部分 = 当前几何部分 + 其他图层部分缓冲 */
            OtherBufferUnion = 5
        };

    } // namespace geos::geom
} // namespace geos


#ifdef _MSC_VER
#pragma warning(pop)
#endif


