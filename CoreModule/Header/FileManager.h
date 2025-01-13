#pragma once
#include "core_define.h"

namespace GameEngine
{
	class COREMODULE_API FileManager
	{
	private:
		FileManager() : m_Root(nullptr) {}
		~FileManager() { delete m_Root; }

	public:
		DECLARE_SINGLETON(FileManager)

		void 				Build_DirectoryTree(const std::string& _directory);
		DirectoryTreeNode* 	Get_RootDirectory() const { return m_Root; }
		DirectoryTreeNode* 	Find_DirectoryNode(DirectoryTreeNode* _root, const std::wstring& _path);

		bool				LoadSceneData(const std::wstring& _path);
		void				Load_SingleTexture(const DirectoryTreeNode* _root);
		void				Load_MultiTexture(const DirectoryTreeNode* _root);
		void 				Release();

	private:
		DirectoryTreeNode* 	build_DirectoryTree(const std::wstring& _directory, const std::wstring& _parentPath = L"");

	private:
		DirectoryTreeNode* 	m_Root;
	};

}
