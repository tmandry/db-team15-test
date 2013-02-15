#ifndef DLLEXPORTINCLUDE_H_
#define DLLEXPORTINCLUDE_H_

#ifdef DBDLL_EXPORTS
#define DBDLL_API __declspec(dllexport)
#else
#define DBDLL_API __declspec(dllimport)
#endif

#endif
