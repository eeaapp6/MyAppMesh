/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKGmshMshIOReader.h
 * @brief  gmsh msh网格文件读取
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-11-29
 *********************************************************************/
#ifndef _FITK_GMSH_MSH_IO_READER_H___
#define _FITK_GMSH_MSH_IO_READER_H___

#include "FITKGmshMshIOAPI.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextReader.h"

namespace Interface
{
    class FITKComponentManager;
}

namespace Gmsh
{
    class FITKGmshMshDataReaderProcessor;
    /**
     * @brief  gmsh msh网格文件读取
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-11-29
     */
    class FITKGmshMshIOReader : public Interface::FITKAbstractTextReader
    {
        Q_OBJECT
    public:
        explicit FITKGmshMshIOReader() = default;
        virtual ~FITKGmshMshIOReader();
        /**
         * @brief    设置集合管理器
         * @param[i] componentManager
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-06
         */
        void setComponentManager(Interface::FITKComponentManager* componentManager);
        /**
         * @brief  重写run函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-29
         */
        void run() override;
        /**
         * @brief    设置Gmsh Msh数据
         * @param[i] data 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-01-28
         */
        void setReaderMshData(FITKGmshMshDataReaderProcessor* data);
        /**
         * @brief    获取Gmsh Msh数据
         * @return   FITKGmshMshDataReaderProcessor *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-03
         */
        FITKGmshMshDataReaderProcessor* getReaderMshData();
        /**
         * @brief 打印控制台消息
         * @param[i] level 打印级别 1 normal 2 warning 3error
         * @param[i] message 数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-29
         */
        void consoleMessage(int level, const QString& message) override;

    protected:
        /**
         * @brief    开始读取数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-03
         */
        bool read();

    protected:
        /**
         * @brief  集合管理器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-12-06
         */
        Interface::FITKComponentManager* _componentMgr{};
        /**
         * @brief  Gmsh Msh数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-03
         */
        FITKGmshMshDataReaderProcessor* _mshDataGmsh{};
    };
}


#endif
