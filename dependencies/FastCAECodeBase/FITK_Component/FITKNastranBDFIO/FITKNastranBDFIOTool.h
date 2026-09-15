/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKNastranBDFIOTool.h
 * @brief  BDF读写工具声明
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-03-12
 *********************************************************************/
#ifndef _FITKNASTRANBDFIOTOOL_H___
#define _FITKNASTRANBDFIOTOOL_H___

#include <QHash>
#include <QList>

namespace Interface
{
    class FITKAbstractIO;
}

namespace AbaqusData
{
    class FITKDataCase;
    class FITKAbaqusPart;
}

namespace Core
{
    class FITKAbstractDataObject;
}

namespace Nastran
{
    class FITKNastranBDFReader;
    class FITKNastranBDFWriter;
    /**
     * @brief  BDF读取工具
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-03-13
     */
    class FITKNastranBDFIOReaderTool
    {
    public:
        explicit FITKNastranBDFIOReaderTool(FITKNastranBDFReader* readerBDF);
        virtual ~FITKNastranBDFIOReaderTool() = default;
        /**
         * @brief  读取映射表类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-03-14
         */
        enum ReadMappingType
        {
            ReadNone = 0,
            ReadPIDSection,
            ReadMIDMaterial,
        };
        /**
         * @brief    读取当前Nastran BDF文本 行分割数据
         * @return   QStringList
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-03-13
         */
        QStringList readNastranBDFLineSpliter();
        /**
         * @brief    string 转 double
         * @param[i] value 
         * @param[o] ok 是否转化成功 
         * @return   double
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-03-14
         */
        double stringToDouble(QString value, bool &ok);
        /**
         * @brief    插入单元和属性
         * @param[i] pID
         * @param[i] elementID
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-03-16
         */
        void insertElementProp(int pID, int elementID);
        /**
         * @brief    插入pID 与 mID的映射表
         * @param[i] pID
         * @param[i] mID
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-03-16
         */
        void insertPIDMID(int pID, QList<int>& mID);
        /**
         * @brief    插入数据到映射表
         * @param[i] type 映射表类型
         * @param[i] readID 文件读取ID
         * @param[i] objId 数据结构ID
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-03-14
         */
        void insertDataMapping(ReadMappingType type, int readID, int objId);
        /**
         * @brief    更新数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-03-16
         */
        void updateData(AbaqusData::FITKDataCase *caseData, AbaqusData::FITKAbaqusPart *partData);

    private:


    private:
        /**
         * @brief  文件读入器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-03-13
         */
        FITKNastranBDFReader* _reader{};
        /**
         * @brief  文件里的pID与单元ID的映射
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-03-16
         */
        QHash<int, QList<int>> _pIdEIdMapping{};
        /**
         * @brief  文件里的pID与mID的映射表
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-03-16
         */
        QHash<int, QList<int>> _pIdmIdMapping{};
        /**
         * @brief  文件id与框架id映射表
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-03-16
         */
        QHash<ReadMappingType, QHash<int, int>> _idIDMapping{};
        
    };

    /**
     * @brief  BDF写出工具
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-03-13
     */
    class FITKNastranBDFIOWriterTool
    {
    public:
        explicit FITKNastranBDFIOWriterTool(FITKNastranBDFWriter* writerBDF);
        virtual ~FITKNastranBDFIOWriterTool() = default;

        /**
         * @brief    通过单元ID获取属性ID
         * @param[i] eID 
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-03-18
         */
        int getPropIDByEID(int eID);
        /**
         * @brief    写入数据
         * @param[i] data
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-03-16
         */
        bool writeData(QStringList& dataList);
        /**
         * @brief    double 转 string
         * @param[i] value 
         * @param[o] ok 是否转化成功 
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-03-17
         */
        QString doubleToString(double value, bool &ok);
        /**
         * @brief    更新数据
         * @param[i] caseData 算例数据
         * @param[i] partData Part数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-02-10
         */
        void updateData(AbaqusData::FITKDataCase *caseData, AbaqusData::FITKAbaqusPart *partData);

    private:
        

    private:
        /**
         * @brief 文件写出器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-19
         */
        FITKNastranBDFWriter* _writer{};
        /**
         * @brief  格式类型 1:短格式  2:长格式
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-03-17
         */
        int _formatType{ 2 };
        /**
         * @brief  单元ID, 属性ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-03-16
         */
        QHash<int, int> _eIDPropIDMapping{};
    };
}
#endif
