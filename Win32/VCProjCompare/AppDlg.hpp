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
#include "ProjectCompare.hpp"

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
	// Methods.
	//

	//! Query if the user wants to scan folders.
	bool isFolderScanSelected() const;

	//! Get the 1st path.
	tstring getPath1() const;

	//! Get the 2nd path.
	tstring getPath2() const;

	//! Display the results list.
	void displayResults(Table& results);

private:
	//
	// Controls
	//
	CRadioBtn	m_folderButton;	//!< Radio button to signal folder comparison.
	CRadioBtn	m_filesButton;	//!< Radio button to signal file comparison.
	CLabel		m_path1Label;	//!< The first path type label.
	CEditBox	m_path1Editor;	//!< The edit box for the first path.
	CLabel		m_path2Label;	//!< The second path type label.
	CEditBox	m_path2Editor;	//!< The edit box for the second path.
	CListView	m_resultsView;	//!< The list view for the results.
	CFont		m_resultsFont;	//!< The font to use for the results view.

	//! The results columns.
	enum Column
	{
		TOOL,
		SETTING,
		BUILD,
		PROJECT,
		PATH,
		VALUE,

		NUM_COLUMNS,
	};

	//
	// Message processors.
	//

	//! Handle dialog creation.
	virtual void OnInitDialog();

	//! Browse for the 1st path.
	void onBrowsePath1();

	//! Browse for the 2nd path.
	void onBrowsePath2();

	//! Switch to comparing files in a folder.
	void onFolderComparison();

	//! Switch to comparing specifc files.
	void onFileComparison();

	//! Show a context menu for the selected result.
	LRESULT onRightClickResult(NMHDR& event);

	//
	// Internal methods.
	//

	//! Browse for a folder path.
	void browseForFolder(CEditBox& pathEditor);

	//! Browse for a file path.
	void browseForFile(CEditBox& pathEditor);
};

#endif // APP_APPDLG_HPP
