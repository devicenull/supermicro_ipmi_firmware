#ifndef _HTML_TAG_H


#ifdef _HTML_TAG_C

typedef struct Tag_Code_Table{
	char *tag;
	char *code;
}St_Code_Table;

typedef struct html_func{

	char *file;
	int  (*func) (const char *,char *,int);
}St_HtmlFuncTag;

#else

extern void html_priv_check(char *buffer, int priv,const char *file);
#endif
#endif
