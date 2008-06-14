////////////////////////////////////////////////////////////////////////////////
//! \file   AppDlg.cpp
//! \brief  The AppDlg class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AppDlg.hpp"
#include <WCL/Path.hpp>
#include "TheApp.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

AppDlg::AppDlg()
	: CMainDlg(IDD_MAIN)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_FOLDER,	&m_rbFolder)
		CTRL(IDC_FILES,		&m_rbFiles)
		CTRL(IDC_LABEL_1,	&m_txtPath1)
		CTRL(IDC_PATH_1,	&m_ebPath1)
		CTRL(IDC_LABEL_2,	&m_txtPath2)
		CTRL(IDC_PATH_2,	&m_ebPath2)
		CTRL(IDC_RESULTS,	&m_lvResults)
	END_CTRL_TABLE

	DEFINE_GRAVITY_TABLE
		CTRLGRAV(IDC_FOLDER,   LEFT_EDGE,  TOP_EDGE, LEFT_EDGE,  TOP_EDGE)
		CTRLGRAV(IDC_FILES,    LEFT_EDGE,  TOP_EDGE, LEFT_EDGE,  TOP_EDGE)
		CTRLGRAV(IDC_LABEL_1,  LEFT_EDGE,  TOP_EDGE, LEFT_EDGE,  TOP_EDGE)
		CTRLGRAV(IDC_PATH_1,   LEFT_EDGE,  TOP_EDGE, RIGHT_EDGE, TOP_EDGE)
		CTRLGRAV(IDC_BROWSE_1, RIGHT_EDGE, TOP_EDGE, RIGHT_EDGE, TOP_EDGE)
		CTRLGRAV(IDC_LABEL_2,  LEFT_EDGE,  TOP_EDGE, LEFT_EDGE,  TOP_EDGE)
		CTRLGRAV(IDC_PATH_2,   LEFT_EDGE,  TOP_EDGE, RIGHT_EDGE, TOP_EDGE)
		CTRLGRAV(IDC_BROWSE_2, RIGHT_EDGE, TOP_EDGE, RIGHT_EDGE, TOP_EDGE)
		CTRLGRAV(IDC_RESULTS,  LEFT_EDGE,  TOP_EDGE, RIGHT_EDGE, BOTTOM_EDGE)
	END_GRAVITY_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_FOLDER,   BN_CLICKED, &AppDlg::OnSelectCompareAll)
		CMD_CTRLMSG(IDC_FILES,    BN_CLICKED, &AppDlg::OnSelectCompareFiles)
		CMD_CTRLMSG(IDC_BROWSE_1, BN_CLICKED, &AppDlg::OnBrowsePath1)
		CMD_CTRLMSG(IDC_BROWSE_2, BN_CLICKED, &AppDlg::OnBrowsePath2)
		CMD_CTRLMSG(IDC_COMPARE,  BN_CLICKED, &AppDlg::OnCompare)
	END_CTRLMSG_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

AppDlg::~AppDlg()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Handle dialog creation.

void AppDlg::OnInitDialog()
{
	// Initialise controls.
	m_rbFolder.Check(true);
	m_rbFiles.Check(false);
	OnSelectCompareAll();

	m_lvResults.InsertColumn(0, TXT("Message"), 100, LVCFMT_LEFT);
	m_lvResults.FullRowSelect(true);
}

////////////////////////////////////////////////////////////////////////////////
//! The path browse button has been pressed.

void AppDlg::OnBrowsePath1()
{
	if (m_rbFolder.IsChecked())
		BrowseForFolder(m_ebPath1);
	else
		BrowseForFile(m_ebPath1);
}

////////////////////////////////////////////////////////////////////////////////
//! The path browse button has been pressed.

void AppDlg::OnBrowsePath2()
{
	BrowseForFile(m_ebPath2);
}

////////////////////////////////////////////////////////////////////////////////
//! The button to select comparison of all files has been pressed.

void AppDlg::OnSelectCompareAll()
{
	// Manually fix-up the other radio button.
	m_rbFiles.Check(false);

	// Set the control labels.
	m_txtPath1.Text(TXT("F&older:"));
	m_txtPath2.Text(TXT("N/A:"));

	// Initialise the paths.
	m_ebPath1.Text(App.m_strLastFolder);
	m_ebPath2.Text(TXT(""));

	// Disable the second path controls.
	Control(IDC_LABEL_2).Enable(false);
	Control(IDC_PATH_2).Enable(false);
	Control(IDC_BROWSE_2).Enable(false);
}

////////////////////////////////////////////////////////////////////////////////
//! The button to select comparison of two files has been pressed.

void AppDlg::OnSelectCompareFiles()
{
	// Manually fix-up the other radio button.
	m_rbFolder.Check(false);

	// Set the control labels.
	m_txtPath1.Text(TXT("File &1:"));
	m_txtPath2.Text(TXT("File &2:"));

	// Initialise the paths.
	m_ebPath1.Text(App.m_strLastFile1);
	m_ebPath2.Text(App.m_strLastFile2);

	// Enable the second path controls.
	Control(IDC_LABEL_2).Enable(true);
	Control(IDC_PATH_2).Enable(true);
	Control(IDC_BROWSE_2).Enable(true);
}

////////////////////////////////////////////////////////////////////////////////
//! Run the comparison.

void AppDlg::OnCompare()
{
	App.m_oAppCmds.OnFileCompare();
}

////////////////////////////////////////////////////////////////////////////////
//! Browse for a folder path.

void AppDlg::BrowseForFolder(CEditBox& ebPath)
{
	// Use the current path as a default.
	CPath strInitialDir = ebPath.Text();

	// Use the application folder, if no default.
	if (strInitialDir.Empty())
		strInitialDir = CPath::ApplicationDir();

	CPath strPath;

	// Query the user.
	if (strPath.SelectDir(*this, TXT("Select the root folder of the .vcproj files"), strInitialDir))
		ebPath.Text(strPath);
}

////////////////////////////////////////////////////////////////////////////////
//! Browse for a file path.

void AppDlg::BrowseForFile(CEditBox& ebPath)
{
	static tchar szExts[] = {	TXT("Project Files (*.vcproj)\0*.vcproj\0")
								TXT("All Files (*.*)\0*.*\0")
								TXT("\0\0")							};

	// Use the current path as a default.
	CPath strInitialDir = ebPath.Text();

	if (strInitialDir.Exists() && !strInitialDir.IsFolder())
		strInitialDir = strInitialDir.Directory();

	// Use the application folder, if no default.
	if (strInitialDir.Empty())
		strInitialDir = CPath::ApplicationDir();

	CPath strPath;

	// Query the user.
	if (strPath.Select(*this, CPath::SelectFile, szExts, TXT("vcproj"), strInitialDir))
		ebPath.Text(strPath);
}
