/**
 * @file   FITKAIAssistantInterface.h
 * @brief  FITKAIAssistant 组件接口声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date   2026-05-14
 */
#ifndef __FITK_AI_ASSISTANT_INTERFACE_H__
#define __FITK_AI_ASSISTANT_INTERFACE_H__

#include "FITK_Component/FITKAIAssistant/FITKAIAssistantAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"

namespace AIAssistant
{
    /**
     * @brief AI Assistant 组件对外接口实现
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2026-05-14
     */
    class FITKAIASSISTANTAPI FITKAIAssistantInterface : public AppFrame::FITKComponentInterface
    {
    public:
        /**
         * @brief 构造函数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        explicit FITKAIAssistantInterface();
        /**
         * @brief 析构函数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        virtual ~FITKAIAssistantInterface() = default;

        /**
         * @brief 返回组件名称，用于组件工厂查找
         * @return 组件名称
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        virtual QString getComponentName() override;
        /**
         * @brief 返回组件窗口
         * @param indexPort 组件端口号
         * @return 组件窗口指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2026-05-14
         */
        virtual QWidget* getWidget(const int indexPort) override;
    };
}

#endif