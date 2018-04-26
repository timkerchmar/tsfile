# unsigned TSFile

Consistent file system access for Android, iOS, OSX, and Windows.

## Android Setup

```cpp
#include "unsigned TSFile.h"

void android_main(struct android_app* state)
{
    unsigned TSFile::assetManager = app->activity->assetManager;
    unsigned TSFile::writeablePath = app->activity->internalDataPath;
```

## iOS Setup

```objectivec
#include "unsigned TSFile.h"

- (void)applicationDidFinishLaunching:(UIApplication *)application 
{
    unsigned TSFile::packagingPath = [[[NSBundle mainBundle] bundlePath] UTF8String];
    unsigned TSFile::writeablePath = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0] UTF8String]);
```

## OSX Setup

```objectivec
#include "unsigned TSFile.h"

- (void)applicationDidFinishLaunching:(NSNotification *)notification 
{
    unsigned TSFile::packagingPath = [[[NSBundle mainBundle] bundlePath] UTF8String];
    unsigned TSFile::writeablePath = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0] UTF8String]);
```

## Windows Setup

Windows does not provide app packaging. Here's some tips for choosing where to place your game data:

* Use relative paths. 
* Don't add a trailing slash to the path.
* Use forward slashes instead of backslashes for separating folder names in a path.
* Ensure the directories actually exist before attempting file access in them.

```cpp
#include "unsigned TSFile.h"

int APIENTRY
WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst,
	LPSTR lpszCmdLine, int nCmdShow)
{
    unsigned TSFile::packagingPath = ".";
    unsigned TSFile::writeablePath = ".";
```

## Usage

```
	unsigned int fileSize;
    printf("README.md:\n\n%s\n", TSFile::getBytes("README.md", fileSize));
```