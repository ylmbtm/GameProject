#ifndef __CMD_CONNECTION_TYPE_H__
#define __CMD_CONNECTION_TYPE_H__

enum ConnectionType
{
	TYPE_UNKNOW = 0,
	TYPE_SVR_CENTER,	//  中心服务器
	TYPE_SVR_GAME,		//	游戏服务器
	TYPE_SVR_PROXY,		//  代理服务器
	TYPE_SVR_LOGIN,		//	登录服务器
	TYPE_SVR_DATABASE,	//  数据库服务器
	TYPE_SVR_STATISTICS,//  统计服务器
	TYPE_SVR_WORLDSVR,	//  世界服务器
	TYPE_CLT_PLAYER		//  客户端
};









#endif /* __CMD_CONNECTION_TYPE_H__ */
