/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFDiscretization.h
 * @brief  离散数据接口声明
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-21
 *********************************************************************/
#ifndef _FITK_OF_DISCREIZATION_H___
#define _FITK_OF_DISCREIZATION_H___

#include "FITKInterfaceFlowOFAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Core
{
    class FITKParameter;
}

namespace Interface
{
    /**
     * @brief  离散数据对象
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    class FITKInterfaceFlowOFAPI FITKOFDiscretization : public Core::FITKAbstractNDataObject
    {
        Q_OBJECT
            FITKCLASS(Interface, FITKOFDiscretization);
    public:
        explicit FITKOFDiscretization();
        virtual ~FITKOFDiscretization();

        /**
         * @brief    仅保留名称一致的对流数据
         * @param[i] nameList 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-23
         */
        void onlyKeepConvection(QStringList nameList);
        /**
         * @brief    获取Time
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        Core::FITKParameter* getTimeOption();

        /**
         * @brief    获取对流变量数量
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        int getConvectionCount();
        /**
         * @brief    获取对流变量名称
         * @param[i] index 
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        QString getConvectionVName(int index);
        /**
         * @brief    获取对流变量
         * @param[i] index 
         * @return   Core::FITKParameter*
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        Core::FITKParameter* getConvectionVPara(int index);
        /**
         * @brief    获取Gradients
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        Core::FITKParameter* getGradients();
        /**
         * @brief  获取Interpolation
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-21
         */
        Core::FITKParameter* getInterpolation();

        /**
         * @brief    最加对流变量
         * @param[i] variable 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        void insertConvection(QString name, Core::FITKParameter* variable);
        /**
         * @brief    清理对流数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-12
         */
        void clearConvection();
        /**
         * @brief    初始化Gradients
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        void initGradients();
        /**
         * @brief    初始化Interpolation
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        void initInterpolation();
        /**
         * @brief    默认的最加湍流变化的变量初始化
         * @param[i] name 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        void initAppendTurbulenceConvection(QString name);

    private:
        /**
         * @brief    清理数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        void clearData();

    private:
        Core::FITKParameter* _timeOption{};
        QList<QPair<QString, Core::FITKParameter*>> _convection{};
        Core::FITKParameter* _gradients{};
        Core::FITKParameter* _interpolation{};
    };
}


#endif
