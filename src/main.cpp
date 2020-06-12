#include <iostream>
#include "lua.hpp"

int main()
{
    {
        lua_State* L = luaL_newstate(); // Create a new state.
        luaL_dostring(L, " x = 42"); // Execute lua code.
        // Get global variable called 'x' and push it to the start of the Lua stack.
        lua_getglobal(L, "x");
        lua_Number x = lua_tonumber(L, 1); // Get the value at the top of the stack.
        std::cout << "Lua says "<< x << std::endl;
        lua_close(L); // Deallocate the LUA state.
    }


    {
        // Types
        //  nil
        //  boolean
        //  light userdata, or a C pointer.
        //  number, of any kind.
        //  string, remember that they can be garbage collected.
        //  table, the only complex datatype. x = { "foo", "bar" }
        //  function
        //  userdata, stuff passed down from the C++ code. Need more examples.
        //  thread/coroutines.

        // The Stack
        lua_State* L = luaL_newstate();

        // Push numbers to the Lua Stack.
        lua_pushnumber(L, 42);  // ->[42]
        lua_pushnumber(L, 52);  // ->[52][42]
        lua_pushnumber(L, 64);  // ->[64][52][42]

        // Retrieve values from the stack.
        lua_Number x = lua_tonumber(L, 3); // Get second stack value, 52
        std::cout << "Lua says "<< x << std::endl;

        // It might be more convenient to index from the botton of the start.
        //      ->[64][52][42]
        //         3    2   1
        //        -1   -2  -3
        lua_Number xx = lua_tonumber(L, -1); // The last thing I pushed to the stack.
        std::cout << "The last thing I pushed to the stack was " << xx << std::endl;

        lua_remove(L, 2); // This should remove 52 from the stack.

        std::cout << "The first thing I pushed was " << lua_tonumber(L,1) << std::endl;


        lua_close(L);

    }

    {
        lua_State* L = luaL_newstate();
        lua_close(L);
    }
}