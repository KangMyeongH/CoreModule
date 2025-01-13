#include "FileManager.h"

#include <codecvt>
#include <fstream>
#include <locale>
#include <sstream>

#include "RenderManager.h"
#include "Scene.h"

IMPLEMENT_SINGLETON(GameEngine::FileManager)

void GameEngine::FileManager::Build_DirectoryTree(const std::string& _directory)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring directory = converter.from_bytes(_directory);

	if (m_Root)
	{
		delete m_Root;
		m_Root = nullptr;
	}

	m_Root = build_DirectoryTree(directory);
}

GameEngine::DirectoryTreeNode* GameEngine::FileManager::Find_DirectoryNode(DirectoryTreeNode* _root, const std::wstring& _path)
{
	if (!_root) return nullptr;

	if (_root->Path == _path)
		return _root;

	for (DirectoryTreeNode* child : _root->Children)
	{
		DirectoryTreeNode* result = Find_DirectoryNode(child, _path);
		if (result)
			return result;
	}

	return nullptr;


	//if (!_path.empty() && m_Root)
	//{
	//	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	//	std::wstring path = converter.from_bytes(_path);

	//	std::vector<std::wstring> pathParts;
	//	std::wstringstream ss(path);
	//	std::wstring part;

	//	while (std::getline(ss, part, L'\\'))
	//	{
	//		if (!part.empty())
	//		{
	//			pathParts.push_back(part);
	//		}
	//	}

	//	DirectoryTreeNode* current = m_Root;
	//	for (const auto& part : pathParts)
	//	{
	//		auto it = std::find_if
	//		(
	//			current->Children.begin(),
	//			current->Children.end(),
	//			[&part](DirectoryTreeNode* node)
	//			{
	//				return node->IsDirectory && node->Name == part;
	//			}
	//		);

	//		if (it == current->Children.end())
	//		{
	//			return nullptr;
	//		}
	//		current = *it;
	//	}

	//	return current;
	//}

	//return nullptr;
}

bool GameEngine::FileManager::LoadSceneData(const std::wstring& _path)
{
	if (_path.empty()) return false;
	std::ifstream inFile(_path);
	if (!inFile.is_open())
	{
		return false;
	}
	nlohmann::ordered_json j;
	inFile >> j;
	Scene::GetInstance().From_Json(j);
	inFile.clear();
	inFile.close();
	return true;
}

void GameEngine::FileManager::Load_SingleTexture(const DirectoryTreeNode* _root)
{
	for (const auto child : _root->Children)
	{
		if (child->IsDirectory)
		{
			Load_SingleTexture(child);
		}

		else
		{
			RenderManager::GetInstance().Add_Texture(child->Path);
		}
	}
}

void GameEngine::FileManager::Load_MultiTexture(const DirectoryTreeNode* _root)
{
	bool isTarget = true;

	for (const auto child : _root->Children)
	{
		if (child->IsDirectory)
		{
			isTarget = false;
			Load_MultiTexture(child);
		}
	}

	if (isTarget)
	{
		RenderManager::GetInstance().Add_MultiTexture(_root->Path);
	}
}

void GameEngine::FileManager::Release()
{
	delete m_Root;
	m_Root = nullptr;
}

GameEngine::DirectoryTreeNode* GameEngine::FileManager::build_DirectoryTree(const std::wstring& _directory, const std::wstring& _parentPath)
{
	// 현재 디렉토리의 전체 경로 계산
	std::wstring currentPath = _parentPath.empty() ? _directory : _parentPath + L"\\" + _directory;

	// 루트 노드 생성
	DirectoryTreeNode* root = new DirectoryTreeNode(_directory, true, _parentPath);

	// Windows API로 디렉토리 탐색
	WIN32_FIND_DATAW findFileData;
	HANDLE hFind = FindFirstFileW((currentPath + L"\\*").c_str(), &findFileData);

	if (hFind == INVALID_HANDLE_VALUE)
		return root;

	do {
		std::wstring name = findFileData.cFileName;

		if (name == L"." || name == L"..")
			continue;

		std::wstring fullPath = currentPath + L"\\" + name;

		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			// 하위 디렉토리 생성 (재귀 호출)
			root->Children.push_back(build_DirectoryTree(name, currentPath));
		}
		else {
			// 파일 노드 생성
			root->Children.push_back(new DirectoryTreeNode(name, false, currentPath));
		}
	} while (FindNextFileW(hFind, &findFileData) != 0);

	FindClose(hFind);
	return root;
}
