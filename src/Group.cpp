#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <thread>
#include <regex>
#include <ctime>

#include <mirai.h>
#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/chrono.h>
#include <nlohmann/json.hpp>
#include "Group.h"
#include <cpr/cpr.h>

using namespace std;
using namespace Cyan;
using json = nlohmann::json;
MiraiBot bot;
void GroupMsgProcessor(GroupMessage gm)
{

}