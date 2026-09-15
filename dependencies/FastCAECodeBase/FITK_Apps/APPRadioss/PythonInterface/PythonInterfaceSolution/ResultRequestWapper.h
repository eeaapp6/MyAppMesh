/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   ResultRequestWapper.h
 * @brief  结果请求相关的python接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-15
 *********************************************************************/
#ifndef _RESULT_REQUEST_WAPPER_H___
#define _RESULT_REQUEST_WAPPER_H___

#include <QObject>
#include <QString>

#include "PythonInterfaceSolutionAPI.h"

namespace Radioss {
    class FITKRadiossResultRequest;
}

/**
 * @brief  结果请求接口封装，供装饰器装饰
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-15
 */
class ResultRequest
{
public:
    /**
     * @brief    默认构造函数
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    explicit ResultRequest() = default;
    /**
     * @brief    析构函数
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    virtual ~ResultRequest() = default;

    /**
     * @brief    获取结果请求对象
     * @return   FITKRadiossResultRequest 指针
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    Radioss::FITKRadiossResultRequest* getResultRequest();
};

/**
 * @brief  结果请求相关PYTHON接口装饰
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-05-15
 */
class PythonInterfaceSolutionAPI ResultRequestWapper : public QObject
{
    Q_OBJECT
public slots:

    /**
     * @brief    创建 ResultRequest 实例
     * @return   新分配的 ResultRequest 指针
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    ResultRequest* new_ResultRequest();

    /**
     * @brief    释放 ResultRequest 实例
     * @param[i] obj 需要释放的 ResultRequest 指针
     * @return   void
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    void delete_ResultRequest(ResultRequest* obj);

    /**
     * @brief    设置动画开始时间
     * @param[i] obj ResultRequest 对象指针
     * @param[i] time 动画开始时间
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setStartTime(ResultRequest* obj, double time);

    /**
     * @brief    设置动画频率
     * @param[i] obj ResultRequest 对象指针
     * @param[i] f 动画频率
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setFrequency(ResultRequest* obj, double f);

    /**
     * @brief    设置时间历程频率
     * @param[i] obj ResultRequest 对象指针
     * @param[i] f 时间历程频率
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setHisFrequency(ResultRequest* obj, double f);

    /**
     * @brief    设置节点数据输出动画类型
     * @param[i] obj ResultRequest 对象指针
     * @param[i] typeList 节点数据输出动画类型列表
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setNodalOutputAnim(ResultRequest* obj, const QVariantList& typeList);

    /**
     * @brief    追加节点数据输出动画类型
     * @param[i] obj ResultRequest 对象指针
     * @param[i] typeList 节点数据输出动画类型列表
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString appendNodalOutputAnim(ResultRequest* obj, const QVariantList& typeList);

    /**
     * @brief    设置单元数据输出动画类型
     * @param[i] obj ResultRequest 对象指针
     * @param[i] typeList 单元数据输出动画类型列表
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setElementalOutputAnim(ResultRequest* obj, const QVariantList& typeList);

    /**
     * @brief    追加单元数据输出动画类型
     * @param[i] obj ResultRequest 对象指针
     * @param[i] typeList 单元数据输出动画类型列表
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString appendElementalOutputAnim(ResultRequest* obj, const QVariantList& typeList);

    /**
     * @brief    设置Brick数据输出动画类型
     * @param[i] obj ResultRequest 对象指针
     * @param[i] typeList Brick数据输出动画类型列表
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setBrickOutputAnim(ResultRequest* obj, const QVariantList& typeList);

    /**
     * @brief    追加Brick数据输出动画类型
     * @param[i] obj ResultRequest 对象指针
     * @param[i] typeList Brick数据输出动画类型列表
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString appendBrickOutputAnim(ResultRequest* obj, const QVariantList& typeList);

    /**
     * @brief    设置Shell数据输出动画类型
     * @param[i] obj ResultRequest 对象指针
     * @param[i] typeList Shell数据输出动画类型列表
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString setShellOutputAnim(ResultRequest* obj, const QVariantList& typeList);

    /**
     * @brief    追加Shell数据输出动画类型
     * @param[i] obj ResultRequest 对象指针
     * @param[i] typeList Shell数据输出动画类型列表
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString appendShellOutputAnim(ResultRequest* obj, const QVariantList& typeList);

    /**
     * @brief    清理结果请求参数
     * @param[i] obj ResultRequest 对象指针
     * @return   QString
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    QString clearResultRequestParam(ResultRequest* obj);

private:
    /**
     * @brief    更新图形和树形事件
     * @param[i] dataObjId 数据对象ID
     * @author   liuzhonghua (liuzhonghuaszch@163.com)
     * @date     2026-05-15
     */
    void updateGraphAndTreeEvent(int dataObjId);
};

#endif // _RESULT_REQUEST_WAPPER_H___