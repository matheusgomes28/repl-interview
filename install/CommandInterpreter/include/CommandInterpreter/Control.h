/**
 * @file Control.h
 * @brief Defines functions used to control the overall state of the library.
 */

#ifndef COMMANDINTERPRETER_CONTROL_H
#define COMMANDINTERPRETER_CONTROL_H

#include "CommandInterpreter/LibExport.h"

/**
 * @brief Initialises resources within the library.
 *
 * Any calls to library functions will be ignored if the library
 * is not initialised.
 *
 * A call to this function should be paired eventually with a call
 * to @a CmdInt_ShutDown(), in order to clean up the resources.
 */
CMDINT_API void CmdInt_Initialise();

/**
 * @brief Cleans up resources within the library.
 *
 * This function should be called once the library has finished
 * being used.
 */
CMDINT_API void CmdInt_ShutDown();

/**
 * @brief Invokes internal processing on the library.
 *
 * This function should be called regularly in order to process commands.
 */
CMDINT_API void CmdInt_Poll();

#endif // COMMANDINTERPRETER_CONTROL_H
