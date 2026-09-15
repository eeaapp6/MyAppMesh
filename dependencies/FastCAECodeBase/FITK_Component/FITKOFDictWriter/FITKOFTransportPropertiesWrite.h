/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFTransportPropertiesWrite.h
 * @brief  TransportProperties 写出接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-27
 *********************************************************************/
#ifndef __FITKFLOWOFTRANSPORTPROPWRITER_H__
#define __FITKFLOWOFTRANSPORTPROPWRITER_H__

#include "FITKOFDictWriterAPI.h"
#include "FITKOFAbstractDictWriter.h"

namespace Interface
{
    class FITKOFTransportPhase;
    class FITKOFTransportProp;
}

namespace DICT
{
    class FITKDictVector;
    class FITKDictArray;
    class FITKDictGroup;
}

namespace Core
{
    class FITKAbstractEasyParam;
}

namespace IO
{
    /**
     * @brief ControlMeshDict 文件写出器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-15
     */
    class FITKOFTransportPropertiesWrite : public FITKOFAbstractDictWriter
    {
    public:
        explicit FITKOFTransportPropertiesWrite();
        virtual ~FITKOFTransportPropertiesWrite();

        /**
         * @brief 设置文件路径
         * @param[i] filePath
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        void setFilePath(const QString& filePath) override;
        /**
         * @brief 重写run函数
         * @return 状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-15
         */
        bool run() override;
        /**
         * @brief 获取文件
         * @return QString
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        QString getFileName() override;

    private:
        /**
         * @brief 设置字典里的FoamFile
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        void dictObjectFoamFile(QString name = QString());

        bool writePhasesFile(Interface::FITKOFTransportProp* transportData);

        bool transportPropW(Interface::FITKOFTransportProp* transportData);

        bool phaseOneW(Interface::FITKOFTransportPhase* phase, DICT::FITKDictGroup* group = nullptr);
        /**
         * @brief    获取参数
         * @param[i] para 参数数据
         * @param[o] key 
         * @param[o] value 
         * @param[i] ok 是否执行
         * @return   bool 状态
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-04
         */
        bool getPara(Core::FITKAbstractEasyParam* para, QString& key, QString& value, bool ok = true);

        QString getVName(QString vName);

        QString getUnit(QString unit);

    private:
        /**
         * @brief 文件名
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        QString m_fileName{};
    };
}



#endif
