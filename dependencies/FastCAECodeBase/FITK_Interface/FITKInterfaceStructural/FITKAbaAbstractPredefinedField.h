/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaAbstractPredefinedField.h
 * @brief  预定义场基类
 * @author LiJin (1003680421@qq.com)
 * @date   2024-05-10
 * 
 */
#ifndef FITKABAABSTRACTPREDEFINEDFIELD_H
#define FITKABAABSTRACTPREDEFINEDFIELD_H
#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKBoundaryLocation.h"
#include "FITKAbaStepRelatedDataObject.h"


namespace Interface
{
    class FITKAbstractFiled;
    /**
     * @brief  预定义场基类
     * @author LiJin (1003680421@qq.com)
     * @date   2024-05-10
     */
    class FITKInterfaceStructuralAPI FITKAbaAbstractPredefinedField: 
        public FITKBoundaryLocation, public FITKAbaStepRelatedDataObject
    {
        Q_OBJECT
    public:
        /**
        * @brief  预定义场类型
        * @author LiJin (1003680421@qq.com)
        * @date   2024-05-10
        */
        enum FITKAbaPredefinedFiledType
        {
            Temperature,
            Velocity
        };
        Q_ENUM(FITKAbaPredefinedFiledType);
    public:
        FITKAbaAbstractPredefinedField() = default;
        ~FITKAbaAbstractPredefinedField() = default;
        /**
         * @brief  获取预定义场类型
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
         */
        virtual FITKAbaPredefinedFiledType getPreDefinedType() const = 0;
        bool hasGetStepManager() override;
        /**
         * @brief  获取预定义场激活状态
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
         */
        bool getActiveState() const;
        /**
         * @brief  设置预定义场激活状态
         * @param[in]
         * @return
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
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

    protected:
        /**
         * @brief  预定义场激活状态
         * @author LiJin (1003680421@qq.com)
         * @date   2024-05-10
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


#endif // FITKABAABSTRACTPREDEFINEDFIELD_H
