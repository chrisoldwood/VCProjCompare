////////////////////////////////////////////////////////////////////////////////
//! \file   AppCmds.cpp
//! \brief  The AppCmds class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AppCmds.hpp"
#include "TheApp.hpp"
#include "AboutDlg.hpp"
#include "ProgressDlg.hpp"
#include "FileListDlg.hpp"
#include <WCL/BusyCursor.hpp>
#include "OptionsDlg.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

AppCmds::AppCmds()
{
	// Define the command table.
	DEFINE_CMD_TABLE
		// File menu.
		CMD_ENTRY(ID_FILE_LIST,		&AppCmds::onFileList,		NULL,	-1)
		CMD_ENTRY(ID_FILE_COMPARE,	&AppCmds::onFileCompare,	NULL,	-1)
		CMD_ENTRY(ID_FILE_REFRESH,	&AppCmds::onFileRefresh,	NULL,	-1)
		CMD_ENTRY(ID_FILE_EXIT,		&AppCmds::onFileExit,		NULL,	-1)
		// Tools menu.
		CMD_ENTRY(ID_TOOLS_OPTIONS,	&AppCmds::onToolsOptions,	NULL,	-1)
		// Help menu.
		CMD_ENTRY(ID_HELP_CONTENTS,	&AppCmds::onHelpContents,	NULL,	-1)
		CMD_ENTRY(ID_HELP_ABOUT,	&AppCmds::onHelpAbout,		NULL,	-1)
	END_CMD_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

AppCmds::~AppCmds()
{
}

////////////////////////////////////////////////////////////////////////////////
//! List the raw project settings.

void AppCmds::onFileList()
{
	CBusyCursor busyCursor;

	// Clear all existing data.
	g_app.m_projects.clear();
	g_app.m_settings.clear();
	g_app.m_results.clear();
	g_app.m_action = TheApp::LIST_SETTINGS;

	// Generate the project settings data.
	if (!doFindAndParseFiles(g_app.m_projects, g_app.m_settings))
		return;

	// Display the raw results.
	listSettings(g_app.m_projects, g_app.m_settings, g_app.m_results);

	g_app.m_appWnd.m_mainView.displayResults(g_app.m_results);
}

////////////////////////////////////////////////////////////////////////////////
//! Compare the files for differences.

void AppCmds::onFileCompare()
{
	CBusyCursor busyCursor;

	// Clear all existing data.
	g_app.m_projects.clear();
	g_app.m_settings.clear();
	g_app.m_results.clear();
	g_app.m_action = TheApp::COMPARE_SETTINGS;

	// Generate the project settings data.
	if (!doFindAndParseFiles(g_app.m_projects, g_app.m_settings))
		return;

	// Compare and display the results.
	compareSettings(g_app.m_projects, g_app.m_settings, g_app.m_results);

	g_app.m_appWnd.m_mainView.displayResults(g_app.m_results);
}

////////////////////////////////////////////////////////////////////////////////
//! Rescan the project files.

void AppCmds::onFileRefresh()
{
	CBusyCursor busyCursor;

	// Clear existing result data.
	g_app.m_settings.clear();
	g_app.m_results.clear();

	// Generate the project settings data.
	if (!doFindAndParseFiles(g_app.m_projects, g_app.m_settings))
		return;

	// Process the results according to the previous action.
	if (g_app.m_action == TheApp::LIST_SETTINGS)
	{
		listSettings(g_app.m_projects, g_app.m_settings, g_app.m_results);
	}
	else // (g_app.m_action == TheApp::COMPARE_SETTINGS)
	{
		compareSettings(g_app.m_projects, g_app.m_settings, g_app.m_results);
	}

	g_app.m_appWnd.m_mainView.displayResults(g_app.m_results);
}

////////////////////////////////////////////////////////////////////////////////
//! Close the application.

void AppCmds::onFileExit()
{
	g_app.m_appWnd.Close();
}

////////////////////////////////////////////////////////////////////////////////
//! Show the application settings dialog.

void AppCmds::onToolsOptions()
{
	OptionsDlg options;

	options.RunModal(g_app.m_appWnd);
}

////////////////////////////////////////////////////////////////////////////////
//! Show the HelpFile.

void AppCmds::onHelpContents()
{
	CBusyCursor busyCursor;

	::ShellExecute(NULL, NULL, CPath::ApplicationDir() / TXT("VCProjCompare.mht"), NULL, NULL, SW_SHOW);
}

////////////////////////////////////////////////////////////////////////////////
//! Show the about dialog.

void AppCmds::onHelpAbout()
{
	AboutDlg dialog;

	dialog.RunModal(g_app.m_appWnd);
}

////////////////////////////////////////////////////////////////////////////////
//! Find and parse the project files into an internal form.

bool AppCmds::doFindAndParseFiles(Projects& projects, ProjectSettings& settings)
{
	AppDlg& view = g_app.m_appWnd.m_mainView;

	// Initialise progress UI.
	ProgressDlg progress(g_app.m_appWnd);	

	progress.updateProgressBar(0);
	progress.setStatus(TXT("Searching for project files..."));

	// Not doing a rescan?
	if (projects.empty())
	{
		// Comparing all files?
		if (view.isFolderScanSelected())
		{
			tstring  folder = view.getPath1();
			FileList files;

			// Search folder for all project files.
			if (!findProjectFiles(folder, files))
				return false;

			if (files.empty())
			{
				g_app.AlertMsg(TXT("No project files were found"));
				return false;
			}

			// Allow user to edit the file list.
			FileListDlg fileListDlg;

			fileListDlg.m_files = files;

			if (fileListDlg.RunModal(g_app.m_appWnd) != IDOK)
				return false;

			// Create project file list.
			for (FileList::const_iterator it = fileListDlg.m_files.begin(); it != fileListDlg.m_files.end(); ++it)
				projects.push_back(ProjectFile(*it));

			// Remember the folder.
			g_app.m_lastFolder = folder;
		}
		// Comparing just two specific files.
		else
		{
			tstring file1 = view.getPath1();
			tstring file2 = view.getPath2();

			// Create project file list.
			projects.push_back(ProjectFile(file1));
			projects.push_back(ProjectFile(file2));

			// Remember the files.
			g_app.m_lastFile1 = file1;
			g_app.m_lastFile2 = file2;
		}
	}

	progress.updateProgressBar(2);
	progress.setStatus(TXT("Reading project files..."));

	// Load the project files.
	if (!readProjectFiles(projects))
		return false;

	progress.updateProgressBar(4);
	progress.setStatus(TXT("Parsing project files..."));

	// Parse the project files.
	if (!parseProjectFiles(projects, settings))
		return false;

	progress.updateProgressBar(5);

	return true;
}
