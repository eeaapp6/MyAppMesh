/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef RESIDAULWIDGET_H
#define RESIDAULWIDGET_H


#include "GUICalculateDialogAPI.h"
#include "GUICalculateWidgetBase.h"

namespace Ui {
	class ResidaulWidget;
}

namespace EventOper {
	class ParaWidgetInterfaceOperator;
}

namespace Interface {
	class FITKPHengLEIResidaulProcess;
}

namespace GUI
{
	class GUICalculateDialogAPI ResidaulWidget : public GUICalculateWidgetBase
	{
		Q_OBJECT
	public:
		ResidaulWidget(EventOper::ParaWidgetInterfaceOperator* oper, QWidget* parent = nullptr);
		~ResidaulWidget();

	private slots:

		//void on_averagerRes_stateChanged(int arg1);

		//void on_maxRes_stateChanged(int arg1);

		//void on_maxResCoorX_stateChanged(int arg1);

		//void on_maxResCoorY_stateChanged(int arg1);

		//void on_maxResCoorZ_stateChanged(int arg1);

		void on_OK_clicked();

		void on_Cancel_clicked();

		//void on_Quit_clicked();

	private:
		Ui::ResidaulWidget *_ui=nullptr;
	};
}
#endif
