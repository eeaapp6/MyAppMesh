/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKUnstructuredMesh.h"
#include "FITKAbstractElement.h"
#include "FITKModelEnum.h"
#include "FITKUnstructuredMeshTopo.h"
#include "FITKGeoToMeshMapper.h"

#include "FITK_Kernel/FITKCore/FITKVec3DAlg.h"

namespace Interface
{
    FITKUnstructuredMesh::FITKUnstructuredMesh()
    {
        _geoToMeshMapper = new FITKGeoToMeshMapper();
    }
    FITKUnstructuredMesh::~FITKUnstructuredMesh()
    {
        if (_meshTopo) delete _meshTopo;

        if (_geoToMeshMapper) {
            delete _geoToMeshMapper;
            _geoToMeshMapper = nullptr;
        }
    }
    Interface::FITKModelEnum::AbsModelType FITKUnstructuredMesh::getAbsModelType()
    {
        //抽象类型
        return FITKModelEnum::AbsModelType::AMTunstructuredMesh;
    }

    bool FITKUnstructuredMesh::isComponentValid(FITKModelEnum::FITKModelSetType compType, const QList<int> & members)
    {
        switch (compType)
        {
        case Interface::FITKModelEnum::FMSNode:
        {
            for (const int & member : members)
            {
                if (!this->getNodeAt(member))
                {
                    return false;
                }
            }

            break;
        }       
        case Interface::FITKModelEnum::FMSElem:
        {
            for (const int & member : members)
            {
                if (!this->getElementAt(member))
                {
                    return false;
                }
            }

            break;
        }
        default:
            return false;
        }

        return true;
    }

    void FITKUnstructuredMesh::update()
    {
        //更新
    }

    void FITKUnstructuredMesh::clearMesh()
    {
        this->removeAllNode();
        this->removeAllElement();
    }

    FITKModelEnum::FITKMeshDim FITKUnstructuredMesh::getMeshDim()
    {
        //错误判断
        if (_elementList.isEmpty()) return FITKModelEnum::FITKMeshDim::FMDimNone;
        FITKAbstractElement* ele = _elementList.at(0);
        if (ele == nullptr) return FITKModelEnum::FITKMeshDim::FMDimNone;
        //第一个单元类型
        FITKModelEnum::FITKEleType type = ele->getEleType();
        FITKModelEnum::FITKMeshDim dim = FITKModelEnum::GetElementDim(type);

        //遍历单元
        const int n = this->getElementCount();
        for (int i =1; i<n; ++i)
        {
            FITKAbstractElement* elei = _elementList.at(i);
            if(elei == nullptr) continue;
            //第i个单元与第一个单元维度是否一致
            FITKModelEnum::FITKEleType typei = elei->getEleType();
            FITKModelEnum::FITKMeshDim dimi = FITKModelEnum::GetElementDim(typei);
            if (dimi != dim) return FITKModelEnum::FITKMeshDim::FMDimMix;
        }
        return dim;

    }

    FITKElemntQuality FITKUnstructuredMesh::checkElementQuality(const int eleIndex)
    {
        FITKElemntQuality q;
        //获取单元
        FITKAbstractElement* ele = this->getElementAt(eleIndex);
        if (ele == nullptr) return q;
        //遍历全部边
        const int n = ele->getEdgeCount();
        QList <double> edgeLength;
        for (int i =0;i< n; ++i)
        {
            QList<int> edge = ele->getEdge(i);
            //边的长度
            double l = this->calMeshEdgeLength(edge);
            if(l >0)
                edgeLength.append(l);
        }
        if (edgeLength.isEmpty()) return q;
        qSort(edgeLength);
        //记录最长最短边
        q._minEdgeLength = edgeLength.first();
        q._maxEdgeLength = edgeLength.last();
        //计算长宽比
        q._aspectRatio = q._maxEdgeLength / q._minEdgeLength;
        return q;
    }

    bool FITKUnstructuredMesh::getElementDirection(double* dir, const int id)
    {
        FITKAbstractElement* ele = this->getElementByID(id);
        if (ele == nullptr) return false;
        int dim = ele->getElementDim();
        switch (dim)
        {
        //线单元方向
        case 1: return this->getLineEleDirection(dir, ele);
        //壳单元方向
        case 2: return this->getShellEleDirection(dir, ele);
        default: break;
        }
        return false;
    }

    bool FITKUnstructuredMesh::hasOrphanMesh()
    {
        // 检查网格节点。
        for (Core::FITKNode* node : _nodeList)
        {
            if (!node)
            {
                continue;
            }

            if (!node->getNativeFlag())
            {
                return true;
            }
        }

        // 检查网格单元。
        for (FITKAbstractElement* ele : _elementList)
        {
            if (!ele)
            {
                continue;
            }

            if (!ele->getNativeFlag())
            {
                return true;
            }
        }

        return false;
    }

    bool FITKUnstructuredMesh::hasNativeMesh()
    {
        // 检查网格节点。
        for (Core::FITKNode* node : _nodeList)
        {
            if (!node)
            {
                continue;
            }

            if (node->getNativeFlag())
            {
                return true;
            }
        }

        // 检查网格单元。
        for (FITKAbstractElement* ele : _elementList)
        {
            if (!ele)
            {
                continue;
            }

            if (ele->getNativeFlag())
            {
                return true;
            }
        }

        return false;
    }

    void FITKUnstructuredMesh::clearNativeMesh()
    {
        // 移除抽象层网格数据。
        int nPoints = this->getNodeCount();
        int nEles = this->getElementCount();

        for (int i = nEles - 1; i >= 0; i--)
        {
            FITKAbstractElement* element = this->getElementAt(i);
            if (!element)
            {
                continue;
            }

            if (!element->getNativeFlag())
            {
                continue;
            }

            this->fastRemoveElementAt(i);
        }

        // 移除节点。
        for (int i = nPoints - 1; i >= 0; i--)
        {
            Core::FITKNode* node = this->getNodeAt(i);
            if (!node)
            {
                continue;
            }

            if (!node->getNativeFlag())
            {
                continue;
            }

            // 循环索引为节点索引。
            this->fastRemoveNodeAt(i);
        }

        // 重新构建ID索引映射。
        this->updateNodeIDIndexMap();
        this->updateElementIDIndexMap();

        // 重建VTK数据。
        update();

        //清除几何与网格映射关系
        if (_geoToMeshMapper) {
            _geoToMeshMapper->clear();
        }
    }

    void FITKUnstructuredMesh::clearOrphanMesh()
    {
        // 移除抽象层网格数据。
        int nPoints = this->getNodeCount();
        int nEles = this->getElementCount();

        for (int i = nEles - 1; i >= 0; i--)
        {
            FITKAbstractElement* element = this->getElementAt(i);            
            if (!element)
            {
                continue;
            }

            if (element->getNativeFlag())
            {
                continue;
            }

            this->fastRemoveElementAt(i);
        }

        // 移除节点。
        for (int i = nPoints - 1; i >= 0; i--)
        {
            Core::FITKNode* node = this->getNodeAt(i);
            if (!node)
            {
                continue;
            }

            if (node->getNativeFlag())
            {
                continue;
            }

            // 循环索引为节点索引。
            this->fastRemoveNodeAt(i);
        }

        // 重新构建ID索引映射。
        this->updateNodeIDIndexMap();
        this->updateElementIDIndexMap();

        // 重建VTK数据。
        update();
    }

    bool FITKUnstructuredMesh::buildUnstructuredMeshTopo()
    {
        if(_meshTopo == nullptr)
            _meshTopo = new FITKUnstructuredMeshTopo(this);
        return _meshTopo->buildUnstructuredMeshTopo();
    }

    double FITKUnstructuredMesh::calMeshEdgeLength(const QList<int> & edge)
    {
        double length = 0;
        //错误判断
        const int n = edge.size();
        if (n < 2) return -1;
        //计算全部的点的长度
        for (int i =0; i< n-1; ++i)
        {
            //相邻两个点的距离
            const Core::FITKNode* node1 = this->getNodeByID(edge[i]);
            const Core::FITKNode* node2 = this->getNodeByID(edge[i+1]);
            if(node1 && node2)
                length += Core::Distance(*node2, *node1);
        }
        return length;
    }

    bool FITKUnstructuredMesh::getLineEleDirection(double* dir, FITKAbstractElement* ele)
    {
        if (!ele) return false;
        //获取第一个边
        int ne = ele->getEdgeCount();
        if (ne <= 0) return false;
        QList<int> ed = ele->getEdge(0);
        if (ed.size() < 2) return false;
        //边上的两个点
        Core::FITKNode* n1 = this->getNodeByID(ed.first());
        Core::FITKNode* n2 = this->getNodeByID(ed.last());
        if (n1 == nullptr || n2 == nullptr) return false;
        //第一点到第二点的矢量
        Core::FITKVec3D v(*n2,*n1);
        for (int i =0; i<3; ++i)
            dir[i] = v[i];
        return true;
    }

    bool FITKUnstructuredMesh::getShellEleDirection(double* dir, FITKAbstractElement* ele)
    {
        if (!ele) return false;
        //至少三条边组成一个面
        int ne = ele->getEdgeCount();
        if (ne <= 2) return false;
        //获取第一条边
        QList<int> ed1 = ele->getEdge(0);
        if (ed1.size() < 2) return false;
        Core::FITKNode* n1 = this->getNodeByID(ed1.first());
        Core::FITKNode* n2 = this->getNodeByID(ed1.last());
        //获取第二条边
        QList<int> ed2 = ele->getEdge(1);
        if (ed2.size() < 2) return false;
        Core::FITKNode* n3 = this->getNodeByID(ed2.last());
        if (n1 == nullptr || n2 == nullptr || n3 == nullptr) return false;
        //两个矢量的叉乘为最终结果
        Core::FITKVec3D v1(*n2, *n1);
        Core::FITKVec3D v2(*n3, *n1);
        Core::FITKPoint p = Core::CrossProduct(v1, v2);
        //赋值输出
        for (int i = 0; i < 3; ++i)
            dir[i] = p[i];
        return true;
    }

    FITKUnstructuredMeshTopo *FITKUnstructuredMesh::getUnstructuredMeshTopo()
    {
        return _meshTopo;
    }

    bool FITKUnstructuredMesh::getPointCoor(int pointID, double* coor, int modelIndex)
    {
        Core::FITKNode* node = this->getNodeByID(pointID);
        if (!node)
        {
            return false;
        }

        node->getCoor(coor);

        return true;
    }

    FITKGeoToMeshMapper * FITKUnstructuredMesh::getGeoToMeshMapper()
    {
        return _geoToMeshMapper;
    }

    unsigned int FITKUnstructuredMesh::getMeshDimBit()
    {
        unsigned int dimBit = 0;
        if (_elementList.isEmpty()) return dimBit; 

        //遍历单元
        const int n = this->getElementCount();
        for (int i =0; i<n; ++i)
        {
            FITKAbstractElement* elei = _elementList.at(i);
            if(elei == nullptr) continue;
         
            FITKModelEnum::FITKEleType typei = elei->getEleType();
            FITKModelEnum::FITKMeshDim dimi = FITKModelEnum::GetElementDim(typei);
            dimBit |= static_cast<unsigned int>(dimi);
        }
        return dimBit;
    }
        
}
