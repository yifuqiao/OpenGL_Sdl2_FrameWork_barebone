#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include <glew.h>
class FileReader
{
public:
	static GLchar* Read(char* filename);
};

