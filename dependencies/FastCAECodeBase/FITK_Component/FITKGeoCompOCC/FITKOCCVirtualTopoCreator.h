/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKOCCVirtualTopoCreator.h
 * @brief  OCC虚拟拓扑创建器
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-08-09
 * 
 */
#ifndef _FITK_VIRTUALTOPO_CREATOR_H___
#define _FITK_VIRTUALTOPO_CREATOR_H___

#include "FITKGeoCompOCCAPI.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"
#include "FITK_Kernel/FITKCore/FITKThreadTask.h"
#include <QHash>

class TopoDS_Shape;

namespace Interface
{
    class FITKVirtualTopoManager;
}

namespace OCC
{
    struct _IndiceMaps;
    class FITKAbstractOCCModel;

    /**
     * @brief OCC几何形状
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-08-09
     */
    class FITKGeoCompOCCAPI FITKOCCTopoShape : public Interface::FITKAbsVirtualTopoShape
    {
    public:
        /**
         * @brief Construct a new FITKOCCTopoShape object
         * @param[i]  shape          OCC形状对象
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        explicit FITKOCCTopoShape(const TopoDS_Shape & shape);
        /**
         * @brief Destroy the FITKOCCTopoShape object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        virtual ~FITKOCCTopoShape() = default;
        /**
         * @brief 判断两个形状是否相同
         * @param[i]  shape          OCC形状对象
         * @return true 相同
         * @return false 不同
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */ 
        bool isSameShape(void* shape) override;
        /**
         * @brief 获取OCC形状对象
         * @return const TopoDS_Shape& OCC形状对象
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */ 
        const TopoDS_Shape& getTopoShape() const;

    private:
         /**
          * @brief occ形状
          * @author libaojun (libaojunqd@foxmail.com)
          * @date 2024-08-09
          */
         TopoDS_Shape* _shape;
    };
     
     /**
      * @brief 几何拓扑创建
      * @author libaojun (libaojunqd@foxmail.com)
      * @date 2024-08-09
      */
    class  FITKOCCVirtualTopoCreator : public Core::FITKThreadTask
    {
    public:
        /**
         * @brief Construct a new FITKOCCVirtualTopoCreator object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        explicit FITKOCCVirtualTopoCreator(const TopoDS_Shape& shape, FITKAbstractOCCModel* shapeAgent,
            const QString& mode, bool& runLabel, bool keepTopos = false);
        /**
         * @brief Destroy the FITKOCCVirtualTopoCreator object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-09
         */
        virtual ~FITKOCCVirtualTopoCreator();

        /**
         * @brief 在线程中执行
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-15
         */
        void run() override;
         
        /**
        * @brief 创建几何拓扑
        * @param[i]  shape          OCC形状
        * @param[o]  topo           几何拓扑管理器
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-08-09
        */
        void createOCCTopos();

    private:
        /**
         * @brief 根节点形状
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-15
         */
        const TopoDS_Shape& _shape;

        /**
         * @brief       OCC模型对象。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-02-04
         */
        FITKAbstractOCCModel* _shapeAgent{ nullptr };

        ///**
        // * @brief 对应的管理器
        // * @author LiBaojun (libaojunqd@foxmail.com)
        // * @date 2024-08-15
        // */
        //Interface::FITKVirtualTopoManager* _topoMgr{};
        /**
         * @brief 名称
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-15
         */
        QString _modelName{};
        /**
         * @brief 临时存储的虚拓扑hash
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-15
         */
        QHash<Interface::FITKGeoEnum::VTopoShapeType,
            QHash<int, Interface::FITKAbsVirtualTopo*>  > _virtualTopoHash{};
        /**
         * @brief 是否在执行创建操作
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-15
         */
        bool& _runLabel;

        /**
         * @brief       是否保留未变化拓扑对象。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-02-04
         */
        bool _keepTopos = true;

    };


    /**
     * @brief 几何拓扑创建
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-08-09
     */
    class  FITKOCCVirtualTopoMapper : public Core::FITKThreadTask
    {
    public: 
        /**
         * @brief Construct a new FITKOCCVirtualTopoMapper object
         * @param[i]  shape           形状
         * @param[i]  shapeAgent      OCC模型
         * @param[i]  topo            该类型的几何拓扑
         * @param[i]  shapeTypeIndex  类型索引
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        explicit FITKOCCVirtualTopoMapper(const TopoDS_Shape& shape, FITKAbstractOCCModel* shapeAgent,
            QHash<Interface::FITKGeoEnum::VTopoShapeType, QHash<int, Interface::FITKAbsVirtualTopo*>> & topos,
            bool keepTopos = false, int cmdId = -1);
        /**
         * @brief Destroy the FITKOCCVirtualTopoMapper object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        virtual ~FITKOCCVirtualTopoMapper();
        /**
         * @brief 执行创建操作
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        void run() override;

    private:
        /**
         * @brief       递归生成虚拓扑对象。
         * @param[in]   shape：被拆分的形状[引用]
         * @param[out]  topos：生成的虚拓扑列表[引用]
         * @param[in]   parent：父节点数据
         * @param[in]   maps：索引字典
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-21
         */
        void recurCreateTopos(const TopoDS_Shape & shape, QList<Interface::FITKAbsVirtualTopo*> & topos, 
            Interface::FITKAbsVirtualTopo* parent, _IndiceMaps* maps);

    private:
        /**
         * @brief 形状
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        const TopoDS_Shape& _shape;

        /**
         * @brief 管理器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        Interface::FITKVirtualTopoManager* _topoMgr{};

        /**
         * @brief 该类型的几何拓扑
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-19
         */
        QHash<Interface::FITKGeoEnum::VTopoShapeType,
            QHash<int, Interface::FITKAbsVirtualTopo*>>& _topos;

        /**
         * @brief       根节点虚拓扑。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-21
         */
        Interface::FITKAbsVirtualTopo* _rootTopo{ nullptr };

        /**
         * @brief       形状字典。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-21
         */
        _IndiceMaps* _maps{ nullptr };

        /**
         * @brief       原始保留拓扑字典，key为模型hashcode。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-02-04
         */
        QHash<int, Interface::FITKAbsVirtualTopo*> _orgKeptTopoHash{};

        /**
         * @brief       是否保留未变化的拓扑对象。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-02-04
         */
        bool _keepTopos = false;

        /**
         * @brief       当前几何命令ID。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-02-04
         */
        int m_cmdId = -1;
    };
}


#endif
