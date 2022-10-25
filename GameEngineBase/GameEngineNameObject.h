#pragma once


class GameEngineNameObject
{

public:
	GameEngineNameObject();
	virtual ~GameEngineNameObject();

public:
	inline void SetName(const std::string& _name)
	{
		objectName_ = _name;
	}

	inline void SetName(std::string_view _name)
	{
		objectName_ = _name;
	}

	std::string GetNameCopy() const
	{
		return objectName_;
	}

	std::string_view GetName() const
	{
		return objectName_;
	}

	const std::string& GetNameConstRef()	
	{
		return objectName_;
	}

	const char* GetNameConstPtr()
	{
		return objectName_.c_str();
	}

private:
	std::string objectName_;

};

