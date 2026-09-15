/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractLicenseChecker.h
 * @brief 许可证检查抽象类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-04-02
 *
 */
#ifndef _FITKABSTRACTLICENSECHECKER_H__
#define _FITKABSTRACTLICENSECHECKER_H__

//#include "FITK_Kernel/FITKCore/FITKAbstractObject.hpp"
#include "FITKAppFrameworkAPI.h"
#include <QString>
#include <QMap>
#include <QStringList>

namespace AppFrame
{
  
    /**
     * @brief 许可证检查抽象类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-04-02
     */
    class FITKAppFrameworkAPI FITKAbstractLicenseChecker// : public Core::FITKAbstractObject
    {
    public:
        /**
         * @brief Construct a new FITKAbstractLicenseChecker object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-02
         */
        explicit FITKAbstractLicenseChecker();
        /**
         * @brief Destroy the FITKAbstractLicenseChecker object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-02
         */
        virtual ~FITKAbstractLicenseChecker() = 0;

        /**
         * @brief 检查许可证是否有效
         * @param[o] errorMessage 错误信息
         * @return true 许可证有效
         * @return false 许可证无效
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-02
         */
        virtual bool checkLicense(QString& errorMessage) = 0;

        /**
         * @brief 检查指定功能的许可证是否有效
         * @param[i] feature 功能名称
         * @return true 功能许可证有效
         * @return false 功能许可证无效
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-02
         */
        virtual bool checkFeatureLicense(const QString& feature);

        /**
         * @brief 添加功能许可证
         * @param[i] feature 功能名称
         * @param[i] enabled 是否启用
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-03
         */
        virtual void addFeatureLicense(const QString& feature, bool enabled = true);

        /**
         * @brief 移除功能许可证
         * @param[i] feature 功能名称
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-03
         */
        virtual void removeFeatureLicense(const QString& feature);
        /**
         * @brief 获取功能许可证状态
         * @param[i] feature 功能名称
         * @return true 功能许可证启用
         * @return false 功能许可证禁用
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-07
         */
        bool isFratureLicenseEnabled(const QString& feature);

        /**
         * @brief 设置许可证状态
         * @param[i] enabled 是否启用
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-03
         */
        void setLicenseEnabled(bool enabled);

        /**
         * @brief 获取许可证状态
         * @return bool 许可证状态
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-03
         */
        bool isLicenseEnabled() const;

    

        /**
         * @brief 获取系统唯一标识符（如MAC地址）
         * @return QString 系统唯一标识符
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-03
         */
        virtual QString getSystemIdentifier();

        /**
         * @brief 获取系统信息列表（如所有网络接口的MAC地址）
         * @return QStringList 系统信息列表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-03
         */
        virtual QStringList getSystemInfoList();

        /**
         * @brief 获取许可证信息列表
         * @return QStringList 许可证信息列表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-03
         */
        virtual QStringList getLicenseInfoList();

        /**
         * @brief 获取许可证文件路径
         * @return QString 许可证文件路径
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-03
         */
        QString getLicenseFile() const;

        /**
         * @brief 设置许可证文件路径
         * @param[i] licenseFile 许可证文件路径
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-03
         */
        void setLicenseFile(const QString& licenseFile);
    protected:
        /**
         * @brief 输出消息
         * @param[i] message 消息内容
         * @param[i] type 消息类型 1: 正常信息 2: 警告信息 3: 错误信息 4: 一般信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-02
         */
        void message( int type , const QString& message);
    protected:
        /**
         * @brief 许可证文件路径
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-02
         */
        QString _licenseFile{};
        /**
         * @brief 许可证是否启用
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-02
         */
        bool m_licenseEnabled{false}; 
        /**
         * @brief 功能许可证映射表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-02
         */
        QMap<QString, bool> m_featureLicenses{};
    };
}

#endif // _FITKABSTRACTLICENSECHECKER_H__ 