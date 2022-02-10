#pragma once
#include "pch.h"
#include "framework.h"

class CServerSocket
{
public:
	static CServerSocket* getInstance() {
		if (m_instance == NULL)
		{
			m_instance = new CServerSocket();
		}
		return m_instance;
	}
	bool InitSocket() {
		//TODO:检查是否连接成功
		if (m_socket == INVALID_SOCKET)
		{
			return false;
		}
		sockaddr_in serv_adr;
		memset(&serv_adr, 0, sizeof(serv_adr));
		serv_adr.sin_family = AF_INET;
		serv_adr.sin_addr.S_un.S_addr = INADDR_ANY;
		serv_adr.sin_port = htons(9527);
		//绑定
		if (bind(m_socket, (sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
			return false;
		}
		//TODO
		if (listen(m_socket, 1) == -1) {
			return false;
		}
		return true;
	}
	bool AcceptClient() {
		sockaddr_in client_adr;
		int client_size = sizeof(client_adr);
		m_client = accept(m_socket, (sockaddr*)&client_adr, &client_size);
		if (m_client == -1) return false;
		return true;
	}
	int DealCommand() {
		if (m_client == -1)
		{
			return false;
		}
		while (true)
		{
			char buffer[1024] = "";
			int len = recv(m_client, buffer, sizeof(buffer), 0);
			if (len <= 0)
			{
				return -1;
			}
			//TODO:处理命令
		}
	}
	bool Send(const char* pData, int nSize) {
		if (m_client == -1)
		{
			return false;
		}
		return send(m_client, pData, nSize, 0) > 0;
	}
private:
	SOCKET m_socket;
	SOCKET m_client;
	CServerSocket& operator = (const CServerSocket& ss) {};
	CServerSocket(const CServerSocket& ss) {
		m_socket = ss.m_socket;
		m_client = ss.m_client;
	}
	CServerSocket() {
		m_client = INVALID_SOCKET;
		if (InitSoketEnv() == FALSE) {
			MessageBox(NULL, _T("初始化socket失败"), _T("初始化错误"), MB_OK | MB_ICONERROR);
			exit(0);
		}
		m_socket = socket(PF_INET, SOCK_STREAM, 0);

	}
	~CServerSocket() {
		closesocket(m_socket);
		WSACleanup();
	}
	BOOL InitSoketEnv() {
		WSADATA data;
		if (WSAStartup(MAKEWORD(1, 1), &data) != 0) {
			return FALSE;
		}
		else {
			return TRUE;
		}
	}
	static CServerSocket* m_instance;
	void realeaseInstance() {
		if (m_instance != NULL)
		{
			CServerSocket* tmp = m_instance;
			m_instance = NULL;
			delete tmp;
		}
	}
	class CHelper
	{
	public:
		CHelper() {
			CServerSocket::getInstance();
		}
		~CHelper() {

		}
	};
	static CHelper m_helper;
};
extern CServerSocket server;
