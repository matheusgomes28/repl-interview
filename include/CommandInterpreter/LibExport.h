/**
 * @file LibExport.h
 * @brief Defines macros for exporting library funtions.
 */

#ifndef COMMANDINTERPRETER_LIBEXPORT_H
#define COMMANDINTERPRETER_LIBEXPORT_H

#if defined(_WIN32) || defined(_WIN64)

#ifdef CMDINT_BUILD_PRODUCER
// The platform is Windows and the library is being built.
// Make sure external functions are exported.
#define CMDINT_API __declspec(dllexport)
#else
// The platform is Windows and the library is being used
// rather than built. Declare external functions as being
// imported by the user's application.
#define CMDINT_API __declspec(dllimport)
#endif // CMDINT_BUILD_PRODUCER

#else

// We're not building on Windows, so we don't need __declspec.
#define CMDINT_API

#endif // defined(_WIN32) || defined(_WIN64)

#endif // COMMANDINTERPRETER_LIBEXPORT_H
