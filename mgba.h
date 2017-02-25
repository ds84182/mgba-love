// This file assumes the following
  // MINIMAL_CORE is not defined
  // PATH_MAX is 4096
  // off_t == intptr_t
  // M_CORE_GBA and M_CORE_GB are defined
  // USE_DEBUGGERS is defined
// Optimally, you should compile with these options by default.
// However, PATH_MAX may need to be changed manually for your platform.

// mgba-util/table.h

struct TableList;

struct Table {
	struct TableList* table;
	size_t tableSize;
	size_t size;
	void (*deinitializer)(void*);
};

void TableInit(struct Table*, size_t initialSize, void (deinitializer(void*)));
void TableDeinit(struct Table*);

void* TableLookup(const struct Table*, uint32_t key);
void TableInsert(struct Table*, uint32_t key, void* value);

void TableRemove(struct Table*, uint32_t key);
void TableClear(struct Table*);

void TableEnumerate(const struct Table*, void (handler(uint32_t key, void* value, void* user)), void* user);
size_t TableSize(const struct Table*);

void HashTableInit(struct Table* table, size_t initialSize, void (deinitializer(void*)));
void HashTableDeinit(struct Table* table);

void* HashTableLookup(const struct Table*, const char* key);
void HashTableInsert(struct Table*, const char* key, void* value);

void HashTableRemove(struct Table*, const char* key);
void HashTableClear(struct Table*);

void HashTableEnumerate(const struct Table*, void (handler(const char* key, void* value, void* user)), void* user);
size_t HashTableSize(const struct Table*);

// mgba-util/configuration.h

struct VFile;

struct Configuration {
	struct Table sections;
	struct Table root;
};

void ConfigurationInit(struct Configuration*);
void ConfigurationDeinit(struct Configuration*);

void ConfigurationSetValue(struct Configuration*, const char* section, const char* key, const char* value);
void ConfigurationSetIntValue(struct Configuration*, const char* section, const char* key, int value);
void ConfigurationSetUIntValue(struct Configuration*, const char* section, const char* key, unsigned value);
void ConfigurationSetFloatValue(struct Configuration*, const char* section, const char* key, float value);

bool ConfigurationHasSection(const struct Configuration*, const char* section);
const char* ConfigurationGetValue(const struct Configuration*, const char* section, const char* key);

void ConfigurationClearValue(struct Configuration*, const char* section, const char* key);

bool ConfigurationRead(struct Configuration*, const char* path);
bool ConfigurationReadVFile(struct Configuration*, struct VFile* vf);
bool ConfigurationWrite(const struct Configuration*, const char* path);
bool ConfigurationWriteSection(const struct Configuration*, const char* path, const char* section);

void ConfigurationEnumerateSections(const struct Configuration* configuration, void (*handler)(const char* sectionName, void* user), void* user);

// mgba/core/config.h

struct mCoreConfig {
	struct Configuration configTable;
	struct Configuration defaultsTable;
	struct Configuration overridesTable;
	char* port;
};

struct mCoreOptions {
	char* bios;
	bool skipBios;
	bool useBios;
	int logLevel;
	int frameskip;
	bool rewindEnable;
	int rewindBufferCapacity;
	float fpsTarget;
	size_t audioBuffers;
	unsigned sampleRate;

	int fullscreen;
	int width;
	int height;
	bool lockAspectRatio;
	bool resampleVideo;
	bool suspendScreensaver;
	char* shader;

	char* savegamePath;
	char* savestatePath;
	char* screenshotPath;
	char* patchPath;

	int volume;
	bool mute;

	bool videoSync;
	bool audioSync;
};

void mCoreConfigInit(struct mCoreConfig*, const char* port);
void mCoreConfigDeinit(struct mCoreConfig*);

bool mCoreConfigLoad(struct mCoreConfig*);
bool mCoreConfigSave(const struct mCoreConfig*);
bool mCoreConfigLoadPath(struct mCoreConfig*, const char* path);
bool mCoreConfigSavePath(const struct mCoreConfig*, const char* path);

void mCoreConfigMakePortable(const struct mCoreConfig*);
void mCoreConfigDirectory(char* out, size_t outLength);

const char* mCoreConfigGetValue(const struct mCoreConfig*, const char* key);
bool mCoreConfigGetIntValue(const struct mCoreConfig*, const char* key, int* value);
bool mCoreConfigGetUIntValue(const struct mCoreConfig*, const char* key, unsigned* value);
bool mCoreConfigGetFloatValue(const struct mCoreConfig*, const char* key, float* value);

void mCoreConfigSetValue(struct mCoreConfig*, const char* key, const char* value);
void mCoreConfigSetIntValue(struct mCoreConfig*, const char* key, int value);
void mCoreConfigSetUIntValue(struct mCoreConfig*, const char* key, unsigned value);
void mCoreConfigSetFloatValue(struct mCoreConfig*, const char* key, float value);

void mCoreConfigSetDefaultValue(struct mCoreConfig*, const char* key, const char* value);
void mCoreConfigSetDefaultIntValue(struct mCoreConfig*, const char* key, int value);
void mCoreConfigSetDefaultUIntValue(struct mCoreConfig*, const char* key, unsigned value);
void mCoreConfigSetDefaultFloatValue(struct mCoreConfig*, const char* key, float value);

void mCoreConfigSetOverrideValue(struct mCoreConfig*, const char* key, const char* value);
void mCoreConfigSetOverrideIntValue(struct mCoreConfig*, const char* key, int value);
void mCoreConfigSetOverrideUIntValue(struct mCoreConfig*, const char* key, unsigned value);
void mCoreConfigSetOverrideFloatValue(struct mCoreConfig*, const char* key, float value);

void mCoreConfigCopyValue(struct mCoreConfig* config, const struct mCoreConfig* src, const char* key);

void mCoreConfigMap(const struct mCoreConfig* config, struct mCoreOptions* opts);
void mCoreConfigLoadDefaults(struct mCoreConfig* config, const struct mCoreOptions* opts);

struct Configuration* mCoreConfigGetInput(struct mCoreConfig*);
struct Configuration* mCoreConfigGetOverrides(struct mCoreConfig*);
const struct Configuration* mCoreConfigGetOverridesConst(const struct mCoreConfig*);

void mCoreConfigFreeOpts(struct mCoreOptions* opts);

// mgba/core/directories.h

struct VDir;

struct mDirectorySet {
             // PATH_MAX
	char baseName[4096];
	struct VDir* base;
	struct VDir* archive;
	struct VDir* save;
	struct VDir* patch;
	struct VDir* state;
	struct VDir* screenshot;
};

void mDirectorySetInit(struct mDirectorySet* dirs);
void mDirectorySetDeinit(struct mDirectorySet* dirs);

void mDirectorySetAttachBase(struct mDirectorySet* dirs, struct VDir* base);
void mDirectorySetDetachBase(struct mDirectorySet* dirs);

struct VFile* mDirectorySetOpenPath(struct mDirectorySet* dirs, const char* path, bool (*filter)(struct VFile*));
struct VFile* mDirectorySetOpenSuffix(struct mDirectorySet* dirs, struct VDir* dir, const char* suffix, int mode);

struct mCoreOptions;
void mDirectorySetMapOptions(struct mDirectorySet* dirs, const struct mCoreOptions* opts);

// mgba/core/input.h

struct Configuration;

enum mInputHat {
	M_INPUT_HAT_NEUTRAL = 0,
	M_INPUT_HAT_UP = 1,
	M_INPUT_HAT_RIGHT = 2,
	M_INPUT_HAT_DOWN = 4,
	M_INPUT_HAT_LEFT = 8
};

struct mInputHatBindings {
	int up;
	int right;
	int down;
	int left;
};

struct mInputPlatformInfo {
	const char* platformName;
	const char** keyId;
	size_t nKeys;
	struct mInputHatBindings hat;
};

struct mInputMap {
	struct mInputMapImpl* maps;
	size_t numMaps;
	const struct mInputPlatformInfo* info;
};

struct mInputAxis {
	int highDirection;
	int lowDirection;
	int32_t deadHigh;
	int32_t deadLow;
};

void mInputMapInit(struct mInputMap*, const struct mInputPlatformInfo* info);
void mInputMapDeinit(struct mInputMap*);

int mInputMapKey(const struct mInputMap*, uint32_t type, int key);
int mInputMapKeyBits(const struct mInputMap* map, uint32_t type, uint32_t bits, unsigned offset);
void mInputBindKey(struct mInputMap*, uint32_t type, int key, int input);
int mInputQueryBinding(const struct mInputMap*, uint32_t type, int input);
void mInputUnbindKey(struct mInputMap*, uint32_t type, int input);

int mInputMapAxis(const struct mInputMap*, uint32_t type, int axis, int value);
int mInputClearAxis(const struct mInputMap*, uint32_t type, int axis, int keys);
void mInputBindAxis(struct mInputMap*, uint32_t type, int axis, const struct mInputAxis* description);
void mInputUnbindAxis(struct mInputMap*, uint32_t type, int axis);
void mInputUnbindAllAxes(struct mInputMap*, uint32_t type);
const struct mInputAxis* mInputQueryAxis(const struct mInputMap*, uint32_t type, int axis);
void mInputEnumerateAxes(const struct mInputMap*, uint32_t type, void (handler(int axis, const struct mInputAxis* description, void* user)), void* user);

int mInputMapHat(const struct mInputMap*, uint32_t type, int id, int direction);
void mInputBindHat(struct mInputMap*, uint32_t type, int id, const struct mInputHatBindings* bindings);
bool mInputQueryHat(const struct mInputMap*, uint32_t type, int id, struct mInputHatBindings* bindings);
void mInputUnbindHat(struct mInputMap*, uint32_t type, int id);
void mInputUnbindAllHats(struct mInputMap*, uint32_t type);

void mInputMapLoad(struct mInputMap*, uint32_t type, const struct Configuration*);
void mInputMapSave(const struct mInputMap*, uint32_t type, struct Configuration*);

bool mInputProfileLoad(struct mInputMap*, uint32_t type, const struct Configuration*, const char* profile);
void mInputProfileSave(const struct mInputMap*, uint32_t type, struct Configuration*, const char* profile);

const char* mInputGetPreferredDevice(const struct Configuration*, const char* platformName, uint32_t type, int playerId);
void mInputSetPreferredDevice(struct Configuration*, const char* platformName, uint32_t type, int playerId, const char* deviceName);

const char* mInputGetCustomValue(const struct Configuration* config, const char* platformName, uint32_t type, const char* key,
                                 const char* profile);
void mInputSetCustomValue(struct Configuration* config, const char* platformName, uint32_t type, const char* key, const char* value,
                          const char* profile);

// mgba/core/interface.h

typedef uint32_t color_t;

struct blip_t;

struct mCoreCallbacks {
	void* context;
	void (*videoFrameStarted)(void* context);
	void (*videoFrameEnded)(void* context);
	void (*coreCrashed)(void* context);
};

struct mAVStream {
	void (*videoDimensionsChanged)(struct mAVStream*, unsigned width, unsigned height);
	void (*postVideoFrame)(struct mAVStream*, const color_t* buffer, size_t stride);
	void (*postAudioFrame)(struct mAVStream*, int16_t left, int16_t right);
	void (*postAudioBuffer)(struct mAVStream*, struct blip_t* left, struct blip_t* right);
};

struct mKeyCallback {
	uint16_t (*readKeys)(struct mKeyCallback*);
};

struct mStopCallback {
	void (*stop)(struct mStopCallback*);
};

struct mRotationSource {
	void (*sample)(struct mRotationSource*);

	int32_t (*readTiltX)(struct mRotationSource*);
	int32_t (*readTiltY)(struct mRotationSource*);

	int32_t (*readGyroZ)(struct mRotationSource*);
};

struct mRTCSource {
	void (*sample)(struct mRTCSource*);

	uint64_t (*unixTime)(struct mRTCSource*);
};

enum mRTCGenericType {
	RTC_NO_OVERRIDE,
	RTC_FIXED,
	RTC_FAKE_EPOCH
};

struct mRTCGenericSource {
	struct mRTCSource d;
	struct mCore* p;
	enum mRTCGenericType override;
	int64_t value;
};

void mRTCGenericSourceInit(struct mRTCGenericSource* rtc, struct mCore* core);

struct mRumble {
	void (*setRumble)(struct mRumble*, int enable);
};

// mgba/core/core.h

enum mPlatform {
	PLATFORM_NONE = -1,
	PLATFORM_GBA,
	PLATFORM_GB,
};

struct mRTCSource;
struct mCoreConfig;
struct mCoreSync;
struct mStateExtdata;
struct mCore {
	void* cpu;
	void* board;
	struct mDebugger* debugger;

	struct mDirectorySet dirs;
	struct mInputMap inputMap;
	struct mCoreConfig config;
	struct mCoreOptions opts;

	bool (*init)(struct mCore*);
	void (*deinit)(struct mCore*);

	enum mPlatform (*platform)(const struct mCore*);

	void (*setSync)(struct mCore*, struct mCoreSync*);
	void (*loadConfig)(struct mCore*, const struct mCoreConfig*);

	void (*desiredVideoDimensions)(struct mCore*, unsigned* width, unsigned* height);
	void (*setVideoBuffer)(struct mCore*, color_t* buffer, size_t stride);

	void (*getPixels)(struct mCore*, const void** buffer, size_t* stride);
	void (*putPixels)(struct mCore*, const void* buffer, size_t stride);

	struct blip_t* (*getAudioChannel)(struct mCore*, int ch);
	void (*setAudioBufferSize)(struct mCore*, size_t samples);
	size_t (*getAudioBufferSize)(struct mCore*);

	void (*setCoreCallbacks)(struct mCore*, struct mCoreCallbacks*);
	void (*setAVStream)(struct mCore*, struct mAVStream*);

	bool (*isROM)(struct VFile* vf);
	bool (*loadROM)(struct mCore*, struct VFile* vf);
	bool (*loadSave)(struct mCore*, struct VFile* vf);
	bool (*loadTemporarySave)(struct mCore*, struct VFile* vf);
	void (*unloadROM)(struct mCore*);
	void (*checksum)(const struct mCore*, void* data, enum mCoreChecksumType type);

	bool (*loadBIOS)(struct mCore*, struct VFile* vf, int biosID);
	bool (*selectBIOS)(struct mCore*, int biosID);

	bool (*loadPatch)(struct mCore*, struct VFile* vf);

	void (*reset)(struct mCore*);
	void (*runFrame)(struct mCore*);
	void (*runLoop)(struct mCore*);
	void (*step)(struct mCore*);

	size_t (*stateSize)(struct mCore*);
	bool (*loadState)(struct mCore*, const void* state);
	bool (*saveState)(struct mCore*, void* state);

	void (*setKeys)(struct mCore*, uint32_t keys);
	void (*addKeys)(struct mCore*, uint32_t keys);
	void (*clearKeys)(struct mCore*, uint32_t keys);

	int32_t (*frameCounter)(const struct mCore*);
	int32_t (*frameCycles)(const struct mCore*);
	int32_t (*frequency)(const struct mCore*);

	void (*getGameTitle)(const struct mCore*, char* title);
	void (*getGameCode)(const struct mCore*, char* title);

	void (*setRTC)(struct mCore*, struct mRTCSource*);
	void (*setRotation)(struct mCore*, struct mRotationSource*);
	void (*setRumble)(struct mCore*, struct mRumble*);

	uint32_t (*busRead8)(struct mCore*, uint32_t address);
	uint32_t (*busRead16)(struct mCore*, uint32_t address);
	uint32_t (*busRead32)(struct mCore*, uint32_t address);

	void (*busWrite8)(struct mCore*, uint32_t address, uint8_t);
	void (*busWrite16)(struct mCore*, uint32_t address, uint16_t);
	void (*busWrite32)(struct mCore*, uint32_t address, uint32_t);

	uint32_t (*rawRead8)(struct mCore*, uint32_t address, int segment);
	uint32_t (*rawRead16)(struct mCore*, uint32_t address, int segment);
	uint32_t (*rawRead32)(struct mCore*, uint32_t address, int segment);

	void (*rawWrite8)(struct mCore*, uint32_t address, int segment, uint8_t);
	void (*rawWrite16)(struct mCore*, uint32_t address, int segment, uint16_t);
	void (*rawWrite32)(struct mCore*, uint32_t address, int segment, uint32_t);

  void* supportsDebuggerType;
  void* debuggerPlatform;
  void* cliDebuggerSystem;
  void* attachDebugger;
  void* detachDebugger;

	struct mCheatDevice* (*cheatDevice)(struct mCore*);

	size_t (*savedataClone)(struct mCore*, void** sram);
	bool (*savedataRestore)(struct mCore*, const void* sram, size_t size, bool writeback);
};

struct mCore* mCoreFind(const char* path);
bool mCoreLoadFile(struct mCore* core, const char* path);

bool mCoreAutoloadSave(struct mCore* core);
bool mCoreAutoloadPatch(struct mCore* core);

bool mCoreSaveState(struct mCore* core, int slot, int flags);
bool mCoreLoadState(struct mCore* core, int slot, int flags);
struct VFile* mCoreGetState(struct mCore* core, int slot, bool write);
void mCoreDeleteState(struct mCore* core, int slot);

void mCoreTakeScreenshot(struct mCore* core);

struct mCore* mCoreFindVF(struct VFile* vf);
enum mPlatform mCoreIsCompatible(struct VFile* vf);

bool mCoreSaveStateNamed(struct mCore* core, struct VFile* vf, int flags);
bool mCoreLoadStateNamed(struct mCore* core, struct VFile* vf, int flags);

void mCoreInitConfig(struct mCore* core, const char* port);
void mCoreLoadConfig(struct mCore* core);
void mCoreLoadForeignConfig(struct mCore* core, const struct mCoreConfig* config);

// mgba-util/vfs.h

typedef intptr_t off_t;

enum {
	MAP_READ = 1,
	MAP_WRITE = 2
};

enum VFSType {
	VFS_UNKNOWN = 0,
	VFS_FILE,
	VFS_DIRECTORY
};

struct VFile {
	bool (*close)(struct VFile* vf);
	off_t (*seek)(struct VFile* vf, off_t offset, int whence);
	ssize_t (*read)(struct VFile* vf, void* buffer, size_t size);
	ssize_t (*readline)(struct VFile* vf, char* buffer, size_t size);
	ssize_t (*write)(struct VFile* vf, const void* buffer, size_t size);
	void* (*map)(struct VFile* vf, size_t size, int flags);
	void (*unmap)(struct VFile* vf, void* memory, size_t size);
	void (*truncate)(struct VFile* vf, size_t size);
	ssize_t (*size)(struct VFile* vf);
	bool (*sync)(struct VFile* vf, const void* buffer, size_t size);
};

struct VDirEntry {
	const char* (*name)(struct VDirEntry* vde);
	enum VFSType (*type)(struct VDirEntry* vde);
};

struct VDir {
	bool (*close)(struct VDir* vd);
	void (*rewind)(struct VDir* vd);
	struct VDirEntry* (*listNext)(struct VDir* vd);
	struct VFile* (*openFile)(struct VDir* vd, const char* name, int mode);
	struct VDir* (*openDir)(struct VDir* vd, const char* name);
	bool (*deleteFile)(struct VDir* vd, const char* name);
};

struct VFile* VFileOpen(const char* path, int flags);

struct VFile* VFileOpenFD(const char* path, int flags);
struct VFile* VFileFromFD(int fd);

struct VFile* VFileFromMemory(void* mem, size_t size);
struct VFile* VFileFromConstMemory(const void* mem, size_t size);
struct VFile* VFileMemChunk(const void* mem, size_t size);

struct VDir* VDirOpen(const char* path);
struct VDir* VDirOpenArchive(const char* path);

struct VDir* VDirOpenZip(const char* path, int flags);

struct VDir* VDirOpen7z(const char* path, int flags);

void separatePath(const char* path, char* dirname, char* basename, char* extension);

struct VFile* VDirFindFirst(struct VDir* dir, bool (*filter)(struct VFile*));
struct VFile* VDirFindNextAvailable(struct VDir*, const char* basename, const char* infix, const char* suffix, int mode);

ssize_t VFileReadline(struct VFile* vf, char* buffer, size_t size);

ssize_t VFileWrite32LE(struct VFile* vf, int32_t word);
ssize_t VFileWrite16LE(struct VFile* vf, int16_t hword);
ssize_t VFileRead32LE(struct VFile* vf, void* word);
ssize_t VFileRead16LE(struct VFile* vf, void* hword);
