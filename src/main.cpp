#include <iostream>
#include "lua.hpp"

int main()
{
    {
        // https://www.youtube.com/watch?v=9OYK10r7iqE&list=PLLwK93hM93Z3nhfJyRRWGRXHaXgNX0Itk&index=2
        lua_State* L = luaL_newstate(); // Create a new state.
        luaL_dostring(L, " x = 42"); // Execute lua code.
        // Get global variable called 'x' and push it to the start of the Lua stack.
        lua_getglobal(L, "x");
        lua_Number x = lua_tonumber(L, 1); // Get the value at the top of the stack.
        std::cout << "Lua says "<< x << std::endl;
        lua_close(L); // Deallocate the LUA state.
    }


    {
        // https://www.youtube.com/watch?v=hP0NCTU81A4&list=PLLwK93hM93Z3nhfJyRRWGRXHaXgNX0Itk&index=3
        //
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
        // https://www.youtube.com/watch?v=tPgPaT7Egds&list=PLLwK93hM93Z3nhfJyRRWGRXHaXgNX0Itk&index=4
        // Functions in LUA are lambdas, which are global.
        // Remember that all the communication is via the LUA stack.
        auto LUA_FILE = "function Return4() return 4 end";

        /**
         * The previous Lua code would be equivalent to:
         *  Return4 = function return 4 end
         *  */

        lua_State* L = luaL_newstate();
        luaL_dostring(L, LUA_FILE);     // This will initialize the LUA script.

        // No we want to get the function and put it onto the lua stack.
        lua_getglobal(L, "Return4");
        if (lua_isfunction(L, -1))
        {
            // This function is expecting the top of the stack to be
            // a function.
            // (<state>, <number of arguments>, <number of return values>, <error handler> )
            lua_pcall(L, 0, 1, 0);
            // Lua is communicating through the stack, so we might return as many variables
            // as we want.
            // The last thing pushed onto the stack should be the return values for the called
            // function.
            lua_Number shouldBe4 = lua_tonumber(L, -1);
            std::cout << "shouldBe4 = " << shouldBe4 << std::endl;
        }
        lua_close(L);
    }

    {
        // https://youtu.be/_727PGdLPH8?list=PLLwK93hM93Z3nhfJyRRWGRXHaXgNX0Itk
        auto LUA_FILE = "function Pythagoras(a, b) return (a*a)+(b*b), a, b end";
        const int NUM_ARGS    = 2;
        const int NUM_RETURNS = 3;
        lua_State* L = luaL_newstate();
        luaL_dostring(L, LUA_FILE);     // This will initialize the LUA script.

        // No we want to get the function and put it onto the lua stack.
        lua_getglobal(L, "Pythagoras");
        if (lua_isfunction(L, -1))
        {
            lua_pushnumber(L, 3);
            lua_pushnumber(L, 4);
            lua_pcall(L, NUM_ARGS, NUM_RETURNS, 0);
             // Current Stack -> [b][a][(a*a)+(b*b)]
            lua_Number c = lua_tonumber(L, -3); // Get return value.
            lua_Number a = lua_tonumber(L, -2);
            lua_Number b = lua_tonumber(L, -1);
            std::cout << "c = " << c << ", ";
            std::cout << "a = " << a << ", ";
            std::cout << "b = " << b << std::endl;
        }
        lua_close(L);
    }
}