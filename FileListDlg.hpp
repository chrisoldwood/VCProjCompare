////////////////////////////////////////////////////////////////////////////////
//! \file   FileListDlg.hpp
//! \brief  The FileListDlg class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef FILELISTDLG_HPP
#define FILELISTDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>
#include "ProjectCompare.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The dialog used to modify the list of project files to scan.

class FileListDlg : public CDialog
{
public:
	//! Default constructor.
	FileListDlg();
	
	//
	// Members.
	//
	FileList	m_files;			//!< The selected files.

private:
	//
	// Controls.
	//
	CListView	m_filesView;		//!< The file list view.

	//
	// Message handlers.
	//

	//! Dialog initialisation handler.
	virtual void OnInitDialog();

	//! OK button handler.
	virtual bool OnOk();

	//! Handle the "Deselect All" button.
	void onClickDeselectAll();
};

#endif // FILELISTDLG_HPP
