/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 *
 * @file   FITKAbaqusPart.h
 * @brief  Abaqus Part 的数据结构
 * @author fulipeng (flipengqd@yeah.net)
 * @date   2024-03-15
 *
 *********************************************************************/
#ifndef __FITKABAQUS_PART_H__
#define __FITKABAQUS_PART_H__

#include "FITKAbaqusDataAPI.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractPart.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"

namespace Interface
{
    class FITKAbstractMesh;
    class FITKAbaMeshPartVTK;
    class FITKComponentManager;
    class FITKSectionAssignManager;
    class FITKAbaPartEngineeringFeature;
    class FITKModelSet;
    class FITKCoordinateSystemManager;
    class FITKAbaMaterialOrientationManager;
    class FITKAbaBeamOrientationManager;
    class FITKAbsGeoCommand;
    class FITKAbsGeoShapeAgent;
    // class FITKDatumList;
}

namespace AbaqusData
{
    class FITKGlobalMeshingParameter;
    class FITKLineMeshingParameter;
    class FITKSurfaceMeshingParameter;
    class FITKVolumeMeshingParameter;
    /**
     * @brief Abaqus Part 的数据结构
     * @author fulipeng (flipengqd@yeah.net)
     * @date 2024-03-15
     */
    class FITKAbaqusDataAPI FITKAbaqusPart : public Interface::FITKAbstractPart
    {
        //FITKSafeDownCastA(FITKAbaqusPart, Core::FITKAbstractDataObject);
        FITKCLASS(AbaqusData, FITKAbaqusPart);
    public:
        explicit FITKAbaqusPart();
        virtual ~FITKAbaqusPart();

        /**
         * @brief       部件是否有几何。
         * @return      是否有几何
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-27
         */
        bool hasGeometry();

        /**
         * @brief       部件是否有网格。
         * @return      是否有网格
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-27
         */
        bool hasMesh();

        /**
         * @brief       部件是否有独立网格。
         * @return      是否有独立网格
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-15
         */
        bool hasOrphanMesh();

        /**
         * @brief       部件是否有关联网格。
         * @return      是否有关联网格
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-27
         */
        bool hasNativeMesh();

        /**
         * @brief       初始化几何部件。
         * @param[in]   geomType：需要实例化的几何命令类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-03
         */
        void initializeGeomPart(Interface::FITKGeoEnum::FITKGeometryComType geomType);

        /**
         * @brief       析构当前部件几何数据。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-03
         */
        void removeGeomPart();

        /**
         * @brief 获取模型类型
         * @return 模型类型
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-03-15
         */
        Interface::FITKModelEnum::AbsModelType getAbsModelType();

        /**
         * @brief 获取part下的网格
         * @return 网格数据
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-03-15
         */
        Interface::FITKAbaMeshPartVTK* getMesh();

        /**
         * @brief       获取抽象类型网格。[重写]
         * @return      网格数据
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-26
         */
        Core::FITKAbstractNDataObject* getAbstractMesh() override;

        /**
         * @brief       获取抽象类型几何。[重写]
         * @return      几何数据
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-26
         */
        Core::FITKAbstractNDataObject* getAbstractGeom() override;

        /**
         * @brief 获取集合/表面管理器
         * @return Interface::FITKModelSetManager*
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-03-17
         */
        Interface::FITKComponentManager* getComponentManager() override;

        /**
        * @brief 根据绝对成员获取集合，不存在则创建新的集合
        * @param[i] t             集合类型
        * @param[i] member        成员
        * @return FITKModelSet*
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-05-29
        */
        Interface::FITKModelSet* getModelSetByMember(Interface::FITKModelEnum::FITKModelSetType t, const QList<int>  &member);

        /**
         * @brief 获取截面（section）指派管理器
         * @return 截面（section）指派管理器
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2024-03-22
         */
        Interface::FITKSectionAssignManager* getSectionAssignManager();
        /**
         * @brief 获取工程特征
         * @return FITKAbaPartEngineeringFeature
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2024-05-27
         */
        Interface::FITKAbaPartEngineeringFeature* getPartEngineerFeature();

        /**
         * @brief 获取坐标管理器
         * @return FITKCoordinateSystemManager
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2024-06-06
         */
        Interface::FITKCoordinateSystemManager* getCoordinateSystemManager();
        /**
         * @brief 获取材料方向管理器
         * @return FITKAbaMaterialOrientationManager
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2024-06-06
         */
        Interface::FITKAbaMaterialOrientationManager* getMaterialOrientationManager();
        /**
         * @brief 获取Beam方向管理器
         * @return Interface::FITKAbaBeamOrientationManager*
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-04
         */
        Interface::FITKAbaBeamOrientationManager* getBeamOrientationManager();

        /**
         * @brief       获取基准元素管理器。
         * @return      基准元素管理器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-02
         */
         // Interface::FITKDatumList* getDatumManager();

         /**
          * @brief       获取基准元素列表。
          * @return      基准元素ID列表
          * @author      ChengHaotian (yeguangbaozi@foxmail.com)
          * @date        2025-04-22
          */
        const QList<int> & getDatumList();

        /**
         * @brief       添加基准元素ID。
         * @param[in]   id：基准元素数据ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-22
         */
        void addDatum(int id);

        /**
         * @brief       移除基准元素ID，析构对象。
         * @param[in]   id：基准元素数据ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-22
         */
        void removeDatum(int id);

        /**
         * @brief 获取模型维度
         * @return FITKModelDim
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-11
         */
        virtual Interface::FITKModelEnum::FITKModelDim getModelDim() override;

        /**
         * @brief 获取点坐标
         * @param[i]  pointID  点ID
         * @param[o]  coor 节点坐标
         * @param[i]  modelIndex 模型索引，用于复合模型（一个模型包含多个子模型）
         * @return  bool 是否获取成功
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-19
         */
        virtual bool getPointCoor(int pointID, double* coor, int modelIndex = 0) override;
        /**
        * @brief 通过命令设置形状代理器ID
        * @param[i]  ID  几何命令ID
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-11-20
        */
        void setGeoCommand(const int cid);
        /**
        * @brief 获取几何命令
        * return  Interface::FITKAbsGeoShapeAgent*
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-11-20
        */
        Interface::FITKAbsGeoCommand* getGeoCommandObj();

        /**
         * @brief       获取指定类型几何命令对象。
         * @return      几何命令
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        template<class T>
        T* getGeoCommandObjAs()
        {
            return dynamic_cast<T*>(getGeoCommandObj());
        }

        /**
        * @brief 获取几何命令ID
        * return  Interface::FITKAbsGeoShapeAgent*
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2025-02-07
        */
        int getGeoCommandObjID();

        /**
         * @brief       获取线网格划分参数施加的线ID列表。
         * @return      线ID列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-10
         */
        QList<int> getExistLineTopoIds();

        /**
         * @brief       获取面网格划分参数施加的面ID列表。
         * @return      面ID列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-25
         */
        QList<int> getExistSurfaceTopoIds();

        /**
         * @brief       获取体网格划分参数施加的体ID列表。
         * @return      体ID列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-25
         */
        QList<int> getExistVolumeTopoIds();

        /**
         * @brief       获取默认网格划分参数。
         * @return      网格划分参数
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-27
         */
        FITKGlobalMeshingParameter* getGlobalMeshingParameters();

        /**
         * @brief       获取线单元划分参数。
         * @param[in]   vTopoID：虚拓扑ID
         * @param[in]   isCreate：是否创建[缺省]
         * @return      线单元划分参数
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        FITKLineMeshingParameter* getLineMeshingParameters(int vTopoID, bool isCreate = true);

        /**
         * @brief       清除线单元划分参数。
         * @param[in]   cmd
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        void clearLineMeshingParameters(Interface::FITKAbsGeoCommand* cmd = nullptr);

        /**
         * @brief   获取面网格划分参数 如果链表中有则直接返回 如果没有则创建一个
         * @param   vTopoID   虚拓扑ID
         * @param   isCreate  是否创建
         * @return  面网格划分参数
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-04-10
         */
        FITKSurfaceMeshingParameter* getSurfaceMeshingParameters(int vTopoID, bool isCreate = true);
        /**
         * @brief   清除所有面网格参数
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-05-14
         */
        void clearSurfaceMeshingParameters(Interface::FITKAbsGeoCommand* cmd = nullptr);
        /**
         * @brief   获取体网格划分参数 如果链表中有则直接返回 如果没有则创建一个
         * @param   vTopoID   虚拓扑ID
         * @param   isCreate  是否创建
         * @return  面网格划分参数
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-04-10
         */
        FITKVolumeMeshingParameter* getVolumeMeshingParameters(int vTopoID, bool isCreate = true);

        /**
         * @brief   清除所有体网格参数
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-05-14
         */
        void clearVolumeMeshingParameters(Interface::FITKAbsGeoCommand* cmd = nullptr);
        /**
         * @brief       清除几何缓存数据：几何发生变化时同步删除其对应的关联网格、表面网格
         *              划分参数、体网格划分参数、全局种子点、边种子点等几何关联数据。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-26
         */
        void clearGeomCache();

        /**
         * @brief       设置数据正在进行修改标记。
         * @param[in]   flag：数据是否正在进行修改
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-02
         */
        void setDataChangingFlag(bool flag);

        /**
         * @brief       获取数据正在进行修改标记。
         * @return      数据是否正在进行修改
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-02
         */
        bool getDataChangingFlag();

        /**
         * @brief       获取几何包围盒。
         * @param[out]  min：最小点
         * @param[out]  max：最大点
         * @return      是否存在包围盒
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-04
         */
        bool getGeomBoundary(double* min, double* max);

        /**
         * @brief       获取网格包围盒。
         * @param[out]  min：最小点
         * @param[out]  max：最大点
         * @return      是否存在包围盒
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-04
         */
        bool getMeshBoundary(double* min, double* max);

        /**
         * @brief       获取部件包围盒。
         * @param[out]  min：最小点
         * @param[out]  max：最大点
         * @return      是否存在包围盒
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-04
         */
        bool getBoundary(double* min, double* max);

        /**
         * @brief       判断模型组件是否有效。[虚函数][重写]
         * @param[in]   compType：组件类型
         * @param[in]   members：组件成员ID
         * @return      是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-02-03
         */
        virtual bool isComponentValid(Interface::FITKModelEnum::FITKModelSetType compType, const QList<int> & members) override;

    private:
        /***
       * @brief 设置数据对象ID，需谨慎使用
       * @param ID 数据对象id
       * @author LiBaojun (libaojunqd@foxmail.com)
       * @date 2024-06-18
       */
        void setAbsDataID(const int id) override;

    private:
        /**
         * @brief 几何模型命令ID
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-03-17
         */
        int _geoCommandID{ -1 };
        /**
         * @brief 网格数据
         * @author fulipeng (flipengqd@yeah.net)
         * @date 2024-03-15
         */
        Interface::FITKAbaMeshPartVTK* _mesh{};
        /**
         * @brief 集合/表面管理器
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-03-17
         */
        Interface::FITKComponentManager* _setSurfManager{};

        /**
         * @brief 截面（section）指派管理器
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2024-03-22
         */
        Interface::FITKSectionAssignManager* _sectionAssignManager{};
        /**
         * @brief 工程特征，包含转动惯量等
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2024-05-27
         */
        Interface::FITKAbaPartEngineeringFeature* _engineeringFeature{};
        /**
         * @brief 局部坐标系
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2024-06-06
         */
        Interface::FITKCoordinateSystemManager* _csysManager{};

        /**
         * @brief 材料方向管理
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2024-06-06
         */
        Interface::FITKAbaMaterialOrientationManager* _materialOrientationManager{};

        /**
         * @brief beam方向管理
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-04
         */
        Interface::FITKAbaBeamOrientationManager* _beamOrientationManager{};

        /**
         * @brief       部件基准元素管理器。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-02
         */
         // Interface::FITKDatumList* m_datumManager{ nullptr };

         /**
          * @brief       部件基准元素ID。
          * @author      ChengHaotian (yeguangbaozi@foxmail.com)
          * @date        2025-04-22
          */
        QList<int> m_datumIDs;

        /**
         * @brief       通用网格划分参数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-27
         */
        FITKGlobalMeshingParameter* m_globalMeshParam{ nullptr };

        /**
         * @brief       线单元划分参数，key为虚拓扑ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        QHash<int, FITKLineMeshingParameter*> _lineMesingParameter{};

        /**
         * @brief   面网格划分参数 key为虚拓扑ID
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-04-10
         */
        QHash<int, FITKSurfaceMeshingParameter*> _surfaceMesingParameter{};

        /**
         * @brief   体网格划分参数 key为虚拓扑ID
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-04-10
         */
        QHash<int, FITKVolumeMeshingParameter*> _volumeMesingParameter{};
        /**
         * @brief       数据正在进行修改标记。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-02
         */
        bool _dataChanging = false;

    };



    /**
     * @brief Abaqus Part 的数据管理器
     * @author fulipeng (flipengqd@yeah.net)
     * @date 2024-03-15
     */
    class FITKAbaqusDataAPI FITKAbaqusPartManager : public Core::FITKAbstractObject,
        public Core::FITKAbstractDataManager<FITKAbaqusPart>
    {
    public:
        explicit FITKAbaqusPartManager() = default;
        virtual ~FITKAbaqusPartManager() = default;
        /**
        * @brief 移除对象
        * @param[i]  obj            需要移除的对象
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-05-03
        */
        void removeDataObj(Core::FITKAbstractDataObject* obj) override;
    };
}



#endif
