#ifndef DIRECTORY_READING_H
#define DIRECTORY_READING_H

#include <stdio.h>

#define DIRECTORY_READING_BUFFER_SIZE 64

#ifdef _WIN64  // windows platform
	#include <windows.h>
#else
	#include <dirent.h>
#endif

typedef enum DirPtrStatus
{
	DIR_PTR_UNINITIALISED,
	DIR_PTR_INITIALISED,
	DIR_PTR_OPEN,
	DIR_PTR_CLOSED
} DirPtrStatus;

#ifdef _WIN64  // windows platform
	typedef struct DirPtr
	{
		DirPtrStatus status;
		char dir_path[DIRECTORY_READING_BUFFER_SIZE];
		HANDLE handle;
		WIN32_FIND_DATA FindFileData;
	} DirPtr;
#else
	typedef struct DirPtr
	{
		DirPtrStatus status;
		char dir_path[DIRECTORY_READING_BUFFER_SIZE];
		DIR* dir_ptr_backend;
		struct dirent *de;
	} DirPtr;
#endif

/**
 * @brief Opens, allocates and returns a new directory pointer object (the base object used to call os-based functions)
 * @return a newly allocated directory pointer
*/
DirPtr* newDirPtr(void);

/**
 * @brief Returns the the path to the directory that the current directory pointer is pointing at
 * @param ptr the directory pointer to query
 * @return the path of the currently working directory
*/
char* currentPath(const DirPtr* const ptr);

/**
 * @brief Attempts to open the directory described in \p dir_path relative to the current working file and sets it to the new directory. Can only be done once per directory pointer.
 * @param ptr the directory pointer to use
 * @param dir_path the relative path of the new directory to open
 * @return 0 on success; 1 on an invalid directory pointer provided; 2 on invalid \p dir_path
*/
int openDir(DirPtr* const ptr, const char* const dir_path);

/**
 * @brief Gets the next (first, if this is first call) file name in the current working directory of \p ptr and stores the name in \p file_name_buffer . Requires having previously opened a directory prior.
 * @param ptr the directory pointer to use
 * @param file_name_buffer A large enough buffer to store file names
 * @return 0 on success; 1 on an invalid pointer provided; 2 on no additional files found (non-Windows); 3 on no additional files found (Windows)
 * @warning no check is completed to ensure prevent buffer overflow on flie_name_buffer and the retrieved file name.
*/
int getNextFile(DirPtr* const ptr, char* const file_name_buffer);

/**
 * @brief Closes the directory pointer \p ptr and frees all associated allocated memory
 * @param ptr the diretory pointer to close
 * @return 0 on success; 1 on invalid (not yet opened) directory pionter provided; 2 on system error
 * @warning DO NOT try to access or manipulate \p ptr in any manner after a successful close
 */
int closeDir(DirPtr* const ptr);

#endif