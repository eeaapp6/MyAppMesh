/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKTecplotDatReader.h
 * @brief  Tecplot中dat文件格式读取
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-09
 *********************************************************************/
#ifndef _FITKTecplotDatReader_H_
#define _FITKTecplotDatReader_H_

#include "FITKVTKAlgorithmAPI.h"
#include <vtkSmartPointer.h>
#include <vtkMultiBlockDataSetAlgorithm.h>
#include <QHash>
#include <QStringList>
#include <vtkDataSetAlgorithm.h>
#include <vtkUnstructuredGridAlgorithm.h>

class vtkMultiBlockDataSet;
class vtkAlgorithmOutput;
class vtkInformation;
class vtkDataSet;

class FITKVTKALGORITHMAPI FITKTecplotDatReader : public vtkUnstructuredGridAlgorithm
{
public:
    /**
     * @brief    创建对象
     * @return   FITKTecplotDatReader*           读取对象
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-02-26
     */
    static FITKTecplotDatReader *New();
    /**
     * @brief    信息打印函数
     * @param[i] os 
     * @param[i] indent 
     * @return   void
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-02-26
     */
    void PrintSelf(ostream &os, vtkIndent indent) override;
    /**
     * @brief    设置文件名称
     * @param[i] fileName                 文件名称
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-02-26
     */
    void setFileName(QString fileName);
    /**
     * @brief    获取文件名称
     * @return   QString                  文件名称
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-02-26
     */
    QString getFileName();
    /**
     * @brief    获取数据块数量
     * @return   int                      数据块数量
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-02-26
     */
    int getNumberOfBlocks();

    void setVisible(int blockIndex, bool vis);

    QStringList getBlockNames();

    QStringList getBlockBCTypes();

    /**
     * @brief    设置输出信息
     * @param[i] nPort 
     * @param[i] info 
     * @return   int
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-02-26
     */
    int FillOutputPortInformation(int nPort, vtkInformation *info) override;
protected:
    /**
     * @brief    构造函数
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-02-26
     */
    FITKTecplotDatReader();
    /**
     * @brief    析构函数
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-02-26
     */
    ~FITKTecplotDatReader();
    /**
     * @brief    数据请求接口
     * @param[i]  
     * @param[i]  
     * @param[i]  
     * @return   int
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-02-26
     */
    int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
    /**
     * @brief    数据处理
     * @param[i] request 
     * @param[i] inputVector 
     * @param[i] outputVector 
     * @return   int
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-02-26
     */
    int ProcessRequest(vtkInformation *request, vtkInformationVector **inputVector, vtkInformationVector *outputVector) override;
private:
    /**
     * @brief    防止拷贝，统一走New
     * @param[i]  
     * @return   void
     * @author   BaGuijun (baguijun@163.com)
     * @date     2025-02-26
     */
    FITKTecplotDatReader(const FITKTecplotDatReader &);
    void operator=(const FITKTecplotDatReader &);

    /**
     * @brief 递归遍历数据块
     * @since Version: 1.0.0
     **/
    void getBlocks(vtkDataObject *blockData);
protected:
    /**
     * @brief  文件名称
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-09
     */
    QString _fileName = "";
private:
    /**
     * @brief  数据块链表
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-02-26
     */
    QList<vtkDataSet *> _blockList = {};
    /**
     * @brief
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-02-26
     */
    QList<bool> _visibleStates = {};
    /**
     * @brief  数据块名称
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-02-26
     */
    QStringList _blockNames = {};
};
#endif