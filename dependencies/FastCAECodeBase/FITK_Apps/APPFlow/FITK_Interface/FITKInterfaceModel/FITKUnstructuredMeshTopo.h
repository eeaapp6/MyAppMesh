/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef __FITK_UNSTRUCTURED_MESH_TOPO_H__
#define __FITK_UNSTRUCTURED_MESH_TOPO_H__

/*********************************************************************
 * @file   FITKUnstructuredMeshTopo.h
 * @brief  非结构化网格拓扑关系类
 * @author libaojun
 * @date   2025-09-05
 *********************************************************************/
#include "FITKInterfaceModelAPI.h"
#include <QObject>
#include <QHash>

namespace Core
{
    class FITKNode;
}

namespace Interface
{
    class FITKAbstractElement;
    class FITKElementList;
    class FITKUnstructuredMesh;
    
    /**
     * @brief  节点关联单元类，记录节点连接的单元信息
     * @author libaojun
     * @date   2025-09-05
     */
    class FITKInterfaceModelAPI NodeElements :public QObject
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
        void appendElement(FITKAbstractElement* element);
        
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
        QList<FITKAbstractElement*> getElements() const;
        
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
        void onElementDestroiedSlot(FITKAbstractElement* ele);
       
    private:
        Core::FITKNode* _node{nullptr};  ///< 节点指针
        QList<FITKAbstractElement*> _elements{};  ///< 关联的单元列表
    };

    /**
     * @brief  非结构化网格拓扑类，管理节点与单元的拓扑关系
     * @author libaojun
     * @date   2025-09-05
     */
    class FITKInterfaceModelAPI FITKUnstructuredMeshTopo: public QObject
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
        explicit FITKUnstructuredMeshTopo(FITKUnstructuredMesh* mesh);
        virtual ~FITKUnstructuredMeshTopo();
        
        /**
         * @brief  构建非结构化网格拓扑关系
         * @return 是否构建成功
         * @author libaojun
         * @date   2025-09-05
         */
        bool buildUnstructuredMeshTopo();

        /**
         * @brief  添加单元拓扑关系
         * @param  element 单元指针
         * @return 是否添加成功
         * @author libaojun
         * @date   2025-09-05
         */
        bool addElementTopo(FITKAbstractElement* element);

        /**
         * @brief  获取节点关联的单元列表
         * @param  nodeID 节点ID
         * @return 单元指针列表，若节点不存在或无关联单元则返回空列表
         * @author libaojun
         * @date   2025-09-05
         */
        QList<FITKAbstractElement*> getNodeElements(const int nodeID) const;
        
    private slots:
        /**
         * @brief  节点销毁槽函数
         * @param  node 被销毁的节点指针
         * @author libaojun
         * @date   2025-09-05
         */
        void onNodeDestroiedSlot(Core::FITKNode* node);

    protected:
        FITKUnstructuredMesh* _mesh{ nullptr };  ///< 非结构化网格指针
        QHash<int, NodeElements*> _nodeElementHash;  ///< 节点-单元映射表，key: nodeID, value: NodeElements*
    };
}

#endif