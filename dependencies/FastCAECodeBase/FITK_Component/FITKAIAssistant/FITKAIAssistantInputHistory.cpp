/**
 * @file   FITKAIAssistantInputHistory.cpp
 * @brief  AI 输入历史管理类实现
 * @author libaojunAI
 * @date   2026-05-19
 */
#include "FITKAIAssistantInputHistory.h"

#include <QEvent>
#include <QKeyEvent>
#include <QLineEdit>

namespace AIAssistant
{
    FITKAIAssistantInputHistory::FITKAIAssistantInputHistory(QObject* parent)
        : QObject(parent)
    {
    }

    void FITKAIAssistantInputHistory::bindInput(QLineEdit* input)
    {
        if (_input != nullptr)
        {
            _input->removeEventFilter(this);
        }

        _input = input;
        if (_input != nullptr)
        {
            _input->installEventFilter(this);
        }

        resetNavigation();
    }

    void FITKAIAssistantInputHistory::recordSubmission(const QString& command)
    {
        const QString trimmed = command.trimmed();
        if (trimmed.isEmpty())
        {
            return;
        }

        if (_history.isEmpty() || _history.last() != trimmed)
        {
            _history.append(trimmed);
            while (_history.size() > _maxHistory)
            {
                _history.removeFirst();
            }
        }

        resetNavigation();
    }

    void FITKAIAssistantInputHistory::resetNavigation()
    {
        _cursor = -1;
        _draft.clear();
    }

    bool FITKAIAssistantInputHistory::eventFilter(QObject* watched, QEvent* event)
    {
        if (watched != _input || _input == nullptr || event == nullptr)
        {
            return QObject::eventFilter(watched, event);
        }

        if (event->type() != QEvent::KeyPress)
        {
            return QObject::eventFilter(watched, event);
        }

        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Up)
        {
            if (_history.isEmpty())
            {
                return true;
            }

            if (_cursor < 0)
            {
                _draft = _input->text();
                _cursor = _history.size() - 1;
            }
            else if (_cursor > 0)
            {
                --_cursor;
            }

            _input->setText(_history.at(_cursor));
            _input->setCursorPosition(_input->text().size());
            return true;
        }

        if (keyEvent->key() == Qt::Key_Down)
        {
            if (_cursor < 0)
            {
                return true;
            }

            if (_cursor < _history.size() - 1)
            {
                ++_cursor;
                _input->setText(_history.at(_cursor));
            }
            else
            {
                _cursor = -1;
                _input->setText(_draft);
                _draft.clear();
            }

            _input->setCursorPosition(_input->text().size());
            return true;
        }

        return QObject::eventFilter(watched, event);
    }
}
