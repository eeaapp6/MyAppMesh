/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCoordinateSystem.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Kernel/FITKCore/FITKVec3DAlg.h"

namespace Interface
{
    FITKCoordinateSystem::FITKCoordinateSystem(FCSysType sysType)
        :_sysType(sysType)
    {
        //创建坐标点
        _originPt = new Core::FITKPoint(0, 0, 0);
        _xPt = new Core::FITKPoint(1, 0, 0);
        _yPt = new Core::FITKPoint(0, 1, 0);
    }

    FITKCoordinateSystem::~FITKCoordinateSystem()
    {
        //释放坐标点
        if (_originPt) delete _originPt;
        if (_xPt) delete _xPt;
        if (_yPt) delete _yPt;
    }

    Interface::FITKCoordinateSystem::FCSysType FITKCoordinateSystem::getSysType()
    {
        return _sysType;
    }

    void FITKCoordinateSystem::setOrigin(const double x, const double y, const double z)
    {
        if (_originPt) _originPt->setCoor(x, y, z);
    }

    void FITKCoordinateSystem::setPoint1(const double x, const double y, const double z)
    {
        if (_xPt) _xPt->setCoor(x, y, z);
    }

    void FITKCoordinateSystem::setPoint2(const double x, const double y, const double z)
    {
        if (_yPt) _yPt->setCoor(x, y, z);
    }

    void FITKCoordinateSystem::getOrigin(double* origin)
    {
        if (_originPt && origin) _originPt->getCoor(origin);
    }

    void FITKCoordinateSystem::getDirection1(double* dir)
    {
        //计算d1
        Core::FITKVec3D d1(Core::Subtract(*_xPt, *_originPt));
        d1.normalize();
        if (dir) d1.getCoor(dir);
    }

    void FITKCoordinateSystem::getDirection2(double* dir,bool v)
    {
        //不强制要求垂直于d1
        if (!v)
        {
            Core::FITKVec3D d1(Core::Subtract(*_yPt, *_originPt));
            //归一化
            d1.normalize();
            if (dir) d1.getCoor(dir);
        }
        //强制垂直于d1
        else
        {
            //先计算的d1和d2，d2为初始解
            Core::FITKVec3D d1(Core::Subtract(*_xPt, *_originPt));
            Core::FITKVec3D d2(Core::Subtract(*_yPt, *_originPt));
            //D3 = D1 x D2
            Core::FITKVec3D d3(Core::CrossProduct(d1, d2));
            //D = D3 x D1
            Core::FITKVec3D d(Core::CrossProduct(d3, d1));
            //归一化
            d.normalize();
            if (dir) return d.getCoor(dir);
        }
    }

    void FITKCoordinateSystem::getDirection3(double* dir)
    {
        //d3 = d1 x d2
        Core::FITKVec3D d1(Core::Subtract(*_xPt, *_originPt));
        Core::FITKVec3D d2(Core::Subtract(*_yPt, *_originPt));
        Core::FITKVec3D d3(Core::CrossProduct(d1, d2));
        //归一化
        d3.normalize();
        if (dir) d3.getCoor(dir);
    }

    void FITKCoordinateSystem::get3DirectionAtPoint(const double* pxyz, double* dirs)
    {
        if (pxyz == nullptr || dirs == nullptr) return;
        //获取三个方向坐标
        double d1[3] = { 0,0,0 }, d2[3] = { 0,0,0 }, d3[33] = { 0,0,0 };
        this->getDirection1(d1);
        this->getDirection2(d2);
        this->getDirection3(d3);
        //赋值
        for (int i = 0; i < 3; ++i)
            dirs[i] = d1[i];
        for (int i = 0; i < 3; ++i)
            dirs[i+3] = d2[i];
        for (int i = 0; i < 3; ++i)
            dirs[i+6] = d3[i];
    }

    bool FITKCoordinateSystem::copy(FITKAbstractDataObject* obj)
    {
        FITKCoordinateSystem* csys = dynamic_cast<FITKCoordinateSystem*>(obj);
        if (obj == nullptr) return false;
        double ori[3] = { 0,0,0 };
        double pt[3] = { 0,0,0 };
        //拷贝原点
        csys->getOrigin(ori);
        this->setOrigin(ori[0], ori[1], ori[2]);
        //拷贝第一点
        csys->getDirection1(pt);
        this->setPoint1(ori[0] + pt[0], ori[1] + pt[1], ori[2] + pt[2]);
        //拷贝第二点
        csys->getDirection2(pt,false);
        this->setPoint2(ori[0] + pt[0], ori[1] + pt[1], ori[2] + pt[2]);

        return Core::FITKAbstractNDataObject::copy(obj);
    }

    QList<FITKCoordinateSystem*> FITKCoordinateSystemManager::getCoordinateSystems(FITKCoordinateSystem::FCSysType type)
    {
        QList<FITKCoordinateSystem*> sysList;
        const int n = this->getDataCount();
        for (int i =0; i<n; ++i)
        {
            //遍历过滤类型
            FITKCoordinateSystem* sys = this->getDataByIndex(i);
            if(sys == nullptr) continue;
            if (type == sys->getSysType())
                sysList.append(sys);
        }
        //返回全部坐标系
        return sysList;
    }

}

