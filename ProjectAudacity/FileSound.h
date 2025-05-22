#pragma once
#include "Sound.h"
#include <string>


class FileSound : public Sound
{
	std::string getFilePath() const;	//shoud there be a reference?
	std::string getFileType() const;

protected:
	std::string filePath;
	enum class FileType
	{
		wav,
		aiff
	} fileType;

	//.wav
	//.AIFF
};

