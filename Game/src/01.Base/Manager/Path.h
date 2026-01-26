#pragma once

class CPath
{
	SINGLE(CPath)
public:
	~CPath() = default;

public:
	static std::vector<std::string> RelativePathToTokens(const std::string_view InRelativePath)
	{
		std::vector<std::string> Subjects;

		size_t StartIndex = 0;
		while (true)
		{
			size_t FinishIndex = InRelativePath.find("/", StartIndex);
			if (FinishIndex == std::string::npos)
				break;
			const std::string_view Subject = InRelativePath.substr(StartIndex, FinishIndex - StartIndex);
			if (Subject != "..")
				Subjects.emplace_back(Subject);
			StartIndex = FinishIndex + 1;
		}
		const std::string_view Subject = InRelativePath.substr(StartIndex, InRelativePath.size() - StartIndex);
		Subjects.emplace_back(Subject);

		return Subjects;
	}

private:

};

