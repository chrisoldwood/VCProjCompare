////////////////////////////////////////////////////////////////////////////////
//! \file   ProjectFile.hpp
//! \brief  The ProjectFile class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef PROJECTFILE_HPP
#define PROJECTFILE_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <XML/Document.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The structure used to hold the filename and XML DOM for a VC++ project file.

struct ProjectFile
{
	//
	// Members.
	//
	tstring				m_pathName;		//!< The project full path name.
	tstring				m_fileName;		//!< The project filename.
	XML::DocumentPtr	m_xmlDoc;		//!< The XML document DOM.

	//! Default constructor.
	ProjectFile(const tstring& pathName);
};

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

inline ProjectFile::ProjectFile(const tstring& pathName)
	: m_pathName(pathName)
	, m_fileName(CPath(pathName.c_str()).FileName())
	, m_xmlDoc()
{
}

#endif // PROJECTFILE_HPP
