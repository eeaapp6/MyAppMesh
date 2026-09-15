/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKRadiossRadWriter.h
 * @brief Radioss Rad文件写入器
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-06-17
 * 
 */
#ifndef _FITKRadiossRadWriter_H_
#define _FITKRadiossRadWriter_H_

#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextWriter.h"

namespace Radioss
{
    class RadWriteInformationMapper;
    class FITKRadiossAbstractAdaptor;
    class FITKRadiossCase;
    
     /**
      * @brief Radioss Rad文件写入器
      * @author LiBaojun (libaojunqd@foxmail.com)
      * @date 2025-06-17
      */
    class FITKRadiossRadWriter : public Interface::FITKAbstractTextWriter
    {
        Q_OBJECT
    public:
        explicit FITKRadiossRadWriter(bool & success);
        virtual ~FITKRadiossRadWriter();

        /**
        * @brief 执行写入操作
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2025-06-17
        */
        virtual void run() override;
        /**
         * @brief    获取映射表
         * @return   RadWriteInformationMapper *
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-08-11
         */
        RadWriteInformationMapper* getWriteInpInformationMapper();

        /**
         * @brief    发送计算进度 
         * @param[i] Progress 进度百分比
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-08-11
         */
        void sendCalculateProgress(int Progress);
        /**
          * @brief 打印控制台消息
          * @param[i] level 打印级别 1 normal 2 warning 3error
          * @param[i] str 打印的信息
          * @author LiBaoJun (libaojunqd@foxmail.com)
          * @date 2025-06-17
          */
        void consoleMessage(int level, const QString& str) override;

    private:
        /**
         * @brief  写入0000文件
         * @param[i]  fileName       文件名,带绝对路径
         * @param[i]  caseData       算例数据
         * @return bool              写入状态 true 成功 false 失败
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-08-27
         */
        bool write0000File(const QString& fileName,Radioss::FITKRadiossCase* caseData);
        /**
         * @brief  写入0001文件
         * @param[i]  fileName       文件名,带绝对路径
         * @param[i]  caseData       算例数据
         * @return bool              写入状态 true 成功 false 失败
         * @author LiBaojun 
         * @date 2025-08-27
         */
        bool write0001File(const QString& fileName,Radioss::FITKRadiossCase* caseData);


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
        RadWriteInformationMapper* _informationWriteRadMapper{};
    };
}

#endif