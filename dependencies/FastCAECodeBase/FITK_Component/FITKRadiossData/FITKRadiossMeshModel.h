/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKRadiossMeshModel.h
 * @brief Radioss网格模型类
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-05-30
 * 
 */
#ifndef FITK_RADIOSS_MESH_MODEL_H
#define FITK_RADIOSS_MESH_MODEL_H

#include "FITKRadiossDataAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"

namespace Interface
{
    class FITKComponentManager;
    class FITKAbstractElement;
    class FITKElementList;
    class FITKNodeList;
}
namespace Core
{
    class FITKNode;
}

namespace Radioss
{
    class  FITKRadiossNodes;
    class  FITKRadiossPartManager;
    class  FITKRadiossSubSetManager;
    class  FITKRadiossWallManager;
    class  FITKRadiossMeshTopo;
    /**
     * @brief Radioss网格模型类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-05-30
     */
    class FITKRadiossDataAPI FITKRadiossMeshModel : public Interface::FITKAbstractModel
    {
    public:
        explicit FITKRadiossMeshModel();
        virtual ~FITKRadiossMeshModel();

        /**
         * @brief 获取模型类型
         * @return AbsModelType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        virtual Interface::FITKModelEnum::AbsModelType getAbsModelType() override;

        /**
         * @brief 获取节点边界框
         * @param[out]  bndBox      边界框数组，大小为6，依次为xmin, xmax, ymin, ymax, zmin, zmax
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-08-26
         */
        void getNodeBndBox(double *bndBox) const;

        /**
         * @brief 获取节点列表
         * @return FITKRadiossNodes* 节点列表指针
         * @author LiBaojun
         */
        FITKRadiossNodes* getNodes() const;
        /**
         * @brief 获取零件管理器
         * @return FITKRadiossPartManager* 零件管理器指针
         * @author LiBaojun
         */
        FITKRadiossPartManager* getPartsManager() const;

        /**
         * @brief 获取组件管理器
         * @return Interface::FITKComponentManager* 组件管理器指针
         * @author LiBaojun
         */
        Interface::FITKComponentManager* getComponentManager() const;
        /**
         * @brief 获取子集管理器
         * @return FITKRadiossSubSetManager* 子集管理器指针
         * @author LiBaojun
         */
        FITKRadiossSubSetManager* getSubSetManager() const;

        /**
         * @brief 获取网格拓扑
         * @return FITKRadiossMeshTopo* 网格拓扑指针
         * @author libaojun
         * @date   2025-09-18
         */
        FITKRadiossMeshTopo* getMeshTopo() const;
        /**
         * @brief    清理数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-04
         */
        void clearData();

    private:
        FITKRadiossNodes* _nodes{nullptr}; // Pointer to nodes
        FITKRadiossPartManager* _partsManager{nullptr}; // Pointer to parts manager
        FITKRadiossSubSetManager* _subSetManager{nullptr}; // Pointer to sub-set manager
        Interface::FITKComponentManager* _componentManager{nullptr}; // Pointer to component manager
        FITKRadiossMeshTopo* _meshTopo{ nullptr }; // Pointer to mesh topology
    };

     /**
     * @brief  节点关联单元类，记录节点连接的单元信息
     * @author libaojun
     * @date   2025-09-05
     */
    class FITKRadiossDataAPI NodeElements :public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief  构造函数
         * @param  node 节点指针
         * @author libaojun
         * @date   2025-09-05
         */
        explicit NodeElements(Core::FITKNode* node);
        virtual ~NodeElements() = default;

        /**
         * @brief  添加关联单元
         * @param  element 单元指针
         * @author libaojun
         * @date   2025-09-05
         */
        void appendElement(Interface::FITKAbstractElement* element);
        
        /**
         * @brief  获取节点指针
         * @return 节点指针
         * @author libaojun
         * @date   2025-09-05
         */
        Core::FITKNode* getNode() const;
        
        /**
         * @brief  获取关联的单元列表
         * @return 单元指针列表
         * @author libaojun
         * @date   2025-09-05
         */
        QList<Interface::FITKAbstractElement*> getElements() const;
        
        /**
         * @brief  获取关联的单元ID列表
         * @return 单元ID列表
         * @author libaojun
         * @date   2025-09-05
         */
        QList<int> getElementIDs() const;

    private slots:
        /**
         * @brief  单元销毁槽函数
         * @param  ele 被销毁的单元指针
         * @author libaojun
         * @date   2025-09-05
         */
        void onElementDestroiedSlot(Interface::FITKAbstractElement* ele);
       
    private:
        Core::FITKNode* _node{nullptr};  ///< 节点指针
        QList<Interface::FITKAbstractElement*> _elements{};  ///< 关联的单元列表
    };

    /**
     * @brief  非结构化网格拓扑类，管理节点与单元的拓扑关系
     * @author libaojun
     * @date   2025-09-05
     */
    class FITKRadiossDataAPI FITKRadiossMeshTopo: public QObject
    {
        Q_OBJECT
        friend class NodeElements;
    public:
        /**
         * @brief  构造函数
         * @param  mesh 非结构化网格指针
         * @author libaojun
         * @date   2025-09-05
         */
        explicit FITKRadiossMeshTopo(FITKRadiossMeshModel* mesh);
        virtual ~FITKRadiossMeshTopo();

        /**
         * @brief  构建非结构化网格拓扑关系
         * @return 是否构建成功
         * @author libaojun
         * @date   2025-09-05
         */
        bool updateAllMeshTopo();
        /**
         * @brief  添加部件单元拓扑关系
         * @param  eleList 单元列表指针
         * @author libaojun
         * @date   2025-09-05
         */
        void addPartTopo( Interface::FITKElementList* eleList);

        /**
         * @brief  添加单元拓扑关系
         * @param  element 单元指针
         * @return 是否添加成功
         * @author libaojun
         * @date   2025-09-05
         */
        bool addElementTopo(Interface::FITKNodeList* nodeList, Interface::FITKAbstractElement* element);

        /**
         * @brief  获取节点关联的单元列表
         * @param  nodeID 节点ID
         * @return 单元指针列表，若节点不存在或无关联单元则返回空列表
         * @author libaojun
         * @date   2025-09-05
         */
        QList<Interface::FITKAbstractElement*> getNodeElements(const int nodeID) const;
        
    private slots:
        /**
         * @brief  节点销毁槽函数
         * @param  node 被销毁的节点指针
         * @author libaojun
         * @date   2025-09-05
         */
        void onNodeDestroiedSlot(Core::FITKNode* node);

    protected:
        FITKRadiossMeshModel* _radiossMesh{ nullptr };  ///< Radioss网格模型指针
        QHash<int, NodeElements*> _nodeElementHash{};  ///< 节点-单元映射表，key: nodeID, value: NodeElements*
    };



} // namespace Radioss

#endif // FITK_RADIOSS_MESH_MODEL_H
