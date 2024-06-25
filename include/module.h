// ⚡🔌 Arduino-CTL 🔌⚡
//
// Copyright (C) 2024 ixaxaar
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef MODULE_H
#define MODULE_H

#include <Arduino.h>
#include <vector>

/**
 * @brief Struct to hold information about a function
 *
 * This struct contains the name of a function and its parameters.
 */
struct FunctionInfo
{
    std::string name;                                        ///< Name of the function
    std::vector<std::pair<std::string, std::string>> params; ///< Vector of parameter name-type pairs
};

/**
 * @brief Interface for Arduino-CTL modules
 *
 * This interface defines the common functionality that all Arduino-CTL modules should implement.
 */
class ModuleInterface
{
public:
    /**
     * @brief Initialize the module
     * @param params Vector of parameter name-value pairs for initialization
     */
    virtual void init(const std::vector<std::pair<std::string, std::string>> &params) = 0;

    /**
     * @brief De-initialize the module
     */
    virtual void deinit() = 0;

    /**
     * @brief Execute a command on the module
     * @param command The command to execute
     * @param params Vector of parameter name-value pairs for the command
     * @return A pair containing the return type as a string and a void pointer to the return value
     */
    virtual std::pair<std::string, void *> execute(const std::string &command, const std::vector<std::pair<std::string, std::string>> &params) = 0;

    /**
     * @brief Get information about the functions supported by this module
     * @return Vector of FunctionInfo structs describing the supported functions
     */
    virtual std::vector<FunctionInfo> getSupportedFunctions() = 0;

    /**
     * @brief Virtual destructor
     */
    virtual ~ModuleInterface() {}
};

#endif // MODULE_H
