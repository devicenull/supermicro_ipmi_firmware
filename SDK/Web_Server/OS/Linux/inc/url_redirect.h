#define URL_MAX_LENGTH		256

#define EXTNAME_DISABLE		0
#define EXTNAME_ENABLE		1

#define JNLP_KVM_TYPE_MACRO "jwsk"
#define JNLP_SOL_TYPE_MACRO "jwss"
#define FILE_TYPE_MACRO 	"file"
#define IMAGE_TYPE_MACRO    "img"

#define JNLP_EXT_NAME ".jnlp"
#define JNLP_FOLDER "/tmp/jnlp"

#define ADMIN_PRIVI 4

enum url_type
{
	NORMAL_TYPE_URL = 0,
	JNLP_TYPE_URL,
	FILE_TYPE_URL,
	IMAGE_TYPE_URL
};

typedef struct {
	char ext_enable;
	char *ext_name;
	char *ext_rootdir;
} St_ExtNameTag;


