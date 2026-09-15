/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKCGNSWriter.h
 * @brief  CGNS 格式写出接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-08-05
 *********************************************************************/

#ifndef _FITKCGNSWRITER_H__
#define _FITKCGNSWRITER_H__

#include "FITKAbstractCGNSWriter.h"
#include <cgnslib.h>

namespace Interface
{
    class FITKNodeList;
    class FITKElementList;
    class FITKAbstractElement;
    class FITKModelSet;
    class FITKAbstractMesh;
}

namespace CGNS
{
    class FITKMeshDataProcessToCGNSData;
}

namespace IO
{
    class FITKCGNSIOInterface;
    /**
     * @brief  CGNS读取
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-09-19
     */
    class FITKCGNSWriter : public FITKAbstractCGNSWriter
    {
    public:
        /**
         * @brief  构造函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-07-29
         */
        explicit FITKCGNSWriter(FITKCGNSIOInterface* io);
        /**
         * @brief  析构函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-07-29
         */
        virtual ~FITKCGNSWriter();
        /**
         * @brief 重写run函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-03-11
         */
        void run() override;

        /**
         * @brief    打印控制台消息
         * @param[i] level 打印级别 1 normal 2 warning 3error
         * @param[i] str 数据
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-04-07
         */
        void consoleMessage(int level, const QString& str) override;

    private:
        /**
         * @brief    执行写出操作
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-15
         */
        bool write();
        /**
         * @brief    写出基节点
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-05
         */
        bool writeBase(const char* base_name);
        /**
         * @brief    写出结构区域
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-04
         */
        bool writeBase_ZoneStructured();
        /**
         * @brief    写出非结构区域
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-04
         */
        bool writeBase_ZoneUnstructured();
        /**
         * @brief    写出网格节点坐标
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-05
         */
        bool writeGridCoordinate();
        /**
         * @brief    写出单元
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-08
         */
        bool writeBase_ZoneUnstructured_Section();
        /**
         * @brief    写出边界数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-05
         */
        bool writeBase_ZoneUnstructured_BC();
        /**
         * @brief    写出边界族
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-06
         */
        bool writeFamily_BC(int type, QString bc_name, int bcID, int& index_bc);
        /**
         * @brief    获取边界类型名称
         * @param[i] type 
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-06
         */
        QString getBCTypeName(int type);
        /**
         * @brief    写出CGNS文件信息
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-04
         */
        bool writeCGNSInfo();

    private:
        /**
         * @brief  组件对象指针
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-15
         */
        FITKCGNSIOInterface* _compInterface{};
        /**
         * @brief  基节点索引号
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-04
         */
        int m_index_base{ -1 };
        /**
         * @brief  区域索引号
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-04
         */
        int m_index_zone{ -1 };
        /**
         * @brief  family索引号
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-04
         */
        int m_index_family{ -1 };
        /**
         * @brief  当前的网格数据索引号
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-05
         */
        int m_current_index_mesh{ -1 };
        /**
         * @brief  数据转换工具
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-08
         */
        CGNS::FITKMeshDataProcessToCGNSData* _tool = nullptr;
    };
}

#endif