////////////////////////////////////////////////////////////////////////////////
//! \file   AppDlg.cpp
//! \brief  The AppDlg class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AppDlg.hpp"
#include <WCL/Path.hpp>
#include "TheApp.hpp"
#include <WCL/ScreenDC.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

AppDlg::AppDlg()
	: CMainDlg(IDD_MAIN)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_FOLDER,	&m_folderButton)
		CTRL(IDC_FILES,		&m_filesButton)
		CTRL(IDC_LABEL_1,	&m_path1Label)
		CTRL(IDC_PATH_1,	&m_path1Editor)
		CTRL(IDC_LABEL_2,	&m_path2Label)
		CTRL(IDC_PATH_2,	&m_path2Editor)
		CTRL(IDC_RESULTS,	&m_resultsView)
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
		CTRLGRAV(IDC_HEADING,  LEFT_EDGE,  TOP_EDGE, RIGHT_EDGE, TOP_EDGE)
		CTRLGRAV(IDC_RESULTS,  LEFT_EDGE,  TOP_EDGE, RIGHT_EDGE, BOTTOM_EDGE)
	END_GRAVITY_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_FOLDER,   BN_CLICKED, &AppDlg::onFolderComparison)
		CMD_CTRLMSG(IDC_FILES,    BN_CLICKED, &AppDlg::onFileComparison)
		CMD_CTRLMSG(IDC_BROWSE_1, BN_CLICKED, &AppDlg::onBrowsePath1)
		CMD_CTRLMSG(IDC_BROWSE_2, BN_CLICKED, &AppDlg::onBrowsePath2)
		NFY_CTRLMSG(IDC_RESULTS,  NM_RCLICK,  &AppDlg::onRightClickResult)
	END_CTRLMSG_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

AppDlg::~AppDlg()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Query if the user wants to scan folders.

bool AppDlg::isFolderScanSelected() const
{
	return m_folderButton.IsChecked();
}

////////////////////////////////////////////////////////////////////////////////
//! Get the 1st path.

tstring AppDlg::getPath1() const
{
	return tstring(m_path1Editor.Text());
}

////////////////////////////////////////////////////////////////////////////////
//! Get the 2nd path.

tstring AppDlg::getPath2() const
{
	return tstring(m_path2Editor.Text());
}

////////////////////////////////////////////////////////////////////////////////
//! Handle dialog creation.

void AppDlg::OnInitDialog()
{
//	m_resultsFont.Create(CLogFont(TXT("Lucida Console"), -CScreenDC().PointSizeToPixels(8)));

	// Initialise controls.
	m_folderButton.Check(true);
	m_filesButton.Check(false);
	onFolderComparison();

//	m_resultsView.Font(m_resultsFont);
	m_resultsView.ImageList(LVSIL_SMALL, IDB_RESICONS, 16, RGB(255, 0, 255));
	m_resultsView.InsertColumn(0, TXT("Tool"),    m_resultsView.StringWidth(20), LVCFMT_LEFT);
	m_resultsView.InsertColumn(1, TXT("Setting"), m_resultsView.StringWidth(25), LVCFMT_LEFT);
	m_resultsView.InsertColumn(2, TXT("Build"),   m_resultsView.StringWidth(15), LVCFMT_LEFT);
	m_resultsView.InsertColumn(3, TXT("Project"), m_resultsView.StringWidth(25), LVCFMT_LEFT);
	m_resultsView.InsertColumn(4, TXT("Path"),    m_resultsView.StringWidth(25), LVCFMT_LEFT);
	m_resultsView.InsertColumn(5, TXT("Value"),   m_resultsView.StringWidth(30), LVCFMT_LEFT);
	m_resultsView.FullRowSelect(true);
}

////////////////////////////////////////////////////////////////////////////////
//! Browse for the 1st path.

void AppDlg::onBrowsePath1()
{
	if (m_folderButton.IsChecked())
		browseForFolder(m_path1Editor);
	else
		browseForFile(m_path1Editor);
}

////////////////////////////////////////////////////////////////////////////////
//! Browse for the 2nd path.

void AppDlg::onBrowsePath2()
{
	browseForFile(m_path2Editor);
}

////////////////////////////////////////////////////////////////////////////////
//! Switch to comparing files in a folder.

void AppDlg::onFolderComparison()
{
	// Manually fix-up the other radio button.
	m_filesButton.Check(false);

	// Set the control labels.
	m_path1Label.Text(TXT("F&older:"));
	m_path2Label.Text(TXT("N/A:"));

	// Initialise the paths.
	m_path1Editor.Text(g_app.m_lastFolder);
	m_path2Editor.Text(TXT(""));

	// Disable the second path controls.
	Control(IDC_LABEL_2).Enable(false);
	Control(IDC_PATH_2).Enable(false);
	Control(IDC_BROWSE_2).Enable(false);
}

////////////////////////////////////////////////////////////////////////////////
//! Switch to comparing specifc files.

void AppDlg::onFileComparison()
{
	// Manually fix-up the other radio button.
	m_folderButton.Check(false);

	// Set the control labels.
	m_path1Label.Text(TXT("File &1:"));
	m_path2Label.Text(TXT("File &2:"));

	// Initialise the paths.
	m_path1Editor.Text(g_app.m_lastFile1);
	m_path2Editor.Text(g_app.m_lastFile2);

	// Enable the second path controls.
	Control(IDC_LABEL_2).Enable(true);
	Control(IDC_PATH_2).Enable(true);
	Control(IDC_BROWSE_2).Enable(true);
}

////////////////////////////////////////////////////////////////////////////////
//! Show a context menu for the selected result.

LRESULT AppDlg::onRightClickResult(NMHDR& /*event*/)
{
	// Only show menu, if a selection.
	if (m_resultsView.IsSelection())
	{
		ToolSetting toolSetting;

		// Get the selected setting.
		size_t selection      = m_resultsView.Selection();
		toolSetting.m_tool    = m_resultsView.ItemText(selection, 0);
		toolSetting.m_setting = m_resultsView.ItemText(selection, 1);

		// Check setting status.
		bool ignored  = (g_app.m_ignoreList.find(toolSetting) != g_app.m_ignoreList.end());
		bool buildDep = (g_app.m_buildDepList.find(toolSetting) != g_app.m_buildDepList.end());

		CPopupMenu menu(IDR_CONTEXT);

		// Enable/Disable relevant commands.
		menu.EnableCmd(ID_CONTEXT_IGNORE, !ignored && !buildDep);
		menu.EnableCmd(ID_CONTEXT_BUILD,  !ignored && !buildDep);

		// Show context menu.
		const MSG& msg = g_app.m_MainThread.CurrentMsg();

		uint cmd = menu.TrackMenu(m_resultsView, msg.pt);

		// Add to ignored settings list?
		if (cmd == ID_CONTEXT_IGNORE)
		{
			g_app.m_ignoreList.insert(toolSetting);
			g_app.m_modified |= TheApp::IGNORE_LIST;
		}
		// Add to build dependent settings list?
		else if (cmd == ID_CONTEXT_BUILD)
		{
			g_app.m_buildDepList.insert(toolSetting);
			g_app.m_modified |= TheApp::BUILDDEP_LIST;
		}
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//! Browse for a folder path.

void AppDlg::browseForFolder(CEditBox& pathEditor)
{
	// Use the current path as a default.
	CPath initialFolder = pathEditor.Text();

	// Use the application folder, if no default.
	if (initialFolder.Empty())
		initialFolder = CPath::ApplicationDir();

	CPath path;

	// Query the user.
	if (path.SelectDir(*this, TXT("Select the root folder of the .vcproj files"), initialFolder))
		pathEditor.Text(path);
}

////////////////////////////////////////////////////////////////////////////////
//! Browse for a file path.

void AppDlg::browseForFile(CEditBox& pathEditor)
{
	static tchar s_exts[] = {	TXT("Project Files (*.vcproj)\0*.vcproj\0")
								TXT("All Files (*.*)\0*.*\0")
								TXT("\0\0")							};

	// Use the current path as a default.
	CPath initialFolder = pathEditor.Text();

	if (initialFolder.Exists() && !initialFolder.IsFolder())
		initialFolder = initialFolder.Directory();

	// Use the application folder, if no default.
	if (initialFolder.Empty())
		initialFolder = CPath::ApplicationDir();

	CPath path;

	// Query the user.
	if (path.Select(*this, CPath::SelectFile, s_exts, TXT("vcproj"), initialFolder))
		pathEditor.Text(path);
}

////////////////////////////////////////////////////////////////////////////////
//! Display the results list.

void AppDlg::displayResults(Table& results)
{
	m_resultsView.DeleteAllItems();

	// Load the table rows.
	for (size_t i = 0; i < results.size(); ++i)
	{
		RowPtr row = results[i];

		m_resultsView.insertItem (i,    row->at(0));
		m_resultsView.setItemText(i, 1, row->at(1));
		m_resultsView.setItemText(i, 2, row->at(2));
		m_resultsView.setItemText(i, 3, row->at(3));
		m_resultsView.setItemText(i, 4, row->at(4));
		m_resultsView.setItemText(i, 5, row->at(5));
	}
}
