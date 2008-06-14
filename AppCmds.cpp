////////////////////////////////////////////////////////////////////////////////
//! \file   AppCmds.cpp
//! \brief  The AppCmds class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AppCmds.hpp"
#include "TheApp.hpp"
#include "AboutDlg.hpp"
//#include <WCL/BusyCursor.hpp>
#include "ProgressDlg.hpp"
#include <WCL/FolderIterator.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

AppCmds::AppCmds()
{
	// Define the command table.
	DEFINE_CMD_TABLE
		// File menu.
		CMD_ENTRY(ID_FILE_EXIT,		&AppCmds::OnFileExit,	NULL,	-1)
		// Help menu.
		CMD_ENTRY(ID_HELP_ABOUT,	&AppCmds::OnHelpAbout,	NULL,	 2)
	END_CMD_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

AppCmds::~AppCmds()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Close the application.

void AppCmds::OnFileExit()
{
	App.m_oAppWnd.Close();
}

////////////////////////////////////////////////////////////////////////////////
//! Compare the files.

void AppCmds::OnFileCompare()
{
	AppDlg& dlgMain = App.m_oAppWnd.m_oAppDlg;

	try
	{
		ProgressDlg dlgProgress(App.m_oAppWnd);	
		FileList	vecFiles;

		dlgProgress.UpdateProgressBar(0);
		dlgProgress.SetStatus(TXT("Searching for project files..."));

		// Comparing all files?
		if (dlgMain.m_rbFolder.IsChecked())
		{
			CPath strFolder = dlgMain.m_ebPath1.Text();

			FindProjectFiles(strFolder, vecFiles);

			// Remember the folder.
			App.m_strLastFolder = strFolder;
		}
		// Comparing just two files.
		else
		{
			CPath strFile1 = dlgMain.m_ebPath1.Text();
			CPath strFile2 = dlgMain.m_ebPath2.Text();

			vecFiles.push_back(tstring(strFile1));
			vecFiles.push_back(tstring(strFile2));

			// Remember the files.
			App.m_strLastFile1 = strFile1;
			App.m_strLastFile2 = strFile2;
		}
/*
		// Nothing to do?
		if (vecFiles.size() < 2)
		{
			App.NotifyMsg(TXT("Not enough files found for a comparison"));
			return;
		}
*/
		dlgProgress.UpdateProgressBar(1);
		dlgProgress.SetStatus(TXT("Reading project files..."));

		// Read in the project files.
		for (FileList::const_iterator it = vecFiles.begin(); it != vecFiles.end(); ++it)
			TRACE1(TXT("%s\n"), it->c_str());

		dlgProgress.UpdateProgressBar(2);
		dlgProgress.SetStatus(TXT("Comparing project files..."));

		// Do the comparison.

		dlgProgress.UpdateProgressBar(3);
	}
	catch(const Core::Exception& e)
	{
		App.FatalMsg(TXT("Failed to compare the project files:-\n\n%s"), e.What());
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Show the about dialog.

void AppCmds::OnHelpAbout()
{
	AboutDlg Dlg;

	Dlg.RunModal(App.m_oAppWnd);
}

////////////////////////////////////////////////////////////////////////////////
//! Find all project files.

void AppCmds::FindProjectFiles(const CPath& strFolder, FileList& vecFiles)
{
	WCL::FolderIterator end;

	for (WCL::FolderIterator it(tstring(strFolder), TXT("*.vcproj")); it != end; ++it)
		vecFiles.push_back(tstring(strFolder / *it));
}
