#include "PreCompile.h"
#include "GameEngineFile.h"
#include "GameEngineDebug.h"

GameEngineFile::GameEngineFile() : filePtr_(nullptr)
{
}

GameEngineFile::GameEngineFile(const char* _path) : filePtr_(nullptr)
{
	path_ = _path;
}

GameEngineFile::GameEngineFile(const std::string& _path) : filePtr_(nullptr)
{
	path_ = _path;
}

GameEngineFile::GameEngineFile(const std::filesystem::path& _path) : filePtr_(nullptr)
{
	path_ = _path;
}

GameEngineFile::GameEngineFile(const GameEngineFile& _other) : filePtr_(nullptr)
{
	path_ = _other.path_;
}

GameEngineFile::GameEngineFile(GameEngineFile&& _other) noexcept : filePtr_(nullptr)
{
	path_ = _other.path_;
}

GameEngineFile::~GameEngineFile()
{
	if (nullptr != filePtr_)
	{
		fclose(filePtr_);
		filePtr_ = nullptr;
	}
}

void GameEngineFile::Open(OpenMode _openMode, FileMode _fileMode)
{
	std::string mode = "";

	// "wb": ���� ���̳ʸ�.
	// "wt": ���� �ؽ�Ʈ.

	// "rb": �д´� ���̳ʸ�.
	// "rt": �д´� �ؽ�Ʈ.


	switch (_openMode)
	{
	case OpenMode::Read:
		mode += "r";
		break;

	case OpenMode::Write:
		mode += "w";
		break;

	default:
		MsgBoxAssert("�Ұ����� ����Դϴ�.");
		return;
	}

	switch (_fileMode)
	{
	case FileMode::Binary:
		mode += "b";
		break;

	case FileMode::Text:
		mode += "t";
		break;

	default:
		MsgBoxAssert("�Ұ����� ����Դϴ�.");
		return;
	}

	int openResult = fopen_s(	//������ ����� ������ ���� �����ڵ��� �޾ƿ��� �Լ�.
		&filePtr_,				//���� ���Ͽ� ���� �����ڵ��� �ּҰ�.
		path_.string().c_str(),	//��ΰ� ���Ե� ���� �̸�.
		mode.c_str()			//���� ���� ���. 
	);

	if (0 != openResult)
	{
		MsgBoxAssert("������ ���������� ���µ� �����߽��ϴ�.");
		return;
	}
}

void GameEngineFile::Close()
{
	if (nullptr != filePtr_)
	{
		fclose(filePtr_);
		filePtr_ = nullptr;
	}
}

void GameEngineFile::Read(void* _readData, size_t _dataSize, size_t _readSize)
{
	fread_s(
		_readData,
		_dataSize,
		_readSize,
		1,			//�д� Ƚ��.
		filePtr_
	);
}

void GameEngineFile::Write(void* _writeData, size_t _writeSize)
{
	fwrite(
		_writeData,
		_writeSize,
		1,			//���� Ƚ��.
		filePtr_
	);
}

std::string GameEngineFile::GetString()
{
	std::string allString;

	uintmax_t fileSize = GetFileSize();

	allString.resize(fileSize);	//unsigned long long == uintmax_t == size_t.

	Read(&allString[0], fileSize, fileSize);

	return allString;
}

uintmax_t GameEngineFile::GetFileSize() const
{
	return std::filesystem::file_size(this->path_);
}

/*static*/ uintmax_t GameEngineFile::GetFileSize(const std::filesystem::path& _path)
{
	return std::filesystem::file_size(_path);
}