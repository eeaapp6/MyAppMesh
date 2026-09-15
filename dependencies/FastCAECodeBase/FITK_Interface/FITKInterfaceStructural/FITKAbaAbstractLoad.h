/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaAbstractLoad.h
 * @brief  载荷基类
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-03
 * 
 */
#ifndef FITKABAABSTRACTLOAD_H
#define FITKABAABSTRACTLOAD_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaStepRelatedDataObject.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractLoadBC.h"


namespace Interface 
{
    class FITKAbstractFiled;
 
    /**
     * @brief  载荷基类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-03
     */
    class FITKInterfaceStructuralAPI FITKAbaAbstractLoad: public FITKAbstractLoad, 
        public FITKAbaStepRelatedDataObject
    {
        Q_OBJECT
    public:
        /**
         * @brief  载荷类型枚举
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        enum FITKAbaLoadType
        {
            ConcentratedForce,
            Moment,
            Pressure,
            BodyForce,
            Gravity,
        };
        Q_ENUM(FITKAbaLoadType);

    public:
        /**
         * @brief  构造
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        FITKAbaAbstractLoad() = default;
        /**
         * @brief  析构
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        ~FITKAbaAbstractLoad() = default;
        /**
         * @brief  获取载荷类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        virtual FITKAbaLoadType getLoadType() const = 0;
        /**
         * @brief  拷贝数据
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        bool copy(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief  在创建所在的分析步中是否有效，当分析步发生变化时调用
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        bool isValidInCreatedStep() override;
        /**
         * @brief  是否成功获取分析步管理器
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-26
         */
        bool hasGetStepManager() override;
        /**
         * @brief  获取载荷激活状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        bool getActiveState() const;
        /**
         * @brief  设置载荷激活状态
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        void setActiveState(bool stat);
        /**
      * @brief  设置解析场/离散场ID
      * @param[i] id
      * @author Libaojun (libaojunqd@foxmail.com)
      * @date   2024-07-12
      */
        void setFieldID(const int id);
        /**
         * @brief  获取解析场/离散场
         * @return FITKAbstractFiled*
         * @author Libaojun (libaojunqd@foxmail.com)
         * @date   2024-07-12
         */
        FITKAbstractFiled* getField();

        /**
         * @brief  获取幅值曲线Id
         * @param[in]  stepId 分析步Id
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        virtual int getAmplitudeId(int stepId = 0);
        /**
         * @brief  设置幅值曲线Id
         * @param[in]  stepId 分析步Id
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-09
         */
        virtual void setAmplitudeId(int id, int stepId = 0);
    protected:
        /**
         * @brief  载荷激活状态
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-03
         */
        bool m_activeState{ true };
        /**
         * @brief  解析场/离散场ID
         * @author Libaojun (libaojunqd@foxmail.com)
         * @date   2024-07-12
         */
        int _fieldID{ -1 };
    };

}

#endif // FITKABAABSTRACTLOAD_H
