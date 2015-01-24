#include <string.h>
#include "SandBox_pri.h"
#include "MEM.h"
#include "UTL.h"

TypeInfo Edge_Type_Info[] = {
	{"dummy",	"dummy", 		-1},
	{"bool",	"Boolean",		sizeof(Edge_Boolean)},
	{"char",	"Char",			sizeof(Edge_Char)},

	{"sbyte",	"SByte",		sizeof(Edge_SByte)},
	{"byte",	"Byte",			sizeof(Edge_Byte)},

	{"i16",		"Int16",		sizeof(Edge_Int16)},
	{"ui16",	"UInt16",		sizeof(Edge_UInt16)},

	{"i32",		"Int32",		sizeof(Edge_Int32)},
	{"ui32",	"UInt32",		sizeof(Edge_UInt32)},

	{"i64",		"Int64",		sizeof(Edge_Int64)},
	{"ui64",	"UInt64",		sizeof(Edge_UInt64)},

	{"single",	"Single",		sizeof(Edge_Single)},
	{"double",	"Double",		sizeof(Edge_Double)},
	{"str",		"String",		sizeof(Edge_Char *)},
};

Edge_Byte *
Edge_byte_serialize(const void *data, int length)
{
	Edge_Byte *ret;

	ret = MEM_malloc(sizeof(Edge_Byte) * length);
	memcpy(ret, data, length);

	return ret;
}

void*
Edge_byte_deserialize(void *dest, const Edge_Byte *data, int length)
{
	return memcpy(dest, data, length);
}

Edge_InfoTable *
Edge_alloc_info_table(Edge_BasicType type)
{
	Edge_InfoTable *ret;

	ret = MEM_malloc(sizeof(Edge_InfoTable));
	ret->type = type;

	return ret;
}

Edge_Value *
Edge_alloc_value(Edge_BasicType type)
{
	Edge_Value *ret;

	ret = MEM_malloc(sizeof(Edge_Value));
	ret->table = Edge_alloc_info_table(type);

	ret->marked = False;
	ret->prev = NULL;
	ret->next = NULL;
	Walle_add_object(ret);

	return ret;
}

Edge_Value *
Edge_create_string(Edge_Byte *data, int *offset)
{
	int length;
	Edge_Value *ret;

	length = Edge_wcslen((Edge_Char *)data);
	while (((Edge_Char *)data)[length] != L'\0') length += 1;
	*offset = sizeof(Edge_Char) * (length + sizeof(Edge_Char));

	ret = Edge_alloc_value(EDGE_STRING);
	if (length <= 0) {
		ret->u.string_value = NULL;
		return ret;
	}

	ret->u.string_value = MEM_malloc(sizeof(Edge_Char) * (length + 1));
	Edge_byte_deserialize(ret->u.string_value, data, sizeof(Edge_Char) * (length + 1));

	return ret;
}

Edge_Value *
Edge_get_init_value(Edge_BasicType type)
{
	Edge_Value *value;

	value = Edge_alloc_value(type);
	switch ((int)type) {
		case EDGE_BOOLEAN:
		case EDGE_CHAR:
		case EDGE_SBYTE:
		case EDGE_INT16:
		case EDGE_INT32:
		case EDGE_INT64:
		case EDGE_BYTE:
		case EDGE_UINT16:
		case EDGE_UINT32:
		case EDGE_UINT64:
			value->u.uint64_value = 0;
			break;
		case EDGE_SINGLE:
		case EDGE_DOUBLE:
			value->u.double_value = 0.0;
			break;
		case EDGE_STRING:
			value->u.string_value = NULL;
			break;
		default:
			printf("bad type: %d\n", type);
	}
	return value;
}