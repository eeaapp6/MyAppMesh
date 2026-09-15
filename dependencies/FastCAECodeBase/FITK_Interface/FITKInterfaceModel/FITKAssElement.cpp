/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAssElement.h"
#include "FITKAbstractModel.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

namespace Interface
{
    //静态单元ID
    static int AssEleID = 0;
    FITKAbstractAssemblyElement::FITKAbstractAssemblyElement()
    {
        _elementID = ++AssEleID;
    }
    int FITKAbstractAssemblyElement::getElementID() const
    {
        return _elementID;
    }

    void FITKAbstractAssemblyElement::setElementID(const int id)
    {
        AssEleID = AssEleID > id ? AssEleID : id;
        _elementID = id;
    }

    /**
     * @brief 拷贝到目标单元
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-24
     */
    void FITKAbstractAssemblyElement::copyTo(FITKAbstractAssemblyElement*)
    {
    }

    FITKModelEnum::FITKModelSetType FITKAssemblyElementSet::getModelSetType()
    {
        return FITKModelEnum::FMSNone;
    }

    void FITKAssemblyElementVertex::copyTo(FITKAbstractAssemblyElement* e)
    {
        //类型转换
        FITKAssemblyElementVertex* v = dynamic_cast<FITKAssemblyElementVertex*>(e);
        if (v) return;
        //设置ID
        v->setModelID(_nodeModelID);
        v->setNodeID(_nodeID);
        v->setSubModelID(_nodeSubModelID);
    }

    void FITKAssemblyElementVertex::setModelID(const int mid)
    {
        _nodeModelID = mid;
    }

    void FITKAssemblyElementVertex::setSubModelID(const int subMid)
    {
        _nodeSubModelID = subMid;
    }

    void FITKAssemblyElementVertex::setNodeID(const int id)
    {
        _nodeID = id;
    }

    void FITKAssemblyElementVertex::getCoor(double* coor)
    {
        //获取模型
        if (coor == nullptr) return;
        auto m = FITKDATAREPO->getTDataByID<FITKAbstractModel>(_nodeModelID);
        if (m == nullptr) return;
        //获取坐标
        m->getPointCoor(_nodeID, coor);
    }

    void FITKAssemblyElementVertex::getNode(int& mid, int& id)
    {
        mid = _nodeModelID;
        id = _nodeID;
    }

    int FITKAssemblyElementVertex::getSubModelID()
    {
        return _nodeSubModelID;
    }

    void FITKAssemblyElementLine::copyTo(FITKAbstractAssemblyElement* e)
    {
        //类型转换
        FITKAssemblyElementLine* l = dynamic_cast<FITKAssemblyElementLine*>(e);
        if (l== nullptr) return;
        //设置节点
        l->setNode1(_node1.first, _node1.second, _subMid1);
        l->setNode2(_node2.first, _node2.second, _subMid2);
    }

    void FITKAssemblyElementLine::setNode1(const int mid, const int id, const int subMid)
    {
        _node1.first = mid;
        _node1.second = id;
        _subMid1 = subMid;
    }

    void FITKAssemblyElementLine::setNode2(const int mid, const int id, const int subMid)
    {
        _node2.first = mid;
        _node2.second = id;
        _subMid2 = subMid;
    }

    void FITKAssemblyElementLine::getNode1(int& mid, int& id)
    {
        mid = _node1.first;
        id = _node1.second;
    }

    void FITKAssemblyElementLine::getNode2(int& mid, int& id)
    {
        mid = _node2.first;
        id = _node2.second;
    }

    int FITKAssemblyElementLine::getSubModelID1()
    {
        return _subMid1;
    }

    int FITKAssemblyElementLine::getSubModelID2()
    {
        return _subMid2;
    }

    void FITKAssemblyElementLine::getCoor1(double* coor1)
    {
        //模型
        if (coor1 == nullptr) return;
        auto m = FITKDATAREPO->getTDataByID<FITKAbstractModel>(_node1.first);
        if (m == nullptr) return;

        //获取坐标
        auto subModel = FITKDATAREPO->getTDataByID<FITKAbstractModel>(_subMid1);
        if (subModel)
        {
            subModel->getPointCoor(_node1.second, coor1);
            m->transformPoint(coor1, coor1);
        }
        else
        {
            m->getPointCoor(_node1.second, coor1);
        }
    }

    void FITKAssemblyElementLine::getCoor2(double* coor2)
    {
        //模型
        if (coor2 == nullptr) return;
        auto m = FITKDATAREPO->getTDataByID<FITKAbstractModel>(_node2.first);
        if (m == nullptr) return;

        //获取坐标
        auto subModel = FITKDATAREPO->getTDataByID<FITKAbstractModel>(_subMid2);
        if (subModel)
        {
            subModel->getPointCoor(_node2.second, coor2);
            m->transformPoint(coor2, coor2);
        }
        else
        {
            m->getPointCoor(_node2.second, coor2);
        }
    }

    QString FITKAssemblyElementLine::getTextLabel1()
    {
        //模型
        auto m = FITKDATAREPO->getTDataByID<FITKAbstractModel>(_node1.first);
        if (m == nullptr) return QString();

        auto subMolde = FITKDATAREPO->getTDataByID<FITKAbstractModel>(_subMid1);
        if (subMolde)
        {
            QString modelNodeTypeName;
            Interface::FITKModelEnum::AbsModelType mType = subMolde->getAbsModelType();
            switch (mType)
            {
            case Interface::FITKModelEnum::AMTMesh:
            case Interface::FITKModelEnum::AMTunstructuredMesh:
            case Interface::FITKModelEnum::AMTStructuredMesh:
            case Interface::FITKModelEnum::AMTunstructuredMeshvtk:
                modelNodeTypeName = "Node";
                break;
            case Interface::FITKModelEnum::AMTGeometry:
                modelNodeTypeName = "Vertex";
                break;
            default:
                return QString();
            }

            return QString("%1 %2[%3]").arg(m->getDataObjectName()).arg(modelNodeTypeName).arg(_node1.second);
        }
        else
        {
            if (m->getAbsModelType() == FITKModelEnum::AbsModelType::Assembly)
                return QString("RP[%1]").arg(_node1.second);
            //组合名称
            return QString("%1 Node[%2]").arg(m->getDataObjectName()).arg(_node1.second);
        }

        return QString();
    }

    QString FITKAssemblyElementLine::getTextLabel2()
    {
        //模型
        auto m = FITKDATAREPO->getTDataByID<FITKAbstractModel>(_node2.first);
        if (m == nullptr) return QString();

        auto subMolde = FITKDATAREPO->getTDataByID<FITKAbstractModel>(_subMid2);
        if (subMolde)
        {
            QString modelNodeTypeName;
            Interface::FITKModelEnum::AbsModelType mType = subMolde->getAbsModelType();
            switch (mType)
            {
            case Interface::FITKModelEnum::AMTMesh:
            case Interface::FITKModelEnum::AMTunstructuredMesh:
            case Interface::FITKModelEnum::AMTStructuredMesh:
            case Interface::FITKModelEnum::AMTunstructuredMeshvtk:
                modelNodeTypeName = "Node";
                break;
            case Interface::FITKModelEnum::AMTGeometry:
                modelNodeTypeName = "Vertex";
                break;
            default:
                return QString();
            }

            return QString("%1 %2[%3]").arg(m->getDataObjectName()).arg(modelNodeTypeName).arg(_node2.second);
        }
        else
        {
            if (m->getAbsModelType() == FITKModelEnum::AbsModelType::Assembly)
                return QString("RP[%1]").arg(_node2.second);
            //组合名称
            return QString("%1 Node[%2]").arg(m->getDataObjectName()).arg(_node2.second);
        }

        return QString();
    }

    void FITKAssemblyElementLine::swap()
    {
        QPair<int, int> t = _node1;
        int ts = _subMid1;

        _node1 = _node2;
        _node2 = t;

        _subMid1 = _subMid2;
        _subMid2 = ts;
    }

}



