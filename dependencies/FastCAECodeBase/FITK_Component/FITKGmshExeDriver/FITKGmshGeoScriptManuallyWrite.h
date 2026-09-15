/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKGmshGeoScriptManuallyWrite.h
 * @brief  手动写出geo脚本文件
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-12-24
 *********************************************************************/
#ifndef _FITK_GMESHEXEC_GEOSCRIPT_MANUALLY_WRITE_H___
#define _FITK_GMESHEXEC_GEOSCRIPT_MANUALLY_WRITE_H___

#include "FITKGmshExeDriverAPI.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextWriter.h"

namespace GmshExe
{
    class GmshPointData;
    /**
     * @brief 手动写出geo脚本文件
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-11-27
     */
    class FITKGmshExeDriverAPI FITKGmshGeoScriptManuallyWrite : public Interface::FITKAbstractTextWriter
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKGmshGeoScriptManuallyWrite object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        explicit FITKGmshGeoScriptManuallyWrite();
        /**
         * @brief Destroy the FITKGmshGeoScriptManuallyWrite object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        virtual ~FITKGmshGeoScriptManuallyWrite();
        /**
         * @brief 设置写出的文件-绝对路径
         * @param[i] fileName 绝对路径
         * @return   void
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-27
         */
        void setFileName(const QString& fileName) override;
        /**
         * @brief    添加点
         * @param[i] pID 点编号
         * @param[i] coor 点坐标
         * @param[i] piontSize 点的大小
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-24
         */
        void addPiont(int pID, double* coor, double piontSize = 1.0);
        /**
         * @brief    添加线
         * @param[i] lID 线编号
         * @param[i] p1ID 点1编号
         * @param[i] p2ID 点2编号
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-24
         */
        void addLine(int lID, int p1ID, int p2ID);
        /**
         * @brief    添加板面
         * @param[i] fID 面编号
         * @param[i] lListIDs 线编号 -> 线编号方向一致为正，线编号方向不一致为负
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-24
         */
        void addPlaneSurface(int fID, QList<int> lListIDs);
        /**
         * @brief    添加体
         * @param[i] vID 体编号
         * @param[i] fListIDs 面编号
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-01-13
         */
        void addVolume(int vID, QList<int> fListIDs);
        /**
         * @brief    删除几何信息
         * @param[i] type 类型 1:point 2:line 3:face
         * @param[i] idLists 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-24
         */
        void deleteGeo(int type, QList<int> idLists);
        /**
         * @brief    添加复合面
         * @param[i] fListIDs 面编号
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-24
         */
        void addCompoundSurface(QList<int> fListIDs);
        /**
         * @brief    添加复合线
         * @param[i] lListIDs 线编号
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-24
         */
        void addCompoundCurve(QList<int> lListIDs);
        /**
         * @brief    设置网格尺寸大小
         * @param[i] size 全局
         * @param[i] min 最小
         * @param[i] max 最大
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-24
         */
        void setMeshSize(double size, double min = 0.0, double max = 1e+22);
        /**
         * @brief    设置细分算法
         * @param[i] type 0：无 1：Quads 2：Hexas
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-01-14
         */
        void setSubdivisionAlgorithm(int type = 0);
        /**
         * @brief    是否重组
         * @param[i] type 0:否 1:是
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-05-21
         */
        void setRecombineAll(int type);
        /**
         * @brief    网格生成的最大维度
         * @param[i] dim 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-24
         */
        void setMeshGenerateDim(int dim);
        /**
         * @brief    运行-写出脚本
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-24
         */
        void run() override;
        /**
         * @brief    打印控制台消息
         * @param[i] level 
         * @param[i] str 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-25
         */
        void consoleMessage(int level, const QString& str) override;

    private:
        QStringList m_scriptCommand{};
    };
}


#endif
