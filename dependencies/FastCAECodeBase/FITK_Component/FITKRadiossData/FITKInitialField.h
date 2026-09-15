/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKInitialField.h
 * @brief  初始化场数据
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-11-07
 *********************************************************************/
#ifndef _FITK_INITIAL_VALUE_H_
#define _FITK_INITIAL_VALUE_H_

#include "FITKAbstractInitialField.h"


namespace Radioss
{
   

    class FITKRadiossDataAPI FITKInitialFieldTra : public FITKAbstractInitialField
    {
        
        FITKCLASS(Radioss, FITKInitialFieldTra); //!< FITK类标识
        RadiossKeyWord(FITKInitialFieldTra,/INIVEL/TRA) //!< Radioss关键字标识

    public:
        explicit FITKInitialFieldTra() = default;
        virtual ~FITKInitialFieldTra() = default;
        /**
         * @brief    获取初始化场类型
         * @return   InitialFieldType
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-07
         */
        InitialFieldType getType() const override;
    };
    /**
     * @brief  角速度
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-11-07
     */
    class FITKRadiossDataAPI FITKInitialFieldAxis : public FITKAbstractInitialField
    {

        FITKCLASS(Radioss, FITKInitialFieldAxis); //!< FITK类标识
        RadiossKeyWord(FITKInitialFieldAxis, /INIVEL/AXIS) //!< Radioss关键字标识

    public:
        explicit FITKInitialFieldAxis() = default;
        virtual ~FITKInitialFieldAxis() = default;
        /**
         * @brief    获取初始化场类型
         * @return   InitialFieldType
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-07
         */
        InitialFieldType getType() const override;
        /**
         * @brief    Set Direction X, Y, and Z
         * @param[i] text 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-07
         */
        void setDir(QString text);
        /**
         * @brief    Get Direction X, Y, and Z
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-07
         */
        QString getDir();
        /**
         * @brief    Set Frame id
         * @param[i] id 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-07
         */
        void setFrameID(int id);
        /**
         * @brief    Get Frame id
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-07
         */
        int getFrameID();
        /**
         * @brief    Set Rotational velocity about the rotational axis
         * @param[i] value 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-07
         */
        void setVr(double value);
        /**
         * @brief    Get Rotational velocity about the rotational axis
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-07
         */
        double getVr();

    private:
        /**
         * @brief  Direction X, Y, and Z
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-07
         */
        QString m_dir{"X"};
        /**
         * @brief  Frame id
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-07
         */
        int m_frame_ID{ -1 };
        /**
         * @brief  	Rotational velocity about the rotational axis
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-07
         */
        double m_rotationalVelocity{ 0.0 };
    };

  
}






#endif