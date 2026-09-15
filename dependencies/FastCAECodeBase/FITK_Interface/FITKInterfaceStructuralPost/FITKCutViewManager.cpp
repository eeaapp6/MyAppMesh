/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCutViewManager.h"

#include "FITKAbstractStruPostVTK.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKDeformFilter.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKSurfaceFilter.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKMeshFeatureVTK.h"

// Cut Filter
#include <vtkTableBasedClipDataSet.h>
#include <vtkCutter.h>
#include <vtkAlgorithm.h>
#include <vtkUnstructuredGrid.h>

// Implicit Function
#include <vtkPlane.h>
#include <vtkCylinder.h>
#include <vtkSphere.h>

namespace Interface
{
    // Base
    //@{
    FITKAbstractViewCut::FITKAbstractViewCut()
    {
        //  初始化算法与过滤器。
        _clipper = vtkTableBasedClipDataSet::New();
        _clipper->SetInsideOut(true);

        _revClipper = vtkTableBasedClipDataSet::New();
        _revClipper->SetInsideOut(false);

        _cutter = vtkCutter::New(); 

        // 创建网格特征数据，关闭节点与单元编号数据。
        _feature = new Interface::FITKMeshFeatureVTK(_clipper->GetOutputPort());
        _feature->setCellAndPointIdsEnable(false, false);
        _feature->setEnabledFeature(false);
    }

    FITKAbstractViewCut::~FITKAbstractViewCut()
    {
        if (_clipper != nullptr) _clipper->Delete();
        if (_revClipper != nullptr) _revClipper->Delete();
        if (_cutter != nullptr) _cutter->Delete();
        if (_function != nullptr) _function->Delete();

        if (_feature)
        {
            delete _feature;
            _feature = nullptr;
        }
    }

    void FITKAbstractViewCut::setUseMeshFeature(bool flag)
    {
        if (_feature)
        {
            _feature->setEnabledFeature(flag);
        }
    }

    bool FITKAbstractViewCut::getUseMeshFeature()
    {
        if (_feature)
        {
            return _feature->getEnabledFeature();
        }

        return false;
    }

    void FITKAbstractViewCut::setVisible(bool visible)
    {
        // 保存可见性。
        _visible = visible;
    }

    bool FITKAbstractViewCut::isVisible()
    {
        // 获取是否可见。
        return _visible;
    }

    void FITKAbstractViewCut::setOrigin(double x, double y, double z)
    {
        // 保存裁切算法中心点。
        _origin[0] = x;
        _origin[1] = y;
        _origin[2] = z;
    }

    void FITKAbstractViewCut::getOrigin(double* coor)
    {
        if (coor == nullptr) return;
        for (int i = 0; i < 3; ++i)
            coor[i] = _origin[i];
    }

    void FITKAbstractViewCut::setModelType(DisplayModelType t)
    {
        _modelType = t;
    }

    Interface::FITKAbstractViewCut::DisplayModelType FITKAbstractViewCut::getModelType()
    {
        return _modelType;
    }

    void FITKAbstractViewCut::setInput(vtkAlgorithm* inp)
    {
        if (inp == nullptr) return;
        if (_cutter) _cutter->SetInputConnection(inp->GetOutputPort());
        if (_clipper) _clipper->SetInputConnection(inp->GetOutputPort());
        if (_revClipper) _revClipper->SetInputConnection(inp->GetOutputPort());

        _input = inp;
    }

    vtkAlgorithm* FITKAbstractViewCut::getOutputAlgorithm()
    {
        if (_clipper == nullptr || _cutter == nullptr || _revClipper == nullptr) return nullptr;
        switch (_modelType)
        {
        case Interface::FITKAbstractViewCut::Slice: return _cutter;
        case Interface::FITKAbstractViewCut::Clip: return _clipper;
        case Interface::FITKAbstractViewCut::RevClip: return _revClipper;
        default: break;
        }
        return nullptr;
    }

    vtkAlgorithm * FITKAbstractViewCut::getOutputAlgorithm(DisplayModelType type)
    {
        if (_modelTypes.contains(type)) {
            if (_clipper == nullptr || _cutter == nullptr || _revClipper == nullptr) return nullptr;
            switch (type)
            {
            case Interface::FITKAbstractViewCut::Slice: return _cutter;
            case Interface::FITKAbstractViewCut::Clip: return _clipper;
            case Interface::FITKAbstractViewCut::RevClip: return _revClipper;
            default: break;
            }
        }
        return nullptr;
    }

    vtkAlgorithmOutput* FITKAbstractViewCut::getOutputPort()
    {
        if (_clipper == nullptr || _cutter == nullptr || _revClipper == nullptr) return nullptr;
        switch (_modelType)
        {
        case Interface::FITKAbstractViewCut::Slice: return _cutter->GetOutputPort();
        case Interface::FITKAbstractViewCut::Clip: return _clipper->GetOutputPort();
        case Interface::FITKAbstractViewCut::RevClip: return _revClipper->GetOutputPort();;
        default: break;
        }
        return nullptr;
    }

    vtkAlgorithmOutput * FITKAbstractViewCut::getOutputPort(DisplayModelType type)
    {
        if (_modelTypes.contains(type)) {
            if (_clipper == nullptr || _cutter == nullptr || _revClipper == nullptr) return nullptr;
            switch (type)
            {
            case Interface::FITKAbstractViewCut::Slice: return _cutter->GetOutputPort();
            case Interface::FITKAbstractViewCut::Clip: return _clipper->GetOutputPort();
            case Interface::FITKAbstractViewCut::RevClip: return _revClipper->GetOutputPort();;
            default: break;
            }
            return nullptr;
        }
        return nullptr;
    }

    vtkDataSet* FITKAbstractViewCut::getOutput()
    {
        if (_clipper == nullptr || _cutter == nullptr || _revClipper == nullptr) return nullptr;
        switch (_modelType)
        {
        case Interface::FITKAbstractViewCut::Slice: return _cutter->GetOutput();
        case Interface::FITKAbstractViewCut::Clip: return _clipper->GetOutput();
        case Interface::FITKAbstractViewCut::RevClip: return _revClipper->GetOutput();
        default: break;
        }
        return nullptr;
    }

    vtkDataSet * FITKAbstractViewCut::getOutput(DisplayModelType type)
    {
        if (_modelTypes.contains(type)) {
            if (_clipper == nullptr || _cutter == nullptr || _revClipper == nullptr) return nullptr;
            switch (type)
            {
            case Interface::FITKAbstractViewCut::Slice: return _cutter->GetOutput();
            case Interface::FITKAbstractViewCut::Clip: return _clipper->GetOutput();
            case Interface::FITKAbstractViewCut::RevClip: return _revClipper->GetOutput();
            default: break;
            }
            return nullptr;
        }
        return nullptr;
    }

    FITKMeshFeatureVTK* FITKAbstractViewCut::getMeshFeature()
    {
        return _feature;
    }

    void FITKAbstractViewCut::update()
    {
        if (_function == nullptr || _clipper == nullptr 
            || _cutter == nullptr || _revClipper == nullptr) return ;

        switch (_modelType)
        {
        case Interface::FITKAbstractViewCut::Slice:
        {
            if (_feature)
            {
                _feature->reset(_cutter->GetOutputPort());
            }

            _cutter->Update();
            break;
        }
        case Interface::FITKAbstractViewCut::Clip:
        {
            if (_feature)
            {
                _feature->reset(_clipper->GetOutputPort());
            }
            _clipper->SetInsideOut(true);
            _clipper->Update();
            break;
        }
        case Interface::FITKAbstractViewCut::RevClip:
        {
            if (_feature)
            {
                _feature->reset(_revClipper->GetOutputPort());
            }
            _revClipper->SetInsideOut(false);
            _revClipper->Update();
            break;
        }
        default: break;
        }
    }

    void FITKAbstractViewCut::getInputBounds(double* bounds, double factor)
    {
        if (!bounds || !_input)
        {
            return;
        }

        // 尝试获取包围盒数据。
        vtkDataSet* dataSet = vtkDataSet::SafeDownCast(_input->GetOutputDataObject(0));
        if (!dataSet)
        {
            return;
        }

        factor = factor > 3 ? 1 : factor;
        factor = qFuzzyCompare(factor, 0) ? 1 : factor;

        double* bds = dataSet->GetBounds();

        double xDelta = (bds[1] - bds[0]) * (factor - 1.) / 2.;
        double yDelta = (bds[3] - bds[2]) * (factor - 1.) / 2.;
        double zDelta = (bds[5] - bds[4]) * (factor - 1.) / 2.;

        bounds[0] = bds[0] - xDelta;
        bounds[1] = bds[1] + xDelta;
        bounds[2] = bds[2] - yDelta;
        bounds[3] = bds[3] + yDelta;
        bounds[4] = bds[4] - zDelta;
        bounds[5] = bds[5] + zDelta;
    }

    void FITKAbstractViewCut::setModelID(int id)
    {
        _modelID = id;
    }

    int FITKAbstractViewCut::getModelID()
    {
        return _modelID;
    }

    QList<Interface::FITKAbstractViewCut::DisplayModelType> FITKAbstractViewCut::getDisplayModelTypes() {
        return _modelTypes;
    }

    void FITKAbstractViewCut::appenDisplayModelType(DisplayModelType type) {
        _modelTypes.append(type);
    }

    void FITKAbstractViewCut::removeDisplayModelType(DisplayModelType type) {
        if (_modelTypes.contains(type)) {
            _modelTypes.removeOne(type);
        }
    }

    void FITKAbstractViewCut::update(int diff)
    {
        Q_UNUSED(diff);
        if (_function == nullptr || _clipper == nullptr
        || _cutter == nullptr || _revClipper == nullptr) return;

        if (_modelTypes.contains(Interface::FITKAbstractViewCut::Slice)) {
            if (_feature)
            {
                _feature->reset(_cutter->GetOutputPort());
            }
            _cutter->Update();
        }

        if (_modelTypes.contains(Interface::FITKAbstractViewCut::Clip)) {
            if (_feature)
            {
                _feature->reset(_clipper->GetOutputPort());
            }
            _clipper->SetInsideOut(true);
            _clipper->Update();
        }

        if (_modelTypes.contains(Interface::FITKAbstractViewCut::RevClip)) {
            if (_feature)
            {
                _feature->reset(_revClipper->GetOutputPort());
            }
            _revClipper->SetInsideOut(false);
            _revClipper->Update();
        }
    }
    //@}

    // Plane
    //@{
    FITKViewCutPlane::FITKViewCutPlane()
    {
        _function = vtkPlane::New();
        _clipper->SetClipFunction(_function);
        _revClipper->SetClipFunction(_function);
        _cutter->SetCutFunction(_function);
    }

    Interface::FITKAbstractViewCut::CutShapeType FITKViewCutPlane::getShapeType()
    {
        return FITKAbstractViewCut::Plane;
    }  

    void FITKViewCutPlane::setNormalAxis(double x, double y, double z)
    {
        _axis[0] = x; _axis[1] = y; _axis[2] = z;

        //vtkPlane* p = vtkPlane::SafeDownCast(_function);
        //if (p == nullptr) return;
    }

    void FITKViewCutPlane::getNormalAxis(double* axis)
    {
        if (axis == nullptr) return;
        for (int i = 0; i < 3; ++i)
            axis[i] = _axis[i];
    }

    void FITKViewCutPlane::update()
    {
        vtkPlane* p = vtkPlane::SafeDownCast(_function);
        if (p == nullptr) return;
        p->SetOrigin(_origin);
        p->SetNormal(_axis[0], _axis[1], _axis[2]);

        FITKAbstractViewCut::update();
    }

    bool FITKViewCutPlane::copy(FITKAbstractDataObject* obj)
    {
        FITKViewCutPlane* newObj = dynamic_cast<FITKViewCutPlane*>(obj);
        if (!newObj)
        {
            return false;
        }

        // 设置方向与中心位置。
        double origin[3]{ 0., 0., 0. };
        this->getOrigin(origin);
        newObj->setOrigin(origin[0], origin[1], origin[2]);

        double noamal[3]{ 0., 0., 0. };
        this->getNormalAxis(noamal);
        newObj->setNormalAxis(noamal[0], noamal[1], noamal[2]);

        return true;
    }
    //@}

    // Cylinder
    //@{
    FITKViewCutCylinder::FITKViewCutCylinder()
    {
        _function = vtkCylinder::New();
        _clipper->SetClipFunction(_function);
        _revClipper->SetClipFunction(_function);
        _cutter->SetCutFunction(_function);
    }

    Interface::FITKAbstractViewCut::CutShapeType FITKViewCutCylinder::getShapeType()
    {
        return FITKAbstractViewCut::Cylinder;
    }

    void FITKViewCutCylinder::setAxis(double x, double y, double z)
    {
        _axis[0] = x; _axis[1] = y; _axis[2] = z;

        //vtkCylinder* cylinder = vtkCylinder::SafeDownCast(_function);
        //if (cylinder == nullptr) return;
    }

    void FITKViewCutCylinder::getAxis(double* axis)
    {
        if (axis == nullptr) return;
        for (int i = 0; i < 3; ++i)
            axis[i] = _axis[i];
    }

    void FITKViewCutCylinder::setRadius(double radius)
    {
        _radius = radius;
    }

    double FITKViewCutCylinder::getRadius()
    {
        return _radius;
    }

    void FITKViewCutCylinder::update()
    {
        vtkCylinder* cylinder = vtkCylinder::SafeDownCast(_function);
        if (cylinder == nullptr) return;
        cylinder->SetCenter(_origin);
        cylinder->SetRadius(_radius);
        cylinder->SetAxis(_axis[0], _axis[1], _axis[2]);

        FITKAbstractViewCut::update();
    }

    bool FITKViewCutCylinder::copy(FITKAbstractDataObject* obj)
    {
        FITKViewCutCylinder* newObj = dynamic_cast<FITKViewCutCylinder*>(obj);
        if (!newObj)
        {
            return false;
        }

        // 设置半径，方向与中心位置。
        double origin[3]{ 0., 0., 0. };
        this->getOrigin(origin);
        newObj->setOrigin(origin[0], origin[1], origin[2]);

        double dire[3]{ 0., 0., 0. };
        this->getAxis(dire);
        newObj->setAxis(dire[0], dire[1], dire[2]);

        newObj->setRadius(this->getRadius());

        return true;
    }
    //@}

    // Sphere
    //@{
    FITKViewCutSphere::FITKViewCutSphere()
    {
        _function = vtkSphere::New();
        _clipper->SetClipFunction(_function);
        _revClipper->SetClipFunction(_function);
        _cutter->SetCutFunction(_function);
    }

    Interface::FITKAbstractViewCut::CutShapeType FITKViewCutSphere::getShapeType()
    {
        return FITKAbstractViewCut::Sphere;
    }

    void FITKViewCutSphere::setRadius(double radius)
    {
        _radius = radius;
    }

    double FITKViewCutSphere::getRadius()
    {
        return _radius;
    }

    void FITKViewCutSphere::update()
    {
        vtkSphere* sphere = vtkSphere::SafeDownCast(_function);
        if (sphere == nullptr) return;
        sphere->SetCenter(_origin);
        sphere->SetRadius(_radius);

        FITKAbstractViewCut::update();
    }

    bool FITKViewCutSphere::copy(FITKAbstractDataObject* obj)
    {
        FITKViewCutSphere* newObj = dynamic_cast<FITKViewCutSphere*>(obj);
        if (!newObj)
        {
            return false;
        }

        // 设置半径与圆心位置。
        double origin[3]{ 0., 0., 0. };
        this->getOrigin(origin);
        newObj->setOrigin(origin[0], origin[1], origin[2]);
        newObj->setRadius(this->getRadius());

        return true;
    }
    //@}

    // Manager
    //@{
    FITKCutViewManager::FITKCutViewManager(FITKAbstractStructuralPostVTK* absPost):_postObj(absPost)
    {
        FITKViewCutPlane* xp = new FITKViewCutPlane;
        xp->setDataObjectName("X-Plane");
        xp->setNormalAxis(1, 0, 0);
        xp->setOrigin(0, 0, 0);
        this->appendDataObj(xp);

        FITKViewCutPlane* yp = new FITKViewCutPlane;
        yp->setDataObjectName("Y-Plane");
        yp->setNormalAxis(0, 1, 0);
        yp->setOrigin(0, 0, 0);
        this->appendDataObj(yp);

        FITKViewCutPlane* zp = new FITKViewCutPlane;
        zp->setDataObjectName("Z-Plane");
        zp->setNormalAxis(0, 0, 1);
        zp->setOrigin(0, 0, 0);
        this->appendDataObj(zp);

        //FITKViewCutCylinder* cc = new FITKViewCutCylinder;
        //cc->setDataObjectName("Cylinder");
        //cc->setAxis(0, 0, 1);
        //cc->setOrigin(0, 0, 0);
        //cc->setRadius(1.);
        //this->appendDataObj(cc);

        //FITKViewCutSphere* cs = new FITKViewCutSphere;
        //cs->setDataObjectName("Sphere");
        //cs->setOrigin(0, 0, 0);
        //cs->setRadius(1.);
        //this->appendDataObj(cs);
    }

    void FITKCutViewManager::appendDataObj(Core::FITKAbstractDataObject* item)
    {
        FITKAbstractViewCut* v = dynamic_cast<FITKAbstractViewCut*>(item);
        if (v == nullptr|| _postObj == nullptr) return;
        auto def = _postObj->getOutputAlgorithm();
        if ( def == nullptr) return;
        v->setModelID(_postObj->getDataObjectID());
        v->setInput(def);
        v->update();
        Core::FITKAbstractDataManager<FITKAbstractViewCut>::appendDataObj(item);
    }

    void FITKCutViewManager::insertDataObj(int index, Core::FITKAbstractDataObject* item)
    {
        FITKAbstractViewCut* v = dynamic_cast<FITKAbstractViewCut*>(item);
        if (v == nullptr || _postObj == nullptr) return;
        auto def = _postObj->getOutputAlgorithm();
        if (def == nullptr) return;
        v->setInput(def);
        v->update();
        Core::FITKAbstractDataManager<FITKAbstractViewCut>::insertDataObj(index, item);

    }
    //@}
}