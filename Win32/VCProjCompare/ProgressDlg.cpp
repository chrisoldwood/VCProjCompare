////////////////////////////////////////////////////////////////////////////////
//! \file   ProgressDlg.cpp
//! \brief  The ProgressDlg class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "ProgressDlg.hpp"
#include "TheApp.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

ProgressDlg::ProgressDlg(CWnd& parent)
	: CDialog(IDD_PROGRESS)
	, m_parent(parent)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_STATUS,	&m_statusLabel)
		CTRL(IDC_PROGRESS,	&m_progressBar)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
	END_CTRLMSG_TABLE

	// Create dialog.
	m_parent.Enable(false);
	RunModeless(parent);
	g_app.m_MainThread.ProcessMsgQueue();
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

ProgressDlg::~ProgressDlg()
{
	// Destroy dialog.
	m_parent.Enable(true);
	Destroy();
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void ProgressDlg::OnInitDialog()
{
	// Initialise controls.
	m_statusLabel.Text(TXT("Initialising..."));
	m_progressBar.Range(0, 5);

	// Display it.
	Centre();
	Show();
}

////////////////////////////////////////////////////////////////////////////////
//! Set the status message.

void ProgressDlg::setStatus(const tchar* status)
{
	m_statusLabel.Text(status);
	m_statusLabel.Update();
}

////////////////////////////////////////////////////////////////////////////////
//! Update the progress bar.

void ProgressDlg::updateProgressBar(uint progress)
{
	m_progressBar.Pos(progress);
	m_progressBar.Update();
}
