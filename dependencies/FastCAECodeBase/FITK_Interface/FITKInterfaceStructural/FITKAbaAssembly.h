/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbaAssembly.h
 * @brief  Abaqus装配体声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-22
 *
 */
#ifndef __FITKABAASSEMBLY_H__
#define __FITKABAASSEMBLY_H__

#include "FITK_Interface/FITKInterfaceModel/FITKAssembly.h"
#include "FITKInterfaceStructuralAPI.h"

namespace Interface
{
    class FITKComponentManager;
    class FITKNodeList;
    class FITKAbaRefPointManager;
    class FITKAbaRefPoint;
    class FITKAbstractModelComponent;
    class FITKAbaTransformManager;
    class FITKAbaWireManager;
    class FITKAbaAssemblyEngineeringFeature;
    class FITKModelSet;
    class FITKAbaConnectorAssignmentManager;

    /**
     * @brief Abaqus装配体set surface信息
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-22
     */
    struct AbaAssComponentInfo
    {
        QString _nameLabel{}; // 名称
        int _instanceID{ -1 }; //instance ID
        int _componentID{ -1 };  //set surface ID
        bool _internal{ false };  //是否内部集合
        bool _isNodeComp{ false }; //是否为节点类型
        bool _inheritFromPart{ false }; //从部件继承
        FITKAbstractModelComponent* _component{}; //set surf 本身对象
    };
    /**
     * @brief Abaqus装配体
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-22
     */
    class FITKInterfaceStructuralAPI FITKAbaAssembly : public Interface::FITKAssembly
    {
        FITKCLASS(Interface, FITKAbaAssembly);
    public:
        /**
         * @brief Construct a new FITKAbaAssembly object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        explicit FITKAbaAssembly() ;
        /**
         * @brief Destroy the FITKAbaAssembly object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        virtual ~FITKAbaAssembly();

        /**
        * @brief 清空数据
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-11-20
        */
        void clear() override; 
        /**
         * @brief 获取节点
         * @return FITKNodeList*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        FITKNodeList* getPoints();
        /**
         * @brief 获取集合表面管理器
         * @return FITKComponentManager*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        FITKComponentManager* getComponentManager() override;
        /**
         * @brief 获取参考点管理器
         * @return FITKAbaRefPointManager*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-29
         */
        FITKAbaRefPointManager* getRefPoints();
        /**
         * @brief 获取坐标变换管理器
         * @return FITKAbaTransformManager*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-25
         */
        FITKAbaTransformManager* getTransformManager();
        /**
         * @brief 获取线特征管理器
         * @return FITKModelWireManager*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-20
         */
        FITKAbaWireManager* getWireManager();
        /**
         * @brief 获取全部的集合信息
         * @param[i] recu 是否递归
         * @return QList<AbaAssComponentInfo>
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-04-29
         */
        QList<AbaAssComponentInfo> getSetInfo(bool recu = true);
        /**
         * @brief 获取全部的表面信息
         * @return QList<AbaAssComponentInfo>
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-04-08
         */
        QList<AbaAssComponentInfo> getSurfaceInfo();

        /**
         * @brief 根据集合获取参考点，不存在则创建参考点
         * @param[i] setid 集合ID，节点类型
         * @param[i] modelID 模型ID，集合所属的装配体ID或者instance等,非instance默认 -1
         * @return FITKAbaRefPoint
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-04-08
         */
        FITKAbaRefPoint* getRefPointBySet( const int setid,  int modelID = -1);
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
         * @brief 移除装配体instance，同步删除与instance相关集合、表面
         * @param[i] obj  装配实例
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-04-08
         */
        virtual void removeDataObj(FITKAbstractDataObject* obj) override;

        /**
         * @brief       按索引移除装配实例，同步删除相关集合与表面。[虚函数][重写]
         * @param[in]   dataIndex：数据索引
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-24
         */
        virtual void removeDataByIndex(int dataIndex) override;

        /**
         * @brief       按数据ID移除装配实例，同步删除相关集合与表面。[虚函数][重写]
         * @param[in]   dataId：数据ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-24
         */
        virtual void removeDataByID(int dataId) override;

        /**
         * @brief 获取工程特征
         * @return FITKAbaPartEngineeringFeature
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2024-05-27
         */
        Interface::FITKAbaAssemblyEngineeringFeature* getAssemblyEngineerFeature();
        /**
         * @brief 获取连接器截面指派管理器
         * @return FITKAbaConnectorAssignmentManager
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2024-06-24
         */
        Interface::FITKAbaConnectorAssignmentManager* getConnectorAssignManager();

        /**
        * @brief 根据绝对成员获取集合，不存在则创建新的集合
        * @param[i] t             集合类型
        * @param[i] modelID       模型ID，装配体或者instance
        * @param[i] member        成员
        * @return FITKModelSet*
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-05-29
        */
        Interface::FITKModelSet* getModelSetByMember(Interface::FITKModelEnum::FITKModelSetType t, 
            const int modelID, const QList<int>  &member);

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
         * @brief 点列表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        FITKNodeList* _points{};
        /**
         * @brief 集合/表面管理器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        FITKComponentManager* _setSurfManager{};
        /**
         * @brief 参考点管理器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-29
         */
        FITKAbaRefPointManager* _refPoints{};
        /**
         * @brief 坐标变换管理器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-29
         */
        FITKAbaTransformManager* _transformManager{};
        /**
        * @brief 线特征管理器
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-06-20
        */
        FITKAbaWireManager*_wireManager{};
        /**
         * @brief 工程特征，包含转动惯量等
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2024-05-27
         */
        Interface::FITKAbaAssemblyEngineeringFeature* _engineeringFeature{};
        /**
         * @brief 连接器指派管理器
         * @author libaojun (libaojunqd@foxmail.conm)
         * @date 2024-06-24
         */
        Interface::FITKAbaConnectorAssignmentManager* _connectorAssignManager{};
    };
}



#endif
