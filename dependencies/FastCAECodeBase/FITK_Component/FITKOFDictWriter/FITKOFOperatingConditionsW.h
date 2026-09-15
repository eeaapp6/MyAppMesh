/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFOperatingConditionsW.h
 * @brief  作用条件字典写出
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-09-05
 *********************************************************************/
#ifndef __FITKFLOWOFOperatingConditionsWRITER_H__
#define __FITKFLOWOFOperatingConditionsWRITER_H__

#include "FITKOFDictWriterAPI.h"
#include "FITKOFAbstractDictWriter.h"

namespace Interface
{
    class FITKOFPhysicsData;
}

namespace DICT
{
    class FITKDictVector;
    class FITKDictArray;
}

namespace IO
{
    /**
     * @brief setFieldsDict 文件写出器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-15
     */
    class FITKOFOperatingConditionsW : public FITKOFAbstractDictWriter
    {
    public:
        explicit FITKOFOperatingConditionsW();
        virtual ~FITKOFOperatingConditionsW();

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
         * @brief    g FoamFile
         * @param[i] location 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-01
         */
        bool gravitationalFoamFile(QString location);
        /**
         * @brief    hRef FoamFile
         * @param[i] location 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-01
         */
        bool accelerationFoamFile(QString location);
        /**
         * @brief    重力文件写出
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-05
         */
        bool gravitationalFileW();
        /**
         * @brief    加速度文件写出
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-05
         */
        bool accelerationFileW();

    private:
        /**
         * @brief 文件名
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        QString m_fileName{};

        Interface::FITKOFPhysicsData* m_physicsData{};
    };
}



#endif
