/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKStruThresholdPostData.h
 * @brief  后处理阈值数据类
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2026-02-10
 *********************************************************************/

#ifndef _FITKStruThresholdPostData_H__
#define _FITKStruThresholdPostData_H__

#include "FITKInterfaceStructuralPostAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITKAbstractStruPostVTK.h"

class vtkThreshold;
class vtkAlgorithmOutput;
class vtkTrivialProducer;

namespace Interface {
    /**
       * @brief  后处理阈值数据类
       * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
       * @date   2026-02-10
       */
    class FITKInterfaceStructuralPostAPI FITKStruThresholdPostData : public Core::FITKAbstractNDataObject {
        FITKCLASS(Interface, FITKStruThresholdPostData);
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-02-10
         */
        FITKStruThresholdPostData();

        /**
         * @brief    析构函数
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-02-10
         */
        virtual ~FITKStruThresholdPostData();

        /**
         * @brief    设置变量名称
         * @param[i] fileName     变量名称
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-02-10
         */
        void setFileName(const QString fileName);

        /**
         * @brief    获取变量名称
         * @return   变量名称
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-02-10
         */
        QString getFileName();

        /**
         * @brief    设置变量类型
         * @param[i] type     变量类型
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-02-10
         */
        void setType(const VariableLocation type);

        /**
         * @brief    获取变量类型
         * @return   VariableLocation    变量类型
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-02-10
         */
        VariableLocation getType();

        /**
         * @brief    设置阈值范围
         * @param[i] minValue     最小值
         * @param[i] maxValue     最大值
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-02-10
         */
        void setThresholdRange(const double minValue, const double maxValue);

        /**
         * @brief    获取阈值范围
         * @return   QPair<double, double>    阈值范围，包含最小值和最大值
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-02-10
         */
        QPair<double, double> getThresholdRange();

        /**
         * @brief    设置阈值函数
         * @param[i] func 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-02-12
         */
        void setThresholdFunction(int func);

        /**
         * @brief    获取阈值函数
         * @return   int
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-02-12
         */
        int getThresholdFunction();

        /**
         * @brief    获取输出数据
         * @return   vtkDataSet *    输出数据对象
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-02-10
         */
        virtual vtkDataSet* getOutput();

        /**
         * @brief    获取算法输出管线
         * @return   vtkAlgorithmOutput *    算法输出管线
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-02-10
         */
        virtual vtkAlgorithmOutput* getOutputAlgorithm();

        vtkAlgorithm* getAlgorithm();

        /**
         * @brief    设置后处理结构数据类型
         * @param[i] type 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-02-11
         */
        virtual void setPostStructualType(const StruPostType type);

        /**
         * @brief    获取后处理结构数据类型
         * @return   StruPostType    后处理结构数据类型
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-02-10
         */
        virtual StruPostType getPostStructualType();

        /**
         * @brief    设置算法输入数据
         * @param[i] inp     输入数据对象
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-02-10
         */
        void setInput(vtkDataSet* inp);

        /**
         * @brief    设置模型ID
         * @param[i] modelId     模型ID
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-02-10
         */
        void setModelID(int modelId);

        /**
         * @brief    获取模型ID
         * @return   int    模型ID
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-02-10
         */
        int getModelID();

        /**
         * @brief    刷新数据
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-02-10
         */
        void update();

        /**
         * @brief    获取阈值过滤器对象
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-02-10
         */
        vtkThreshold* getThresholdFilter();

        /**
         * @brief    获取当前场分量
         * @return   当前场分量
         * @author   WuYizhen (wyz_hanhan@163.com)
         * @date     2026-01-05
         */
        static QString getCurrentField();

        /**
         * @brief    设置当前场分量
         * @param[i] 场分量
         * @author   WuYizhen (wyz_hanhan@163.com)
         * @date     2026-01-05
         */
        static void setCurrentField(QString field);

        /**
         * @brief    添加场分量对应的范围
         * @param[i] 场分量以及其对应的范围
         * @author   WuYizhen (wyz_hanhan@163.com)
         * @date     2026-01-05
         */
        static void addNameRange(QString name, QPair<double, double> range);

        /**
         * @brief    获取场分量以及其对应的范围
         * @param[i] 场分量名称
         * @return   场分量对应的范围
         * @author   WuYizhen (wyz_hanhan@163.com)
         * @date     2026-01-05
         */
        QPair<double, double> getRangeByName(QString name);

    private:
        /**
         * @brief  阈值过滤器对象
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2026-02-11
         */
        vtkThreshold* _thresholdFilter = nullptr;

        /**
         * @brief  变量名称
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2026-02-11
         */
        QString _fileName = "";

        /**
         * @brief  模型ID
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2026-02-11
         */
        int _modelID = -1;

        /**
         * @brief  变量类型
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2026-02-11
         */
        VariableLocation _fileType = VariableLocation::VALOCNode;

        /**
         * @brief  后处理结构数据类型
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2026-02-11
         */
        StruPostType _postType = StruPostType::SPTNone;

        /**
         * @brief  场分量名称
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2026-02-11
         */
        static QString _currentField;

        /**
         * @brief  场分量名称与范围映射表
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2026-02-11
         */
        static QHash<QString, QPair<double, double>> _nameRange;
    };

    /**
     * @brief  后处理阈值数据管理类
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2026-02-11
     */
    class FITKInterfaceStructuralPostAPI FITKStruThresholdPostDataManager
        : public Core::FITKAbstractDataManager<FITKStruThresholdPostData> {
    public:
        /**
         * @brief    构造函数
         * @param[i] absPost  后处理数据对象
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-02-10
         */
        explicit FITKStruThresholdPostDataManager(FITKAbstractStructuralPostVTK* absPost);

        /**
         * @brief    析构函数
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-02-10
         */
        virtual ~FITKStruThresholdPostDataManager() = default;

        /**
         * @brief    追加对象
         * @param[i] item
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-02-10
         */
        void appendDataObj(Core::FITKAbstractDataObject* item) override;

        /**
         * @brief    插入对象
         * @param[i] index     索引值
         * @param[i] item
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-02-10
         */
        void insertDataObj(int index, Core::FITKAbstractDataObject* item) override;

    private:
        /**
         * @brief  管理可视化对象
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2026-02-10
         */
        FITKAbstractStructuralPostVTK* _absPost = nullptr;
    };
}
#endif // !_FITKStruThresholdPostData_H__
