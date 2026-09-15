/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file FITKSystemInfo.h
 * @author libaojun
 * @brief  系统信息类头文件
 * @date 2025-12-31
 */
#ifndef __FITKSYSTEM__INFO__H__
#define __FITKSYSTEM__INFO__H__

#include <QString>
#include "FITKCoreAPI.h"

namespace Core
{
    /**
     * @brief 操作系统枚举类型
     */
    enum class FITKOperatingSystem 
    {
        FITK_OS_UNKNOWN = 0,
        FITK_OS_WINDOWS,
        FITK_OS_LINUX,
        FITK_OS_MACOS
    };  
    /**
     * @brief 系统架构枚举类型
     */
    enum class FITKArchitecture
    {
        FITK_ARCH_UNKNOWN = 0,
        FITK_ARCH_X86,
        FITK_ARCH_X64,
        FITK_ARCH_ARM,
        FITK_ARCH_ARM64
    };
    /*
     * @brief CPU类型枚举
     */ 
    enum CPUType
    {
        FITK_CPU_UNKNOWN = 0,
        FITK_CPU_INTEL,
        FITK_CPU_AMD, 
    };
    /*
     * @brief GPU类型枚举
     */
    enum class GPUType
    {
        FITK_GPU_UNKNOWN = 0,
        FITK_GPU_NVIDIA,
        FITK_GPU_AMD,
        FITK_GPU_INTEL
    };

    /**
     * @brief  系统信息类
     * @details 用于检测和获取操作系统类型、系统架构类型、CPU类型和GPU类型等信息
     */
    class FITKCoreAPI FITKSystemInfo
    {
    public:
        /**
         * @brief Construct a new FITKSystemInfo object
         */
        explicit FITKSystemInfo();
        /**
         * @brief Destroy the FITKSystemInfo object
         */
        virtual ~FITKSystemInfo() = default;
        /**
         * @brief 获取操作系统类型
         * @return FITKOperatingSystem 
         */
        FITKOperatingSystem getOperatingSystem() const;
        /**
         * @brief 获取系统架构类型
         * @return FITKArchitecture 
         */
        FITKArchitecture getArchitecture() const;
        /**
         * @brief  获取CPU类型
         * @return CPUType 
         */
        CPUType getCPUType() const;
        /**
         * @brief 获取GPU类型
         * @return GPUType 
         */
        GPUType getGPUType() const;

    private:
        /**
         * @brief  检测操作系统类型
         */
        void detectOSInfo();
        /**
         * @brief 检测系统架构类型
         */
        void detectArchInfo();
        /**
         * @brief  检测Windows平台的CPU和GPU类型
         * 
         */
        void detectInfoWin();
        /**
         * @brief 检测Windows平台的GPU类型
         */
        void detectGPUInfoWin();
        /**
         * @brief  检测Windows平台的CPU类型
         */
        void detectCPUInfoWin();

    private:
        FITKOperatingSystem m_osType{ FITKOperatingSystem::FITK_OS_UNKNOWN}; // 操作系统类型
        FITKArchitecture m_archType{ FITKArchitecture::FITK_ARCH_UNKNOWN };  // 系统架构类型
        CPUType m_cpuType{ CPUType::FITK_CPU_UNKNOWN };  // CPU类型
        GPUType m_gpuType{ GPUType::FITK_GPU_UNKNOWN };  // GPU类型


    };
} // namespace Core



#endif // __FITKSYSTEM__INFO__H__