/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKBEMDoubleAsymptoticApproximation.h
 * @brief  边界元法双重渐近近似（DAA）类(/BEM/DAA)
 *         Doubly Asymptotic Approximation for Underwater Explosion,
 *         where the fluid mass matrix is computed by boundary element method.
 * @author liuzhonghua
 * @date   2025-04-27
 *********************************************************************/
#ifndef FITK_BEM_DoubleAsymptoticApproximation_H
#define FITK_BEM_DoubleAsymptoticApproximation_H

#include "FITKAbstractBEM.h"

namespace Radioss
{
    /**
     * @brief  边界元法双重渐近近似类(/BEM/DAA)
     * @details
     * @author liuzhonghua
     * @date   2025-04-27
     */
    class FITKRadiossDataAPI FITKBEMDoubleAsymptoticApproximation : public FITKAbstractBEM
    {
        Q_OBJECT
        FITKCLASS(Radioss, FITKBEMDoubleAsymptoticApproximation); //!< FITK类标识
        RadiossKeyWord(FITKBEMDoubleAsymptoticApproximation, /BEM/DAA) //!< Radioss关键字标识
    public:
        /**
         * @brief  默认构造函数
         * @author liuzhonghua
         * @date   2025-04-27
         */
        explicit FITKBEMDoubleAsymptoticApproximation();

        /**
         * @brief  析构函数
         * @author liuzhonghua
         * @date   2025-04-27
         */
        virtual ~FITKBEMDoubleAsymptoticApproximation();

        /**
         * @brief  获取边界元法类型
         * @return BEMType 返回 BEM_DAA 类型，表示双重渐近近似方法
         * @author liuzhonghua
         * @date   2025-04-27
         */
        FITKAbstractBEM::BEMType getBEMType() override;

        // ==================== 表面标识符相关方法 ====================
        /**
         * @brief  设置表面标识符
         * @param id 与DAA模型关联的表面标识符
         * @note   该表面定义了与流体相互作用的结构表面
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setSurfId(const int id);

        /**
         * @brief  获取表面标识符
         * @return int 关联表面的标识符
         * @author liuzhonghua
         * @date   2025-04-27
         */
        int getSurfId() const;

        // ==================== 重力标识符相关方法 ====================
        /**
         * @brief  设置重力标识符
         * @param id 重力载荷的标识符，用于考虑重力影响
         * @note   当grav_ID > 0时，将考虑重力的影响
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setGravId(const int id);

        /**
         * @brief  获取重力标识符
         * @return int 重力载荷的标识符
         * @author liuzhonghua
         * @date   2025-04-27
         */
        int getGravId() const;

        // ==================== 流体密度相关方法 ====================
        /**
         * @brief  设置流体密度
         * @param value 流体密度值，单位：kg/m³
         * @note   默认值为1000.0 kg/m³（水的密度）
         *         流体密度是水下爆炸仿真中的重要参数
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setRho(const double value);

        /**
         * @brief  获取流体密度
         * @return double 流体密度值，单位：kg/m³
         * @author liuzhonghua
         * @date   2025-04-27
         */
        double getRho() const;

        // ==================== 声速相关方法 ====================
        /**
         * @brief  设置声速
         * @param value 声波在流体中的传播速度，单位：m/s
         * @note   默认值为1524.0 m/s（水中声速）
         *         声速影响冲击波在水中的传播特性
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setC(const double value);

        /**
         * @brief  获取声速
         * @return double 声波传播速度，单位：m/s
         * @author liuzhonghua
         * @date   2025-04-27
         */
        double getC() const;

        // ==================== 环境压力相关方法 ====================
        /**
         * @brief  设置环境压力
         * @param value 流体环境压力，单位：Pa
         * @note   默认值为101325.0 Pa（标准大气压）
         *         环境压力是流体域的初始压力状态
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setPinf(const double value);

        /**
         * @brief  获取环境压力
         * @return double 流体环境压力，单位：Pa
         * @author liuzhonghua
         * @date   2025-04-27
         */
        double getPinf() const;

        // ==================== Standoff点坐标相关方法 ====================
        /**
         * @brief  设置Standoff点X坐标
         * @param value Standoff点在X方向的位置，单位：m
         * @note   Standoff点是爆炸位置到结构物的参考点
         *         该点定义了爆炸源与结构表面之间的距离
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setXs(const double value);

        /**
         * @brief  获取Standoff点X坐标
         * @return double Standoff点在X方向的坐标，单位：m
         * @author liuzhonghua
         * @date   2025-04-27
         */
        double getXs() const;

        /**
         * @brief  设置Standoff点Y坐标
         * @param value Standoff点在Y方向的位置，单位：m
         * @note   Standoff点是爆炸位置到结构物的参考点
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setYs(const double value);

        /**
         * @brief  获取Standoff点Y坐标
         * @return double Standoff点在Y方向的坐标，单位：m
         * @author liuzhonghua
         * @date   2025-04-27
         */
        double getYs() const;

        /**
         * @brief  设置Standoff点Z坐标
         * @param value Standoff点在Z方向的位置，单位：m
         * @note   Standoff点是爆炸位置到结构物的参考点
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setZs(const double value);

        /**
         * @brief  获取Standoff点Z坐标
         * @return double Standoff点在Z方向的坐标，单位：m
         * @author liuzhonghua
         * @date   2025-04-27
         */
        double getZs() const;

        // ==================== BEM求解器标志相关方法 ====================
        /**
         * @brief  设置BEM求解器标志
         * @param flag 求解器类型标志
         * @note   iform = 0: 球面波（Spherical wave）使用Pmax和Theta
         *         iform = 2: 函数定义（Function definition）使用fct_IDp和Fscalep
         *         默认值为0
         *         该参数决定了加载方式的定义方式
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setIform(const int flag);

        /**
         * @brief  获取BEM求解器标志
         * @return int 求解器类型标志
         * @author liuzhonghua
         * @date   2025-04-27
         */
        int getIform() const;

        // ==================== 压力输出标志相关方法 ====================
        /**
         * @brief  设置压力输出标志
         * @param flag 压力输出控制标志
         * @note   用于控制是否输出压力结果
         *         不同的标志值对应不同的输出选项
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setIpri(const int flag);

        /**
         * @brief  获取压力输出标志
         * @return int 压力输出控制标志
         * @author liuzhonghua
         * @date   2025-04-27
         */
        int getIpri() const;

        // ==================== 加载标志相关方法 ====================
        /**
         * @brief  设置加载标志
         * @param flag 载荷施加控制标志
         * @note   用于控制水下爆炸载荷的施加方式
         *         影响载荷在结构上的分布特性
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setIpres(const int flag);

        /**
         * @brief  获取加载标志
         * @return int 载荷施加控制标志
         * @author liuzhonghua
         * @date   2025-04-27
         */
        int getIpres() const;

        // ==================== 刚度公式相关方法 ====================
        /**
         * @brief  设置刚度公式标志
         * @param flag 刚度矩阵计算公式标志
         * @note   用于选择不同的刚度计算方法
         *         影响流体结构耦合的计算精度
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setKform(const int flag);

        /**
         * @brief  获取刚度公式标志
         * @return int 刚度矩阵计算公式标志
         * @author liuzhonghua
         * @date   2025-04-27
         */
        int getKform() const;

        // ==================== 自由表面选项相关方法 ====================
        /**
         * @brief  设置自由表面选项
         * @param flag 自由表面边界条件选项
         * @note   控制是否考虑自由表面反射效应
         *         freessurf = 0: 无自由表面
         *         freessurf = 1: 忽略自由表面
         *         freessurf = 2: 考虑自由表面反射
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setFreesurf(const int flag);

        /**
         * @brief  获取自由表面选项
         * @return int 自由表面边界条件选项
         * @author liuzhonghua
         * @date   2025-04-27
         */
        int getFreesurf() const;

        // ==================== 后流计算相关方法 ====================
        /**
         * @brief  设置后流计算标志
         * @param flag 后流效应计算控制标志
         * @note   用于控制爆炸后流体流动效应的计算
         *         afterflow = 0: 不计算后流
         *         afterflow = 1: 计算后流（默认）
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setAfterflow(const int flag);

        /**
         * @brief  获取后流计算标志
         * @return int 后流效应计算控制标志
         * @author liuzhonghua
         * @date   2025-04-27
         */
        int getAfterflow() const;

        // ==================== 时间积分方法相关方法 ====================
        /**
         * @brief  设置时间积分方法标志
         * @param flag 时间积分方法选择标志
         * @note   用于选择数值时间积分方法
         *         不同的时间积分方法影响计算精度和稳定性
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setIntegr(const int flag);

        /**
         * @brief  获取时间积分方法标志
         * @return int 时间积分方法选择标志
         * @author liuzhonghua
         * @date   2025-04-27
         */
        int getIntegr() const;

        // ==================== Iform = 1: 球面波参数相关方法 ====================
        /**
         * @brief  设置最大压力值
         * @param value 球面波的最大压力峰值，单位：Pa
         * @note   仅当 iform = 1（球面波）时使用
         *         该参数定义爆炸冲击波的峰值压力
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setPm(const double value);

        /**
         * @brief  获取最大压力值
         * @return double 球面波的最大压力峰值，单位：Pa
         * @author liuzhonghua
         * @date   2025-04-27
         */
        double getPm() const;

        /**
         * @brief  设置衰减时间
         * @param value 压力波衰减特征时间，单位：ms
         * @note   仅当 iform = 1（球面波）时使用
         *         控制压力随时间衰减的速率
         *         典型值为冲击波的特征衰减时间
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setTheta(const double value);

        /**
         * @brief  获取衰减时间
         * @return double 压力波衰减特征时间，单位：ms
         * @author liuzhonghua
         * @date   2025-04-27
         */
        double getTheta() const;

        /**
         * @brief  设置衰减系数
         * @param value 压力波的空间衰减系数
         * @note   仅当 iform = 1（球面波）时使用
         *         控制压力随距离的衰减
         *         影响冲击波的远场传播特性
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setAlpha(const double value);

        /**
         * @brief  获取衰减系数
         * @return double 压力波的空间衰减系数
         * @author liuzhonghua
         * @date   2025-04-27
         */
        double getAlpha() const;

        /**
         * @brief  设置压力衰减系数
         * @param value 压力波形参数α'
         * @note   仅当 iform = 1（球面波）时使用
         *         用于调整压力波形的形状
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setAlphap(const double value);

        /**
         * @brief  获取压力衰减系数
         * @return double 压力波形参数α'
         * @author liuzhonghua
         * @date   2025-04-27
         */
        double getAlphap() const;

        // ==================== Iform = 2: 函数定义参数相关方法 ====================
        /**
         * @brief  设置压力函数标识符
         * @param id 用户定义的函数标识符，用于描述压力随时间变化
         * @note   仅当 iform = 2（函数定义）时使用
         *         该函数定义爆炸载荷的压力-时间曲线
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setFctIdp(const int id);

        /**
         * @brief  获取压力函数标识符
         * @return int 用户定义的函数标识符
         * @author liuzhonghua
         * @date   2025-04-27
         */
        int getFctIdp() const;

        /**
         * @brief  设置压力函数缩放因子
         * @param value 压力函数的缩放系数
         * @note   仅当 iform = 2（函数定义）时使用
         *         用于调整压力函数的幅值
         *         默认值为1.0
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setFscalep(const double value);

        /**
         * @brief  获取压力函数缩放因子
         * @return double 压力函数的缩放系数
         * @author liuzhonghua
         * @date   2025-04-27
         */
        double getFscalep() const;

        /**
         * @brief  设置爆炸电荷X坐标
         * @param value 爆炸源在X方向的位置，单位：m
         * @note   仅当 iform = 2（函数定义）时使用
         *         定义爆炸源的空间位置
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setXc(const double value);

        /**
         * @brief  获取爆炸电荷X坐标
         * @return double 爆炸源在X方向的坐标，单位：m
         * @author liuzhonghua
         * @date   2025-04-27
         */
        double getXc() const;

        /**
         * @brief  设置爆炸电荷Y坐标
         * @param value 爆炸源在Y方向的位置，单位：m
         * @note   仅当 iform = 2（函数定义）时使用
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setYc(const double value);

        /**
         * @brief  获取爆炸电荷Y坐标
         * @return double 爆炸源在Y方向的坐标，单位：m
         * @author liuzhonghua
         * @date   2025-04-27
         */
        double getYc() const;

        /**
         * @brief  设置爆炸电荷Z坐标
         * @param value 爆炸源在Z方向的位置，单位：m
         * @note   仅当 iform = 2（函数定义）时使用
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setZc(const double value);

        /**
         * @brief  获取爆炸电荷Z坐标
         * @return double 爆炸源在Z方向的坐标，单位：m
         * @author liuzhonghua
         * @date   2025-04-27
         */
        double getZc() const;

        // ==================== 自由表面平面参数相关方法 ====================
        /**
         * @brief  设置自由表面点X坐标
         * @param value 自由表面平面参考点在X方向的坐标，单位：m
         * @note   当 grav_ID > 0 或 Freesurf = 2 时使用
         *         用于定义自由表面的位置
         *         该点与dir_x, dir_y, dir_z共同确定自由表面平面
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setXa(const double value);

        /**
         * @brief  获取自由表面点X坐标
         * @return double 自由表面平面参考点在X方向的坐标，单位：m
         * @author liuzhonghua
         * @date   2025-04-27
         */
        double getXa() const;

        /**
         * @brief  设置自由表面点Y坐标
         * @param value 自由表面平面参考点在Y方向的坐标，单位：m
         * @note   当 grav_ID > 0 或 Freesurf = 2 时使用
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setYa(const double value);

        /**
         * @brief  获取自由表面点Y坐标
         * @return double 自由表面平面参考点在Y方向的坐标，单位：m
         * @author liuzhonghua
         * @date   2025-04-27
         */
        double getYa() const;

        /**
         * @brief  设置自由表面点Z坐标
         * @param value 自由表面平面参考点在Z方向的坐标，单位：m
         * @note   当 grav_ID > 0 或 Freesurf = 2 时使用
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setZa(const double value);

        /**
         * @brief  获取自由表面点Z坐标
         * @return double 自由表面平面参考点在Z方向的坐标，单位：m
         * @author liuzhonghua
         * @date   2025-04-27
         */
        double getZa() const;

        /**
         * @brief  设置自由表面法向量X分量
         * @param value 自由表面平面法向量在X方向的分量
         * @note   当 grav_ID > 0 或 Freesurf = 2 时使用
         *         与dir_y和dir_z共同定义自由表面的朝向
         *         法向量应为单位向量
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setDirX(const double value);

        /**
         * @brief  获取自由表面法向量X分量
         * @return double 自由表面平面法向量在X方向的分量
         * @author liuzhonghua
         * @date   2025-04-27
         */
        double getDirX() const;

        /**
         * @brief  设置自由表面法向量Y分量
         * @param value 自由表面平面法向量在Y方向的分量
         * @note   当 grav_ID > 0 或 Freesurf = 2 时使用
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setDirY(const double value);

        /**
         * @brief  获取自由表面法向量Y分量
         * @return double 自由表面平面法向量在Y方向的分量
         * @author liuzhonghua
         * @date   2025-04-27
         */
        double getDirY() const;

        /**
         * @brief  设置自由表面法向量Z分量
         * @param value 自由表面平面法向量在Z方向的分量
         * @note   当 grav_ID > 0 或 Freesurf = 2 时使用
         *         与dir_x和dir_y共同定义自由表面的朝向
         *         默认值为1.0（Z轴正方向）
         * @author liuzhonghua
         * @date   2025-04-27
         */
        void setDirZ(const double value);

        /**
         * @brief  获取自由表面法向量Z分量
         * @return double 自由表面平面法向量在Z方向的分量
         * @author liuzhonghua
         * @date   2025-04-27
         */
        double getDirZ() const;

    private:
        /**
         * @brief  关联表面标识符
         * @note   定义与流体相互作用的结构表面
         */
        int m_surf_id{ 0 };

        /**
         * @brief  重力载荷标识符
         * @note   当值大于0时，考虑重力影响
         */
        int m_grav_id{ 0 };

        /**
         * @brief  流体密度
         * @note   默认值为1000.0 kg/m³（水的密度）
         */
        double m_rho{ 1000.0 };

        /**
         * @brief  声波传播速度
         * @note   默认值为1500.0 m/s（水中声速）
         */
        double m_c{ 1500.0 };

        /**
         * @brief  环境压力
         * @note   默认值为101325.0 Pa（标准大气压）
         */
        double m_p_inf{ 101325.0 };

        /**
         * @brief  Standoff点X坐标
         * @note   爆炸位置到结构物的参考点X坐标
         */
        double m_xs{ 0.0 };

        /**
         * @brief  Standoff点Y坐标
         * @note   爆炸位置到结构物的参考点Y坐标
         */
        double m_ys{ 0.0 };

        /**
         * @brief  Standoff点Z坐标
         * @note   爆炸位置到结构物的参考点Z坐标
         */
        double m_zs{ 0.0 };

        /**
         * @brief  BEM求解器标志
         * @note   0: 球面波, 2: 函数定义
         */
        int m_iform{ 0 };

        /**
         * @brief  压力输出标志
         * @note   控制压力结果的输出选项
         */
        int m_ipri{ 0 };

        /**
         * @brief  载荷施加标志
         * @note   控制载荷的施加方式
         */
        int m_ipres{ 1 };

        /**
         * @brief  刚度公式标志
         * @note   选择刚度矩阵的计算方法
         */
        int m_kform{ 1 };

        /**
         * @brief  自由表面选项
         * @note   0: 无, 1: 忽略, 2: 考虑
         */
        int m_freesurf{ 0 };

        /**
         * @brief  后流计算标志
         * @note   控制爆炸后流体流动效应的计算
         */
        int m_afterflow{ 1 };

        /**
         * @brief  时间积分方法标志
         * @note   选择数值时间积分方法
         */
        int m_integr{ 1 };

        /**
         * @brief  最大压力值
         * @note   仅当 iform = 1 时使用，单位：Pa
         */
        double m_pm{ 0.0 };

        /**
         * @brief  衰减时间
         * @note   仅当 iform = 1 时使用，单位：ms
         */
        double m_theta{ 0.0 };

        /**
         * @brief  空间衰减系数
         * @note   仅当 iform = 1 时使用
         */
        double m_alpha{ 0.0 };

        /**
         * @brief  压力衰减系数
         * @note   仅当 iform = 1 时使用
         */
        double m_alpha_p{ 0.0 };

        /**
         * @brief  压力函数标识符
         * @note   仅当 iform = 2 时使用
         */
        int m_fct_idp{ 0 };

        /**
         * @brief  压力函数缩放因子
         * @note   仅当 iform = 2 时使用
         */
        double m_fscalep{ 1.0 };

        /**
         * @brief  爆炸电荷X坐标
         * @note   仅当 iform = 2 时使用，单位：m
         */
        double m_xc{ 0.0 };

        /**
         * @brief  爆炸电荷Y坐标
         * @note   仅当 iform = 2 时使用，单位：m
         */
        double m_yc{ 0.0 };

        /**
         * @brief  爆炸电荷Z坐标
         * @note   仅当 iform = 2 时使用，单位：m
         */
        double m_zc{ 0.0 };

        /**
         * @brief  自由表面参考点X坐标
         * @note   当 grav_ID > 0 或 Freesurf = 2 时使用
         */
        double m_xa{ 0.0 };

        /**
         * @brief  自由表面参考点Y坐标
         * @note   当 grav_ID > 0 或 Freesurf = 2 时使用
         */
        double m_ya{ 0.0 };

        /**
         * @brief  自由表面参考点Z坐标
         * @note   当 grav_ID > 0 或 Freesurf = 2 时使用
         */
        double m_za{ 0.0 };

        /**
         * @brief  自由表面法向量X分量
         * @note   当 grav_ID > 0 或 Freesurf = 2 时使用
         */
        double m_dir_x{ 0.0 };

        /**
         * @brief  自由表面法向量Y分量
         * @note   当 grav_ID > 0 或 Freesurf = 2 时使用
         */
        double m_dir_y{ 0.0 };

        /**
         * @brief  自由表面法向量Z分量
         * @note   当 grav_ID > 0 或 Freesurf = 2 时使用
         *         默认值为1.0（Z轴正方向）
         */
        double m_dir_z{ 1.0 };
    };

} // namespace Radioss

#endif // FITK_BEM_DoubleAsymptoticApproximation_H
