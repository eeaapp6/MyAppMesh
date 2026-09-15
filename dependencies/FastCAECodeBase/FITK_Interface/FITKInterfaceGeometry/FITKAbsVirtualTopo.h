/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKAbsVirtualTopo.h
 * @brief 几何虚拟拓扑接口
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-08-09
 * 
 */
#ifndef _FITK_ABS_VIRTUAL_TOPO_H___
#define _FITK_ABS_VIRTUAL_TOPO_H___

#include "FITKInterfaceGeometryAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITKGeoEnum.h"
#include <qcolor.h>
#include <QMutex>
#include <QColor>

namespace Interface
{
    /**
     * @brief 几何形状代理
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-08-09
     */
    class FITKInterfaceGeometryAPI FITKAbsVirtualTopoShape
    {
    public:
        explicit FITKAbsVirtualTopoShape() = default;
        virtual ~FITKAbsVirtualTopoShape() = 0;
        /**
         * @brief 是否为同一个形状
         * @param[i] 是同一个形状
         * @return true 
         * @return false 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        virtual bool isSameShape( void* ) = 0;
    };
    /**
     * @brief 几何虚拓扑对象
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-08-09
     */
    class FITKInterfaceGeometryAPI FITKAbsVirtualTopo : public Core::FITKAbstractDataObject
    {
        FITKCLASS(Interface, FITKAbsVirtualTopo);
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKAbsVirtualTopo object
         * @param[i]  type           类型
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        explicit FITKAbsVirtualTopo(FITKGeoEnum::VTopoShapeType type = FITKGeoEnum::VSNone);
        /**
         * @brief 析构，不会释放子拓扑内存
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        virtual ~FITKAbsVirtualTopo();
        /**
         * @brief 获取类型
         * @return VTopoShapeType 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        FITKGeoEnum::VTopoShapeType getShapeType()const;
        /**
         * @brief 设置类型
         * @param[i]  s           类型
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */ 
        void setShapeType(FITKGeoEnum::VTopoShapeType s);
        /**
         * @brief 添加子拓扑
         * @param[i]  subTopo           子拓扑
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        void addSubTopo(FITKAbsVirtualTopo* subTopo);

        /**
         * @brief       移除子拓扑。
         * @param[in]   subTopo：子拓扑
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-07
         */
        void removeSubTopo(FITKAbsVirtualTopo* subTopo);

        /**
         * @brief 获取子拓扑数量
         * @return int 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        int getSubTopoCount();
        /**
         * @brief 获取子拓扑
         * @param[i]  index          索引
         * @return FITKAbsVirtualTopo* 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        FITKAbsVirtualTopo* getSubTopo(const int index);

        /**
         * @brief 获取父拓扑数量
         * @return int
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        int getParentTopoCount();
        /**
         * @brief 获取父拓扑
         * @param[i]  index          索引
         * @return FITKAbsVirtualTopo*
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        FITKAbsVirtualTopo* getParentTopo(const int index);

        /**
         * @brief 获取根拓扑
         * @return FITKAbsVirtualTopo*
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-08-09
         */
        FITKAbsVirtualTopo* getRootTopo();

        template<class T>
        /**
         * @brief 获取子拓扑，并强制转换类型
         * @param[i]  index       索引
         * @return T* 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        T* getTSubTopo(const int index)
        {
            return dynamic_cast<T*>(this->getSubTopo(index));
        }

        /**
         * @brief       清理父拓扑列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-26
         */
        void clearParentTopo();

        /**
         * @brief 设置形状
         * @param[i]  shape          形状
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        void setShape(FITKAbsVirtualTopoShape* shape);
        /**
         * @brief 获取形状
         * @return FITKAbsVirtualTopoShape* 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        FITKAbsVirtualTopoShape* getShape();

        template<class T>
        /**
         * @brief 获取形状，并强制转换类型
         * @return T* 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        T* getShapeT()
        {
            return dynamic_cast<T*>(_shape);
        }
        /**
         * @brief 设置索引标记
         * @param[i] index  索引
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-27
         */
        void setIndexLabel(const int index);
        /**
         * @brief 获取索引标记
         * @return int
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-27
         */
        int getIndexLabel() const;

        /**
         * @brief       设置几何标记。
         * @param[in]   tag：几何标记
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-26
         */
        void setTag(const int tag);

        /**
         * @brief       获取几何标记
         * @return      几何标记
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-26
         */
        int getTag();

        /**
         * @brief       设置创建该拓扑的几何命令ID。
         * @param[in]   id：几何命令ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-18
         */
        void setCreatedGeoCmdID(int id);

        /**
         * @brief       获取创建该拓扑的几何命令ID。
         * @return      几何命令ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-18
         */
        int getCreatedGeoCmdID();

        /**
         * @brief       获取合并到的拓扑对象。
         * @return      合并到的拓扑对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-02
         */
        FITKAbsVirtualTopo* getCombinedToTopo();

        /**
         * @brief       获取合并拓扑对应的真实拓扑对象数量。
         * @return      合并的子拓扑对象数量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-25
         */
        int getCombinedTopoCount();

        /**
         * @brief       根据索引获取合并拓扑对应的真实拓扑对象。
         * @param[in]   index：索引
         * @return      合并的子拓扑对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-25
         */
        FITKAbsVirtualTopo* getCombinedTopo(const int index);

        /**
         * @brief       递归获取当前拓扑合并的子拓扑对象。
         * @return      递归查询的合并的子拓扑对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-25
         */
        QList<FITKAbsVirtualTopo*> getCombinedBasicToposRecur();

        /**
         * @brief       设置合并到的拓扑对象。
         * @param[in]   vTopo：拓扑对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-02
         */
        void setCombinedToTopo(FITKAbsVirtualTopo* vTopo);

        /**
        * @brief       添加被合并拓扑对象。
        * @param[in]   vTopo：拓扑对象
        * @return      是否成功追加合并拓扑
        * @author      ChengHaotian (yeguangbaozi@foxmail.com)
        * @date        2025-06-25
        */
        bool addCombinedTopo(FITKAbsVirtualTopo* vTopo);

        /**
         * @brief       移除被合并拓扑对象。
         * @param[in]   vTopo：拓扑对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-02
         */
        void removeCombinedTopo(FITKAbsVirtualTopo* vTopo);

        /**
         * @brief       清空被合并拓扑对象。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-07
         */
        void clearCombinedTopo();

        /**
         * @brief 清空子拓扑， 不会清理内存
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        void clearSubTopo();

        /**
         * @brief       设置颜色。
         * @param[in]   color：颜色
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        void setColor(const QColor & color);

        /**
         * @brief       获取模型颜色。
         * @return      颜色
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        QColor getColor() const;

    protected:
        /**
        * @brief 添加父对象
        * @param[i]  pTopo       父对象
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-08-09
        */
        void addParentTopo(FITKAbsVirtualTopo* pTopo);

        /**
         * @brief       移除父拓扑。
         * @param[in]   pTopo：父拓扑
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-07
         */
        void removeParentTopo(FITKAbsVirtualTopo* pTopo);

    protected:
        /**
         * @brief 父对象
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        QList<FITKAbsVirtualTopo*> _parents{};
        /**
         * @brief 形状类型
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        FITKGeoEnum::VTopoShapeType _topoShapeType{ FITKGeoEnum::VSNone };
        /**
         * @brief 形状
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */ 
        FITKAbsVirtualTopoShape* _shape{};
        /**
         * @brief 子拓扑
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        QList<FITKAbsVirtualTopo*> _subTopo{};
        /**
         * @brief 索引标记
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-27
         */
        int _indexLabel{ -1 };

        /**
         * @brief       几何引擎提供的几何标记。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-26
         */
        int _tag = -1;

        /**
         * @brief       合并到的拓扑对象。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-02
         */
        FITKAbsVirtualTopo* _combinedToTopo{ nullptr };

        /**
         * @brief       合并拓扑对应的子拓扑对象。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-25
         */
        QList<FITKAbsVirtualTopo*> _combinedTopos{};

        /**
         * @brief       创建拓扑的几何命令ID。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-18
         */
        int m_createdCmdId = -1;

        /**
         * @brief       模型颜色。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        QColor m_color;

    };

}


#endif
