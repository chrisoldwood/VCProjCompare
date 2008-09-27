////////////////////////////////////////////////////////////////////////////////
//! \file   AppWnd.cpp
//! \brief  The AppWnd class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AppWnd.hpp"
#include "TheApp.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

AppWnd::AppWnd()
	: CDlgFrame(IDR_APPICON, m_mainView, false)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

AppWnd::~AppWnd()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Handle window creation.

void AppWnd::OnCreate(const CRect& clientRect)
{
	//
	// Create and attach the components.
	//
	m_accelTable.LoadRsc(IDR_APPACCEL);
	Accel(&m_accelTable);

	m_menu.LoadRsc(IDR_APPMENU);
	Menu(&m_menu);

	m_statusBar.Create(*this, IDC_STATUS_BAR, clientRect);
	StatusBar(&m_statusBar);

	m_mainView.RunModeless(*this);
	ActiveDlg(&m_mainView);

	// Call base class.
	CDlgFrame::OnCreate(clientRect);
}

////////////////////////////////////////////////////////////////////////////////
//! Handle the window closing.

void AppWnd::OnClose()
{
	// Fetch windows final placement.
	g_app.m_lastWndPos = Placement();
}
