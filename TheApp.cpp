////////////////////////////////////////////////////////////////////////////////
//! \file   TheApp.cpp
//! \brief  The TheApp class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "TheApp.hpp"
#include <WCL/IniFile.hpp>

////////////////////////////////////////////////////////////////////////////////
// Global variables.

//! The application singleton instance.
TheApp App;

////////////////////////////////////////////////////////////////////////////////
// Constants.

//! The .ini file format version number.
const tchar* TheApp::INI_FILE_VER = TXT("1.0");

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

TheApp::TheApp()
	: CApp(m_oAppWnd, m_oAppCmds)
	, m_strLastFolder()
	, m_strLastFile1()
	, m_strLastFile2()
{

}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

TheApp::~TheApp()
{

}

////////////////////////////////////////////////////////////////////////////////
//! Handle application startup.

bool TheApp::OnOpen()
{
	// These cannot be initialised in the ctor.
	m_strLastFolder = CPath::ApplicationDir();
	m_strLastFile1 = CPath::ApplicationDir();
	m_strLastFile2 = CPath::ApplicationDir();

	// Set the app title.
	m_strTitle = TXT("Visual C++ Project Compare");

	// Load settings.
	LoadConfig();
	
	// Create the main window.
	if (!m_oAppWnd.Create())
		return false;

	// Show it.
	if (ShowNormal() && !m_rcLastPos.Empty())
		m_oAppWnd.Move(m_rcLastPos);

	m_oAppWnd.Show(m_iCmdShow);

	// Update UI.
	m_oAppCmds.UpdateUI();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Handle application shutdown.

bool TheApp::OnClose()
{
	// Save settings.
	SaveConfig();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Load the application settings.

void TheApp::LoadConfig()
{
	CIniFile oIniFile;

	// Read the file version.
	CString strVer = oIniFile.ReadString(TXT("Version"), TXT("Version"), INI_FILE_VER);

	// Read the UI settings.
	m_rcLastPos     = oIniFile.ReadRect  (TXT("UI"), TXT("MainWindow"), m_rcLastPos);
	m_strLastFolder = oIniFile.ReadString(TXT("UI"), TXT("LastFolder"), m_strLastFolder);
	m_strLastFile1  = oIniFile.ReadString(TXT("UI"), TXT("LastFile1"),  m_strLastFile1);
	m_strLastFile2  = oIniFile.ReadString(TXT("UI"), TXT("LastFile2"),  m_strLastFile2);
}

////////////////////////////////////////////////////////////////////////////////
//! Save the application settings.

void TheApp::SaveConfig()
{
	CIniFile oIniFile;

	// Write the file version.
	oIniFile.WriteString(TXT("Version"), TXT("Version"), INI_FILE_VER);

	// Write the UI settings.
	oIniFile.WriteRect  (TXT("UI"), TXT("MainWindow"), m_rcLastPos);
	oIniFile.WriteString(TXT("UI"), TXT("LastFolder"), m_strLastFolder);
	oIniFile.WriteString(TXT("UI"), TXT("LastFile1"),  m_strLastFile1);
	oIniFile.WriteString(TXT("UI"), TXT("LastFile2"),  m_strLastFile2);
}
