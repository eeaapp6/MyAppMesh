/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorCase.h
 * @brief  Radioss适配器 用于读取RAD文件和写出RAD文件。
 * @author zhangjingx (hiter25@outlook.com)
 * @date   2025-07-02
 *********************************************************************/
#ifndef _FITKRADIOSSADAPTORCASE_H_
#define _FITKRADIOSSADAPTORCASE_H_
#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKRadiossCase;
 
    /**
     * @brief  Radioss RAD 读写适配器
     * @author zhangjingx (hiter25@outlook.com)
     * @date   2025-07-02
     */
    class  FITKRadiossAdaptorCase : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorCase() = default;
        ~FITKRadiossAdaptorCase() = default;
        /**
         * @brief    获取适配器数据类型名
         * @return   QString 适配器数据类型名
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-02
         */
        QString getAdaptorClass() override;
        /**
         * @brief    适配器读取
         * @return   bool true成功，false失败
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-02
         */
        bool adaptR() override;
        /**
         * @brief    适配器写出
         * @return   bool true成功，false失败
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-02
         */
        bool adaptW() override;

    private:
        /**
         * @brief    读取版本、单位制
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-08
         */
        bool readBegin();
        /**
         * @brief    读取标题
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-08
         */
        bool readTitle();
    private:
        /**
         * @brief    写出文件头
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-08-12
         */
        bool writeHead();
        /**
         * @brief    写出BEGIN
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-08-12
         */
        bool writeBegin();
        /**
         * @brief    写出TITLE
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-08-12
         */
        bool writeTitle();
        /**
         * @brief    写出材料
         * @return   bool
         * @author   libaojun
         * @date     2025-08-25
         */
        bool writeMaterials();
        /**
         * @brief    写出NODE
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-08-12
         */
        bool writeNode();
        /**
         * @brief    写出PART
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-08-13
         */
        bool writePart();
        /**
        * @brief    写出属性默认值
        * @return   bool
        * @author   wangning (2185896382@qq.com)
        * @date     2025-08-28
        */
        bool writeDefaultPropValue();
        /**
         * @brief    写出属性(截面)
         * @return   bool
         * @author   wangning (2185896382@qq.com)
         * @date     2025-08-28
         */
        bool writeProperty();
        /**
         * @brief    写出曲线
         * @return   bool
         * @author   wangning (2185896382@qq.com)
         * @date     2025-08-28
         */
        bool writeCurve();
        /**
         * @brief    写出面
         * @return   bool
         * @author   wangning (2185896382@qq.com)
         * @date     2025-08-28
         */
        bool writeSurface();
        /**
         * @brief    写出刚性墙
         * @return   bool
         * @author   wangning (2185896382@qq.com)
         * @date     2025-08-28
         */
        bool writeRigidWall();
        /**
         * @brief    写出重力
         * @return   bool
         * @author   wangning (2185896382@qq.com)
         * @date     2025-08-28
         */
        bool writeGravity();
        /**
        * @brief    写出初始场
        * @return   bool
        * @author   wangning (2185896382@qq.com)
        * @date     2025-09-05
        */
        bool writeInitialField();
        /**
         * @brief    写出节点组
         * @return   bool
         * @author   wangning (2185896382@qq.com)
         * @date     2025-09-05
         */
        bool writeNodeSet();
        /**
        * @brief    写出边界
        * @return   bool
        * @author   wangning (2185896382@qq.com)
        * @date     2025-09-05
        */
        bool writeBC();
        /**
        * @brief    写出子集
        * @return   bool
        * @author   wangning (2185896382@qq.com)
        * @date     2025-09-05
        */
        bool writeSubset();
        /**
        * @brief    写出默认相互作用
        * @return   bool
        * @author   wangning (2185896382@qq.com)
        * @date     2025-09-05
        */
        bool writeDefaultInteraction();
        /**
        * @brief    写出相互作用
        * @return   bool
        * @author   wangning (2185896382@qq.com)
        * @date     2025-09-05
        */
        bool writeInteraction();
        /**
        * @brief    写出连接
        * @return   bool
        * @author   wangning (2185896382@qq.com)
        * @date     2025-09-05
        */
        bool writeConnection();
        /**
        * @brief    写出探针
        * @return   bool
        * @author   wangning (2185896382@qq.com)
        * @date     2025-09-05
        */
        bool writeProbe();
        /**
        * @brief    写出时间历程
        * @return   bool
        * @author   wangning (2185896382@qq.com)
        * @date     2025-09-05
        */
        bool writeTimerHistory();
        /**
        * @brief    写出状态方程
        * @return   bool
        * @author   wangning (2185896382@qq.com)
        * @date     2025-09-05
        */
        bool writeEOS();
        /**
        * @brief    写出粒子全局属性
        * @return   bool
        * @author   wangning (2185896382@qq.com)
        * @date     2025-09-05
        */
        bool writeSPHGLO();
        /**
        * @brief    写出失效
        * @return   bool
        * @author   wangning (2185896382@qq.com)
        * @date     2025-09-05
        */
        bool writeFailure();
        /**
         * @brief    写出边界元法
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-04-27
         */
        bool writeBEM();

    private:
        /**
         * @brief  算例数据
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-08
         */
         FITKRadiossCase* _caseData{};
    };
    /**
     * @brief 注册适配器
     * @date   2025-07-07
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKRadiossCase, FITKRadiossAdaptorCase);
    RegRadiossRadAdaptor(/BEGIN, FITKRadiossAdaptorCase);
    RegRadiossRadAdaptor(/TITLE, FITKRadiossAdaptorCase,1);
}
#endif // !_FITKRADIOSSADAPTORCASE_H_