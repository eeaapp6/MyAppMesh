/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKOFDictWriterIO.h
 * @brief Dict文件写出接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date 2024-07-29
 */
#ifndef __FITKOFDICTWRITERIO_H__
#define __FITKOFDICTWRITERIO_H__

#include "FITKOFDictWriterAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"

namespace IO
{
    /**
     * @brief Dict写出类型
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-29
     */
    enum DictWriteType
    {
        SnappyHexMesh,
        BlockMesh,
        Control,
        CreatePatch,
        TurbulenceProp,
        TransportProp,
        FvSchemes,
        FvSolution,
        FvOptions,
        ChangeDictionaryDict,
        InitialDir,
        SetFieldsDict,
        OperatingConditions,
        BoundaryMesh,
        ThermophysicalProperties,
        RadiationProperties,
    };
    /**
     * @brief FITKOFDictWriterIO 文件写出器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-15
     */
    class FITKOFDictWriterAPI FITKOFDictWriterIO : public QObject,
        public AppFrame::FITKComponentInterface
    {
    public:
        explicit FITKOFDictWriterIO();
        virtual ~FITKOFDictWriterIO();

        /**
         * @brief 设置写出类型
         * @param[i] type 类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        void setReadLevel(QList<DictWriteType> typeList);
        /**
         * @brief 设置文件路径
         * @param[i] filePath
         * @return 状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        bool setFilePath(const QString& filePath);
        /**
         * @brief 获取部件名称，不能重复  return "IO::FITKOFDictWriterIO"
         * @return QString
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        virtual QString getComponentName() override;
        /**
         * @brief    设置写出求解字典文件
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-28
         */
        void setPhysicsDictW();
        /**
         * @brief 重写run函数
         * @return 状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-15
         */
        bool exec();
        /**
         * @brief 获取文件
         * @return QString
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        QString getFilePath();
        /**
         * @brief 运算符<< 追加写出类型
         * @param[i] type 类型
         * @return FITKOFDictWriterIO
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-29
         */
        FITKOFDictWriterIO &operator<<(const DictWriteType type);

    private:
        void setPhysicsDictWFile();

    private:
        /**
         * @brief 写出类型链表
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-29
         */
        QList<DictWriteType> m_writeTypeList;
        /**
         * @brief 文件路径
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-29
         */
        QString m_filePath;
    };
}



#endif
