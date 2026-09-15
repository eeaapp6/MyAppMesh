/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKRadiossRadReader.h
 * @brief Radioss Rad文件读取器
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-06-17
 * 
 */
#ifndef _FITKRadiossRadReader_H_
#define _FITKRadiossRadReader_H_

#include "FITKRadiossRadIOInterface.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextReader.h"

ForwardDeclarNS(Core, FITKAbstractDataObject);

namespace Radioss
{
    class RadKeyLineSpliter;
    class RadReadInformationMapper;
    class FITKRadiossAbstractAdaptor;

    /**
     * @brief Radioss Rad文件读取器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-06-17
     */
    class FITKRadiossRadReader : public Interface::FITKAbstractTextReader
    {
        Q_OBJECT
    public:
        explicit FITKRadiossRadReader(bool & success);
        virtual ~FITKRadiossRadReader();
        /**
        * @brief 执行读取操作
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2025-06-17
        */
        virtual void run() override;
        /**
         * @brief 加载文件
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-29
         */
        virtual bool loadFile() override;
        /**
         * @brief    是否跳过空行
         * @param[i] isSkip 
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-04
         */
        void setSkipBlankLine(bool isSkip);
        /**
         * @brief    读取一行数据
         * @return   QString
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-04
         */
        virtual QString readLine() override;
        /**
         * @brief    回退上一行数据
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-04
         */
        virtual void backLine() override;
        /**
         * @brief    获取映射表
         * @return   RadReadInformationMapper *
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-02
         */
        RadReadInformationMapper* getReadRadInformationMapper();
        /**
         * @brief    获取分割器
         * @return   RadKeyLineSpliter *
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-08
         */
        RadKeyLineSpliter* getKeyLineSpliter();
        /**
          * @brief 打印控制台消息
          * @param[i] level 打印级别 1 normal 2 warning 3error
          * @param[i] str 打印的信息
          * @author LiBaojun (libaojunqd@foxmail.com)
          * @date 2025-06-17
          */
        void consoleMessage(int level, const QString& str) override;
        /**
         * @brief    创建适配器
         * @param[i] line 
         * @return   FITKRadiossAbstractAdaptor *
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-09
         */
        FITKRadiossAbstractAdaptor* createAdaptor(const QString & line);
        /**
         * @brief    清理求解设置与求解请求
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-29
         */
        void clearSolverSettingRequest();

    private:
        /**
         * @brief 成功标记
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-06-17
         */
        bool & _success;
        /**
         * @brief  
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-02
         */
        RadKeyLineSpliter* _keyLineSpliter{};
        /**
         * @brief  
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-02
         */
        RadReadInformationMapper* _informationReadRadMapper{};
        /**
         * @brief  是否跳过空行
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-02
         */
        bool _isSkipBlankLine{ true };
    };
}

#endif