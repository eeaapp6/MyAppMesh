/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFTransportProp.h
 * @brief  材料数据
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-06-17
 *********************************************************************/
#ifndef _FITK_OF_TRANSPORT_PROP__H___
#define _FITK_OF_TRANSPORT_PROP__H___
 
#include "FITKInterfaceFlowOFAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"


namespace Core
{
    class FITKParameter;
}

namespace Interface
{
    class FITKAbsOFTransportModel;
    /**
     * @brief 相位
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-17
     */
    class FITKInterfaceFlowOFAPI FITKOFTransportPhase
    {
    public:
        explicit FITKOFTransportPhase();
        virtual ~FITKOFTransportPhase();
        /**
         * @brief    获取材料相位名称
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        QString getPhaseName();
        /**
         * @brief    获取相位数据
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        Core::FITKParameter* getPhaseAdditionalData();
        /**
         * @brief    获取运输模型
         * @return   FITKAbsOFTransportModel *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        FITKAbsOFTransportModel* getTransportModel();
        /**
         * @brief    设置相位名称
         * @param[i] name 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        void setPhaseName(QString name);
        /**
         * @brief    设置相位的其余数据
         * @param[i] data 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        void setPhaseAdditionalData(Core::FITKParameter* data);
        /**
         * @brief    设置运输模型
         * @param[i] modelData 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        void setTransportModel(FITKAbsOFTransportModel* modelData);

    private:
        /**
         * @brief  相位名称
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-17
         */
        QString _name{};
        /**
         * @brief  相位其余数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-17
         */
        Core::FITKParameter* _additional{};
        /**
         * @brief  运输模型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-17
         */
        FITKAbsOFTransportModel* _transportModel{};
    };
    /**
     * @brief  运输模型属性
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-17
     */
    class FITKInterfaceFlowOFAPI FITKOFTransportProp : public Core::FITKAbstractNDataObject
    {
        Q_OBJECT
            FITKCLASS(Interface, FITKOFTransportProp);
    public:
        explicit FITKOFTransportProp();
        virtual ~FITKOFTransportProp();
        /**
         * @brief    获取相位数量
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        int getPhasesCount();
        /**
         * @brief    获取相位
         * @param[i] index 
         * @return   FITKOFTransportPhase *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        FITKOFTransportPhase* getPhase(int index);
        /**
         * @brief    获取传输附加数据
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        Core::FITKParameter* getTransportAdditionalData();
        /**
         * @brief    添加相位数据
         * @param[i] phase 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        void appendPhase(FITKOFTransportPhase* phase);
        /**
         * @brief    设置传输附加数据
         * @param[i] tranAddData 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        void setTransportAdditionalData(Core::FITKParameter* tranAddData);

    protected:
    private:
        /**
         * @brief  传输附加数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-17
         */
        Core::FITKParameter* _additionalTrans{};
        /**
         * @brief  相位数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-17
         */
        QList<FITKOFTransportPhase*> _phases{};
    };
}


#endif
