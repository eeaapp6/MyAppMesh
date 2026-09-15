/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractGUIObject.h
 * @brief GUI对象基类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-23
 *
 */
#ifndef __FITKGUIABSTRACTOBJECT_H__
#define __FITKGUIABSTRACTOBJECT_H__

#include "FITKCoreAPI.h"
#include "FITKCoreMacros.h"
#include "FITKAbstractObject.hpp"
#include <QHash>

namespace Core
{
    /**
     * @brief GUI窗体基类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-02-23
     */
    class FITKCoreAPI FITKAbstractGUIObject : public FITKAbstractObject
    {
    public:
        /**
         * @brief Construct a new FITKAbstractGUIObject object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-23
         */
        explicit FITKAbstractGUIObject();
        /**
         * @brief Destroy the FITKAbstractGUIObject object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-23
         */
        virtual ~FITKAbstractGUIObject() = 0;
        /**
         * @brief 获取窗体ID
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-23
         */
        int getGUIObjectID() const;

        /**
       * @brief 保存脚本
       * @param[i]  script  脚本
       * @author LiBaojun (libaojunqd@foxmail.com)
       * @date 2024-10-31
       */
        virtual void saveScript(const QStringList& script);
        /**
         * @brief 保存脚本
         * @param[i]  script  脚本
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-31
         */
        virtual void saveScript(const QString& script);
        /**
         * @brief 是否包含附加信息
         * @param[i]  key            附加信息标记
         * @return true        包含
         * @return false       不包含
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-11-22
         */
        bool isContainsInfo(const QString& key);
        /**
         * @brief 获取信息
         * @param[i]  key            附加信息标记
         * @return QVariant 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-11-22
         */
        QVariant getAdditionalInfo(const QString& key);
        /**
         * @brief 设置附加信息
         * @param[i]  key            附加信息标记
         * @param[i]  va             附加信息
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-11-22
         */
        void setAdditionalInfo(const QString& key, const QVariant& va);
        /**
         * @brief 移除附加信息
         * @param[i]  key            附加信息标记
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-11-22
         */
        void removeAdditionalInfo(const QString& key);
        /**
         * @brief 清除附加信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-11-22
         */ 
        void clearAdditionalInfo();

    private:
        /**
         * @brief 窗体ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-23
         */
        int _guiID{ 0 };
        /**
         * @brief 窗体附加信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-11-22
         */
        QHash<QString, QVariant> _additionalInfo{};
    };
}


#endif // !__FITKGUIObject
