
typedef unsigned char BYTE;

enum 
{
	CT,  //Chassis Type\n
	CP,  //Chassis Part number
	CS,  //Chassis Serial number\n"
	BDT, //Board Mfg. Date/Time (YYYYMMDDhhmm)
	BM,  //Board Manufacturer
	BPN, //Board Product Name
	BS,  //Board Serial Name
	BP,  //Board Part Number
	PM,  //Product Manufacturer
	PN,  //Product Name
	PPM, //Product Part/Model Number
	PV,  //Product Version
	PS,  //Product Serial Number
	PAT  //Asset Tag
};

struct fru_header {
	BYTE version;
	BYTE internal;
	BYTE chassis;
	BYTE board;
	BYTE product;
	BYTE multi;
	BYTE pad;
	BYTE checksum;
};

struct fru_area_internal {
	BYTE useData[8];
};

struct fru_area_chassis {
	BYTE area_ver;
	BYTE area_len;
	BYTE type;
	BYTE part_len;
	char part[64];
	BYTE serial_len;
	char serial[64];
	BYTE custom_len;
	char custom[128];
};

struct fru_area_board {
	BYTE area_ver;
	BYTE area_len;
	BYTE lang;
	BYTE mfg_date_time[3];
	BYTE mfg_len;
	char mfg[64];
	BYTE prod_len;
	char prod[64];
	BYTE serial_len;
	char serial[64];
	BYTE part_len;
	char part[64];
	BYTE fru_len;
	char fru[64];
	BYTE custom_len;
	char custom[128];
};

struct fru_area_product {
	BYTE area_ver;
	BYTE area_len;
	BYTE lang;
	BYTE mfg_len;
	char mfg[64];
	BYTE name_len;
	char name[64];
	BYTE part_len;
	char part[64];
	BYTE version_len;
	char version[64];
	BYTE serial_len;
	char serial[64];
	BYTE asset_len;
	char asset[64];
	BYTE fru_len;
	char fru[64];
	BYTE custom_len;
	char custom[128];
};

struct fru_area_multi {
	BYTE rec_type_id;
	BYTE flag;
	BYTE rec_len;
	BYTE rec_checksum;
	BYTE head_checksum;
	char data[64];
};

BYTE fruNetFu;
BYTE fruGetInventoryCmd;
BYTE fruReadFRUCmd;
BYTE fruWriteFRUCmd;
BYTE fruInventoryAreaInfo[3];
BYTE fruWriteBuffer[8192];

struct fru_header			header;
struct fru_area_internal	internal;
struct fru_area_chassis		chassis;
struct fru_area_board		board;
struct fru_area_product		product;
struct fru_area_multi		multi[32];
int fru_area_multi_len;
int fruWriteBufferLength;
char hasInternal;
char hasChassis;
char hasBoard;
char hasProduct;
char hasMulti;

BYTE getFRUInventoryAreaInfo();
BYTE readFRUbySegment(BYTE* fruData);
BYTE unlockFRU();
BYTE lockFRU();
BYTE writeFRU();
BYTE writeFRUfromFile(char *fileName);
BYTE writeFRUbySegment(BYTE ls, BYTE ms, BYTE* fruData, BYTE length);

char* getChassisType(int index);
void loadFRUSourceSetting();
void parseFRUData(BYTE* data, BYTE printFRUData);
void getMfgDateTime(char* formatedDateTime, BYTE *mfgDateTime);
void printFRUInventoryAreaInfo();
void parseFRUInternalUse(int offset, BYTE* data, BYTE printFRUData);
void parseFRUChassisInfo(int offset, BYTE* data, BYTE printFRUData);
void parseFRUBoardInfo(int offset, BYTE* data, BYTE printFRUData);
void parseFRUProductInfo(int offset, BYTE* data, BYTE printFRUData);
void parseFRUMultiRecord(int offset, BYTE* data, BYTE printFRUData);
void getFRUField(BYTE b_Idx,BYTE *out_buf);
void setFRUField(BYTE b_Idx, char *value);
void toFRURaw();
void strTimeToStructTm(char *str, struct tm *userInputTm);
void userInputTimeToMfgDateTimeFormat(struct tm *userInputTm,BYTE *mfgDateTime);
void showChassisTypeHelp();
void showFRUHelp();

static char * chassis_type_desc[] = {
	"Unspecified",			//0
	"Other",				//1
	"Unknown",				//2
	"Desktop",				//3
	"Low Profile Desktop",	//4
	"Pizza Box",			//5
	"Mini Tower",			//6
	"Tower",				//7
	"Portable",				//8
	"LapTop",				//9
	"Notebook",				//A
	"Hand Held",			//B
	"Docking Station",		//C
	"All in One",			//D
	"Sub Notebook",			//E
	"Space-saving",			//F
	"Lunch Box",			//10
	"Main Server Chassis",	//11
	"Expansion Chassis",	//12
	"SubChassis",			//13
	"Bus Expansion Chassis",//14
	"Peripheral Chassis",	//15
	"RAID Chassis",			//16
	"Rack Mount Chassis",	//17
	"Unspecified",			//18
	"Unspecified",			//19
	"Unspecified",			//1A
	"Unspecified",			//1B
	"Unspecified",			//1C
	"Unspecified",			//1D
	"Unspecified",			//1D
	"Unspecified",			//1F
	"Unspecified",			//20
	"Unspecified",			//21
	"Unspecified",			//22
	"Unspecified",			//23
	"Unspecified",			//24
	"Unspecified",			//25
	"Unspecified",			//26
	"Blade"					//27
};