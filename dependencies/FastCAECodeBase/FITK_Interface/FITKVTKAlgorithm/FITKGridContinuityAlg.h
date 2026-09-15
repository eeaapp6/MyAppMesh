/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKGridContinuityAlg.h
 * @brief  网格连续性判断
 * @author BaGuijun (baguijun@163.com)
 * @date   2026-01-13
 * 原理:
 *      第一步:通过DataSet中任取一点(该点上必须有单元),通过波前法查找所有相邻单元中所有的点.
 *      第二步:剔除已找到的点,从未找到的点中继续执行第一步逻辑,直至所有点都已经被查找过.
 *      第三步:通过上述两步可以得到所有点被使用状态集合.
 *      第四步:判断所有有效点(点上有单元)是否被使用.
 *********************************************************************/
#ifndef _FITKGridContinuityAlg_H
#define _FITKGridContinuityAlg_H

#include "FITKVTKAlgorithmAPI.h"

#include <vtkSetGet.h>
#include <vtkDataSetAlgorithm.h>

class vtkDataSet;

/**
 * @brief  PolyData连续性判断
 * @author BaGuijun (baguijun@163.com)
 * @date   2026-01-13
 */
class FITKVTKALGORITHMAPI FITKGridContinuityAlg : public vtkDataSetAlgorithm
{
public:
    vtkTypeMacro(FITKGridContinuityAlg, vtkDataSetAlgorithm);
    static FITKGridContinuityAlg* New();
    /**
     * @brief    信息输出
     * @param[i] os                         打印内容
     * @param[i] indent                     缩进级别
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-01-13
     */
    void PrintSelf(ostream& os, vtkIndent indent) override;
    /**
     * @brief    获取连接性
     * @return   bool                       是否连接
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-01-20
     */
    bool getContinuity();
protected:
    /**
     * @brief    构造函数
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-01-13
     */
    FITKGridContinuityAlg();
    /**
     * @brief    析构函数
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-01-13
     */
    ~FITKGridContinuityAlg() override;
    /**
     * @brief    算法执行
     * @param[i] request                    请求对象
     * @param[i] inputVector                输入端口
     * @param[i] outputVector               输出端口
     * @return   int                        执行结果
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-01-13
     */
    int RequestData(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector)override;
    /**
     * @brief    通过点ID查找关联点
     * @param[i] pointIDs                    点ID集合
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-01-15
     */
    void findConnectedPoints(std::vector<int> pointIDs);
protected:
    /**
     * @brief  输入数据
     * @author BaGuijun (baguijun@163.com)
     * @date   2026-01-13
     */
    vtkDataSet* _inputData = nullptr;
    /**
     * @brief  是否是连续的
     * @author BaGuijun (baguijun@163.com)
     * @date   2026-01-15
     */
    bool _isContinuity = false;
    /**
     * @brief  点被使用标签
     * @author BaGuijun (baguijun@163.com)
     * @date   2026-01-15
     */
    std::vector<bool> _pointUseInfo = {};
    /**
     * @brief  点所包含的单元ID
     * @author BaGuijun (baguijun@163.com)
     * @date   2026-01-15
     */
    std::vector<std::vector<int>> _pointCellInfo = {};
    /**
     * @brief  单元所包含的点ID
     * @author BaGuijun (baguijun@163.com)
     * @date   2026-01-15
     */
    std::vector<std::vector<int>> _cellPointInfo = {};
};

#endif