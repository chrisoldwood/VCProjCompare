////////////////////////////////////////////////////////////////////////////////
//! \file   TheApp.hpp
//! \brief  The TheApp class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_THEAPP_HPP
#define APP_THEAPP_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/App.hpp>
#include "AppWnd.hpp"
#include "AppCmds.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The application singleton.

class TheApp : public CApp
{
public:
	//
	// Types.
	//

	//! The modified settings flags.
	enum Settings
	{
		PROJECT_LIST	= 0x0001,	//!< The list of project dependent settings.
		BUILDDEP_LIST	= 0x0002,	//!< The list of build dependent settings.
	};

	//! The type of action.
	enum Action
	{
		LIST_SETTINGS		= 1,	//!< List the project settings.
		COMPARE_SETTINGS	= 2,	//!< Compare the project settings.
	};

public:
	//! Constructor.
	TheApp();

	//! Destructor.
	virtual ~TheApp();

	//
	// Public members.
	//
	AppWnd			m_appWnd;			//!< The main window.
	AppCmds			m_appCmds;			//!< The command handler.

	//
	// Application settings.
	//
	uint			m_modified;			//!< Bitmask of modified settings.
	CRect			m_lastWndPos;		//!< Main window previous position.
	tstring			m_lastFolder;		//!< The last folder to be compared.
	tstring			m_lastFile1;		//!< The last file to be compared.
	tstring			m_lastFile2;		//!< The last file to be compared
	ToolSettings	m_projectDepList;	//!< The settings that are project dependent.
	ToolSettings	m_buildDepList;		//!< The settings that are build dependent.

	//
	// Application Data.
	//
	Action			m_action;			//!< The last action performed.
	Projects        m_projects;			//!< The project files scanned.
	ProjectSettings m_settings;			//!< The parsed project files.
	Table			m_results;			//!< The comparison results.

private:
	//
	// Template methods.
	//

	//! Handle application startup.
	virtual	bool OnOpen();

	//! Handle application shutdown.
	virtual	bool OnClose();

	//
	// Internal methods.
	//

	//! Load the application settings.
	void loadConfig();

	//! Save the application settings.
	void saveConfig();
};

//! The application singleton instance.
extern TheApp g_app;

#endif // APP_THEAPP_HPP
