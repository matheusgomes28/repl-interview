
#ifndef REPL_INTERPRETER_EXPORT_H
#define REPL_INTERPRETER_EXPORT_H

#ifdef REPL_INTERPRETER_STATIC_DEFINE
#  define REPL_INTERPRETER_EXPORT
#  define REPL_INTERPRETER_NO_EXPORT
#else
#  ifndef REPL_INTERPRETER_EXPORT
#    ifdef repl_interpreter_EXPORTS
        /* We are building this library */
#      define REPL_INTERPRETER_EXPORT 
#    else
        /* We are using this library */
#      define REPL_INTERPRETER_EXPORT 
#    endif
#  endif

#  ifndef REPL_INTERPRETER_NO_EXPORT
#    define REPL_INTERPRETER_NO_EXPORT 
#  endif
#endif

#ifndef REPL_INTERPRETER_DEPRECATED
#  define REPL_INTERPRETER_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef REPL_INTERPRETER_DEPRECATED_EXPORT
#  define REPL_INTERPRETER_DEPRECATED_EXPORT REPL_INTERPRETER_EXPORT REPL_INTERPRETER_DEPRECATED
#endif

#ifndef REPL_INTERPRETER_DEPRECATED_NO_EXPORT
#  define REPL_INTERPRETER_DEPRECATED_NO_EXPORT REPL_INTERPRETER_NO_EXPORT REPL_INTERPRETER_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef REPL_INTERPRETER_NO_DEPRECATED
#    define REPL_INTERPRETER_NO_DEPRECATED
#  endif
#endif

#endif /* REPL_INTERPRETER_EXPORT_H */
