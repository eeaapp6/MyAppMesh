/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAlgGlyph.h
 * @brief  图像符号算法
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-23
 *********************************************************************/
#ifndef _FITKAlgGlyph_H
#define _FITKAlgGlyph_H

#include "FITKInterfaceCFDPostAPI.h"

#include <vtkPolyDataAlgorithm.h>
#include <vtkSetGet.h>
#include <QString>

class vtkPolyData;

namespace Interface
{
    /**
     * @brief  图像符号算法
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-24
     */
    class FITKInterfaceCFDPostAPI FITKAlgGlyph :public vtkPolyDataAlgorithm
    {
    public:
        /**
         * @brief    创建接口
         * @return   FITKAlgGlyph*
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-24
         */
        static FITKAlgGlyph* New();

        void PrintSelf(ostream& os, vtkIndent indent);
        /**
         * @brief    获取矢量名称
         * @return   QString       矢量名称
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        QString getVectorName();
        /**
         * @brief    设置矢量名称
         * @param[i] vectorName    矢量名称
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        void setVectorName(QString vectorName);
        /**
         * @brief    获取箭头维度
         * @return   int            维度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        int getTipResolution();
        /**
         * @brief    设置箭头维度
         * @param[i] tipResolution  维度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        void setTipResolution(int tipResolution);
        /**
         * @brief    获取箭头半径
         * @return   double         半径
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        double getTipRadius();
        /**
         * @brief    设置箭头半径
         * @param[i] tipRadius      半径
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        void setTipRadius(double tipRadius);
        /**
         * @brief    获取箭头长度
         * @return   double         长度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        double getTipLength();
        /**
         * @brief    设置箭头长度
         * @param[i] tipLength      长度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        void setTipLength(double tipLength);
        /**
         * @brief    获取箭柄维度
         * @return   int           维度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        int getShaftResolution();
        /**
         * @brief    设置箭柄维度
         * @param[i] shaftResolution
         * @return   void
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        void setShaftResolution(int shaftResolution);
        /**
         * @brief    获取箭柄半径
         * @return   double         长度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        double getShaftRadius();
        /**
         * @brief    设置箭柄半径
         * @param[i] tipLength      长度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        void setShaftRadius(double shaftRadius);
        /**
         * @brief    获取箭头缩放比例
         * @return   double         比例
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        double getScaleFactor();
        /**
         * @brief    设置箭头缩放比例
         * @param[i] tipLength      比例
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        void setScaleFactor(double scaleFactor);
        /**
         * @brief    获取是否使用点数据
         * @return   bool      是否使用点数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        bool getPointDataUsed();
        /**
         * @brief    获取是否使用点数据
         * @param[i] pointDataUsed  是否使用点数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        void setPointDataUsed(bool pointDataUsed);
        /**
         * @brief    获取点的数量
         * @return   int           数量
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        int getPointNum();
        /**
         * @brief    设置点的数量
         * @param[i] pointNum      数量
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        void setPointNum(int pointNum);
        /**
         * @brief    获取最大采样点
         * @return   int           最大采样点
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        int getMaxNumber();
        /**
         * @brief    设置最大采样点
         * @param[i] pointNum      最大采样点
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        void setMaxNumber(int maxNumber);
        /**
         * @brief    获取箭头比例模式
         * @return   int           箭头比例模式:0-scalar,1-vector,2-vectorcomponent,3-off
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        int getScaleMode();
        /**
         * @brief    设置箭头比例模式
         * @param[i] pointNum      箭头比例模式:0-scalar,1-vector,2-vectorcomponent,3-off
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        void setScaleMode(int scaleMode);
    protected:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-24
         */
        FITKAlgGlyph();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-24
         */
        ~FITKAlgGlyph();
        /**
         * @brief    输出端口说明
         * @param[i] port           端口
         * @param[i] info           信息
         * @return   int
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-23
         */
        int FillOutputPortInformation(int port, vtkInformation* info) override;
        /**
         * @brief    输出端口说明
         * @param[i] port           端口
         * @param[i] info           信息
         * @return   int
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-23
         */
        int FillInputPortInformation(int port, vtkInformation* info) override;
        /**
         * @brief    处理外部请求
         * @param[i] request
         * @param[i] inputVector
         * @param[i] outputVector
         * @return   int
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-23
         */
        int ProcessRequest(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector);
        /**
         * @brief    算法实现
         * @param[i] request
         * @param[i] inputVector
         * @param[i] outputVector
         * @return   int
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-23
         */
        int RequestData(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector);
    private:
        /**
         * @brief    提取点
         * @param[i] inputData          输入数据
         * @return   bool               是否成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-23
         */
        bool extractPoints(vtkDataSet* inputData);

        void extractPointDataArray(vtkDataSet* inputData);
    private:
        /**
         * @brief  数据对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-23
         */
        vtkPolyData* _polyData = nullptr;
        /**
         * @brief  矢量名称
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-30
         */
        QString _vectorName = "";
        /**
         * @brief  箭头维度
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-30
         */
        int _tipResolution = 6;
        /**
         * @brief  箭头半径
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-30
         */
        double _tipRadius = 0.2;
        /**
         * @brief  箭头长度
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-30
         */
        double _tipLength = 0.1;
        /**
         * @brief  箭柄维度
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-30
         */
        int _shaftResolution = 6;
        /**
         * @brief  箭柄半径
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-30
         */
        double _shaftRadius = 0.1;
        /**
         * @brief  箭头缩放比例
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-30
         */
        double _scaleFactor = 0.01;
        /**
         * @brief  使用点数据
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-30
         */
        bool _pointDataUsed = true;
        /**
         * @brief  点数量
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-30
         */
        int _pointNum = 0;
        /**
         * @brief  最大采样点
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-30
         */
        int _maxNumber = 100000;
        /**
         * @brief  箭头比例模式:0-scalar,1-vector,2-vectorcomponent,3-off
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-30
         */
        int _scaleMode = 1;
    };
}
#endif
