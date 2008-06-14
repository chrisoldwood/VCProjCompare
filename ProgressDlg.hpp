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
	ProgressDlg(CWnd& wndParent);
	
	//! Destructor.
	virtual ~ProgressDlg();
	
	//
	// Members.
	//

	//! Set the status message.
	void SetStatus(const tchar* pszStatus);

	//! Update the progress bar.
	void UpdateProgressBar(uint nPos);

private:
	//
	// Members.
	//
	CWnd&			m_wndParent;		//!< The parent window.
	
	//
	// Controls.
	//
	CLabel			m_txtLabel;			//!< The status label.
	CProgressBar	m_pbProgress;		//!< The progress bar.

	//
	// Message handlers.
	//

	//! Dialog initialisation handler.
	virtual void OnInitDialog();
};

#endif // PROGRESSDLG_HPP
