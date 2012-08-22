////////////////////////////////////////////////////////////////////////////////
//! \file   AppCmds.hpp
//! \brief  The AppCmds class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_APPCMDS_HPP
#define APP_APPCMDS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CmdCtrl.hpp>
#include <XML/Document.hpp>
#include "ProjectCompare.hpp"

// Forward declarations.
class AppWnd;

////////////////////////////////////////////////////////////////////////////////
//! The application command controller.

class AppCmds : public CCmdControl
{
public:
	//! Constructor.
	AppCmds(AppWnd& appWnd);

	//! Destructor.
	virtual ~AppCmds();

	//
	// Commands.
	//

	//! List the raw project settings.
	void onFileList();

	//! Compare the files for differences.
	void onFileCompare();

	//! Rescan the project files.
	void onFileRefresh();

	//! Close the application.
	void onFileExit();

	//! Show the application settings dialog.
	void onToolsOptions();

	//! Show the HelpFile.
	void onHelpContents();

	//! Show the about dialog.
	void onHelpAbout();

	//
	// UI handlers.
	//

private:
	//
	// Internal methods.
	//

	//! Find and parse the project files into an internal form.
	static bool doFindAndParseFiles(Projects& projects, ProjectSettings& settings);
};

#endif // APP_APPCMDS_HPP
