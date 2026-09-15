/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKIBEStrDataSetReader.h
 * @brief  云道结构后处理文件读取算法类
 * @author BaGuijun (baguijun@163.com)
 * @date   2026-01-21
 *********************************************************************/
#ifndef _FITKIBEStrDataSetReader_H
#define _FITKIBEStrDataSetReader_H

#include "FITKInterfaceStructuralPostAPI.h"

#include <QObject>
#include <QHash>

#include <vtkUnstructuredGridAlgorithm.h>
#include <vtkCellType.h>
#include <vtkSmartPointer.h>

namespace H5 {
    class H5File;
}

/**
 * @brief  云道结构后处理文件读取算法类
 * @author BaGuijun (baguijun@163.com)
 * @date   2026-01-21
 */
class FITKInterfaceStructuralPostAPI FITKIBEStrDataSetReader : public vtkUnstructuredGridAlgorithm
{
public:
    vtkTypeMacro(FITKIBEStrDataSetReader, vtkUnstructuredGridAlgorithm);
    static FITKIBEStrDataSetReader* New();
    /**
     * @brief    信息输出
     * @param[i] os                         打印内容
     * @param[i] indent                     缩进级别
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-01-21
     */
    void PrintSelf(ostream& os, vtkIndent indent) override;
    /**
     * @brief    设置网格文件路径
     * @param[i] meshFile                   网格文件路径
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-01-21
     */
    void setMeshFile(const QString& meshFile);
    /**
     * @brief    设置结果文件路径
     * @param[i] resultFile                 结构文件路径
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-01-21
     */
    void setResultFile(const QString& resultFile);
    /**
     * @brief    设置时间步
     * @param[i] 时间步
     * @author   WuYizhen (wyz_hanhan@163.com)
     * @date     2026-01-22
     */
    void setStep(int step);
    /**
     * @brief    获取当前时间步
     * @return   当前时间步
     * @author   WuYizhen (wyz_hanhan@163.com)
     * @date     2026-01-23
     */
    int getCurrentStep();
    /**
     * @brief    获取时间步总数
     * @return   时间步总数
     * @author   WuYizhen (wyz_hanhan@163.com)
     * @date     2026-01-22
     */
    int getTotalStep();
protected:
    /**
     * @brief    构造函数
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-01-21
     */
    FITKIBEStrDataSetReader();
    /**
     * @brief    析构函数
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-01-21
     */
    ~FITKIBEStrDataSetReader() override;
    /**
     * @brief    输出端口说明
     * @param[i] port           端口
     * @param[i] info           信息
     * @return   int
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-01-21
     */
    int FillOutputPortInformation(int port, vtkInformation* info) override;

    int ProcessRequest(vtkInformation *request, vtkInformationVector **inputVector, vtkInformationVector *outputVector) override;
    /**
     * @brief    算法执行
     * @param[i] request                    请求对象
     * @param[i] inputVector                输入端口
     * @param[i] outputVector               输出端口
     * @return   int                        执行结果
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-01-21
     */
    int RequestData(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector)override;
private:
    /**
     * @brief    获取当前时间步的组名
     * @return   组名
     * @author   WuYizhen (wyz_hanhan@163.com)
     * @date     2026-01-22;
     */
    QString getCurrentGroupName();

    /**
     * @brief    根据路径名称执行单元的添加到网格中
     * @param[i] 文件引用
     * @param[i] 文件内部路径名称
     * @param[i] 网格指针
     * @param[i] 场量名称
     * @author   WuYizhen (wyz_hanhan@163.com)
     * @date     2026-01-22
     */
    void addCellByPath(H5::H5File &file ,QString path, vtkUnstructuredGrid* output, QString fieldName);

    /**
     * @brief    根据路径名称执行节点的添加到网格中
     * @param[i] 文件引用
     * @param[i] 文件内部路径名称
     * @param[i] 网格指针
     * @param[i] 场量名称
     * @author   WuYizhen (wyz_hanhan@163.com)
     * @date     2026-01-22
     */
    void addPointByPath(H5::H5File &file, QString path, vtkUnstructuredGrid* output, QString fieldName);
protected:
    /**
     * @brief  网格文件路径
     * @author BaGuijun (baguijun@163.com)
     * @date   2026-01-21
     */
    QString _meshFile = "";
    /**
     * @brief  结果文件路径
     * @author BaGuijun (baguijun@163.com)
     * @date   2026-01-21
     */
    QString _resultFile = "";
    /**
     * @brief  当前时间步
     * @author WuYizhen (wyz_hanhan@163.com)
     * @date   2026-01-22
     */
    int _step = 0;
    /**
     * @brief  时间步总数
     * @author WuYizhen (wyz_hanhan@163.com)
     * @date   2026-01-22
     */
    int _totalSteps = 0;
    /**
     * @brief  节点总数
     * @author WuYizhen (wyz_hanhan@163.com)
     * @date   2026-01-22
     */
    int _nodesNum = -1;
    /**
     * @brief  单元总数
     * @author WuYizhen (wyz_hanhan@163.com)
     * @date   2026-01-22
     */
    int _cellNum = -1;
    /**
     * @brief  点单元对应点数量字典以及vtk里面的类型
     * @author WuYizhen (wyz_hanhan@163.com)
     * @date   2026-01-22
     */
    const QHash<int, QPair<int, int>> _hashMap = {
        {102, {2, VTK_LINE}},
        {103, {3, VTK_QUADRATIC_EDGE}},
        {203, {3, VTK_TRIANGLE}},
        {206, {6, VTK_QUADRATIC_TRIANGLE}},
        {204, {4, VTK_QUAD}},
        {208, {8, VTK_QUADRATIC_QUAD}},
        {304, {4, VTK_TETRA}},
        {310, {10, VTK_QUADRATIC_TETRA}},
        {305, {5, VTK_PYRAMID}},
        {313, {13, VTK_QUADRATIC_PYRAMID}},
        {306, {6, VTK_WEDGE}},
        {315, {15, VTK_QUADRATIC_WEDGE}},
        {308, {8, VTK_HEXAHEDRON}},
        {320, {20, VTK_QUADRATIC_HEXAHEDRON}},
    };
    /**
     * @brief  网格数据缓存
     * @author WuYizhen (wyz_hanhan@163.com)
     * @date   2026-01-25
     */
    vtkSmartPointer<vtkUnstructuredGrid> _cachedMesh = nullptr;
    /**
     * @brief  上一次读取的网格文件
     * @author WuYizhen (wyz_hanhan@163.com)
     * @date   2026-01-25
     */
    QString _lastFilePath = "";
};

#endif


