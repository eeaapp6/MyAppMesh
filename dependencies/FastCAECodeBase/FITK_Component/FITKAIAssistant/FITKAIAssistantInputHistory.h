/**
 * @file   FITKAIAssistantInputHistory.h
 * @brief  AI 输入历史管理类声明
 * @author libaojunAI
 * @date   2026-05-19
 */
#ifndef __FITK_AI_ASSISTANT_INPUT_HISTORY_H__
#define __FITK_AI_ASSISTANT_INPUT_HISTORY_H__

#include <QObject>
#include <QStringList>

class QLineEdit;
class QEvent;

namespace AIAssistant
{
    /**
     * @brief 管理输入历史并处理上下键回填
     * @author libaojunAI
     * @date 2026-05-19
     */
    class FITKAIAssistantInputHistory : public QObject
    {
    public:
        /**
         * @brief 构造函数
         * @param parent 父对象
         */
        explicit FITKAIAssistantInputHistory(QObject* parent = nullptr);

        /**
         * @brief 绑定输入框并接管上下键导航
         * @param input 输入框对象
         */
        void bindInput(QLineEdit* input);

        /**
         * @brief 记录一次提交命令
         * @param command 命令文本
         */
        void recordSubmission(const QString& command);

        /**
         * @brief 重置导航状态
         */
        void resetNavigation();

    protected:
        /**
         * @brief 事件过滤，处理上下键历史回填
         */
        bool eventFilter(QObject* watched, QEvent* event) override;

    private:
        QLineEdit* _input{};
        QStringList _history{};
        int _cursor{-1};
        QString _draft{};
        int _maxHistory{200};
    };
}

#endif
