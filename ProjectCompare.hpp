////////////////////////////////////////////////////////////////////////////////
//! \file   ProjectCompare.hpp
//! \brief  The functions used to perform the project comparison.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef PROJECTCOMPARE_HPP
#define PROJECTCOMPARE_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/Path.hpp>
#include "ProjectFile.hpp"
#include "ToolSetting.hpp"
#include <set>

////////////////////////////////////////////////////////////////////////////////
// Type aliases.

//! A collection of filenames.
typedef std::vector<tstring> FileList;
//! A collection of vc++ projects.
typedef std::vector<ProjectFile> Projects;

//! The collection of project values.
typedef std::vector<tstring> Values;
//! The map of configuration name to project values.
typedef std::map<tstring, Values> ConfigValues;
//! The map of setting name to configurations.
typedef std::map<tstring, ConfigValues> ConfigSettings;
//! The map of tool name to settings.
typedef std::map<tstring, ConfigSettings> ProjectSettings;

//! The row of project setting values.
typedef std::vector<tstring> Row;
//! The row smart-pointer type.
typedef Core::SharedPtr<Row> RowPtr;
//! The table of project settings.
typedef std::vector<RowPtr> Table;

//! A set of tool settings.
typedef std::set<ToolSetting> ToolSettings;

////////////////////////////////////////////////////////////////////////////////
// Functions.

// Find project files in a folder and its children.
bool findProjectFiles(const tstring& folder, FileList& files); // throw (Win32Exception)

// Read in all the project files.
bool readProjectFiles(Projects& projects);

// Parse the project files into a single structure.
bool parseProjectFiles(const Projects& projects, ProjectSettings& settings);

// Convert the raw project settings data to the table output format.
void listSettings(const Projects& projects, const ProjectSettings& settings, Table& results);

// Compare the project settings data and output a table of the differences.
void compareSettings(const Projects& projects, const ProjectSettings& settings, Table& results);

#endif // PROJECTCOMPARE_HPP
