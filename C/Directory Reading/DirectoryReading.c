#include "DirectoryReading.h"

DirPtr* newDirPtr()
{
	DirPtr* ptr = calloc(1, sizeof(DirPtr));
	ptr->status = DIR_PTR_UNINITIALISED;
	return ptr;
}

char* currentPath(const DirPtr* const ptr)
{
	return ptr->dir_path;
}

#ifdef _WIN64 // windows platform

int openDir(DirPtr* const ptr, const char* const dir_path)
{
	if (ptr->status != DIR_PTR_UNINITIALISED)
	{
		return 1;
	}
	snprintf(ptr->dir_path, DIRECTORY_READING_BUFFER_SIZE, "%s\\*", dir_path);
	ptr->status = DIR_PTR_INITIALISED;

	ptr->handle = FindFirstFile(ptr->dir_path, &ptr->FindFileData);
	if (ptr->handle == INVALID_HANDLE_VALUE)
	{
		return 2;
	}
	ptr->status = DIR_PTR_OPEN;

	return 0;
}

int getNextFile(DirPtr* const ptr, char* const file_name_buffer)
{
	if (ptr->status == DIR_PTR_OPEN)
	{
		do {
			if (FindNextFile(ptr->handle, &ptr->FindFileData) == NULL)
			{
				return 3;
			}
		}
		while (ptr->FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
	}
	else
	{
		return 1;
	}

	sprintf(file_name_buffer, "%s", ptr->FindFileData.cFileName);
	return 0;
}

int closeDir(DirPtr* ptr)
{
	if (ptr->status != DIR_PTR_OPEN)
	{
		return 1;
	}
	if (FindClose(ptr->handle))
	{
		free(ptr);
		return 0;
	}
	return 2;
}

#else // TODO - BELOW IS NOT YET TESTED!!!!!!

int openDir(DirPtr* const ptr, const char* const dir_path)
{
	if (ptr->status != DIR_PTR_UNINITIALISED && ptr->status != DIR_PTR_INITIALISED)
	{
		return 1;
	}
	ptr->dir_ptr_backend = opendir(dir_path);
	if (ptr->dir_ptr_backend == NULL)
	{
		return 2;
	}
	snprintf(ptr->dir_path, DIRECTORY_READING_BUFFER_SIZE, "%s", dir_path);
	ptr->status = DIR_PTR_OPEN;
	return 0;
}

int getNextFile(DirPtr* const ptr, char* const file_name_buffer)
{
	if (ptr->status != DIR_PTR_OPEN)
	{
		return 1;
	}
	ptr->de = readdir(ptr->dir_ptr_backend);
	if (ptr->de == NULL)
	{
		return 2;
	}
	sprintf(file_name_buffer, "%s", ptr->de->d_name);
	return 0;
}

int closeDir(DirPtr* ptr)
{
	if (ptr->status != DIR_PTR_OPEN)
	{
		return 1;
	}
	if (closedir(ptr->dir_ptr_backend))
	{
		return 2;
	}
	ptr->status = DIR_PTR_CLOSED;
	free(ptr);
	return 0;
}

#endif