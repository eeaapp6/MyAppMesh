/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFSetFieldsDictWriter.h
 * @brief  setFieldsDict 文件写出接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-09-04
 *********************************************************************/
#ifndef __FITKFLOWOFSETFIELDSWRITER_H__
#define __FITKFLOWOFSETFIELDSWRITER_H__

#include "FITKOFDictWriterAPI.h"
#include "FITKOFAbstractDictWriter.h"

namespace Interface
{
    class FITKOFGeometryPatch;
}

namespace DICT
{
    class FITKDictVector;
    class FITKDictGroup;
}

namespace IO
{
    /**
     * @brief setFieldsDict 文件写出器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-15
     */
    class FITKOFSetFieldsDictWriter : public FITKOFAbstractDictWriter
    {
    public:
        explicit FITKOFSetFieldsDictWriter();
        virtual ~FITKOFSetFieldsDictWriter();

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
        /**
         * @brief    设置场写出
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-05
         */
        bool setFieldsDictW();

        bool writeFieldValue(Interface::FITKOFGeometryPatch* patch, DICT::FITKDictGroup* group);

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
