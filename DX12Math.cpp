#include "DX12Math.h"

namespace DX12Math
{
	void SinCos(float& sin_, float& cos_, float angle)
	{
		sin_ = sin(angle);
		cos_ = cos(angle);
	}

	float ChangeRadians(const float& angle)
	{
		float radian = angle * AX_PIF / 180;
		return radian;
	}

	float ChangeDira(const float& angle)
	{
		float radian = angle * 180 / AX_PIF;
		return radian;
	}

	std::vector<std::string> getFileNames(std::string folderPath)
	{
		{
			std::filesystem::directory_iterator iter(folderPath), end;
			std::error_code err;
			std::vector<std::string> file_names;

			for (; iter != end && !err; iter.increment(err))
			{
				const std::filesystem::directory_entry entry = *iter;

				file_names.push_back(entry.path().string());
				printf("%s\n", file_names.back().c_str());
			}

			return file_names;
		}
	}
}