/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractLicenseChecker.cpp
 * @brief 许可证检查抽象类实现
 * @author LiBaoJun (libaojunqd@foxmail.com)
 * @date 2025-04-02
 *
 */
#include "FITKAbstractLicenseChecker.h"
#include "FITKMessage.h"
#include <QNetworkInterface>
#include <QSysInfo>

namespace AppFrame
{
    FITKAbstractLicenseChecker::FITKAbstractLicenseChecker()
        : m_licenseEnabled(false)
    {
        // 默认启用许可证
    }

    FITKAbstractLicenseChecker::~FITKAbstractLicenseChecker()
    {
    }
    bool FITKAbstractLicenseChecker::checkFeatureLicense(const QString& feature)
    {
        return false;
    }
    bool FITKAbstractLicenseChecker::checkLicense(QString& errorMessage)
    {
        return false;
    }  
 
    

    void FITKAbstractLicenseChecker::message(int type,const QString& message)
    {
        switch (type)
        {
        case 1:
            FITKMessageNormal(message);
            break;
        case 2:
            FITKMessageWarning(message);
            break;
        case 3:
            FITKMessageError(message);
            break;
        case 4:
            FITKMessageInfo(message);
            break;
        default:
            FITKMessageInfo(message);
            break;
        }
    }

    void FITKAbstractLicenseChecker::addFeatureLicense(const QString& feature, bool enabled)
    {
        m_featureLicenses[feature] = enabled;
        message(1, QString("Feature license '%1' added, status: %2").arg(feature).arg(enabled ? "Enabled" : "Disabled"));
    }
    bool FITKAbstractLicenseChecker::isFratureLicenseEnabled(const QString& feature)
    {
        if (m_featureLicenses.contains(feature))
        {
            return m_featureLicenses.value(feature);
        }
        return false;
    }

    void FITKAbstractLicenseChecker::removeFeatureLicense(const QString& feature)
    {
        if (m_featureLicenses.remove(feature) > 0)
        {
            message(1, QString("Feature license '%1' removed").arg(feature));
        }
        else
        {
            message(2, QString("Feature license '%1' does not exist").arg(feature));
        }
    }

    void FITKAbstractLicenseChecker::setLicenseEnabled(bool enabled)
    {
        m_licenseEnabled = enabled;
        message(1, QString("License status set to: %1").arg(enabled ? "Enabled" : "Disabled"));
    }
    
    bool FITKAbstractLicenseChecker::isLicenseEnabled() const
    {
        return m_licenseEnabled;
    }

    QString FITKAbstractLicenseChecker::getSystemIdentifier()
    {
        // 获取第一个有效的MAC地址作为系统标识符
        QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
        for (const QNetworkInterface& interface : interfaces)
        {
            // 跳过回环接口和非活动接口
            if (interface.flags().testFlag(QNetworkInterface::IsLoopBack) || 
                !interface.flags().testFlag(QNetworkInterface::IsUp) || 
                !interface.flags().testFlag(QNetworkInterface::IsRunning))
            {
                continue;
            }

            // 获取MAC地址
            QString macAddress = interface.hardwareAddress();
            if (!macAddress.isEmpty())
            {
                return macAddress;
            }
        }

        // 如果没有找到有效的MAC地址，则使用机器唯一ID
        return QSysInfo::machineUniqueId();
    }

    QStringList FITKAbstractLicenseChecker::getSystemInfoList()
    {
        QStringList systemInfoList;
        
        // 添加所有网络接口的MAC地址
        QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
        for (const QNetworkInterface& interface : interfaces)
        {
            // 跳过回环接口
            if (interface.flags().testFlag(QNetworkInterface::IsLoopBack))
            {
                continue;
            }

            // 获取MAC地址
            QString macAddress = interface.hardwareAddress();
            if (!macAddress.isEmpty())
            {
                systemInfoList.append(QString("MAC: %1 (%2)").arg(macAddress).arg(interface.name()));
            }
        }

        // 添加其他系统信息
        systemInfoList.append(QString("Hostname: %1").arg(QSysInfo::machineHostName()));
        systemInfoList.append(QString("OS: %1 %2").arg(QSysInfo::productType()).arg(QSysInfo::productVersion()));
        systemInfoList.append(QString("Architecture: %1").arg(QSysInfo::currentCpuArchitecture()));
        systemInfoList.append(QString("Machine ID: %1").arg(QSysInfo::machineUniqueId().constData()));

        return systemInfoList;
    }
    QStringList FITKAbstractLicenseChecker::getLicenseInfoList()
    {
        return QStringList();
    }
    QString FITKAbstractLicenseChecker::getLicenseFile() const
    {
        return _licenseFile;
    }
    void FITKAbstractLicenseChecker::setLicenseFile(const QString& licenseFile)
    {
        _licenseFile = licenseFile;
    }
}