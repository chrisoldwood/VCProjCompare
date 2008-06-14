////////////////////////////////////////////////////////////////////////////////
//! \file   AppDlg.hpp
//! \brief  The AppDlg class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_APPDLG_HPP
#define APP_APPDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/MainDlg.hpp>
#include <WCL/CommonUI.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The application main dialog. This is the dialog that sits in the centre of
//! application window.

class AppDlg : public CMainDlg
{
public:
	//! Constructor.
	AppDlg();

	//! Destructor.
	virtual ~AppDlg();

	//
	// Controls
	//
	CRadioBtn	m_rbFolder;		//!< Radio button to signal folder comparison.
	CRadioBtn	m_rbFiles;		//!< Radio button to signal file comparison.
	CLabel		m_txtPath1;		//!< The first path type label.
	CEditBox	m_ebPath1;		//!< The edit box for the first path.
	CLabel		m_txtPath2;		//!< The second path type label.
	CEditBox	m_ebPath2;		//!< The edit box for the second path.
	CListView	m_lvResults;	//!< The list view for the results.

private:
	//
	// Message processors.
	//

	//! Handle dialog creation.
	virtual void OnInitDialog();

	//! The path browse button has been pressed.
	void OnBrowsePath1();

	//! The path browse button has been pressed.
	void OnBrowsePath2();

	//! The button to select comparison of all files has been pressed.
	void OnSelectCompareAll();

	//! The button to select comparison of two files has been pressed.
	void OnSelectCompareFiles();

	//! Run the comparison.
	void OnCompare();

	//
	// Internal methods.
	//

	//! Browse for a folder path.
	void BrowseForFolder(CEditBox& ebPath);

	//! Browse for a file path.
	void BrowseForFile(CEditBox& ebPath);
};

#endif // APP_APPDLG_HPP
