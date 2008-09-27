////////////////////////////////////////////////////////////////////////////////
//! \file   FileListDlg.cpp
//! \brief  The FileListDlg class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "FileListDlg.hpp"
#include "Resource.h"

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

FileListDlg::FileListDlg()
	: CDialog(IDD_FILELIST)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_FILES, &m_filesView)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_NONE, BN_CLICKED, &FileListDlg::onClickDeselectAll)
	END_CTRLMSG_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void FileListDlg::OnInitDialog()
{
	// Initialise controls.
	m_filesView.InsertColumn(0, TXT("Filename"), m_filesView.StringWidth(70), LVCFMT_LEFT);
	m_filesView.CheckBoxes(true);

	// Populate controls.
	for (FileList::const_iterator it = m_files.begin(); it != m_files.end(); ++it)
	{
		int i = m_filesView.AppendItem(it->c_str());
		m_filesView.SetChecked(i, true);
	}
}

////////////////////////////////////////////////////////////////////////////////
//! OK button handler.

bool FileListDlg::OnOk()
{
	FileList files;

	// Copy only the selected filenames to the output list.
	for (size_t i = 0; i < m_files.size(); ++i)
	{
		if (m_filesView.IsChecked(i))
			files.push_back(m_files[i]);
	}

	// Validate file list.
	if (files.empty())
	{
		AlertMsg(TXT("No project files have been selected"));
		return false;
	}

	m_files = files;

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Handle the "Deselect All" button.

void FileListDlg::onClickDeselectAll()
{
	size_t count = m_filesView.ItemCount();

	for (size_t i = 0; i < count; ++i)
		m_filesView.SetChecked(i, false);
}
