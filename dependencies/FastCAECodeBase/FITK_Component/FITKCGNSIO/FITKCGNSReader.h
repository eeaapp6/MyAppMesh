/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKCGNSReader.h
 * @brief  CGNS 格式读取接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-09-19
 *********************************************************************/

#ifndef _FITKFCGNSREADER_H__
#define _FITKFCGNSREADER_H__

#include "FITKAbstractCGNSReader.h"
#include <cgnslib.h>

namespace Interface
{
    class FITKNodeList;
    class FITKElementList;
}

namespace IO
{
    class FITKCGNSIOInterface;
    /**
     * @brief  CGNS读取
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-09-19
     */
    class FITKCGNSReader : public FITKAbstractCGNSReader
    {
    public:
        /**
         * @brief  构造函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-07-29
         */
        explicit FITKCGNSReader(FITKCGNSIOInterface* io);
        /**
         * @brief  析构函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-07-29
         */
        virtual ~FITKCGNSReader();
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
         * @brief    执行读取操作
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-15
         */
        bool read();
        /**
         * @brief    读取基节点
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-04
         */
        bool readBase();
        /**
         * @brief    读取Family
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-04
         */
        bool readBase_Family();
        /**
         * @brief    读取结构区域
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-04
         */
        bool readBase_ZoneStructured();
        /**
         * @brief    读取非结构区域
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-04
         */
        bool readBase_ZoneUnstructured();
        /**
         * @brief    读取网格单元
         * @param[i] index_section 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-04
         */
        bool readBase_ZoneUnstructured_Section(int index_section);
        /**
         * @brief    读取边界数据
         * @param[i] index_BC 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-05
         */
        bool readBase_ZoneStructured_BC(int index_BC);
        /**
         * @brief    读取边界数据
         * @param[i] index_BC 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-05
         */
        bool readBase_ZoneUnstructured_BC(int index_BC);
        /**
         * @brief    读取网格节点坐标
         * @param[i] vertext_num
         * @param[i] range_from
         * @param[i] range_to
         * @param[i] nodeList
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-04
         */
        bool readGridCoordinate(int vertext_num, /*int*/ cgsize_t range_from[3], /*int*/ cgsize_t range_to[3], Interface::FITKNodeList* nodeList);
        /**
         * @brief    读取单元
         * @param[i] elementList 
         * @param[i] start_index 
         * @param[i] end_index 
         * @param[i] type 
         * @param[i] elements 
         * @param[i] element_data_size 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-04
         */
        void readElementCells(Interface::FITKElementList* elementList, int start_index, int end_index, int type, cgsize_t* elements, int element_data_size);
        /**
         * @brief    读取CGNS文件信息
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-04
         */
        bool readCGNSInfo();
        /**
         * @brief    创建网格存储数据空间
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-05
         */
        bool createMeshData();

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
    };

}

#endif