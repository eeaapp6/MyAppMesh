/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKUnstructuredMeshVTK.h"
#include "FITKMeshFeatureVTK.h"
#include "FITKMeshVTKMap.h"

#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Interface/FITKInterfaceModel/FITKGeoToMeshMapper.h"

// VTK
#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridWriter.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkCellData.h>

// For VTK 7
//@{
#include <vtkSmartPointer.h>
#include <vtkIdList.h>
#include <vtkCell.h>
#include <vtkCellType.h>
#include <vtkIntArray.h>
//@}

#include "FITKInterfaceModel/FITKAbstractElement.h"
#include "FITKInterfaceModel/FITKElementFactory.h"
#include "FITKInterfaceModel/FITKElementHex.h"
#include "FITKInterfaceModel/FITKElementQuad.h"
#include "FITKInterfaceModel/FITKElementTet.h"
#include "FITKInterfaceModel/FITKElementTri.h"

#include <QHash>
#include <QColor>

#include "FITK_Kernel/FITKCore/FITKLibInfo.h"

FITKLIBINFOREGISTER(FITKInterfaceMesh,FITKInterfaceMeshVersion);

namespace Interface
{
    QString FITKUnstructuredMeshVTK::GetOrphanMeshTagName()
    {
        return "OrphanElementTag";
    }

    FITKUnstructuredMeshVTK::FITKUnstructuredMeshVTK()
    {
        //创建VTK的网格对象
        _vtkDataSet = vtkUnstructuredGrid::New();
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        _vtkDataSet->SetPoints(points);

        _orphanMeshTagArrayPt = vtkIntArray::New();
        _orphanMeshTagArrayPt->SetName(GetOrphanMeshTagName().toUtf8());
        _orphanMeshTagArrayPt->SetNumberOfComponents(1);
        
        _orphanMeshTagArrayCell = vtkIntArray::New();
        _orphanMeshTagArrayCell->SetName(GetOrphanMeshTagName().toUtf8());
        _orphanMeshTagArrayCell->SetNumberOfComponents(1);

        _vtkDataSet->GetPointData()->AddArray(_orphanMeshTagArrayPt);
        _vtkDataSet->GetCellData()->AddArray(_orphanMeshTagArrayCell);

        _feature = new FITKMeshFeatureVTK(_vtkDataSet);
    }

    FITKUnstructuredMeshVTK::~FITKUnstructuredMeshVTK()
    {
        // 移除特征数据。
        //@{
        if (_feature)
        {
            delete _feature;
            _feature = nullptr;
        }
        //@}

        //内存回收
        if (_vtkDataSet != nullptr)
            _vtkDataSet->Delete();

        if (_orphanMeshTagArrayPt)
        {
            _orphanMeshTagArrayPt->Delete();
            _orphanMeshTagArrayPt = nullptr;
        }

        if (_orphanMeshTagArrayCell)
        {
            _orphanMeshTagArrayCell->Delete();
            _orphanMeshTagArrayCell = nullptr;
        }
    }

    FITKModelEnum::AbsModelType FITKUnstructuredMeshVTK::getAbsModelType()
    {
        return FITKModelEnum::AbsModelType::AMTunstructuredMeshvtk;
    }

    void FITKUnstructuredMeshVTK::update()
    {
        if (!_modified) return;

        _vtkDataSet->Modified();

        if (_feature)
            _feature->update();

        _modified = false;
    }

    const QList<int> FITKUnstructuredMeshVTK::getCellIndiceByDim(int dim)
    {
        QList<int> indice;
        if (dim < 0 || dim > 3)
        {
            return indice;
        }

        for (int i = 0 ; i < _elementList.count() ; i ++)
        {
            FITKAbstractElement* element = _elementList[i];
            if (dim == element->getElementDim())
            {
                indice.push_back(i);
            }
        }

        return indice;
    }

    int FITKUnstructuredMeshVTK::addNode(Core::FITKNode* node)
    {
        if (node)
        {
            bool flag = node->getNativeFlag();

            _orphanMeshTagArrayPt->InsertNextValue(flag ? 0 : 1);
        }
        else
        {
            _orphanMeshTagArrayPt->InsertNextValue(1);
        }

        return FITKUnstructuredMesh::addNode(node);
    }

    int FITKUnstructuredMeshVTK::addNode(const double x, const double y, const double z, bool isNative, unsigned int tag)
    {
        //追加节点，vtk中的节点顺序与节点列表一致
        Core::FITKNode* node = new Core::FITKNode(x, y, z);
        node->setNativeFlag(isNative);
        node->setTag(tag);
        int id = FITKNodeList::addNode(node);

        _orphanMeshTagArrayPt->InsertNextValue(isNative ? 0 : 1);

        vtkPoints* points = _vtkDataSet->GetPoints();
        if (points == nullptr) return -1;
        points->InsertNextPoint(x, y, z);
        _modified = true;
        return id;
    }


    void FITKUnstructuredMeshVTK::addNode(const int id, const double x, const double y, const double z, bool isNative, unsigned int tag)
    {
        //追加节点，vtk中的节点顺序与节点列表一致
        Core::FITKNode* node = new Core::FITKNode(id, x, y, z);
        node->setNativeFlag(isNative);
        node->setTag(tag);
        FITKNodeList::addNode(node);

        _orphanMeshTagArrayPt->InsertNextValue(isNative ? 0 : 1);

        vtkPoints* points = _vtkDataSet->GetPoints();
        if (points == nullptr) return;
        points->InsertNextPoint(x, y, z);
        _modified = true;
    }

    void FITKUnstructuredMeshVTK::appendElement(FITKAbstractElement* element)
    {
        if (element == nullptr) return;
        Interface::FITKModelEnum::FITKEleType type = element->getEleType();
        if (!eleTypeHash.contains(type)) return;
        //追加到单元中
        FITKElementList::appendElement(element);

        int count = element->getNodeCount();
        vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();
        for (int i = 0; i < count; ++i)
        {
            //查找节点索引
            const int nodeID = element->getNodeID(i);
            const int index = this->getNodeIndexByID(nodeID);
            idList->InsertNextId(index);
        }
        _vtkDataSet->InsertNextCell(eleTypeHash[type], idList);

        bool flag = element->getNativeFlag();
        _orphanMeshTagArrayCell->InsertNextValue(flag ? 0 : 1);

        _modified = true;
    }

    void FITKUnstructuredMeshVTK::removeAllElement()
    {
        FITKElementList::removeAllElement();

        // 额外清除关联网格标记。
        if (_orphanMeshTagArrayCell)
        {
            _orphanMeshTagArrayCell->Reset();
            _orphanMeshTagArrayCell->Resize(0);
        }
    }

    int FITKUnstructuredMeshVTK::removeElementAt(const int index)
    {
        int eId = FITKElementList::removeElementAt(index);

        // 额外清除关联网格标记。
        if (eId > 0 && _orphanMeshTagArrayCell)
        {
            int nVals = _orphanMeshTagArrayCell->GetNumberOfValues();
            for (int i = index; i < nVals - 1; i++)
            {
                _orphanMeshTagArrayCell->SetValue(i, _orphanMeshTagArrayCell->GetValue(i + 1));
            }

            _orphanMeshTagArrayCell->Resize(nVals - 1);
        }

        return eId;
    }

    int FITKUnstructuredMeshVTK::removeElementByID(const int id)
    {
        int iEle = FITKElementList::removeElementByID(id);

        // 额外清除关联网格标记。
        if (iEle >= 0 && _orphanMeshTagArrayCell)
        {
            int nVals = _orphanMeshTagArrayCell->GetNumberOfValues();
            for (int i = iEle; i < nVals - 1; i++)
            {
                _orphanMeshTagArrayCell->SetValue(i, _orphanMeshTagArrayCell->GetValue(i + 1));
            }

            _orphanMeshTagArrayCell->Resize(nVals - 1);
        }

        return iEle;
    }

    bool FITKUnstructuredMeshVTK::writeToFile(const QString & file)
    {
        //写出文件
        vtkSmartPointer<vtkUnstructuredGridWriter> writer = vtkSmartPointer<vtkUnstructuredGridWriter>::New();
        writer->SetFileName(file.toStdString().c_str());
        writer->SetFileTypeToASCII();
        // writer->SetFileTypeToBinary();
        writer->SetInputData(_vtkDataSet);
        bool flag = writer->Write();
        return flag;
    }

    void FITKUnstructuredMeshVTK::reverseEleByID(const int id)
    {
        const int index = getEleIndexByID(id);
        this->reverseEleByIndex(index);
    }

    void FITKUnstructuredMeshVTK::reverseEleByIndex(const int index)
    {
        FITKUnstructuredMesh::reverseEleByIndex(index);

        auto cell = _vtkDataSet->GetCell(index);

        if (cell == nullptr) return;
        auto points = cell->GetPointIds();
        if (points == nullptr) return;
        //交换首尾节点编号
        const int np = points->GetNumberOfIds();

        vtkIdType* pts = new vtkIdType[np];
        for (int i = 0; i < np; ++i)
        {
            pts[i] = points->GetId(np - i - 1);
        }

        // 替换单元。
        _vtkDataSet->ReplaceCell(index, np, pts);

        // 已经过测试可删除。
        delete pts;

        _modified = true;
    }

    FITKMeshFeatureVTK* FITKUnstructuredMeshVTK::getMeshFeature()
    {
        return _feature;
    }

    vtkUnstructuredGrid* FITKUnstructuredMeshVTK::getGrid()
    {
        return _vtkDataSet;
    }

    bool FITKUnstructuredMeshVTK::getBounds(double bds6[6])
    {
        if (!_vtkDataSet)
        {
            return false;
        }

        if (_vtkDataSet->GetNumberOfCells() == 0 &&
            _vtkDataSet->GetNumberOfPoints() == 0)
        {
            return false;
        }

        double* bds = _vtkDataSet->GetBounds();
        for (int i = 0; i < 6; i++)
        {
            bds6[i] = bds[i];
        }

        return true;
    }

    int FITKUnstructuredMeshVTK::getNumberOfCells()
    {
        if (!_vtkDataSet)
            return 0;

        return _vtkDataSet->GetNumberOfCells();
    }

    void FITKUnstructuredMeshVTK::reConstructure()
    {
        updateNodeIDIndexMap();
        updateElementIDIndexMap();

        // 重置网格数据，重新构建。
        _vtkDataSet->Reset();

        int nPtArrs = _vtkDataSet->GetPointData()->GetNumberOfArrays();
        int nCellArrs = _vtkDataSet->GetCellData()->GetNumberOfArrays();

        // 移除节点数组。
        for (int i = nPtArrs - 1; i >= 0; i--)
        {
            _vtkDataSet->GetPointData()->RemoveArray(i);
        }

        // 移除单元数组。
        for (int i = nCellArrs - 1; i >= 0; i--)
        {
            _vtkDataSet->GetCellData()->RemoveArray(i);
        }

        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        _vtkDataSet->SetPoints(points);

        // 添加独立网格标记数组。
        if (_orphanMeshTagArrayPt)
        {
            _orphanMeshTagArrayPt->Resize(0);
            _orphanMeshTagArrayPt->SetNumberOfComponents(1);
            _vtkDataSet->GetPointData()->AddArray(_orphanMeshTagArrayPt);
        }

        if (_orphanMeshTagArrayCell)
        {
            _orphanMeshTagArrayCell->Resize(0);
            _orphanMeshTagArrayCell->SetNumberOfComponents(1);
            _vtkDataSet->GetCellData()->AddArray(_orphanMeshTagArrayCell);
        }

        double pos3[3]{ 0., 0., 0. };
        for (Core::FITKNode* node : _nodeList)
        {
            node->getCoor(pos3);
            points->InsertNextPoint(pos3);

            // 追加独立网格节点标记。
            bool flag = node->getNativeFlag();
            _orphanMeshTagArrayPt->InsertNextValue(flag ? 0 : 1);
        }

        for (FITKAbstractElement* element : _elementList)
        {
            int count = element->getNodeCount();
            vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();
            for (int i = 0; i < count; ++i)
            {
                //查找节点索引
                const int nodeID = element->getNodeID(i);
                const int index = this->getNodeIndexByID(nodeID);
                idList->InsertNextId(index);
            }

            Interface::FITKModelEnum::FITKEleType type = element->getEleType();
            _vtkDataSet->InsertNextCell(eleTypeHash[type], idList);

            // 追加独立网格单元标记。
            bool flag = element->getNativeFlag();
            _orphanMeshTagArrayCell->InsertNextValue(flag ? 0 : 1);
        }
    }


    void FITKUnstructuredMeshVTK::transformVTKMesh(vtkUnstructuredGrid* grid)
    {
        if (grid == nullptr) return;
        this->removeAllNode();
        this->removeAllElement();
        _vtkDataSet->CopyStructure(grid);
        //复制节点 
        const int npt = _vtkDataSet->GetNumberOfPoints();
        for (int i = 0; i < npt; ++i)
        {
            double coor[3] = { 0,0,0 };
            _vtkDataSet->GetPoint(i, coor);
            FITKNodeList::addNode(coor[0], coor[1], coor[2]);
        }
        //复制单元 
        const int ncell = _vtkDataSet->GetNumberOfCells();

        int iCell = 1;
        for (int i = 0; i < ncell; ++i)
        {
            vtkCell* cell = _vtkDataSet->GetCell(i);
            if (cell == nullptr) continue;
            int cT = cell->GetCellType();
            //查询单元类型
            Interface::FITKModelEnum::FITKEleType t = eleTypeHash.key(cT);
            FITKAbstractElement* ele = FITKElementFactory::createElement(t);
            if (ele == nullptr) continue;
            //单元节点
            QList<int> cellID{};
            const int ncellPt = cell->GetNumberOfPoints();
            for (int cpt = 0; cpt < ncellPt; ++cpt)
            {
                int nodeid = cell->GetPointId(cpt);
                cellID.append(nodeid + 1);
            }
            ele->setNodeID(cellID);
            ele->setEleID(iCell);
            FITKElementList::appendElement(ele);
            //_elementList.append(ele);
            iCell++;
        }
    }

    void FITKUnstructuredMeshVTK::clearMesh()
    {
        this->removeAllNode();
        this->removeAllElement();

        _vtkDataSet->GetPoints()->Reset();

        int nPtArrs = _vtkDataSet->GetPointData()->GetNumberOfArrays();
        int nCellArrs = _vtkDataSet->GetCellData()->GetNumberOfArrays();

        // 移除节点数组。
        for (int i = nPtArrs - 1; i >= 0; i--)
        {
            _vtkDataSet->GetPointData()->RemoveArray(i);
        }

        // 移除单元数组。
        for (int i = nCellArrs - 1; i >= 0; i--)
        {
            _vtkDataSet->GetCellData()->RemoveArray(i);
        }

        _vtkDataSet->Reset();

        // 添加独立网格标记数组。
        if (_orphanMeshTagArrayPt)
        {
            _orphanMeshTagArrayPt->Reset();
            _orphanMeshTagArrayPt->Resize(0);
            _vtkDataSet->GetPointData()->AddArray(_orphanMeshTagArrayPt);
        }

        if (_orphanMeshTagArrayCell)
        {
            _orphanMeshTagArrayCell->Reset();
            _orphanMeshTagArrayCell->Resize(0);
            _vtkDataSet->GetCellData()->AddArray(_orphanMeshTagArrayCell);
        }

        //清除几何与网格映射关系
        if (_geoToMeshMapper) {
            _geoToMeshMapper->clear();
        }
    }

    void FITKUnstructuredMeshVTK::clearNativeMesh()
    {
        clearVTKMesh();

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
        reConstructure();
        update();

        //清除几何与网格映射关系
        if (_geoToMeshMapper) {
            _geoToMeshMapper->clear();
        }
    }

    void FITKUnstructuredMeshVTK::clearOrphanMesh()
    {
        clearVTKMesh();

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
        reConstructure();
        update();
    }

    void FITKUnstructuredMeshVTK::replaceNodesID(const QHash<int, int> & nodes)
    {
        FITKElementList::replaceNodesID(nodes);
        //重置单元
        this->reConstructure();
        this->update();
    }

    void FITKUnstructuredMeshVTK::clearVTKMesh()
    {
        // 清空VTK数据。
        _vtkDataSet->GetPoints()->Reset();
        _vtkDataSet->Reset();

        int nPtArrs = _vtkDataSet->GetPointData()->GetNumberOfArrays();
        int nCellArrs = _vtkDataSet->GetCellData()->GetNumberOfArrays();

        // 移除节点数组。
        for (int i = nPtArrs - 1; i >= 0; i--)
        {
            _vtkDataSet->GetPointData()->RemoveArray(i);
        }

        // 移除单元数组。
        for (int i = nCellArrs - 1; i >= 0; i--)
        {
            _vtkDataSet->GetCellData()->RemoveArray(i);
        }
    }

}
