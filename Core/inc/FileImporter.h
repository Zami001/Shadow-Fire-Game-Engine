#pragma once

#include <Core.h>
#include <stdint.h>
#include <filesystem>
#include <fstream>
#include <Logging/Log.h>
#include <Asserts.h>
#include <iostream>

template<typename OutputType>
class FileImporter {
public:
	FileImporter() = default;
	virtual ~FileImporter() = default;

protected:
	bool InsertZeroChar = false;

	virtual bool ConvertData(OutputType& Output, const char* data, size_t dataLength) = 0;

	inline void ConvertToTextLines(std::vector<std::string>& OutLines, const char* data, size_t length) {
		OutLines.clear();

		std::string line;
		const char* LineStart = data;
		while (const char* LineEnd = reinterpret_cast<const char*>(memchr(LineStart, '\n', length - (LineStart - data)))) {
			OutLines.emplace_back(LineStart, LineEnd - LineStart);
			LineStart = LineEnd + 1;
		}

		OutLines.emplace_back(LineStart, length - (LineStart - data));
	}

public:
	inline bool ImportFile(const std::filesystem::path& FilePath, OutputType& Output) {
		std::filesystem::path FullPath = FilePath;
		if (!FilePath.is_absolute()) {
			FullPath = std::filesystem::absolute(FilePath);
		}

		bool exists = std::filesystem::exists(FullPath);

		if (!exists) {
			SF_LOG(File Import, Warning, "File not found - %s", FilePath.string().c_str())
			return false;
		}

		std::ifstream file;

		file.open(FullPath.c_str(), std::ifstream::in | std::ifstream::binary);

		file.seekg(0, file.end);
		size_t length = file.tellg();
		file.seekg(0, file.beg);

		size_t totalLength = length;
		if (InsertZeroChar) totalLength++;

		char* data = new char[totalLength];

		file.read(data, length);
		if (InsertZeroChar) {
			data[totalLength - 1] = 0;
		}

		bool result = false;
		if (file) {
			if ((result = ConvertData(Output, data, totalLength))) {
				SF_LOG(File Import, Log, "File imported successfully - %s", FilePath.string().c_str())
			} else {
				SF_LOG(File Import, Error, "File import failed - %s", FilePath.string().c_str())
			}
		}

		delete[] data;
		return result;
	}

};