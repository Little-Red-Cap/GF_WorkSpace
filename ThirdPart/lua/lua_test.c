/*
官网下载lua资源包，下载地址如下：
    https://www.lua.org/
    https://github.com/rjpcomputing/luaforwindows/releases
    lua: Lua 国内镜像 (gitee.com)
 * */

#include <stdio.h>
#include "../lua/src//lua.h"
#include "../lua/src//lauxlib.h"
#include "../lua/src//lualib.h"

// Lua脚本
const char* lua_script = "\
    print(\"Hello World\") \
";


static int func_fun(lua_State *pState)
{
    (void) pState;
    for (char i = 0; i < 3; i++)
        printf("Hello World by %s\n", LUA_VERSION);
    return 1;
}

//luaL_Reg func = {.name = "func", .func = func_fun};

static const struct luaL_Reg func[] =
{
        {"func", func_fun},
        {NULL, NULL}
};

int main()
{
    // 初始化Lua状态机
    lua_State *Lua_obj = luaL_newstate();
    luaL_openlibs(Lua_obj);

    // 执行Lua脚本
    luaL_dostring(Lua_obj, lua_script);

    luaopen_base(Lua_obj);  // 注册基础函数
    luaL_setfuncs(Lua_obj, func, 0);// 注册自定义函数
    luaL_dostring(Lua_obj, "func()");

    // 释放Lua状态机
    lua_close(Lua_obj);

    return 0;
}
