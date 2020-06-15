#include <iostream>
#include "lua.hpp"
#include <assert.h>
/**
 * Hello fellow copy-paster, here is the result of my work after following that
 * course on Youtube.
 *
 * This is one of those times where the trip has more value than the destination,
 * look up the original code from the maker of the video series at:
 *      https://github.com/davepoo/EmbeddingLuaTutorial
 *
 * Happy Hacking
*/
void printVideoSeparator(int n)
{
    std::cout << "------------- Video " << n << " -------------" << std::endl;
}

int main()
{
    {
        printVideoSeparator(2);
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
        printVideoSeparator(3);
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
        printVideoSeparator(4);
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
        printVideoSeparator(5);
        // https://youtu.be/_727PGdLPH8?list=PLLwK93hM93Z3nhfJyRRWGRXHaXgNX0Itk
        auto LUA_FILE = "function Pythagoras(a, b) return (a*a)+(b*b), a, b end";
        const int NUM_ARGS    = 2;
        const int NUM_RETURNS = 3;
        lua_State* L = luaL_newstate();
        luaL_dostring(L, LUA_FILE);     // This will initialize the LUA script.

        // Now we want to get the function and put it onto the lua stack.
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

    {
        // https://youtu.be/UOdcaIe9Cqg?list=PLLwK93hM93Z3nhfJyRRWGRXHaXgNX0Itk
        printVideoSeparator(6);
        // Return the number of values to leave on the stack.
        std::cout << "Native Pythagoras: ";
        auto NativePythagoras = [](lua_State* L) -> int
        {
            /* We expect the arguments to be on the stack. */
            lua_Number a = lua_tonumber(L, -2);
            lua_Number b = lua_tonumber(L, -1);
            lua_Number csqr = (a*a) + (b*b);
            /* Execute normal code in C++, push result(s) to the stack. */
            lua_pushnumber(L, csqr);
            return 1; // Number of results to the stack.
        };
        lua_State* L = luaL_newstate();
        lua_pushcfunction(L, NativePythagoras); // Push function to the stack.
        lua_setglobal(L, "NativePythagoras");   // Give it a name.

        auto LUA_FILE = R"(
            function Pythagoras(a, b)
                csqr = NativePythagoras(a, b)
                return csqr, a, b
            end
            )";

        const int NUM_ARGS    = 2;
        const int NUM_RETURNS = 3;
        luaL_dostring(L, LUA_FILE);     // This will initialize the LUA script.

        // Now we want to get the function and put it onto the lua stack.
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

    {
        printVideoSeparator(7);

        typedef struct
        {
            int x;
            int y;

            void Move(int velX, int velY)
            {
                x += velX;
                y += velY;
            }
        } Sprite;

        auto CreateSprite = [](lua_State* L) -> int
        {
            /*
                We are going to create a new type. Lua is responsible for destroying it.
                If the variable goes out of scope then it will be garbage collected and
                the memory will be freed.
            */
            Sprite* sprite = (Sprite*) lua_newuserdata(L, sizeof(Sprite));
            sprite->x = 0;
            sprite->y = 0;
            // This type is now on the Lua stack.
            return 1; // Number of return values on the stack.
        };

        auto MoveSprite = [](lua_State* L) -> int
        {
            /* Assuming a Sprite is given as argument, third element on the stack. */
            Sprite* sprite = (Sprite*) lua_touserdata(L, -3);
            /* Second and first argumets on the stack are the values to assign to the
               sprite.
            */
            lua_Number velX = lua_tonumber(L, -1);
            lua_Number velY = lua_tonumber(L, -2);
            sprite->Move((int) velX, (int) velY);
            return 0; /* Return nothing. */
        };

        auto LUA_FILE = R"(
            sprite = CreateSprite()
            MoveSprite(sprite, 5, 7)
        )";
        lua_State* L = luaL_newstate();
        // Insert native code into the stack.
        lua_pushcfunction(L, CreateSprite);
        // Give the value on the stack, a function, a globally identifiable name.
        lua_setglobal(L, "CreateSprite");
        // Do the same thing with the MoveSprite code. Push it, name it.
        lua_pushcfunction(L, MoveSprite);
        lua_setglobal(L, "MoveSprite");

        // Now both functions are globally accessible from any LUA
        // function.

        // Process code given as a string.
        luaL_dostring(L, LUA_FILE);
        // Retrieve global function into the stack for execution.
        lua_getglobal(L, "sprite");
        if (lua_isuserdata(L, -1))
        {
            std::cout << "We got a sprite from Lua." << std::endl;
            // Then retrieve it.
            Sprite* sprite = (Sprite*) lua_touserdata(L, -1);
            std::cout << "Sprite: " << sprite->x << " " << sprite->y << std::endl;
        }
        else
        {
            std::cout << "We DIDNT get a sprite from Lua. " << std::endl;
        }
        lua_close(L);
    }

    {
        printVideoSeparator(8);
        //https://youtu.be/bSL-3081FYI?list=PLLwK93hM93Z3nhfJyRRWGRXHaXgNX0Itk
        // Tables
        auto LUA_FILE = R"(
            x = { dave = "busy", ian = "idle" }
        )";

        lua_State* L = luaL_newstate();

        // Load Lua code.
        luaL_dostring(L, LUA_FILE);
        // Push global table 'x' into stack.
        lua_getglobal(L, "x");

        assert(lua_istable(L, -1)); // ->[x]
        /* Now we need to push the keys for the values we are interested on. */
        lua_pushstring(L, "dave"); // ->['dave'][x]
        lua_gettable(L, -2); // Pop the key 'dave', push the value.
        assert (lua_isstring(L, -1)); // "busy" should be a string.
        const char* daveIs = lua_tostring(L, -1); // get 'busy'.
        /*
        Remember that when you get a pointer from LUA, it is a pointer to a value
        within the Lua memory, that is subject to garbage collection, so make sure to
        copy it to the current scope if you want to save it for later.
        */
        std::cout << "daveIs = " << daveIs << std::endl;

        /* Syntactic sugar for what we did before. */
        lua_getglobal(L, "x");
        lua_getfield(L, -1, "ian");
        const char* ianIs = lua_tostring(L, -1);
        std::cout << "ianIs = " << ianIs << std::endl;

        /* Push new value to the table. */


        lua_close(L); /* x will be garbage collected. */
    }
}