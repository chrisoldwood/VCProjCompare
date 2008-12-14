////////////////////////////////////////////////////////////////////////////////
//! \file   TheApp.cpp
//! \brief  The TheApp class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "TheApp.hpp"
#include <Core/StringUtils.hpp>
#include <WCL/BusyCursor.hpp>
#include <WCL/AppConfig.hpp>
#include <Core/ConfigurationException.hpp>
#include <limits>

////////////////////////////////////////////////////////////////////////////////
// Global variables.

//! The application singleton instance.
TheApp g_app;

////////////////////////////////////////////////////////////////////////////////
// Constants.

//! The configuration data publisher name.
const tchar* PUBLISHER = TXT("Chris Oldwood");
//! The configuration data application name.
const tchar* APPLICATION = TXT("VC++ Project Compare");
//! The configuration data format version.
const tchar* CONFIG_VERSION = TXT("1.0");

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
	m_strTitle = APPLICATION;

	try
	{
		// Load settings.
		loadConfig();
	}
	catch (const Core::Exception& e)
	{
		FatalMsg(TXT("Failed to configure the application:-\n\n%s"), e.What());
		return false;
	}
	
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
	try
	{
		// Save settings.
		saveConfig();
	}
	catch (const Core::Exception& e)
	{
		FatalMsg(TXT("Failed to save the application configuration:-\n\n%s"), e.What());
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Load the application settings.

void TheApp::loadConfig()
{
	CBusyCursor    busyCursor;
	WCL::AppConfig appConfig(PUBLISHER, APPLICATION);

	// Read the config data version.
	tstring version = appConfig.readString(appConfig.DEFAULT_SECTION, TXT("Version"), CONFIG_VERSION);

	if (version != CONFIG_VERSION)
		throw Core::ConfigurationException(Core::Fmt(TXT("The configuration data is incompatible - '%s'"), version.c_str()));

	// Read the UI settings.
	m_lastWndPos = appConfig.readValue<CRect>(TXT("UI"), TXT("MainWindow"), m_lastWndPos);
	m_lastFolder = appConfig.readString(TXT("UI"), TXT("LastFolder"), m_lastFolder);
	m_lastFile1  = appConfig.readString(TXT("UI"), TXT("LastFile1"),  m_lastFile1);
	m_lastFile2  = appConfig.readString(TXT("UI"), TXT("LastFile2"),  m_lastFile2);

	// Read the ignored settings list.
	const size_t max = std::numeric_limits<size_t>::max();

	for (size_t i = 0; i != max; ++i)
	{
		tstring entry = Core::Fmt(TXT("%u"), i);

		WCL::AppConfig::StringArray values;
		appConfig.readList(TXT("Project Dependent"), entry, TXT(""), values);

		if (values.size() != 2)
			break;

		m_projectDepList.insert(ToolSetting(values[0], values[1]));
	}

	// Read the build dependent settings list.
	for (size_t i = 0; i != max; ++i)
	{
		tstring entry = Core::Fmt(TXT("%u"), i);

		WCL::AppConfig::StringArray values;
		appConfig.readList(TXT("Build Dependent"), entry, TXT(""), values);

		if (values.size() != 2)
			break;

		m_buildDepList.insert(ToolSetting(values[0], values[1]));
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Save the application settings.

void TheApp::saveConfig()
{
	CBusyCursor    busyCursor;
	WCL::AppConfig appConfig(PUBLISHER, APPLICATION);

	// Write the config data version.
	appConfig.writeString(appConfig.DEFAULT_SECTION, TXT("Version"), CONFIG_VERSION);

	// Write the UI settings.
	appConfig.writeValue<CRect>(TXT("UI"), TXT("MainWindow"), m_lastWndPos);
	appConfig.writeString(TXT("UI"), TXT("LastFolder"), m_lastFolder);
	appConfig.writeString(TXT("UI"), TXT("LastFile1"),  m_lastFile1);
	appConfig.writeString(TXT("UI"), TXT("LastFile2"),  m_lastFile2);

	// Write the ignored settings list.
	if (g_app.m_modified & TheApp::PROJECT_LIST)
	{
		appConfig.deleteSection(TXT("Project Dependent"));

		size_t i = 0;

		for (ToolSettings::const_iterator it = m_projectDepList.begin(); it != m_projectDepList.end(); ++it, ++i)
		{
			const ToolSetting& toolSetting = *it;

			tstring entry = Core::Fmt(TXT("%u"), i);
			tstring value = toolSetting.m_tool + TXT(",") + toolSetting.m_setting;

			appConfig.writeString(TXT("Project Dependent"), entry, value);
		}
	}

	// Write the build dependent settings list.
	if (g_app.m_modified & TheApp::BUILDDEP_LIST)
	{
		appConfig.deleteSection(TXT("Build Dependent"));

		size_t i = 0;

		for (ToolSettings::const_iterator it = m_buildDepList.begin(); it != m_buildDepList.end(); ++it, ++i)
		{
			const ToolSetting& toolSetting = *it;

			tstring entry = Core::Fmt(TXT("%u"), i);
			tstring value = toolSetting.m_tool + TXT(",") + toolSetting.m_setting;

			appConfig.writeString(TXT("Build Dependent"), entry, value);
		}
	}
}
