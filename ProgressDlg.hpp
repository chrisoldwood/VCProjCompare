////////////////////////////////////////////////////////////////////////////////
//! \file   ProgressDlg.hpp
//! \brief  The ProgressDlg class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef PROGRESSDLG_HPP
#define PROGRESSDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The dialog used to display the comparison progress.

class ProgressDlg : public CDialog
{
public:
	//! Default constructor.
	ProgressDlg(CWnd& parent);
	
	//! Destructor.
	virtual ~ProgressDlg();
	
	//
	// Members.
	//

	//! Set the status message.
	void setStatus(const tchar* status);

	//! Update the progress bar.
	void updateProgressBar(uint progress);

private:
	//
	// Members.
	//
	CWnd&			m_parent;			//!< The parent window.
	
	//
	// Controls.
	//
	CLabel			m_statusLabel;		//!< The status label.
	CProgressBar	m_progressBar;		//!< The progress bar.

	//
	// Message handlers.
	//

	//! Dialog initialisation handler.
	virtual void OnInitDialog();
};

#endif // PROGRESSDLG_HPP
