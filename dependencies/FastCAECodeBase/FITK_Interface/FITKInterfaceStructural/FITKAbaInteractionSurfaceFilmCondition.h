/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_ABAINTERACTIONCONTACTTANGENTIALBEHAVIOR_H__
#define _FITK_ABAINTERACTIONCONTACTTANGENTIALBEHAVIOR_H__

#include "FITKAbaAbstractInteraction.h"

namespace Interface 
{
    class FITKAbaAbstractAmplitude;

    /**
     * @brief  表面热传导条件类
     * @author libaojun
     * @date 2025-06-04
     */
class FITKInterfaceStructuralAPI FITKAbaInteractionSurfaceFilmCondition :
     public FITKAbaAbstractInteraction
{
    Q_OBJECT
    FITKCLASS(Interface, FITKAbaInteractionSurfaceFilmCondition)
public:
    /**
     * @brief  构造函数 
     * @author libaojun
     * @date 2025-06-04
     */
    explicit FITKAbaInteractionSurfaceFilmCondition() = default;

    /**
     * @brief  析构函数
     * @author libaojun
     * @date 2025-06-04
     */
    virtual ~FITKAbaInteractionSurfaceFilmCondition() = default;

    /**
     * @brief  获取相互作用类型
     * @author libaojun
     * @date 2025-06-04
     */
    FITKAbaInteractionType getInteractionType() const override;
    /**
     * @brief  在分析步中是否被调整
     * @param[in] stepId 分析步ID
     * @return true: 被调整; false: 未被调整
     * @author libaojun
     * @date   2025-07-09
     */
    bool isModifiedInStep(int stepId) const override;
    /**
     * @brief  激活状态发生变化
     * @return void
     * @author libaojun
     * @date   2025-07-09
     */
    void activeStateChanged() override;

    /**
     * @brief  获取表面热传导系数
     * @return double 表面热传导系数
     * @author libaojun
     * @date 2025-06-04
     */
    double getFilmCoefficient() const;
    /**
     * @brief  设置表面热传导系数
     * @param[in] filmCoefficient 表面热传导系数
     * @author libaojun
     * @date 2025-06-04
     */
    void setFilmCoefficient(double filmCoefficient);
    /**
     * @brief  获取表面热传导系数幅值ID
     * @return int 表面热传导系数幅值ID
     * @author libaojun
     * @date 2025-06-04
     */
    int getFilmCoefficientAmplitudeID() const;
    /**
     * @brief  获取表面热传导系数幅值对象
     * @return FITKAbaAbstractAmplitude* 表面热传导系数幅值对象
     * @author libaojun
     * @date 2025-06-04
     */
    FITKAbaAbstractAmplitude *getFilmCoefficientAmplitudeObj() const;

    /**
     * @brief  设置表面热传导系数幅值ID
     * @param[in] amplitudeID 表面热传导系数幅值ID
     * @author libaojun
     * @date 2025-06-04
     */
    void setFilmCoefficientAmplitudeID(int amplitudeID);
    /**
     * @brief  获取热源温度
     * @return double 热源温度
     * @author libaojun
     * @date 2025-06-04
     */
    double getSinkTemperature() const;
    /**
     * @brief  设置热源温度
     * @param[in] sinkTemperature 热源温度
     * @author libaojun
     * @date 2025-06-04
     */
    void setSinkTemperature(double sinkTemperature);
    /**
     * @brief  获取热源温度幅值ID
     * @return int 热源温度幅值ID
     * @author libaojun
     * @date 2025-06-04
     */
    int getSinkTemperatureAmplitudeID() const;
    /**
     * @brief  设置热源温度幅值ID
     * @param[in] amplitudeID 热源温度幅值ID
     * @author libaojun
     * @date 2025-06-04
     */
    void setSinkTemperatureAmplitudeID(int amplitudeID);
    /**
     * @brief  获取热源温度幅值对象
     * @return FITKAbaAbstractAmplitude* 热源温度幅值对象
     * @author libaojun
     * @date 2025-06-04
     */
    FITKAbaAbstractAmplitude *getSinkTemperatureAmplitudeObj() const;

private:
    double m_filmCoefficient{0.0}; ///< 表面热传导系数 
    int m_filmCoefficientAmplitudeID{0}; ///< 表面热传导系数幅值ID
    double m_sinkTemperature{0.0}; ///< 热源温度
    int m_sinkTemperatureAmplitudeID{0}; ///< 热源温度幅值ID

     /**
     * @brief  后续分析步中调整的数据
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-25
     */
    QMap<int, int> m_modifiedDatas{};
};

} // namespace Interface

#endif // _FITK_ABAINTERACTIONCONTACTTANGENTIALBEHAVIOR_H__