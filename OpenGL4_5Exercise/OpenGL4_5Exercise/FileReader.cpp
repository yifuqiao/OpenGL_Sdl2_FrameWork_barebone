#include "FileReader.h"
#include <assert.h>

GLchar* FileReader::Read(GLchar* filename)
{
	FILE *pFile = fopen(filename, "rt");
	if (NULL != pFile)
	{
		fseek(pFile, 0, SEEK_END);
		size_t length = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);
		GLchar *buffer = (GLchar*)malloc(length);	// extra byte for the terminator

		for (size_t i = 0; i < length; ++i)
		{
			char c = fgetc(pFile);
			if (EOF == c)
			{
				buffer[i] = '\0';
				break;
			}
			buffer[i] = c;
		}

		fclose(pFile);
		return buffer;
	}
	return NULL;
}
