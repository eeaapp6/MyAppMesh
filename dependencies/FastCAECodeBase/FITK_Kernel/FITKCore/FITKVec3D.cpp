/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKVec3D.h"
#include <cmath>

namespace Core
{
    FITKPoint::FITKPoint()
    {}

    FITKPoint::FITKPoint(double x, double y, double z)
    {
        _coor[0] = x;
        _coor[1] = y;
        _coor[2] = z;
    }

    FITKPoint::FITKPoint(double* c)
    {
        //c的长度必须大于等于3，超出3的截断
        if (c == nullptr) return;
        for (int i = 0; i < 3; ++i)
            _coor[i] = c[i];
    }

    FITKPoint::FITKPoint(const FITKPoint& point)
    {
        //拷贝对象
        _coor[0] = point.x();
        _coor[1] = point.y();
        _coor[2] = point.z();
    }

    double FITKPoint::x() const
    {
        //x坐标
        return _coor[0];
    }
    double FITKPoint::y() const
    {
        //y坐标
        return _coor[1];
    }
    double FITKPoint::z() const
    {
        //z坐标
        return _coor[2];
    }


    void FITKPoint::getCoor(double* c) const
    {
        //错误判断
        if (c == nullptr) return;
        //记录坐标
        for (int i = 0; i < 3; ++i)
            c[i] = _coor[i];
    }


    void FITKPoint::setCoor(double x, double y, double z)
    {
        _coor[0] = x;
        _coor[1] = y;
        _coor[2] = z;
    }

    double FITKPoint::operator[](const int & index)
    {
        if (index < 0 || index > 2) return 0;
        return _coor[index];
    }

    void FITKPoint::operator=(const FITKPoint & p)
    {
        _coor[0] = p.x();
        _coor[1] = p.y();
        _coor[2] = p.z();
    }

    FITKNode::FITKNode(const int id, const double x, const double y, const double z)
    {
        //拷贝数据
        _coor[0] = x;
        _coor[1] = y;
        _coor[2] = z;
        _nodeID = id;
    }

    FITKNode::FITKNode(const int id, const double* c)
    {
        //错误判断
        if (c == nullptr) return;
        //记录坐标
        for (int i = 0; i < 3; ++i)
            _coor[i] = c[i];
       //记录ID
        _nodeID = id;
    }

    FITKNode::~FITKNode()
    {
        emit nodeDestroiedSignal(this);
    }

    void FITKNode::setNodeID(const int id)
    {
        _nodeID = id;
    }

    int FITKNode::getNodeID() const
    {
        return _nodeID;
    }

    void FITKNode::setTag(unsigned int tag)
    {
        // 设置标记。
        _tag = tag;
    }

    unsigned int FITKNode::getTag() const
    {
        return _tag;
    }

    void FITKNode::setNativeFlag(bool native)
    {
        // 设置关联性。
        _nativeFlag = native;
    }

    bool FITKNode::getNativeFlag() const
    {
        return _nativeFlag;
    }

    FITKVec3D::FITKVec3D(const FITKPoint& p)
    {
        _coor[0] = p.x();
        _coor[1] = p.y();
        _coor[2] = p.z();
    }

    FITKVec3D::FITKVec3D(const FITKPoint& endPt, const FITKPoint& startPt)
    {
        _coor[0] = endPt.x() - startPt.x();
        _coor[1] = endPt.y() - startPt.y();
        _coor[2] = endPt.z() - startPt.z();
    }

    double FITKVec3D::getNorm() const
    {
        double m = 0;
        for (int i = 0; i < 3; ++i)
            m += _coor[i] * _coor[i];
        return sqrt(m);
    }

    void FITKVec3D::normalize()
    {
        double m = this->getNorm();
        if (m < 1e-9) return;
        for (int i = 0; i < 3; ++i)
            _coor[i] /= m;
    }

}
