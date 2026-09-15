/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKXMLMultiBlockDataWriter.h
 * @brief  VTK中vtm文件读取算法拓展
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-05-21
 *********************************************************************/
#ifndef FITKXMLMultiBlockDataWriter_H
#define FITKXMLMultiBlockDataWriter_H

#include "FITKVTKAlgorithmAPI.h"
#include "FITKXMLCompositeDataWriter.h"

#include <iostream>
#include <map>

class vtkXMLDataElement;
class vtkCompositeDataSet;

class FITKVTKALGORITHMAPI FITKXMLMultiBlockDataWriter : public FITKXMLCompositeDataWriter
{
    vtkTypeMacro(FITKXMLMultiBlockDataWriter, FITKXMLCompositeDataWriter);
public:
    /**
     * @brief    创建函数
     * @return   FITKXMLMultiBlockDataWriter*         读取对象
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-05-21
     */
    static FITKXMLMultiBlockDataWriter* New();
    /**
     * @brief    通过区域块索引设置名称
     * @param[i] comIndex                  区域块索引
     * @param[i] comName                   区域块名称
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-05-21
     */
    void setComNameByIndex(int comIndex, char* comName);
protected:
    /**
     * @brief    构造函数
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-05-21
     */
    FITKXMLMultiBlockDataWriter();
    /**
     * @brief    析构函数
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-05-21
     */
    ~FITKXMLMultiBlockDataWriter() override;
    /**
     * @brief    书写函数重写
     * @param[i] compositeData                数据块
     * @param[i] parent                       xml
     * @param[i] writerIdx                     
     * @return   int
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-05-21
     */
    int WriteComposite(vtkCompositeDataSet* compositeData, vtkXMLDataElement* parent, int& writerIdx) override;

    vtkStdString CreatePieceFileNameFITK(int piece);
protected:
    /**
     * @brief  区域块名称
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-05-21
     */
    std::map<int, char*> _comNameMap = {};
};

#endif