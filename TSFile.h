#include <string>

#ifdef ANDROID
#include <jni.h>
#include <android/asset_manager.h>
#endif

class TSFile 
{
public:
#ifdef ANDROID
	static AAssetManager *assetManager;
#endif
	static std::string packagingPath, writeablePath;
    static std::string getPath(const std::string& filename);
    static std::string getLocalPath(const std::string& filename);
	
    static char* getBytes(std::string filename, unsigned int &fileSize, bool nullTerminate = false, bool prePackaged = true);
    static bool putBytes(std::string filename, unsigned int fileSize, char* data);
    static bool remove(std::string filename);
};
