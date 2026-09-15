/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossResultRequest.h"

namespace Radioss
{
    FITKRadiossResultRequest::FITKRadiossResultRequest()
    {
        _nodalAnims << NodalOutputAnimationsType::Nodal_Displacement << NodalOutputAnimationsType::Nodal_Velocity << NodalOutputAnimationsType::Nodal_Acceleration;
        _elementalAnims << ElementalOutputAnimationsType::Elemental_SpecificEnergyDensity;
    }

    FITKRadiossResultRequest::~FITKRadiossResultRequest()
    {
    }

    void FITKRadiossResultRequest::setStartTime(const double time)
    {
        //设置动画开始时间
        _startTimeAnims = time;
    }

    double FITKRadiossResultRequest::getStartTime() const
    {
        //获取动画开始时间
        return _startTimeAnims;
    }

    void FITKRadiossResultRequest::setFrequency(const double f)
    {
        //设置动画频率
        _frequencyAnims = f;
    }

    double FITKRadiossResultRequest::getFrequency() const
    {
        //获取动画频率
        return _frequencyAnims;
    }

    void FITKRadiossResultRequest::setHisFrequency(const double f)
    {
        //设置时间历程频率
        _frequencyHistory = f;
    }

    double FITKRadiossResultRequest::getHisFrequency() const
    {
        //获取时间历程频率
        return _frequencyHistory;
    }

    void FITKRadiossResultRequest::setNodalOutputAnim(QList<NodalOutputAnimationsType> anim)
    {
        //设置节点数据动画
        _nodalAnims = anim;
    }

    QList<FITKRadiossResultRequest::NodalOutputAnimationsType> FITKRadiossResultRequest::getNodalOutputAnim()
    {
        //获取节点数据动画
        return _nodalAnims;
    }

    void FITKRadiossResultRequest::setElementalOutputAnim(QList<ElementalOutputAnimationsType> anim)
    {
        //设置单元数据动画
        _elementalAnims = anim;
    }

    QList<FITKRadiossResultRequest::ElementalOutputAnimationsType> FITKRadiossResultRequest::getElementalOutputAnim()
    {
        //获取单元数据动画
        return _elementalAnims;
    }

    void FITKRadiossResultRequest::setBrickOutputAnim(QList<BrickOutputAnimationsType> anim)
    {
        //设置Brick数据动画
        _brickAnims = anim;
    }

    QList<FITKRadiossResultRequest::BrickOutputAnimationsType> FITKRadiossResultRequest::getBrickOutputAnim()
    {
        //获取Brick数据动画
        return _brickAnims;
    }

    void FITKRadiossResultRequest::setShellOutputAnim(QList<ShellOutputAnimationsType> anim)
    {
        //设置Shell数据动画
        _shellAnims = anim;
    }

    QList<FITKRadiossResultRequest::ShellOutputAnimationsType> FITKRadiossResultRequest::getShellOutputAnim()
    {
        //获取Shell数据动画
        return _shellAnims;
    }

    void FITKRadiossResultRequest::clearParam()
    {
        //清理参数
        _nodalAnims.clear();
        _elementalAnims.clear();
        _brickAnims.clear();
        _shellAnims.clear();
    }

} // namespace Radioss

