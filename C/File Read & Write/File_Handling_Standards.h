#ifndef FILE_HANDLING_STANDARDS_H
#define FILE_HANDLING_STANDARDS_H

enum SaveFileAttributeTypes {
	FILE_TYPE_CORE,
	FILE_TYPE_NUM
};

#define FILE_SEP '='
#define FILE_PTR_PREFIX '>'
#define FILE_ID_SEP '_'
#define FILE_ATTR_ID '\t'
#define FILE_NEXT_ATTR_SEP '\n'
#define FILE_ARR_ITEM_SEP ';'

#define FILE_CORE_ID ("Core")

#endif