/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperGraphPostProcess.h
 * @brief  后处理可视化事件操作器
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-09-10
 *********************************************************************/
#ifndef _OPERGRAPHPOSTPROCESS_H__
#define _OPERGRAPHPOSTPROCESS_H__

#include "OperGraphEvent3D.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace GUI
{
    class GraphArea;
}
namespace GUIOper
{
    /**
     * @brief  后处理可视化事件操作器
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-09-10
     */
    class OperGraphPostProcess : public OperGraphEvent3D
    {
        Q_OBJECT
    public:
        /**
         * @brief  构造函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-10
         */
        explicit OperGraphPostProcess() = default;
        /**
         * @brief  析构函数
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-10
         */
        ~OperGraphPostProcess() = default;
        /**
         * @brief    更新全部
         * @param[i] forceUpdate 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-10
         */
        void updateAll(bool forceUpdate = false) override;
        /**
         * @brief    根据数据对象ID更新可视化对象
         * @param[i] dataObjId 
         * @param[i] forceUpdate 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-11
         */
        void updateGraph(int dataObjId, bool forceUpdate = false, bool isFitView = false) override;
        /**
         * @brief    切换场量数据
         * @param[i] dataId                     数据id
         * @param[i] type                       场量类型（1：点 2：单元）
         * @param[i] fieldName                  场量名称
         * @param[i] compIndex                  分量（1：X 2：Y 3：Z）
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-17
         */
        void changeField(int dataId, int type, QString fieldName, int compIndex = 0) override;
        /**
         * @brief    更新文本信息
         * @param[i] dataId 
         * @param[i] info 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-18
         */
        void updateTextInfo(int dataId, QString info) override;
        /**
         * @brief    设置范围颜色
         * @param[i] isOpenOutRanger                 是否显示范围之外的颜色
         * @param[i] minRangeColor                   范围最小颜色
         * @param[i] maxRangeColor                   范围最大颜色
         * @param[i] transparency                    透明度（0-1）
         * @param[i] postId                          后处理对象id -1：全局
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-09
         */
        virtual void setRangeColor(bool isOpenOutRanger, QColor minRangeColor, QColor maxRangeColor, double transparency, int postId = -1) override;
        /**
         * @brief    获取端点颜色
         * @param[i] isOutRanger       是否获取显示范围之外的颜色
         * @param[o] minRangeColor    maxRangeColor
         **/
        virtual void getRangeColor(bool isOutRanger, int postId, QColor& minRangeColor, QColor& maxRangeColor)override;
        /**
         * @brief    设置端点范围
         * @param[i] postId                          后处理对象id
         * @param[i] range[2]                 [0]最小值，[1]最大值
         **/
        virtual void setRange(int postId, double* range)override;
        /**
         * @brief    获取端点范围
         * @param[i] postId                          后处理对象id
         **/
        virtual double* getRange(int postId)override;

        /**
         * @brief    设置范围之外的颜色
         * @param[i] isOpenOutRanger                  是否开启范围
         * @param[i] aboveRange                       超出范围颜色[4]
         * @param[i] belowRange                       低于范围颜色[4]
         * @param[i] colorNum                         颜色数量（不包含范围之外的数量）
         * @param[i] postId                           后处理对象id  -1:全局
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-09
         */
        virtual void setOutRangerColor(bool isOpenOutRanger, double* aboveRGBA, double* belowRGBA, int colorNum, int postId = -1) override;
        /**
        * @brief    设置全局颜色设置标志
        * @param[i] isSet
        * @return   void
        */
        void setRangerColorSetFlag(bool isSet)override;
        bool getRangerColorSetFlag(int postId)override;
        /**
         * @brief    设置颜色数量
         * @param[i] isOpenOutRanger                  是否开启范围
         * @param[i] colorNum                         颜色数量（不包含范围之外的数量）
         * @param[i] postId                           后处理对象id  -1:全局
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-11-07
         */
        virtual void setRangerColorNum(bool isOpenOutRanger, int colorNum, int postId = -1) override;
        /**
         * @brief    获取全局颜色数量
         * @param[i] postId 
         * @return   int
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-11-07
         */
        virtual int getRangerColorNum(int postId = -1) override;
        /**
         * @brief    清空全部窗口可视化对象
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2026-03-18
         */
        void finalize() override;
    };
    Register2FITKOperatorRepo(GraphPostProcess, OperGraphPostProcess);
}
#endif // !_OPERGRAPHPOSTPROCESS_H__
