/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorSolverRequest.h
 * @brief  Radioss适配器 用于结果请求的读写
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-10-29
 *********************************************************************/
#ifndef _FITKRADIOSSADAPTORSOLVERREQUEST_H_
#define _FITKRADIOSSADAPTORSOLVERREQUEST_H_
#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossResultRequest.h"

namespace Radioss
{
    class FITKRadiossCase;
 
    /**
     * @brief  Radioss RAD 读写适配器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-07-02
     */
    class FITKRadiossAdaptorSolverRequest : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorSolverRequest() = default;
        ~FITKRadiossAdaptorSolverRequest() = default;
        /**
         * @brief    获取适配器数据类型名
         * @return   QString 适配器数据类型名
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-02
         */
        QString getAdaptorClass() override;
        /**
         * @brief    适配器读取
         * @return   bool true成功，false失败
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-02
         */
        bool adaptR() override;
        /**
         * @brief    适配器写出
         * @return   bool true成功，false失败
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-02
         */
        bool adaptW() override;

    private:
        /**
         * @brief    读取历程频率
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        bool readHisFrequency();
        /**
         * @brief    写出历程频率
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        bool writeHisFrequency();
        /**
         * @brief    读取动画参数
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-11
         */
        bool readAnimationParam();
        /**
         * @brief    读取动画时间步
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        bool readAnimationTimeStep();
        /**
         * @brief    写出动画时间步
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        bool writeAnimationTimeStep();
        /**
         * @brief    读取动画节点数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        bool readAnimationVECT();
        /**
         * @brief    写出动画节点数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        bool writeAnimationVECT();
        /**
         * @brief    读取动画单元数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        bool readAnimationElement();
        /**
         * @brief    读取动画体单元数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        bool readAnimationBrick();
        /**
         * @brief    读取动画壳单元数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        bool readAnimationShell();
        /**
         * @brief    写出动画单元数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        bool writeAnimationElement();
        /**
         * @brief    数据提取
         * @param[i] line
         * @return   QStringList
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        QStringList getKeywordExtraction(QString line);
    };
    /**
     * @brief 注册适配器
     * @date   2025-07-07
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKRadiossResultRequest, FITKRadiossAdaptorSolverRequest);
    RegRadiossRadAdaptor(/ANIM, FITKRadiossAdaptorSolverRequest, 1);
    RegRadiossRadAdaptor(/TFILE, FITKRadiossAdaptorSolverRequest, 2);
}
#endif // !_FITKRADIOSSADAPTORSOLVERREQUEST_H_