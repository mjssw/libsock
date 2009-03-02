// (C) Copyright 2008 - 2009 by CCTEC ENGINEERING CO., LTD
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//

//-----------------------------------------------------------------------------
//----- $Id$
//-----------------------------------------------------------------------------

#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>

namespace CCTEC 
{
    // TODO: Thread-local storage.

    // TODO: Is file exists. 
    inline bool IsFileExists(const char *filename) 
    {
        struct stat buf;

        if (stat(filename, &buf) < 0) 
        {
            if (errno == ENOENT) return false;
        }
        return true;
    }

    // TODO: To get the number of processors.
    inline unsigned int GetProcessorNum() 
    {
        FILE *pStream = NULL;
        char *pLine = NULL;
        int pLineSize = 1024;

        if (IsFileExists("/proc/cpuinfo")) 
        {
            pStream = popen("cat /proc/cpuinfo | grep processor | awk '{a++} END {print a}'", 
                "r");
            pLine = new char[pLineSize];
            if (fgets(pLine, pLineSize, pStream)) 
            {
#if DEBUG
                printf("DEBUG at %d: GetProcessorNum %s\n", __LINE__, pLine);
#endif
                if (strlen(pLine)) return atoi(pLine);
            }
            if (pLine) delete pLine; pLine = NULL;
            if (pStream) pclose(pStream); pStream = NULL;
        }
        return 1;
    };
};

#endif // __THREADPOOL_H__

