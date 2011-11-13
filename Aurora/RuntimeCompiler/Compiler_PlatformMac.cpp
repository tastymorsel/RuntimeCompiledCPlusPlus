//
// Copyright (c) 2010-2011 Matthew Jack and Doug Binks
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

// RuntimeDLLTest01.cpp : Defines the entry point for the console application.
//
// Notes:
//   - We use a single intermediate directory for compiled .obj files, which means
//     we don't support compiling multiple files with the same name. Could fix this
//     with either mangling names to include paths,  or recreating folder structure
//
//

#include "Compiler.h"

#include <string>
#include <sstream>
#include <vector>
#include <set>
#include "boost/algorithm/string.hpp"

#include "assert.h"

#include "ICompilerLogger.h"

#include <sys/param.h>
#include <mach-o/dyld.h>

using boost::filesystem::path;

using namespace std;

const std::string	c_CompletionToken( "_COMPLETION_TOKEN_" );

void ReadAndHandleOutputThread( void *arg );
void WriteInput( FILE *writeHere, std::string& input  );

class PlatformCompilerImplData
{
public:
	PlatformCompilerImplData()
		: m_bCompileIsComplete( false ),
		m_pFileHandle(0)
	{
	}

	void InitialiseProcess()
	{
		char filename[MAXPATHLEN];
		unsigned int size = MAXPATHLEN;
		int rv = _NSGetExecutablePath(filename, &size);
		path cdPath( filename );
		cdPath = cdPath.parent_path() / "../Examples/ConsoleExample";

		m_pFileHandle = popen("/bin/bash", "r+");
		std::string str = "cd " + cdPath.string() + "\n";

		fwrite(str.c_str(), str.size(), 1, m_pFileHandle);
		fflush(m_pFileHandle);
	}

	FILE				*m_pFileHandle;
	volatile bool		m_bCompileIsComplete;
	ICompilerLogger*	m_pLogger;
};

Compiler::Compiler() 
	: m_pImplData( 0 )
{
}

Compiler::~Compiler()
{
	pclose(m_pImplData->m_pFileHandle);
}

const std::wstring Compiler::GetObjectFileExtension() const
{
	return L".o";
}

bool Compiler::GetIsComplete() const
{
	return m_pImplData->m_bCompileIsComplete;
}

void Compiler::Initialise( ICompilerLogger * pLogger )
{
	assert( pLogger );

	m_pImplData = new PlatformCompilerImplData;
	m_pImplData->m_pLogger = pLogger;
}

void Compiler::RunCompile( const std::vector<boost::filesystem::path>& filesToCompile,
					 const std::vector<boost::filesystem::path>& includeDirList,
					 const boost::filesystem::path& outputFile )
{
	m_pImplData->m_bCompileIsComplete = false;
	//optimization and c runtime
#ifdef _DEBUG
	std::string flags = "-c -g -fpic -fno-weak";
#else
	std::string flags = "-c -O3 -g -fpic -fno-weak";
#endif
	if( NULL == m_pImplData->m_pFileHandle )
	{
		m_pImplData->InitialiseProcess();
	}

	//create include path search string
	std::string strIncludeFiles;
	for( size_t i = 0; i < includeDirList.size(); ++i )
	{
		strIncludeFiles += " -I " + includeDirList[i].string();
	}


	// Create include path search string
	std::string strFilesToCompile;
	std::string strObjectFiles;
	std::set<std::string> filteredPaths;
	for( size_t i = 0; i < filesToCompile.size(); ++i )
	{
		std::string strPath = filesToCompile[i].string();
		std::set<std::string>::const_iterator it = filteredPaths.find(strPath);
		if (it == filteredPaths.end())
		{
			strFilesToCompile += " " + strPath;

			boost::filesystem::path path(strPath);
			path = path.replace_extension(".o");
			strObjectFiles += " ";
			std::string s = path.filename().string();
			strObjectFiles += s;

			filteredPaths.insert(strPath);
		}
		else
		{
			m_pImplData->m_pLogger->LogWarning("[RuntimeCompiler] Ignoring duplicate path: %s\n", strPath.c_str());
		}
	}

	while(1)
	{
		const int pos = strObjectFiles.find(".cpp");
		if (pos==-1) break;
		strObjectFiles.replace(pos,4,".o");
	}

	std::string cmdToSend = "g++ " + flags + " -D PLATFORM_MAC ";

	cmdToSend += " " + strIncludeFiles + " " + strFilesToCompile + " && g++ -dynamiclib -weak_reference_mismatches=weak -fvisibility=hidden -current_version 1.0 " + strObjectFiles + " -o \"" + outputFile.string() + "\"\necho " + c_CompletionToken + "\n";
    
	WriteInput( m_pImplData->m_pFileHandle, cmdToSend );

	m_pImplData->m_bCompileIsComplete = true;
}


#if 0
void ReadAndHandleOutputThread( void *arg )
{
	PlatformCompilerImplData* pImpl = (PlatformCompilerImplData*)arg;

    char lpBuffer[1024];
    int nBytesRead;
 	bool bReadActive = true;
	bool bReadOneMore = false;
    while( bReadActive )
    {
		if (!ReadFile(pImpl->m_CmdProcessOutputRead,lpBuffer,sizeof(lpBuffer)-1,
										&nBytesRead,NULL) || !nBytesRead)
		{
			bReadActive = false;
			if (GetLastError() != ERROR_BROKEN_PIPE)	//broken pipe is OK
			{
				pImpl->m_pLogger->LogError( "[RuntimeCompiler] Redirect of compile output failed on read\n" );
			}
		}
		else
		{
			// Display the characters read in logger.
			lpBuffer[nBytesRead]=0;

			//fist check for completion token...
			std::string buffer( lpBuffer );
			size_t found = buffer.find( c_CompletionToken );
			if( found != std::string::npos )
			{
				//we've found the completion token, which means we quit
				buffer = buffer.substr( 0, found );
				pImpl->m_pLogger->LogInfo("[RuntimeCompiler] Complete\n");
				pImpl->m_bCompileIsComplete = true;
			}
			if( bReadActive || buffer.length() ) //don't output blank last line
			{
				//check if this is an error
				size_t found = buffer.find( " : error " );
				if( found != std::string::npos )
				{
					//OutputDebugStringA( buffer.c_str() );
					pImpl->m_pLogger->LogError( "%s", buffer.c_str() );
				}
				else
				{
					pImpl->m_pLogger->LogInfo( "%s", buffer.c_str() );
				}
			}
		}
     }
}
#endif

void WriteInput( FILE *writeHere, std::string& input  )
{
    int nBytesWritten;
	size_t length = input.length();

	printf("RUNNING COMMAND: %s\n", input.c_str());

	fwrite(input.c_str(), length, 1, writeHere);
	fflush(writeHere);

	bool keepreading = true;

	while (keepreading)
	{
		char data[4096] = {0};
		int s = fscanf(writeHere, "%4095s", data);

		printf("OUTPUT: %s\n", data);

		if (s > 0)
		{
			if (strcmp(data, c_CompletionToken.c_str()) == 0)
			{
				keepreading = false;
			}
		}
		else
			keepreading = false;
	}
}
