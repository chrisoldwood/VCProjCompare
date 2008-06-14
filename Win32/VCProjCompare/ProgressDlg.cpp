////////////////////////////////////////////////////////////////////////////////
//! \file   ProgressDlg.cpp
//! \brief  The ProgressDlg class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "ProgressDlg.hpp"
#include "TheApp.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

ProgressDlg::ProgressDlg(CWnd& wndParent)
	: CDialog(IDD_PROGRESS)
	, m_wndParent(wndParent)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_STATUS,	&m_txtLabel)
		CTRL(IDC_PROGRESS,	&m_pbProgress)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
	END_CTRLMSG_TABLE

	m_wndParent.Enable(false);
	RunModeless(wndParent);
	App.m_MainThread.ProcessMsgQueue();
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

ProgressDlg::~ProgressDlg()
{
	m_wndParent.Enable(true);
	Destroy();
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void ProgressDlg::OnInitDialog()
{
	// Initialise controls.
	m_txtLabel.Text(TXT("Initialising..."));
	m_pbProgress.Range(0, 3);

	// Display it.
	Centre();
	Show();
}

////////////////////////////////////////////////////////////////////////////////
//! Set the status message.

void ProgressDlg::SetStatus(const tchar* pszStatus)
{
	m_txtLabel.Text(pszStatus);
	m_txtLabel.Update();
}

////////////////////////////////////////////////////////////////////////////////
//! Update the progress bar.

void ProgressDlg::UpdateProgressBar(uint nPos)
{
	m_pbProgress.Pos(nPos);
	m_pbProgress.Update();
}
