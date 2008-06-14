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

// Forward declarations.
class CPath;

////////////////////////////////////////////////////////////////////////////////
//! The application command controller.

class AppCmds : public CCmdControl
{
public:
	//! Constructor.
	AppCmds();

	//! Destructor.
	virtual ~AppCmds();

	//
	// Commands.
	//

	//! Close the application.
	void OnFileExit();

	//! Compare the files.
	void OnFileCompare();

	//! Show the about dialog.
	void OnHelpAbout();

	//
	// UI handlers.
	//

private:
	//! The collection of filenames.
	typedef std::vector<tstring> FileList;

	//
	// Internal methods.
	//

	//! Find all project files.
	void FindProjectFiles(const CPath& strFolder, FileList& vecFiles);
};

#endif // APP_APPCMDS_HPP
