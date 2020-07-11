/*
    Taken from https://github.com/MayaPosch/Sarge.

    Copyright (c) 2019, Maya Posch
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
          notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
          notice, this list of conditions and the following disclaimer in the
          documentation and/or other materials provided with the distribution.
        * Neither the name of the <organization> nor the
          names of its contributors may be used to endorse or promote products
          derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include <map>
#include <memory>
#include <string>
#include <vector>


struct Argument
{
    Argument() : hasValue(false), parsed(false)
    { /* */
    }
    std::string arg_short;
    std::string arg_long;
    std::string description;
    bool hasValue;
    std::string value;
    bool parsed;
};


class Sarge
{
    std::vector<std::unique_ptr<Argument>> args;
    std::map<std::string, Argument *> argNames;
    bool parsed = false;
    int flagCounter = 0;
    std::string execName;
    std::string description;
    std::string usage;
    std::vector<std::string> textArguments;

public:
    void setArgument(std::string arg_short,
                     std::string arg_long,
                     std::string desc,
                     bool hasVal);

    void setArguments(std::vector<Argument> args);

    void setDescription(std::string desc)
    {
        this->description = desc;
    }

    void setUsage(std::string use)
    {
        this->usage = use;
    }

    bool parseArguments(int argc, char **argv);

    bool getFlag(std::string arg_flag, std::string &arg_value);

    bool exists(std::string arg_flag);

    bool getTextArgument(uint32_t index, std::string &value);

    void printHelp();

    int flagCount()
    {
        return flagCounter;
    }

    std::string executableName()
    {
        return execName;
    }
};
