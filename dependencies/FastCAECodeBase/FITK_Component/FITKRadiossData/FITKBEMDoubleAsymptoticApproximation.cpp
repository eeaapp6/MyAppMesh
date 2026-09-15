/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKBEMDoubleAsymptoticApproximation.h"

namespace Radioss
{
    FITKBEMDoubleAsymptoticApproximation::FITKBEMDoubleAsymptoticApproximation()
    {
        //默认构造函数
    }

    FITKBEMDoubleAsymptoticApproximation::~FITKBEMDoubleAsymptoticApproximation()
    {
        //析构函数
    }

    FITKAbstractBEM::BEMType FITKBEMDoubleAsymptoticApproximation::getBEMType()
    {
        //返回BEM类型
        return BEMType::BEM_DAA;
    }

    // ==================== 表面标识符相关方法 ====================
    void FITKBEMDoubleAsymptoticApproximation::setSurfId(const int id)
    {
        //设置表面标识符
        m_surf_id = id;
    }

    int FITKBEMDoubleAsymptoticApproximation::getSurfId() const
    {
        //获取表面标识符
        return m_surf_id;
    }

    // ==================== 重力标识符相关方法 ====================
    void FITKBEMDoubleAsymptoticApproximation::setGravId(const int id)
    {
        //设置重力标识符
        m_grav_id = id;
    }

    int FITKBEMDoubleAsymptoticApproximation::getGravId() const
    {
        //获取重力标识符
        return m_grav_id;
    }

    // ==================== 流体密度相关方法 ====================
    void FITKBEMDoubleAsymptoticApproximation::setRho(const double value)
    {
        //设置流体密度
        m_rho = value;
    }

    double FITKBEMDoubleAsymptoticApproximation::getRho() const
    {
        //获取流体密度
        return m_rho;
    }

    // ==================== 声速相关方法 ====================
    void FITKBEMDoubleAsymptoticApproximation::setC(const double value)
    {
        //设置声速
        m_c = value;
    }

    double FITKBEMDoubleAsymptoticApproximation::getC() const
    {
        //获取声速
        return m_c;
    }

    // ==================== 环境压力相关方法 ====================
    void FITKBEMDoubleAsymptoticApproximation::setPinf(const double value)
    {
        //设置环境压力
        m_p_inf = value;
    }

    double FITKBEMDoubleAsymptoticApproximation::getPinf() const
    {
        //获取环境压力
        return m_p_inf;
    }

    // ==================== Standoff点X坐标相关方法 ====================
    void FITKBEMDoubleAsymptoticApproximation::setXs(const double value)
    {
        //设置Standoff点X坐标
        m_xs = value;
    }

    double FITKBEMDoubleAsymptoticApproximation::getXs() const
    {
        //获取Standoff点X坐标
        return m_xs;
    }

    // ==================== Standoff点Y坐标相关方法 ====================
    void FITKBEMDoubleAsymptoticApproximation::setYs(const double value)
    {
        //设置Standoff点Y坐标
        m_ys = value;
    }

    double FITKBEMDoubleAsymptoticApproximation::getYs() const
    {
        //获取Standoff点Y坐标
        return m_ys;
    }

    // ==================== Standoff点Z坐标相关方法 ====================
    void FITKBEMDoubleAsymptoticApproximation::setZs(const double value)
    {
        //设置Standoff点Z坐标
        m_zs = value;
    }

    double FITKBEMDoubleAsymptoticApproximation::getZs() const
    {
        //获取Standoff点Z坐标
        return m_zs;
    }

    // ==================== BEM求解器标志相关方法 ====================
    void FITKBEMDoubleAsymptoticApproximation::setIform(const int flag)
    {
        //设置BEM求解器标志
        m_iform = flag;
    }

    int FITKBEMDoubleAsymptoticApproximation::getIform() const
    {
        //获取BEM求解器标志
        return m_iform;
    }

    // ==================== 压力输出标志相关方法 ====================
    void FITKBEMDoubleAsymptoticApproximation::setIpri(const int flag)
    {
        //设置压力输出标志
        m_ipri = flag;
    }

    int FITKBEMDoubleAsymptoticApproximation::getIpri() const
    {
        //获取压力输出标志
        return m_ipri;
    }

    // ==================== 加载标志相关方法 ====================
    void FITKBEMDoubleAsymptoticApproximation::setIpres(const int flag)
    {
        //设置加载标志
        m_ipres = flag;
    }

    int FITKBEMDoubleAsymptoticApproximation::getIpres() const
    {
        //获取加载标志
        return m_ipres;
    }

    // ==================== 刚度公式相关方法 ====================
    void FITKBEMDoubleAsymptoticApproximation::setKform(const int flag)
    {
        //设置刚度公式标志
        m_kform = flag;
    }

    int FITKBEMDoubleAsymptoticApproximation::getKform() const
    {
        //获取刚度公式标志
        return m_kform;
    }

    // ==================== 自由表面选项相关方法 ====================
    void FITKBEMDoubleAsymptoticApproximation::setFreesurf(const int flag)
    {
        //设置自由表面选项
        m_freesurf = flag;
    }

    int FITKBEMDoubleAsymptoticApproximation::getFreesurf() const
    {
        //获取自由表面选项
        return m_freesurf;
    }

    // ==================== 后流计算相关方法 ====================
    void FITKBEMDoubleAsymptoticApproximation::setAfterflow(const int flag)
    {
        //设置后流计算标志
        m_afterflow = flag;
    }

    int FITKBEMDoubleAsymptoticApproximation::getAfterflow() const
    {
        //获取后流计算标志
        return m_afterflow;
    }

    // ==================== 时间积分方法相关方法 ====================
    void FITKBEMDoubleAsymptoticApproximation::setIntegr(const int flag)
    {
        //设置时间积分方法标志
        m_integr = flag;
    }

    int FITKBEMDoubleAsymptoticApproximation::getIntegr() const
    {
        //获取时间积分方法标志
        return m_integr;
    }

    // ==================== Iform = 1: 球面波参数相关方法 ====================
    void FITKBEMDoubleAsymptoticApproximation::setPm(const double value)
    {
        //设置最大压力值
        m_pm = value;
    }

    double FITKBEMDoubleAsymptoticApproximation::getPm() const
    {
        //获取最大压力值
        return m_pm;
    }

    void FITKBEMDoubleAsymptoticApproximation::setTheta(const double value)
    {
        //设置衰减时间
        m_theta = value;
    }

    double FITKBEMDoubleAsymptoticApproximation::getTheta() const
    {
        //获取衰减时间
        return m_theta;
    }

    void FITKBEMDoubleAsymptoticApproximation::setAlpha(const double value)
    {
        //设置衰减系数
        m_alpha = value;
    }

    double FITKBEMDoubleAsymptoticApproximation::getAlpha() const
    {
        //获取衰减系数
        return m_alpha;
    }

    void FITKBEMDoubleAsymptoticApproximation::setAlphap(const double value)
    {
        //设置压力衰减系数
        m_alpha_p = value;
    }

    double FITKBEMDoubleAsymptoticApproximation::getAlphap() const
    {
        //获取压力衰减系数
        return m_alpha_p;
    }

    // ==================== Iform = 2: 函数定义参数相关方法 ====================
    void FITKBEMDoubleAsymptoticApproximation::setFctIdp(const int id)
    {
        //设置压力函数标识符
        m_fct_idp = id;
    }

    int FITKBEMDoubleAsymptoticApproximation::getFctIdp() const
    {
        //获取压力函数标识符
        return m_fct_idp;
    }

    void FITKBEMDoubleAsymptoticApproximation::setFscalep(const double value)
    {
        //设置压力函数缩放因子
        m_fscalep = value;
    }

    double FITKBEMDoubleAsymptoticApproximation::getFscalep() const
    {
        //获取压力函数缩放因子
        return m_fscalep;
    }

    void FITKBEMDoubleAsymptoticApproximation::setXc(const double value)
    {
        //设置爆炸电荷X坐标
        m_xc = value;
    }

    double FITKBEMDoubleAsymptoticApproximation::getXc() const
    {
        //获取爆炸电荷X坐标
        return m_xc;
    }

    void FITKBEMDoubleAsymptoticApproximation::setYc(const double value)
    {
        //设置爆炸电荷Y坐标
        m_yc = value;
    }

    double FITKBEMDoubleAsymptoticApproximation::getYc() const
    {
        //获取爆炸电荷Y坐标
        return m_yc;
    }

    void FITKBEMDoubleAsymptoticApproximation::setZc(const double value)
    {
        //设置爆炸电荷Z坐标
        m_zc = value;
    }

    double FITKBEMDoubleAsymptoticApproximation::getZc() const
    {
        //获取爆炸电荷Z坐标
        return m_zc;
    }

    // ==================== 自由表面平面参数相关方法 ====================
    void FITKBEMDoubleAsymptoticApproximation::setXa(const double value)
    {
        //设置自由表面点X坐标
        m_xa = value;
    }

    double FITKBEMDoubleAsymptoticApproximation::getXa() const
    {
        //获取自由表面点X坐标
        return m_xa;
    }

    void FITKBEMDoubleAsymptoticApproximation::setYa(const double value)
    {
        //设置自由表面点Y坐标
        m_ya = value;
    }

    double FITKBEMDoubleAsymptoticApproximation::getYa() const
    {
        //获取自由表面点Y坐标
        return m_ya;
    }

    void FITKBEMDoubleAsymptoticApproximation::setZa(const double value)
    {
        //设置自由表面点Z坐标
        m_za = value;
    }

    double FITKBEMDoubleAsymptoticApproximation::getZa() const
    {
        //获取自由表面点Z坐标
        return m_za;
    }

    void FITKBEMDoubleAsymptoticApproximation::setDirX(const double value)
    {
        //设置自由表面法向量X分量
        m_dir_x = value;
    }

    double FITKBEMDoubleAsymptoticApproximation::getDirX() const
    {
        //获取自由表面法向量X分量
        return m_dir_x;
    }

    void FITKBEMDoubleAsymptoticApproximation::setDirY(const double value)
    {
        //设置自由表面法向量Y分量
        m_dir_y = value;
    }

    double FITKBEMDoubleAsymptoticApproximation::getDirY() const
    {
        //获取自由表面法向量Y分量
        return m_dir_y;
    }

    void FITKBEMDoubleAsymptoticApproximation::setDirZ(const double value)
    {
        //设置自由表面法向量Z分量
        m_dir_z = value;
    }

    double FITKBEMDoubleAsymptoticApproximation::getDirZ() const
    {
        //获取自由表面法向量Z分量
        return m_dir_z;
    }

} // namespace Radioss
