//
//  FileQueue.cpp
//  NTFSTypeFixer
//
//  Created by Sun Junwen on 13-7-5.
//  Copyright (c) 2013 Sun Junwen. All rights reserved.
//

#include "FileQueue.h"

#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <list>
#include <set>

using namespace std;

typedef set<string> StrSet;

static StrList s_fileList;
static pthread_mutex_t s_mtxList = PTHREAD_MUTEX_INITIALIZER;

void LockFileQueue()
{
    pthread_mutex_lock(&s_mtxList);
}

void UnlockFileQueue()
{
    pthread_mutex_unlock(&s_mtxList);
}

/*
 Must LockFileQueue() first,
 and UnlockFileQueue() after finished opertion.
 */
string PopFile()
{
    if(s_fileList.size() == 0)
        return "";
    
    string ret;
    ret = s_fileList.front();
    s_fileList.pop_front();
    
    return ret;
}

StrList::iterator FileQueueBegin()
{
    return s_fileList.begin();
}

StrList::iterator InsertFile(const string& strFilePath)
{
    return InsertFile(strFilePath, s_fileList.begin());
}

/*
 Must LockFileQueue() first,
 and UnlockFileQueue() after finished opertion.
 */
StrList::iterator InsertFile(const string& strFilePath,
                             StrList::const_iterator itrWhere)
{
    return s_fileList.insert(itrWhere, strFilePath);
}

StrList::iterator InsertDir(const string& strDirPath,
                            bool fAddSubDir)
{
    StrList::iterator itrFile = FileQueueBegin();
    
    DIR *pDir;
    struct dirent *pDirent;
    
    pDir = opendir(strDirPath.c_str());
    if(pDir)
    {
        struct stat fileStat;
        while((pDirent = readdir(pDir)) != NULL)
        {
            string strPath(pDirent->d_name);
            if(strPath != "." && strPath != ".." &&
               strPath != ".DS_Store" && strPath != ".Trashes" &&
               strPath != ".TemporaryItems")
            {
                strPath = strDirPath + strPath;
                stat(strPath.c_str(), &fileStat);
                if(fileStat.st_mode & S_IFREG)
                    itrFile = InsertFile(strPath, itrFile);
                else if((fileStat.st_mode & S_IFDIR) && fAddSubDir)
                    itrFile = InsertFile(strPath, itrFile);
            }
        }
        
        closedir(pDir);
    }

    return itrFile;
}
