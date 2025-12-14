#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

#include <stdio.h>
#include "..\Strings\String.h"
#include "File_Handling_Standards.h"

enum WriteType {
	NEW_STRUCT_WRITE,
	ADD_ATTRIBUTE_WRITE,
	BLANK_LINE
};

struct SaveStateQueue {
	struct SaveStateQueue* next;
	enum SaveFileAttributeTypes type;
	void* data;
};

struct LoadStateIdList {
	struct LoadStateIdList* next;
	int id; // Key
	void* data; // Value
};

//#################################################################################
//
//	Object Type Handling
//
//#################################################################################

static inline String* appendAttributeTypeName(String* const buffer, const enum SaveFileAttributeTypes attributeType)
{
	switch(attributeType)
	{
		case FILE_TYPE_CORE:
            appendChars(buffer, FILE_CORE_ID);
			break;
		default:
            appendCharsN(buffer, "Default", 7);
			break;
	}

	return buffer;
}

static inline enum SaveFileAttributeTypes getAttributeTypeFromString(const String* const id_string)
{
	if (compareChars(id_string, FILE_CORE_ID) == 0)
	{
		return FILE_TYPE_CORE;
	}
	else
	{
		return (enum SaveFileAttributeTypes) -1;
	}
}
//---------------------------------------------------------------------------------

//#################################################################################
//
//	Save String Formatting
//
//#################################################################################

static inline String* appendSaveFormatName(String* const buffer, const enum SaveFileAttributeTypes attributeType, const int id)
{
	appendAttributeTypeName(buffer, attributeType);
    appendChar(buffer, FILE_ID_SEP);
	appendIntDec(buffer, id);

	return buffer;
}

static inline String* appendSaveFormatPtrString(String* const buffer, const enum SaveFileAttributeTypes attributeType, const int id)
{
	appendChar(buffer, FILE_PTR_PREFIX);
	appendSaveFormatName(buffer, attributeType, id);
	return buffer;
}

static inline String* appendSaveFormatStringAttribute(String* const buffer, const char* const attribute_name, const char* const str)
{
	appendChars(buffer, attribute_name);
	appendChar(buffer, FILE_SEP);
	appendChars(buffer, str);
	return buffer;
}

static inline String* appendSaveFormatIntegerAttribute(String* const buffer, const char* const attribute_name, const int integer)
{
	appendChars(buffer, attribute_name);
	appendChar(buffer, FILE_SEP);
	appendIntDec(buffer, integer);
	return buffer;
}

/* TODO
static inline char* getSaveFormatUnsignedIntegerAttribute(char buffer[BUF_SIZE], const char* attribute_name, const uint_least64_t u_integer)
{
	snprintf(buffer, BUF_SIZE, "%s%c%i", attribute_name, SAVE_FILE_SEP, u_integer);
	return buffer;
}
*/

/* TODO
static inline char* getSaveFormatFloatAttribute(char buffer[BUF_SIZE], const char* attribute_name, const float float_val)
{
	snprintf(buffer, BUF_SIZE, "%s%c%f", attribute_name, SAVE_FILE_SEP, float_val);
	return buffer;
}
*/

static inline String* getSaveFormatPointerAttribute(String* const buffer, const char* const attribute_name, const enum SaveFileAttributeTypes attributeType, const int id)
{
	appendChars(buffer, attribute_name);
	appendChar(buffer, FILE_SEP);
	appendSaveFormatPtrString(buffer, attributeType, id);

	return buffer;
}
//---------------------------------------------------------------------------------

//#################################################################################
//
//	Extracting Object Information
//
//#################################################################################

static inline int extractObjectId(const String* const buffer)
{
	int i = 0;
	int search = 1;
	while (search)
	{
		switch (getCharIndexed(buffer, i))
		{
			case FILE_ID_SEP:
				search = 0;
				break;
			case '\0':
				return -1;
		}
		i++;
	}

	int result = convertStringToIntDec(buffer, i);

	if (result == 0 && getCharIndexed(buffer, i) != '0')
	{
		return -1;
	}

	return result;
}

static inline void copySplitAttribute(const String* const buffer, String* attribute_name_buffer, String* attribute_value_buffer)
{
	int i = 0;
	int search = 1;
	while (search)
	{
		switch (getCharIndexed(buffer, i))
		{
			case FILE_SEP:
				search = 0;
				break;
			case '\0':
				return;
		}
		i++;
	}

	writeStringN(attribute_name_buffer, buffer, i-1);
	writeStringNOffset(attribute_value_buffer, buffer, buffer->str_length-i, i);
}

static inline void extractAttributeNameCopy(const String* const buffer, String* attribute_name_buffer)
{
	int i = 0;
	int search = 1;
	while (search)
	{
		switch (getCharIndexed(buffer, i))
		{
			case FILE_SEP:
				search = 0;
				break;
			case '\0':
				return;
		}
		i++;
	}
	i--;

	writeStringN(attribute_name_buffer, buffer, i);
}

static inline void extractAttributeValueCopy(const String* const buffer, String* attribute_value_buffer)
{
	int i = 0;
	int search = 1;
	while (search)
	{
		switch (getCharIndexed(buffer, i))
		{
			case FILE_SEP:
				search = 0;
				break;
			case '\0':
				return;
		}
		i++;
	}

	writeStringNOffset(attribute_value_buffer, buffer, buffer->str_length-i, i);
}
//---------------------------------------------------------------------------------

//#################################################################################
//
//	Save File Handling
//
//#################################################################################

static inline int writeToFile(FILE* const fptr, const enum WriteType write_func, const String* const str)
{
	switch (write_func)
	{
		case NEW_STRUCT_WRITE:
		return fprintf(fptr, "%s%c", getCharArr(str), FILE_NEXT_ATTR_SEP);
		case ADD_ATTRIBUTE_WRITE:
		return fprintf(fptr, "%c%s%c", FILE_ATTR_ID, getCharArr(str), FILE_NEXT_ATTR_SEP);
		case BLANK_LINE:
		return fprintf(fptr, "%c", FILE_NEXT_ATTR_SEP);
		default:
		return fprintf(fptr, "%s%c", getCharArr(str), FILE_NEXT_ATTR_SEP);
	}
}

static inline int getNextDataPoint(FILE* const fptr, String* buffer)
{
	char new_char = fgetc(fptr);
	if (feof(fptr)) return 0;
	while (new_char != FILE_NEXT_ATTR_SEP)
	{
		appendChar(buffer, new_char);
		new_char = fgetc(fptr);
		if (feof(fptr)) return 0;
	}
	return 1;
}
//---------------------------------------------------------------------------------

//#################################################################################
//
//	Load and Save Queue Handling
//
//#################################################################################

static inline struct SaveStateQueue* createQueue(const enum SaveFileAttributeTypes type, void* const base_item)
{
	struct SaveStateQueue* item = (struct SaveStateQueue*) calloc(1, sizeof(struct SaveStateQueue));
	
	item->type = type;
	item->data = base_item;
	item->next = NULL;
	
	return item;
}

static inline void appendToQueue(struct SaveStateQueue* const head_of_queue, const enum SaveFileAttributeTypes type, void* const data)
{
	struct SaveStateQueue* base_item = head_of_queue;
	while (base_item->next != NULL)
	{
		base_item = base_item->next;
	}
	base_item->next = (struct SaveStateQueue*) calloc(1, sizeof(struct SaveStateQueue));
	
	base_item = base_item->next;
	base_item->type = type;
	base_item->data = data;
	base_item->next = NULL;
}

static inline void cleanUpQueue(struct SaveStateQueue* head_of_queue)
{
	struct SaveStateQueue* current_item = head_of_queue;
	while (current_item->next != NULL)
	{
		current_item = current_item->next;
		free(head_of_queue);
		head_of_queue = current_item;
	}
	free(head_of_queue);
}
//---------------------------------------------------------------------------------

#endif