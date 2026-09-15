/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAbstractEquationOfState.h
 * @brief  EOS状态方程基类声明
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-10-28
 *********************************************************************/
#ifndef __FITKABSTRACT_EQUATIONOFSTATE_H__
#define __FITKABSTRACT_EQUATIONOFSTATE_H__

#include "FITKAbstractRadiossData.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Radioss
{
    class FITKEquationOfStateManager;
     /**
      * @brief  EOS状态方程基类
      * @author liuzhonghua (liuzhonghuaszch@163.com)
      * @date   2025-10-28
      */
    class FITKRadiossDataAPI FITKAbstractEquationOfState : public Core::FITKAbstractNDataObject,
        public FITKAbstractRadiossData
    {
        Q_OBJECT
    public:
        enum EOS_Type
        {
            Unknown = -1,
            Polynomial,  //多项式
        };
        Q_ENUM(EOS_Type);
        /*只有以下材料有且必须有状态方程
          LAW3(HYDPLA)
          LAW4(HYD_JCOOK)
          LAW6(HYDRO or HYD_VISC)
          LAW10(DPRAG1)
          LAW12(3D_COMP)
          LAW49(STEINB)
          LAW102(DPRAG2)
          LAW103(HENSEL - SPITTEL)
         */
    public:
        explicit FITKAbstractEquationOfState() =default;
        virtual ~FITKAbstractEquationOfState() =0;

        virtual EOS_Type getEOSType() = 0;
        /**
         * @brief 获取材料数量
         */
        int getMaterialIDCount();
        /**
         * @brief    添加材料ID,检测其他状态方程中是否有该材料ID，若存在，则移除
         */
        void addMaterialID(int matID, FITKEquationOfStateManager*mgr=nullptr);
        /**
         * @brief    获取材料ID
         */
        QList<int> getMaterialIDs();
        /**
         * @brief 移除材料ID
         */
        void removeMaterialID(int matID);
        /**
         * @brief 检查材料ID是否存在,存在返回true
         */
        bool containsMaterialID(int matID);
        /**
         * @brief 清空所有材料ID
         */
        void clearMaterialIDs();
        /**
         * @brief    Set Reference density
         * @param[i] value
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        void setP0(double value);
        /**
         * @brief    Get Reference density
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        double getP0();

    private:
        /**
         * @brief  材料ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-28
         */
        QList<int> m_matIDs{}; 
        /**
         * @brief  Reference density
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-28
         */
        double m_p0{ 0.0 };
    };
    /**
     * @brief  状态方程管理器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-10-28
     */
    class FITKRadiossDataAPI FITKEquationOfStateManager :  public Core::FITKAbstractDataManager<FITKAbstractEquationOfState>
    {
    public:
        explicit FITKEquationOfStateManager() = default;
        virtual ~FITKEquationOfStateManager() = default;
    };
}


#endif