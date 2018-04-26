#include "TSFile.h"

#ifdef ANDROID
#include <android/asset_manager_jni.h>
#endif

std::string TSFile::packagingPath("");
std::string TSFile::writeablePath("");

#ifdef ANDROID
AAssetManager *TSFile::assetManager = NULL;
#endif

std::string TSFile::getPackagingPath(const std::string& filename) {
    std::string returnVal = packagingPath;
    returnVal += "/";
    returnVal += filename;
    return returnVal;
}

std::string TSFile::getWritablePath(const std::string& filename) {
	std::string returnVal = writeablePath;
	returnVal += "/";
	returnVal += filename;
	return returnVal;
}

bool TSFile::remove(std::string filename) 
{
	return ::remove(getPath(filename).c_str()) == 0;
}

char* TSFile::getBytes(const std::string& filename, unsigned int &fileSize, bool nullTerminate, bool prePackaged) 
{
    std::string path = prePackaged ? getPackagingPath(filename) : getWritablePath(filename);

    FILE * pFile;
    size_t result;

#ifdef ANDROID
    AAsset *asset;
    if (prePackaged)
    {
        asset = AAssetManager_open(assetManager, filename.c_str(), AASSET_MODE_BUFFER);
        if (!asset) {
            printf("Unable to find asset %s\n", filename.c_str());
            return NULL;
        }
        fileSize = AAsset_getLength(asset);
    }
    else
    {
#endif
        pFile = fopen ( path.c_str() , "rb" );
        if (pFile == NULL) {
            printf("could not find file %s\n", filename.c_str());
            return NULL;
        }
        
        // obtain file size:
        fseek (pFile , 0, SEEK_END);
        fileSize = (unsigned int)ftell (pFile);
        rewind (pFile);
#ifdef ANDROID
    }
#endif

	// allocate memory to contain the whole file:
    char * buffer;
	size_t bufferSize = fileSize;
	if(nullTerminate) bufferSize++;
	buffer = (char*) malloc (sizeof(char) * bufferSize);
	
	if (buffer == NULL) {
		printf("Memory error %s\n", filename.c_str());
		return NULL;
	}

#ifdef ANDROID
    if (prePackaged)
    {
        char *src = (char *)AAsset_getBuffer(asset);
        memcpy(buffer, src, fileSize);
    }
    else
    {
#endif
        // copy the file into the buffer:
        result = fread (buffer,1,fileSize,pFile);
        if (result != fileSize) {
            printf("Expected to read %d bytes from %s, but only read %d bytes\n", (int)fileSize, filename.c_str(), (int)result);
            return NULL;
        }
        
        /* the whole file is now loaded in the memory buffer. */
        
        // terminate
        fclose (pFile);
#ifdef ANDROID
    }
#endif
	
	if(nullTerminate)
    {
		buffer[fileSize] = 0;
		fileSize++;
	}

	return buffer;
}

bool TSFile::putBytes(const std::string& filename, unsigned int fileSize, char* data)
{
    std::string path = getLocalPath(filename);
    
    FILE * pFile;
    size_t result;
    
    pFile = fopen ( path.c_str() , "wb" );
    if (pFile == NULL) {
        printf("could not open file %s for write\n", filename.c_str());
        return false;
    }
    
    // copy the file into the buffer:
    result = fwrite(data, 1, fileSize, pFile);
    if (result != fileSize) {
        printf("Expected to write %d bytes to %s, but only wrote %d bytes\n", (int)fileSize, filename.c_str(), (int)result);
        return false;
    }
    
    fclose (pFile);
    
    return true;
}
