/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractStruPostVTK.h
 * @brief 结构分析结果抽象类
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-05-19
 *
 */
#ifndef __FITKINTERFACESTRUPOSTVTK_H_ABS_
#define __FITKINTERFACESTRUPOSTVTK_H_ABS_

#include "FITKInterfaceStructuralPostAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKVarientParams.h"

class vtkSelectionNode;
class vtkSelection;
class vtkExtractSelection;
class vtkIdTypeArray;
class vtkDataSetReader;
class vtkAlgorithm;
class vtkDataSet;
class vtkCellDataToPointData;
class vtkIdFilter;
class vtkPassThrough;

class FITKDeformFilter;
class FITKShellFeatureEdges;
class FITKSurfaceFilter;

namespace Interface
{
    class FITKCutViewManager;
    class FITKAbstractViewCut;
    class FITKStruPostMessageText;
    class FITKMeshFeatureVTK;
    class FITKPostGroupManager;
    class FITKStruIsoPostDataManager;

    /**
     * @brief 结构分析后处理结果
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-05-19
     */
    enum StruPostType
    {
        SPTNone,
        SPTStatic,
        SPTDynamicExplict,
    };
    /**
     * @brief  结构分析后处理文件类型
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-03-25
     */
    enum class StruPostFileType
    {
        SPFTNone,                //空
        SPFTVTK,                 //VTK
        SPFTVTP,                 //VTP
        SPFTVTU,                 //VTU
        SPFTIBE,                 //IBE
    };
    /**
     * @brief 变量位置
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-05-19
     */
    enum class VariableLocation
    {
        VALOCNode,
        VALOCEle,
    };
    /**
     * @brief 结构分析后处理抽象类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-05-19
     */
    class FITKInterfaceStructuralPostAPI FITKAbstractStructuralPostVTK
        : public Core::FITKAbstractNDataObject, public Core::FITKVarientParams
    {
        FITKCLASS(Interface, FITKAbstractStructuralPostVTK);
    public:
        /**
         * @brief    构造函数
         * @param[i] type                    文件类型（vtk、vtp...）
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-25
         */
        explicit FITKAbstractStructuralPostVTK(StruPostFileType type);
        /**
         * @brief Destroy the FITKAbstractStructuralPostVTK object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-19
         */
        virtual ~FITKAbstractStructuralPostVTK() = 0;
        /**
         * @brief 获取结构分析后处理类型
         * @return StruPostType 结构分析后处理类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-19
         */
        virtual StruPostType getPostStructualType() = 0;
        /**
         * @brief 设置前处理关联算例ID
         * @param[i]  id             前处理ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-19
         */
        void setPreCaseID(const int id);
        /**
         * @brief 获取前处理关联分析ID
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-19
         */
        int getPreCaseID();
        /**
         * @brief 获取文件读取器
         * @return FITKDataSetReader*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-20
         */
        vtkAlgorithm* getDataSetReader();
        /**
         * @brief    获取输出
         * @return   vtkDataSet*          数据输出
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-24
         */
        virtual vtkDataSet* getOutput();
        /**
         * @brief 获取变形过滤器
         * @return FITKDeformFilter*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-20
         */
        FITKDeformFilter* getDeformFilter();

        /**
         * @brief       获取当前可见的切片数据对象。
         * @return      切片数据
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-09
         */
        FITKAbstractViewCut* getVisibleViewCut();

        /**
         * @brief       开启或关闭网格特征生成。
         * @param[in]   flag：标记
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-21
         */
        void setUseMeshFeature(bool flag);

        /**
         * @brief       获取是否使用网格特征。
         * @return      是否开启
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-21
         */
        bool getUseMeshFeature();

        /**
         * @brief       获取网格特征数据。
         * @return      网格特征数据
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-17
         */
        FITKMeshFeatureVTK* getMeshFeature();

        /**
         * @brief       获取输出算法。
         * @return      vtk算法
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-17
         */
        vtkAlgorithm* getOutputAlgorithm();

        /**
         * @brief       获取渲染输出算法。
         * @return      vtk算法
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-01-22
         */
        vtkAlgorithm* getRenderOutputAlgorithm();

        /**
         * @brief       获取分组数据提取器。
         * @return      分组数据提取器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-05
         */
        vtkExtractSelection* getDisplayGroupExtractor();
        /**
         * @brief 获取变量名称列表
         * @param[i]  loc            变量位置
         * @return QStringList
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-20
         */
        QStringList getVariables(VariableLocation loc);
        /**
         * @brief 获取变量组分名称
         * @param[i]  loc            变量位置
         * @param[i]  variaable      变量名称
         * @return QStringList
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-20
         */
        QStringList getVariableComponents(VariableLocation loc, const QString& variaable );
        /**
         * @brief    获取场量范围
         * @param[i] loc                        场量类型
         * @param[i] varName                    场量名称
         * @param[o] range                      场量范围[2]
         * @param[i] componentIndex             分量索引
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-12-18
         */
        void getVariableRange(VariableLocation loc, const QString& varName, double* range, int componentIndex);
        /**
         * @brief       获取单元总数。
         * @return      单元总数
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-18
         */
        int getNumberOfCells();

        /**
         * @brief 下一帧
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-21
         */
        virtual void nextFrame();
        /**
         * @brief 上一帧
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-21
         */
        virtual void previousFrame();
        /**
         * @brief 第一帧
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-21
         */
        virtual void firstFrame();
        /**
         * @brief 最后一帧
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-21
         */
        virtual void lastFrame();
        /**
         * @brief    设置当前帧数（初始帧数为0）
         * @param[i] curFrame                   当前帧数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-09-16
         */
        virtual void setCurFrame(int curFrame);
        /**
         * @brief    获取总帧数
         * @return   int                        总帧数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-09-16
         */
        virtual int frameNum();
        /**
         * @brief    获取当前帧数
         * @return   int                        当前帧数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-09-17
         */
        virtual int getCurFrame();
        /**
         * @brief 是否打开变形
         * @param[i]  enable   是否允许变形
         
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-20
         */
        void enableDeform(bool enable);

        /**
         * @brief 获取变形因子
         * @param[i]  fac      变形因子
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-03
         */
        void setDeformFactor(const double fac);

        /**
         * @brief 获取变形因子
         * @return double
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-03
         */
        double getDeformFactor();
        /**
         * @brief 设置不变形因子
         * @param[i]  fac           不变形因子
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-03
         */
        void setUnDeformFactor(const double fac);
        /**
         * @brief 获取不变形因子
         * @return double
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-03
         */
        double getUnDeformFactor();
        /**
         * @brief 设置变形变量
         * @param[i]  va             变量名
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-03
         */
        void setDeformVariable(const QString& va);
        /**
         * @brief 获取变形变量名称
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-03
         */
        QString getDeformVariable();
        /**
         * @brief 获取切面管理器
         * @return FITKCutViewManager
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-17
         */
        FITKCutViewManager* getCutViewManager();

        /**
         * @brief    获取等值面管理器
         * @return   等值面管理器
         * @author   WuYizhen (wyz_hanhan@163.com)
         * @date     2025-12-23
         */
        FITKStruIsoPostDataManager* getIsoViewManager();

        /**
         * @brief       显示或隐藏裁切器。
         * @param[in]   visible：显示或隐藏
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-06
         */
        void setShowCutWidget(bool visible);

        /**
         * @brief       裁切器是否显示。
         * @return      显示或隐藏
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-06
         */
        bool getShowCutWidget();

        /**
         * @brief       设置隐藏或显示裁切/切片。
         * @param[in]   flag：是或否
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-11
         */
        void setShowViewCut(bool flag);

        /**
         * @brief       裁切/切片是否显示。
         * @return      是或否
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-11
         */
        bool getShowViewCut();

        /**
         * @brief       设置是否激活分组显示。
         * @param[in]   flag：是否激活
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-05
         */
        void setEnableDisplayGroup(bool flag);

        /**
         * @brief       获取分组显示是否激活。
         * @return      是否激活。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-05
         */
        bool getEnableDisplayGroup();

        /**
         * @brief       设置显示分组单元索引列表。
         * @param[in]   indice：单元索引列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-05
         */
        void setDisplayGroupEleIndice(const QList<int> & indice);

        /**
         * @brief       获取显示分组单元索引列表。
         * @return      单元索引列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-18
         */
        QList<int> getDisplayGroupEleIndice();

        /**
         * @brief       获取显示分组单元数量。
         * @return      数量。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-10
         */
        int getNumberOfDisplayGroupMember();

        /**
         * @brief       获取显示分组名称。
         * @return      显示分组名称
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-10
         */
        QString getDisplayGroupName();

        /**
         * @brief       获取当前后处理数据显示分组管理器。
         * @return      显示分组管理器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-19
         */
        FITKPostGroupManager* getDisplayGroupManager();

        /**
         * @brief       设置显示分组名称。
         * @param[in]   name：名称
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-10
         */
        void setDisplayGroupName(QString name);

        //// New Version. 2024/10/17
        ///**
        // * @brief       激活显示分组。
        // * @param[in]   groupID：显示分组数据ID（-1为分组All）[缺省]
        // * @author      ChengHaotian (yeguangbaozi@foxmail.com)
        // * @date        2024-10-17
        // */
        //void setDisplayGroupID(int groupID = -1);
        /**
         * @brief    获取信息文本对象
         * @return   FITKStruPostMessageText*     信息文本对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-03
         */
        FITKStruPostMessageText* getMessageTextData();
        /**
         * @brief    更新文本
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-03
         */
        virtual void updateText();
        /**
         * @brief    设置单元数据是否转化为点数据
         * @param[i] isCellToPoint              单元数据是否转化为点数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-09-17
         */
        void setIsCellDataToPointData(bool isCellToPoint);
        /**
         * @brief    设置等值面显示的时候，模型是否显示
         * @param[i] 模型是否显示
         * @author   WuYizhen (wyz_hanhan@163.com)
         * @date     2025-12-31
         */
        void setModelVisible(bool flag);
        /**
         * @brief    获取等值面显示的时候，模型是否显示
         * @return   模型是否显示
         * @author   WuYizhen(wyz_hanhan@163.com)
         * @date     2025-12-31
         */
        bool getModelVisible();
        /**
         * @brief    获取h5文件的时间步
         * @return   h5文件的当前时间步和总的时间步
         * @author   WuYizhen (wyz_hanhan@163.com)
         * @date     2026-01-23
         */
        QPair<int, int> getH5FileStep();
    protected:
        /**
         * @brief 设置输入文件
         * @param[i]  file         输入文件
         * @note  云道读取.h5文件  @param[i] file         网格文件
         *                         @param[i] resultFile   结果文件
         *                         @param[i] step         时间步
         *                         @author   WuYizhen (wyz_hanhan@163.com)
         *                         @date     2026-01-22
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-20
         */
        void setInputFile(const QString & file, const QString & resultFile = "", int step = 0);
        /**
         * @brief 获取输入文件
         * @return         输入文件
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-20
         */
        QString getInputFile();

        /**
         * @brief       重置显示分组单元索引列表为全部单元。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-17
         */
        void resetDisplayGroup();

    protected:
        /**
         * @brief  文件类型
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-24
         */
        StruPostFileType _type = StruPostFileType::SPFTNone;
         /**
          * @brief 关联的前处理分析ID
          * @author LiBaojun (libaojunqd@foxmail.com)
          * @date 2024-05-19
          */
        int _preCaseID{ -1 };
        /**
         * @brief 读取文件
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-20
         */
        vtkAlgorithm* _dataSetReader{};

        /**
         * @brief  单元数据转为点数据
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-09-17
         */
        vtkCellDataToPointData* _cellDataToPoint = nullptr;
        vtkIdFilter* _idFilter = nullptr;
        /**
         * @brief 变形过滤器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-20
         */
        FITKDeformFilter* _deformFilter{};

        /**
         * @brief       网格特征数据。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-17
         */
        FITKMeshFeatureVTK* _feature{ nullptr };

        /**
         * @brief 不变形系数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-03
         */
        double _unDeformFactor{ 0.0 };
        /**
         * @brief 变形因子
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-03
         */
        double _deformFactor{1.0};
        /**
         * @brief 是否开启变形
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-03
         */
        bool _enableDeform{false};
        /**
         * @brief 切面管理器 
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-17
         */
        FITKCutViewManager* _cutViewManager{};

        /**
         * @brief  等值面管理器
         * @author WuYizhen (wyz_hanhan@163.com)
         * @date   2025-12-23
         */
        FITKStruIsoPostDataManager* _isoViewManager{};

        /**
         * @brief       显示或隐藏裁切器。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-06
         */
        bool _cutWidgetVisible = false;

        /**
         * @brief       显示或隐藏裁切/切片。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-11
         */
        bool _cutViewVisible = false;

        /**
         * @brief       是否激活分组显示。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-05
         */
        bool _enableDisplayGroup = false;

        /**
         * @brief       显示或隐藏模型
         * @author      WuYizhen (wyz_hanhan@163.com)
         * @date        2025-12-31
         */
        bool _isHide = false;
        /**
         * @brief       数据提取器节点。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-05
         */
        vtkSelectionNode* _selectNode{ nullptr };

        /**
         * @brief       提取区域。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-05
         */
        vtkSelection* _section{ nullptr };

        /**
         * @brief       分组数据提取器。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-05
         */
        vtkExtractSelection* _extractSelection{ nullptr };

        /**
         * @brief       分组显示提取索引数组。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-05
         */
        vtkIdTypeArray* _idArray{ nullptr };

        /**
         * @brief       显示分组名称。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-10
         */
        QString _groupName = "";// "DisplayGroup";
        /**
         * @brief  文本数据类
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-12-03
         */
        FITKStruPostMessageText* _textData = nullptr;

        /**
         * @brief       显示分组管理器。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-19
         */
        FITKPostGroupManager* m_displayGroupManager{ nullptr };

        /**
         * @brief       渲染使用的过滤器。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-21
         */
        vtkPassThrough* m_renderAlg{ nullptr };

    };
}


#endif
