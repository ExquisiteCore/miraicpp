// 注意: 本项目的所有源文件都必须是 UTF-8 编码

// 这是一个“反撤回”机器人
// 在群里回复 “/anti-recall enabled.” 或者 “撤回没用” 之后
// 如果有人在群里撤回，那么机器人会把撤回的内容再发出来

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <thread>
#include <regex>
#include <ctime>
#include "Group.h"
#include <mirai.h>
#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/chrono.h>
#include <nlohmann/json.hpp>

#include <cpr/cpr.h>
#include <httplib.h>
#include "myheader.h"




using namespace std;
using namespace Cyan;
using json = nlohmann::json;

int main()
{
#if defined(WIN32) || defined(_WIN32)
	// 切换代码页，让 CMD 可以显示 UTF-8 字符
	system("chcp 65001");    
#endif



	MiraiBot bot;
	SessionOptions opts = SessionOptions::FromJsonFile("./config.json");	

	while (true)
	{
		try
		{
			cout << "尝试与 mirai-api-http 建立连接..." << endl;
			bot.Connect(opts);
			break;
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
		}
		MiraiBot::SleepSeconds(1);
	}
	cout << "Bot Working..." << endl;


	//Message GroupMessage
	bot.On<Message>([&](Message gm)
		{

			string plain = gm.MessageChain.GetPlainText();
			string msg = plain;
			if (msg.find("照片") != string::npos)
			{
				GroupImage img = bot.UploadGroupImage("C:/Users/xiaol/Desktop/vue/1.png", false);
				gm.Reply(MessageChain().Image(img));
			}
			if (msg.find("查询玩家") != string::npos)
			{
				string xboxid = msg.substr(strlen("查询玩家"));
				if (xboxid.empty())
				{
					gm.Reply(MessageChain().Plain("使用方式：查询玩家[xboxid]"));
					return;
				}
				else
				{
					cpr::Response r = cpr::Get(cpr::Url{ "https://api.blackbe.work/openapi/v3/utils/xuid" },
						cpr::Parameters{ {"gamertag", xboxid} });
					json id = r.text;
					id = json::parse(r.text);
					auto gat1 = id["message"].get<std::string>();
					auto gat2 = id["data"].at("xuid").get<std::string>();

					string tx = gat1+"该玩家Xuid："+gat2;
					gm.Reply(MessageChain().Plain(tx));
					return;
				}
			}
			if (msg.find("查询服务器") != string::npos)
			{
				string host = msg.substr(strlen("查询服务器"));
				if (host.empty())
				{
					gm.Reply(MessageChain().Plain("使用方式：查询服务器[ip]"));
					return;
				}
				else
				{
					cpr::Response r = cpr::Get(cpr::Url{ "https://motdbe.blackbe.work/status_img" },
					cpr::Parameters{ {"host", host} });
					//string str = r.url;
					string Img = "https://motdbe.blackbe.work/status_img?host="+host;
					GroupImage motd = bot.UploadGroupImage(Img, true);
					gm.Reply(MessageChain().Image(motd));
					return;
				}
			}
		});

	// 在失去与mah的连接后重连
	bot.On<LostConnection>([&](LostConnection e)
		{
			cout << e.ErrorMessage << " (" << e.Code << ")" << endl;
			while (true)
			{
				try
				{
					cout << "尝试连接 mirai-api-http..." << endl;
					bot.Reconnect();
					cout << "与 mirai-api-http 重新建立连接!" << endl;
					break;
				}
				catch (const std::exception& ex)
				{
					cout << ex.what() << endl;
				}
				MiraiBot::SleepSeconds(1);
			}
		});

	string cmd;
	while (cin >> cmd)
	{
		if (cmd == "exit")
		{
			// 程序结束前必须调用 Disconnect，否则 mirai-api-http 会内存泄漏。
			bot.Disconnect();
			break;
		}
	}

	return 0;
}