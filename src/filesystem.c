
/* Copyright 2009 Ralf Schmitt
 *
 * This file is part of volsort.
 *
 * volsort is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * volsort is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with volsort. If not, see <http://www.gnu.org/licenses/>.
 */

#include "filesystem.h"

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

#include "string.h"

static mode_t mode = 0777;
#define THIS_FILE ((strrchr(__FILE__, '/') ?: __FILE__ - 1) + 1)

int32_t fsCopyFile(char const * src, char const * dest)
{
	int inFh, ouFh;
	char buf[512];
	int bytes;

	if ((inFh = open(src, O_RDONLY)) == -1)
	{
#ifdef DEBUG_FILESYSTEM_ON
		commonPrintf("open %s failed!\n",src);
#endif
		switch(errno)
		{
			// pathname  already  exists  (not  necessarily  as  a directory)
			case EEXIST:
#ifdef DEBUG_FILESYSTEM_ON
				/*                commonPrintf("Error : %s\n",strerror(errno));*/
				/*                commonPrintf("Error : %s\n",src);*/
#endif
				break;
				// pathname  points  outside  your  accessible address space.
			case EFAULT:
#ifdef DEBUG_FILESYSTEM_ON
				commonPrintf("Error : %s\n",strerror(errno));
				commonPrintf("Error : %s\n",src);
#endif
				break;
				// The parent directory does not allow  write  permis-
				// sion  to  the process, or one of the directories in
				// pathname did not allow search (execute) permission.
			case EACCES:
#ifdef DEBUG_FILESYSTEM_ON
				/*                commonPrintf("Error : %s\n",strerror(errno));*/
				/*                commonPrintf("Error : %s\n",src);*/
#endif
				break;
				// pathname was too long
			case ENAMETOOLONG:
#ifdef DEBUG_FILESYSTEM_ON
				commonPrintf("Error : %s\n",strerror(errno));
				commonPrintf("Error : %s\n",src);
#endif
				break;
				// A directory component in pathname does not exist or
				// is a dangling symbolic link.
			case ENOENT:
#ifdef DEBUG_FILESYSTEM_ON
				commonPrintf("Error : %s\n",strerror(errno));
				commonPrintf("Error : %s\n",src);
#endif
				break;
				// A component used as a directory in pathname is not,
				// in fact, a directory.
			case ENOTDIR:
#ifdef DEBUG_FILESYSTEM_ON
				commonPrintf("Error : %s\n",strerror(errno));
				commonPrintf("Error : %s\n",src);
#endif
				break;
				// Insufficient kernel memory was available.
			case ENOMEM:
#ifdef DEBUG_FILESYSTEM_ON
				commonPrintf("Error : %s\n",strerror(errno));
				commonPrintf("Error : %s\n",src);
#endif
				break;
		}
		close(inFh);
		return -1;
	}

	/*    if((ouFh = open(dest, O_RDWR|O_CREAT|O_TRUNC)) == -1)*/
	if ((ouFh = open(dest, O_WRONLY | O_CREAT)) == -1)
	{
#ifdef DEBUG_FILESYSTEM_ON
		commonPrintf("open %s failed!\n",dest);
#endif
		switch(errno)
		{
			// pathname  already  exists  (not  necessarily  as  a directory)
			case EEXIST:
#ifdef DEBUG_FILESYSTEM_ON
				/*                commonPrintf("Error : %s\n",strerror(errno));*/
				/*                commonPrintf("Error : %s\n",dest);*/
#endif
				break;
				// pathname  points  outside  your  accessible address space.
			case EFAULT:
#ifdef DEBUG_FILESYSTEM_ON
				commonPrintf("Error : %s\n",strerror(errno));
				commonPrintf("Error : %s\n",dest);
#endif
				break;
				// The parent directory does not allow  write  permis-
				// sion  to  the process, or one of the directories in
				// pathname did not allow search (execute) permission.
			case EACCES:
#ifdef DEBUG_FILESYSTEM_ON
				/*                commonPrintf("Error : %s\n",strerror(errno));*/
				/*                commonPrintf("Error : %s\n",dest);*/
#endif
				break;
				// pathname was too long
			case ENAMETOOLONG:
#ifdef DEBUG_FILESYSTEM_ON
				commonPrintf("Error : %s\n",strerror(errno));
				commonPrintf("Error : %s\n",dest);
#endif
				break;
				// A directory component in pathname does not exist or
				// is a dangling symbolic link.
			case ENOENT:
#ifdef DEBUG_FILESYSTEM_ON
				commonPrintf("Error : %s\n",strerror(errno));
				commonPrintf("Error : %s\n",dest);
#endif
				break;
				// A component used as a directory in pathname is not,
				// in fact, a directory.
			case ENOTDIR:
#ifdef DEBUG_FILESYSTEM_ON
				commonPrintf("Error : %s\n",strerror(errno));
				commonPrintf("Error : %s\n",dest);
#endif
				break;
				// Insufficient kernel memory was available.
			case ENOMEM:
#ifdef DEBUG_FILESYSTEM_ON
				commonPrintf("Error : %s\n",strerror(errno));
				commonPrintf("Error : %s\n",dest);
#endif
				break;
		}
		close(inFh);
		close(ouFh);
		return -1;
	}

	while ((bytes = read(inFh, buf, sizeof(buf))) > 0)
		write(ouFh, buf, bytes);

	close(inFh);
	close(ouFh);

	return 1;
}

void fsMkDirP(char const * dir)
{
	char const * pos = dir;
	char substr[1024];
	memset(substr,'\0',1024);

	// loop over all /'es in directory string
	while ((pos = strchr(pos,'/')) != NULL)
	{
		// if first char is '/' we can skip
		if (pos == dir)
		{
			pos++;
			continue;
		}
		// create the substring
		strncpy(substr, dir, pos-dir);
		fsMkDir(substr);
		pos++;
	}
	// append the rest
	if (dir[strlen(dir)-1] != '/')
	{
		strcpy(substr,dir);
		fsMkDir(substr);
	}
}

int32_t fsMkDir(char const * dir)
{
	int32_t ret = mkdir(dir, mode);

	if (ret == -1)
	{
		switch(errno)
		{
			// pathname  already  exists  (not  necessarily  as  a directory)
			case EEXIST:
#ifdef DEBUG_FILESYSTEM_ON
				/*                commonPrintf("Error : %s\n",strerror(errno));*/
				/*                commonPrintf("Error : %s\n",dir);*/
#endif
				break;
				// pathname  points  outside  your  accessible address space.
			case EFAULT:
#ifdef DEBUG_FILESYSTEM_ON
				commonPrintf("Error : %s\n",strerror(errno));
				commonPrintf("Error : %s\n",dir);
#endif
				break;
				// The parent directory does not allow  write  permis-
				// sion  to  the process, or one of the directories in
				// pathname did not allow search (execute) permission.
			case EACCES:
#ifdef DEBUG_FILESYSTEM_ON
				/*                commonPrintf("Error : %s\n",strerror(errno));*/
				/*                commonPrintf("Error : %s\n",dir);*/
#endif
				break;
				// pathname was too long
			case ENAMETOOLONG:
#ifdef DEBUG_FILESYSTEM_ON
				commonPrintf("Error : %s\n",strerror(errno));
				commonPrintf("Error : %s\n",dir);
#endif
				break;
				// A directory component in pathname does not exist or
				// is a dangling symbolic link.
			case ENOENT:
#ifdef DEBUG_FILESYSTEM_ON
				commonPrintf("Error : %s\n",strerror(errno));
				commonPrintf("Error : %s\n",dir);
#endif
				break;
				// A component used as a directory in pathname is not,
				// in fact, a directory.
			case ENOTDIR:
#ifdef DEBUG_FILESYSTEM_ON
				commonPrintf("Error : %s\n",strerror(errno));
				commonPrintf("Error : %s\n",dir);
#endif
				break;
				// Insufficient kernel memory was available.
			case ENOMEM:
#ifdef DEBUG_FILESYSTEM_ON
				commonPrintf("Error : %s\n",strerror(errno));
				commonPrintf("Error : %s\n",dir);
#endif
				break;
		}
	}
	return ret;
}

bool fsExists(char const * elem)
{
	struct stat attribut;
	if (stat(elem, &attribut) == -1)
	{
		return FALSE;
	}
	return TRUE;
}

bool fsIsDir(char const * elem)
{
	struct stat attribut;

	if (stat(elem, &attribut) == -1)
	{
#ifdef DEBUG_FILESYSTEM_ON
		commonPrintf("ERROR stat for %s\n", elem);
#endif
		return EXIT_FAILURE;
	}

	if (attribut.st_mode & S_IFDIR)
		return TRUE;
	else
		return FALSE;
}

bool fsIsFile(char const * elem)
{
	struct stat attribut;

	if (stat(elem, &attribut) == -1)
	{
#ifdef DEBUG_FILESYSTEM_ON
		commonPrintf("2ERROR stat for %s\n",elem);
#endif
		return EXIT_FAILURE;
	}

	if (attribut.st_mode & S_IFREG)
		return TRUE;
	else
		return FALSE;
}

uint64_t fsSize(char const * elem)
{
	struct stat attribut;

	if (stat(elem, &attribut) == -1)
	{
#ifdef DEBUG_FILESYSTEM_ON
		commonPrintf("ERROR stat for %s\n", elem);
#endif
		return EXIT_FAILURE;
	}
	else
	{
		return attribut.st_size;
	}
}

void fsScanDir(FsNode *parent)
{
#ifdef DEBUG_FILESYSTEM_ON
	commonPrintf("fsScanDir %s\n", strPtr(fsnGetFolder(parent)));
#endif

	DIR * dirHandle;
	struct dirent * dirEntry;

	dirHandle = opendir(strPtr(fsnGetFolder(parent)));

	if (dirHandle)
	{
		// first loop to gather information about directory elements
		while (0 != (dirEntry = readdir(dirHandle)))
		{
			if (!strcmp(dirEntry->d_name, ".") || !strcmp(dirEntry->d_name,".."))
				continue;

			// constructing current path
			String * currentFile = strCreate(0, strPtr(fsnGetFolder(parent)));
			strAppendCstr(currentFile, "/");
			strAppendCstr(currentFile, dirEntry->d_name);

			if (!fsExists(strPtr(currentFile)))
				continue;

			// creating a new node
			FsNode * currentFileNode = fsnCreate(
					0,
					parent,
					dirEntry->d_name,
					strPtr(fsnGetFolder(parent)));

			// is this entry a directory or a file
			if (fsIsDir(strPtr(currentFile)))
			{
				fsnSetDir(currentFileNode, TRUE);
				strAppendCstr(fsnGetFolder(currentFileNode), "/");
				strAppendCstr(fsnGetFolder(currentFileNode), dirEntry->d_name);
			}
			else
			{
				fsnSetDir(currentFileNode,FALSE);
				fsnSetFileSize(currentFileNode, fsSize(strPtr(currentFile)));
			}

			nodeSetDepth((Node *) currentFileNode, nodeGetDepth((Node *) parent)+1);

			// appending new node to the list of childs of parent one
			fsnAddChild(parent,currentFileNode);

			// destruct our temp string
			(* currentFile->pVTable[0].pFunc) (TRUE, currentFile);
		}
		closedir(dirHandle);

		// another loop (but with closed filedeskriptor) to start recursions
		Element * fsnIt = listFirst(nodeGetChildrenList((Node *) parent));
		while (fsnIt)
		{
			FsNode * fsn = (FsNode *) elemData(fsnIt);

			if (fsnIsDir(fsn))
				fsScanDir(fsn);

			fsnIt = elemNext(fsnIt);
		}
	}
}
