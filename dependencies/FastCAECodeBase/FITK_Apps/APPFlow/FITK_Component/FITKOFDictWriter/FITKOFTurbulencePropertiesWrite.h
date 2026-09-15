/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFTurbulencePropertiesWrite.h
 * @brief  TurbulenceProperties 写出接口 -momentumTransport
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-26
 *********************************************************************/
#ifndef __FITKFLOWOFTURBULENCEPROPWRITER_H__
#define __FITKFLOWOFTURBULENCEPROPWRITER_H__

#include "FITKOFDictWriterAPI.h"
#include "FITKOFAbstractDictWriter.h"

namespace Interface
{
    class FITKAbstractOFTurbulence;
}

namespace DICT
{
    class FITKDictGroup;
}

namespace Core
{
    class FITKParameter;
}

namespace IO
{
    /**
     * @brief ControlMeshDict 文件写出器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-15
     */
    class FITKOFTurbulencePropertiesWrite : public FITKOFAbstractDictWriter
    {
    public:
        explicit FITKOFTurbulencePropertiesWrite();
        virtual ~FITKOFTurbulencePropertiesWrite();

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
        void dictObjectFoamFile();

        bool turbulencePropW();

        QString turbModelName(QString name);

        bool parametersW(Core::FITKParameter* para, DICT::FITKDictGroup* group);

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
