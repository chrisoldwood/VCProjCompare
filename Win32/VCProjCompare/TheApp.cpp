////////////////////////////////////////////////////////////////////////////////
//! \file   TheApp.cpp
//! \brief  The TheApp class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "TheApp.hpp"
#include <WCL/IniFile.hpp>
#include <Core/StringUtils.hpp>
#include <WCL/StrTok.hpp>
#include <WCL/StrArray.hpp>
#include <WCL/BusyCursor.hpp>

////////////////////////////////////////////////////////////////////////////////
// Global variables.

//! The application singleton instance.
TheApp g_app;

////////////////////////////////////////////////////////////////////////////////
// Constants.

//! The .ini file format version number.
const tchar* INI_FILE_VER = TXT("1.0");

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

TheApp::TheApp()
	: CApp(m_appWnd, m_appCmds)
	, m_modified(0)
	, m_lastFolder()
	, m_lastFile1()
	, m_lastFile2()
	, m_action(LIST_SETTINGS)
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
	m_lastFolder = CPath::ApplicationDir();
	m_lastFile1 = CPath::ApplicationDir();
	m_lastFile2 = CPath::ApplicationDir();

	// Set the app title.
	m_strTitle = TXT("Visual C++ Project Compare");

	// Load settings.
	loadConfig();
	
	// Create the main window.
	if (!m_appWnd.Create())
		return false;

	// Show it.
	if (ShowNormal() && !m_lastWndPos.Empty())
		m_appWnd.Move(m_lastWndPos);

	m_appWnd.Show(m_iCmdShow);

	// Update UI.
	m_appCmds.UpdateUI();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Handle application shutdown.

bool TheApp::OnClose()
{
	// Save settings.
	saveConfig();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Load the application settings.

void TheApp::loadConfig()
{
	CIniFile    iniFile;
	CBusyCursor busyCursor;

	// Read the file version.
	tstring version = iniFile.ReadString(TXT("Version"), TXT("Version"), INI_FILE_VER);

	// Read the UI settings.
	m_lastWndPos = iniFile.ReadRect  (TXT("UI"), TXT("MainWindow"), m_lastWndPos);
	m_lastFolder = iniFile.ReadString(TXT("UI"), TXT("LastFolder"), m_lastFolder);
	m_lastFile1  = iniFile.ReadString(TXT("UI"), TXT("LastFile1"),  m_lastFile1);
	m_lastFile2  = iniFile.ReadString(TXT("UI"), TXT("LastFile2"),  m_lastFile2);

	// Read the ignored settings list.
	size_t count = iniFile.ReadUInt(TXT("Ignore"), TXT("Count"), 0);

	for (size_t i = 0; i != count; ++i)
	{
		tstring entry = Core::Fmt(TXT("Item[%u]"), i);
		tstring value = iniFile.ReadString(TXT("Ignore"), entry.c_str(), tstring());

		CStrArray fields;

		if (CStrTok::Split(value.c_str(), TXT(','), fields) != 2)
			continue;

		m_ignoreList.insert(ToolSetting(tstring(fields[0]), tstring(fields[1])));
	}

	// Read the build dependent settings list.
	count = iniFile.ReadUInt(TXT("Build Dependent"), TXT("Count"), 0);

	for (size_t i = 0; i != count; ++i)
	{
		tstring entry = Core::Fmt(TXT("Item[%u]"), i);
		tstring value = iniFile.ReadString(TXT("Build Dependent"), entry.c_str(), tstring());

		CStrArray fields;

		if (CStrTok::Split(value.c_str(), TXT(','), fields) != 2)
			continue;

		m_buildDepList.insert(ToolSetting(tstring(fields[0]), tstring(fields[1])));
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Save the application settings.

void TheApp::saveConfig()
{
	CIniFile    iniFile;
	CBusyCursor busyCursor;

	// Write the file version.
	iniFile.WriteString(TXT("Version"), TXT("Version"), INI_FILE_VER);

	// Write the UI settings.
	iniFile.WriteRect  (TXT("UI"), TXT("MainWindow"), m_lastWndPos);
	iniFile.WriteString(TXT("UI"), TXT("LastFolder"), m_lastFolder);
	iniFile.WriteString(TXT("UI"), TXT("LastFile1"),  m_lastFile1);
	iniFile.WriteString(TXT("UI"), TXT("LastFile2"),  m_lastFile2);

	// Write the ignored settings list.
	if (g_app.m_modified & TheApp::IGNORE_LIST)
	{
		iniFile.WriteUInt(TXT("Ignore"), TXT("Count"), m_ignoreList.size());

		size_t i = 0;

		for (ToolSettings::const_iterator it = m_ignoreList.begin(); it != m_ignoreList.end(); ++it, ++i)
		{
			const ToolSetting& toolSetting = *it;

			tstring entry = Core::Fmt(TXT("Item[%u]"), i);
			tstring value = toolSetting.m_tool + TXT(",") + toolSetting.m_setting;

			iniFile.WriteString(TXT("Ignore"), entry.c_str(), value.c_str());
		}
	}

	// Write the build dependent settings list.
	if (g_app.m_modified & TheApp::BUILDDEP_LIST)
	{
		iniFile.WriteUInt(TXT("Build Dependent"), TXT("Count"), m_buildDepList.size());

		size_t i = 0;

		for (ToolSettings::const_iterator it = m_buildDepList.begin(); it != m_buildDepList.end(); ++it, ++i)
		{
			const ToolSetting& toolSetting = *it;

			tstring entry = Core::Fmt(TXT("Item[%u]"), i);
			tstring value = toolSetting.m_tool + TXT(",") + toolSetting.m_setting;

			iniFile.WriteString(TXT("Build Dependent"), entry.c_str(), value.c_str());
		}
	}
}
