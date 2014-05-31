


#ifndef _LDAP_H
#define _LDAP_H

#include <lber.h>

#include <ldap_features.h>

LDAP_BEGIN_DECL

#define LDAP_VERSION1	1
#define LDAP_VERSION2	2
#define LDAP_VERSION3	3

#define LDAP_VERSION_MIN	LDAP_VERSION2
#define	LDAP_VERSION		LDAP_VERSION2
#define LDAP_VERSION_MAX	LDAP_VERSION3


#define LDAP_API_VERSION	3001
#define LDAP_VENDOR_NAME	"OpenLDAP"

#define LDAP_API_FEATURE_X_OPENLDAP LDAP_VENDOR_VERSION

#if defined( LDAP_API_FEATURE_X_OPENLDAP_REENTRANT ) || \
	( defined( LDAP_THREAD_SAFE ) && \
		defined( LDAP_API_FEATURE_X_OPENLDAP_THREAD_SAFE ) )
#	define	LDAP_API_FEATURE_THREAD_SAFE 1
#endif
#if defined( LDAP_THREAD_SAFE ) && \
	defined( LDAP_API_FEATURE_X_OPENLDAP_THREAD_SAFE )
#endif


#define LDAP_PORT		389		
#define LDAPS_PORT		636		

#define LDAP_ROOT_DSE				""
#define LDAP_NO_ATTRS				"1.1"
#define LDAP_ALL_USER_ATTRIBUTES	"*"
#define LDAP_ALL_OPERATIONAL_ATTRIBUTES	"+" 

#define LDAP_MAXINT (2147483647)



#define LDAP_OPT_API_INFO			0x0000
#define LDAP_OPT_DESC				0x0001 
#define LDAP_OPT_DEREF				0x0002
#define LDAP_OPT_SIZELIMIT			0x0003
#define LDAP_OPT_TIMELIMIT			0x0004
#define LDAP_OPT_REFERRALS			0x0008
#define LDAP_OPT_RESTART			0x0009
#define LDAP_OPT_PROTOCOL_VERSION		0x0011
#define LDAP_OPT_SERVER_CONTROLS		0x0012
#define LDAP_OPT_CLIENT_CONTROLS		0x0013
#define LDAP_OPT_API_FEATURE_INFO		0x0015
#define LDAP_OPT_HOST_NAME			0x0030
#define LDAP_OPT_RESULT_CODE			0x0031
#define LDAP_OPT_ERROR_NUMBER			LDAP_OPT_RESULT_CODE
#define LDAP_OPT_DIAGNOSTIC_MESSAGE		0x0032
#define LDAP_OPT_ERROR_STRING			LDAP_OPT_DIAGNOSTIC_MESSAGE
#define LDAP_OPT_MATCHED_DN			0x0033

#define LDAP_OPT_API_EXTENSION_BASE 0x4000  

#define LDAP_OPT_DEBUG_LEVEL		0x5001	
#define LDAP_OPT_TIMEOUT			0x5002	
#define LDAP_OPT_REFHOPLIMIT		0x5003	
#define LDAP_OPT_NETWORK_TIMEOUT	0x5005	
#define LDAP_OPT_URI				0x5006
#define LDAP_OPT_REFERRAL_URLS      0x5007  
#define LDAP_OPT_SOCKBUF            0x5008  
#define LDAP_OPT_DEFBASE		0x5009	
#define	LDAP_OPT_CONNECT_ASYNC		0x5010	

#define LDAP_OPT_X_TLS				0x6000
#define LDAP_OPT_X_TLS_CTX			0x6001	
#define LDAP_OPT_X_TLS_CACERTFILE	0x6002
#define LDAP_OPT_X_TLS_CACERTDIR	0x6003
#define LDAP_OPT_X_TLS_CERTFILE		0x6004
#define LDAP_OPT_X_TLS_KEYFILE		0x6005
#define LDAP_OPT_X_TLS_REQUIRE_CERT	0x6006
#define LDAP_OPT_X_TLS_CIPHER_SUITE	0x6008
#define LDAP_OPT_X_TLS_RANDOM_FILE	0x6009
#define LDAP_OPT_X_TLS_SSL_CTX		0x600a
#define LDAP_OPT_X_TLS_CRLCHECK		0x600b
#define LDAP_OPT_X_TLS_CONNECT_CB	0x600c
#define LDAP_OPT_X_TLS_CONNECT_ARG	0x600d
#define LDAP_OPT_X_TLS_DHFILE		0x600e
#define LDAP_OPT_X_TLS_NEWCTX		0x600f
#define LDAP_OPT_X_TLS_CRLFILE		0x6010	

#define LDAP_OPT_X_TLS_NEVER	0
#define LDAP_OPT_X_TLS_HARD		1
#define LDAP_OPT_X_TLS_DEMAND	2
#define LDAP_OPT_X_TLS_ALLOW	3
#define LDAP_OPT_X_TLS_TRY		4

#define LDAP_OPT_X_TLS_CRL_NONE	0
#define LDAP_OPT_X_TLS_CRL_PEER	1
#define LDAP_OPT_X_TLS_CRL_ALL	2

#define LDAP_OPT_X_SASL_MECH			0x6100
#define LDAP_OPT_X_SASL_REALM			0x6101
#define LDAP_OPT_X_SASL_AUTHCID			0x6102
#define LDAP_OPT_X_SASL_AUTHZID			0x6103
#define LDAP_OPT_X_SASL_SSF				0x6104 
#define LDAP_OPT_X_SASL_SSF_EXTERNAL	0x6105 
#define LDAP_OPT_X_SASL_SECPROPS		0x6106 
#define LDAP_OPT_X_SASL_SSF_MIN			0x6107
#define LDAP_OPT_X_SASL_SSF_MAX			0x6108
#define	LDAP_OPT_X_SASL_MAXBUFSIZE		0x6109

#define LDAP_OPT_PRIVATE_EXTENSION_BASE 0x7000  


#define LDAP_OPT_SUCCESS	0
#define	LDAP_OPT_ERROR		(-1)

#define LDAP_OPT_ON		((void *) &ber_pvt_opt_on)
#define LDAP_OPT_OFF	((void *) 0)

typedef struct ldapapiinfo {
	int		ldapai_info_version;		
#define LDAP_API_INFO_VERSION	(1)
	int		ldapai_api_version;			
	int		ldapai_protocol_version;	
	char	**ldapai_extensions;		
	char	*ldapai_vendor_name;		
	int		ldapai_vendor_version;		
} LDAPAPIInfo;

typedef struct ldap_apifeature_info {
	int		ldapaif_info_version;		
#define LDAP_FEATURE_INFO_VERSION (1)	
	char*	ldapaif_name;				
	int		ldapaif_version;			
} LDAPAPIFeatureInfo;


typedef struct ldapcontrol {
	char *			ldctl_oid;			
	struct berval	ldctl_value;		
	char			ldctl_iscritical;	
} LDAPControl;

#define LDAP_CONTROL_MANAGEDSAIT	"2.16.840.1.113730.3.4.2"  
#define LDAP_CONTROL_PROXY_AUTHZ	"2.16.840.1.113730.3.4.18" 
#define LDAP_CONTROL_SUBENTRIES		"1.3.6.1.4.1.4203.1.10.1"  

#define LDAP_CONTROL_VALUESRETURNFILTER "1.2.826.0.1.3344810.2.3"

#define LDAP_CONTROL_ASSERT				"1.3.6.1.1.12"			
#define LDAP_CONTROL_PRE_READ			"1.3.6.1.1.13.1"		
#define LDAP_CONTROL_POST_READ			"1.3.6.1.1.13.2"		

#define LDAP_CONTROL_SORTREQUEST    "1.2.840.113556.1.4.473" 
#define LDAP_CONTROL_SORTRESPONSE	"1.2.840.113556.1.4.474" 

#define LDAP_CONTROL_PAGEDRESULTS	"1.2.840.113556.1.4.319"   

#define LDAP_SYNC_OID			"1.3.6.1.4.1.4203.1.9.1"
#define LDAP_CONTROL_SYNC		LDAP_SYNC_OID ".1"
#define LDAP_CONTROL_SYNC_STATE	LDAP_SYNC_OID ".2"
#define LDAP_CONTROL_SYNC_DONE	LDAP_SYNC_OID ".3"
#define LDAP_SYNC_INFO			LDAP_SYNC_OID ".4"

#define LDAP_SYNC_NONE					0x00
#define LDAP_SYNC_REFRESH_ONLY			0x01
#define LDAP_SYNC_RESERVED				0x02
#define LDAP_SYNC_REFRESH_AND_PERSIST	0x03

#define LDAP_SYNC_REFRESH_PRESENTS		0
#define LDAP_SYNC_REFRESH_DELETES		1

#define LDAP_TAG_SYNC_NEW_COOKIE		((ber_tag_t) 0x80U)
#define LDAP_TAG_SYNC_REFRESH_DELETE	((ber_tag_t) 0xa1U)
#define LDAP_TAG_SYNC_REFRESH_PRESENT	((ber_tag_t) 0xa2U)
#define	LDAP_TAG_SYNC_ID_SET			((ber_tag_t) 0xa3U)

#define LDAP_TAG_SYNC_COOKIE			((ber_tag_t) 0x04U)
#define LDAP_TAG_REFRESHDELETES			((ber_tag_t) 0x01U)
#define LDAP_TAG_REFRESHDONE			((ber_tag_t) 0x01U)
#define LDAP_TAG_RELOAD_HINT			((ber_tag_t) 0x01U)

#define LDAP_SYNC_PRESENT				0
#define LDAP_SYNC_ADD					1
#define LDAP_SYNC_MODIFY				2
#define LDAP_SYNC_DELETE				3


#define LDAP_CONTROL_PASSWORDPOLICYREQUEST	"1.3.6.1.4.1.42.2.27.8.5.1"
#define LDAP_CONTROL_PASSWORDPOLICYRESPONSE	"1.3.6.1.4.1.42.2.27.8.5.1"

#define LDAP_CONTROL_NOOP				"1.3.6.1.4.1.4203.666.5.2"
#define LDAP_CONTROL_NO_SUBORDINATES	"1.3.6.1.4.1.4203.666.5.11"
#define LDAP_CONTROL_RELAX				"1.3.6.1.4.1.4203.666.5.12"
#define LDAP_CONTROL_MANAGEDIT			LDAP_CONTROL_RELAX
#define LDAP_CONTROL_SLURP				"1.3.6.1.4.1.4203.666.5.13"
#define LDAP_CONTROL_VALSORT			"1.3.6.1.4.1.4203.666.5.14"
#define LDAP_CONTROL_DONTUSECOPY		"1.3.6.1.4.1.4203.666.5.15"



#define LDAP_CONTROL_X_CHAINING_BEHAVIOR	"1.3.6.1.4.1.4203.666.11.3"

#define	LDAP_CHAINING_PREFERRED				0
#define	LDAP_CHAINING_REQUIRED				1
#define LDAP_REFERRALS_PREFERRED			2
#define LDAP_REFERRALS_REQUIRED				3

#define LDAP_CONTROL_X_INCREMENTAL_VALUES	"1.2.840.113556.1.4.802"
#define LDAP_CONTROL_X_DOMAIN_SCOPE			"1.2.840.113556.1.4.1339"
#define LDAP_CONTROL_X_PERMISSIVE_MODIFY	"1.2.840.113556.1.4.1413"
#define LDAP_CONTROL_X_SEARCH_OPTIONS		"1.2.840.113556.1.4.1340"
#define LDAP_SEARCH_FLAG_DOMAIN_SCOPE 1 
#define LDAP_SEARCH_FLAG_PHANTOM_ROOT 2 
#define LDAP_CONTROL_X_TREE_DELETE		"1.2.840.113556.1.4.805"

#define LDAP_CONTROL_X_EXTENDED_DN		"1.2.840.113556.1.4.529"

#ifdef LDAP_DEVEL
#define LDAP_CONTROL_X_SESSION_TRACKING		"1.3.6.1.4.1.21008.108.63.1"
#define LDAP_CONTROL_X_SESSION_TRACKING_RADIUS_ACCT_SESSION_ID \
						LDAP_CONTROL_X_SESSION_TRACKING ".1"
#define LDAP_CONTROL_X_SESSION_TRACKING_RADIUS_ACCT_MULTI_SESSION_ID \
						LDAP_CONTROL_X_SESSION_TRACKING ".2"
#define LDAP_CONTROL_X_SESSION_TRACKING_USERNAME \
						LDAP_CONTROL_X_SESSION_TRACKING ".3"
#endif 

#define LDAP_CONTROL_DUPENT_REQUEST		"2.16.840.1.113719.1.27.101.1"
#define LDAP_CONTROL_DUPENT_RESPONSE	"2.16.840.1.113719.1.27.101.2"
#define LDAP_CONTROL_DUPENT_ENTRY		"2.16.840.1.113719.1.27.101.3"
#define LDAP_CONTROL_DUPENT	LDAP_CONTROL_DUPENT_REQUEST

#define LDAP_CONTROL_PERSIST_REQUEST				"2.16.840.1.113730.3.4.3"
#define LDAP_CONTROL_PERSIST_ENTRY_CHANGE_NOTICE	"2.16.840.1.113730.3.4.7"
#define LDAP_CONTROL_PERSIST_ENTRY_CHANGE_ADD		0x1
#define LDAP_CONTROL_PERSIST_ENTRY_CHANGE_DELETE	0x2
#define LDAP_CONTROL_PERSIST_ENTRY_CHANGE_MODIFY	0x4
#define LDAP_CONTROL_PERSIST_ENTRY_CHANGE_RENAME	0x8

#define LDAP_CONTROL_VLVREQUEST    	"2.16.840.1.113730.3.4.9"
#define LDAP_CONTROL_VLVRESPONSE    "2.16.840.1.113730.3.4.10"


#define	LDAP_NOTICE_OF_DISCONNECTION	"1.3.6.1.4.1.1466.20036" 
#define LDAP_NOTICE_DISCONNECT LDAP_NOTICE_OF_DISCONNECTION

#define LDAP_EXOP_START_TLS		"1.3.6.1.4.1.1466.20037"	

#define LDAP_EXOP_MODIFY_PASSWD	"1.3.6.1.4.1.4203.1.11.1"	
#define LDAP_TAG_EXOP_MODIFY_PASSWD_ID	((ber_tag_t) 0x80U)
#define LDAP_TAG_EXOP_MODIFY_PASSWD_OLD	((ber_tag_t) 0x81U)
#define LDAP_TAG_EXOP_MODIFY_PASSWD_NEW	((ber_tag_t) 0x82U)
#define LDAP_TAG_EXOP_MODIFY_PASSWD_GEN	((ber_tag_t) 0x80U)

#define LDAP_EXOP_CANCEL		"1.3.6.1.1.8"					
#define LDAP_EXOP_X_CANCEL		LDAP_EXOP_CANCEL

#define	LDAP_EXOP_REFRESH		"1.3.6.1.4.1.1466.101.119.1"	
#define	LDAP_TAG_EXOP_REFRESH_REQ_DN	((ber_tag_t) 0x80U)
#define	LDAP_TAG_EXOP_REFRESH_REQ_TTL	((ber_tag_t) 0x81U)
#define	LDAP_TAG_EXOP_REFRESH_RES_TTL	((ber_tag_t) 0x80U)

#define LDAP_EXOP_WHO_AM_I		"1.3.6.1.4.1.4203.1.11.3"		
#define LDAP_EXOP_X_WHO_AM_I	LDAP_EXOP_WHO_AM_I

#define LDAP_EXOP_TURN		"1.3.6.1.1.19"				
#define LDAP_EXOP_X_TURN	LDAP_EXOP_TURN

#define LDAP_X_DISTPROC_BASE		"1.3.6.1.4.1.4203.666.11.6"
#define LDAP_EXOP_X_CHAINEDREQUEST	LDAP_X_DISTPROC_BASE ".1"
#define LDAP_FEATURE_X_CANCHAINOPS	LDAP_X_DISTPROC_BASE ".2"
#define LDAP_CONTROL_X_RETURNCONTREF	LDAP_X_DISTPROC_BASE ".3"
#define LDAP_URLEXT_X_LOCALREFOID	LDAP_X_DISTPROC_BASE ".4"
#define LDAP_URLEXT_X_REFTYPEOID	LDAP_X_DISTPROC_BASE ".5"
#define LDAP_URLEXT_X_SEARCHEDSUBTREEOID \
					LDAP_X_DISTPROC_BASE ".6"
#define LDAP_URLEXT_X_FAILEDNAMEOID	LDAP_X_DISTPROC_BASE ".7"
#define LDAP_URLEXT_X_LOCALREF		"x-localReference"
#define LDAP_URLEXT_X_REFTYPE		"x-referenceType"
#define LDAP_URLEXT_X_SEARCHEDSUBTREE	"x-searchedSubtree"
#define LDAP_URLEXT_X_FAILEDNAME	"x-failedName"

#ifdef LDAP_DEVEL
#define LDAP_X_TXN						"1.3.6.1.4.1.4203.666.11.7" 
#define LDAP_EXOP_X_TXN_START			LDAP_X_TXN ".1"
#define LDAP_CONTROL_X_TXN_SPEC			LDAP_X_TXN ".2"
#define LDAP_EXOP_X_TXN_END				LDAP_X_TXN ".3"
#define LDAP_EXOP_X_TXN_ABORTED_NOTICE	LDAP_X_TXN ".4"
#endif

#define LDAP_FEATURE_ALL_OP_ATTRS	"1.3.6.1.4.1.4203.1.5.1"	
#define LDAP_FEATURE_OBJECTCLASS_ATTRS \
	"1.3.6.1.4.1.4203.1.5.2" 
#define LDAP_FEATURE_ABSOLUTE_FILTERS "1.3.6.1.4.1.4203.1.5.3"  
#define LDAP_FEATURE_LANGUAGE_TAG_OPTIONS "1.3.6.1.4.1.4203.1.5.4"
#define LDAP_FEATURE_LANGUAGE_RANGE_OPTIONS "1.3.6.1.4.1.4203.1.5.5"
#define LDAP_FEATURE_MODIFY_INCREMENT "1.3.6.1.1.14"

#define LDAP_FEATURE_SUBORDINATE_SCOPE \
	"1.3.6.1.4.1.4203.666.8.1" /* "children" */
#define LDAP_FEATURE_CHILDREN_SCOPE LDAP_FEATURE_SUBORDINATE_SCOPE





#define LDAP_TAG_MESSAGE	((ber_tag_t) 0x30U)	
#define LDAP_TAG_MSGID		((ber_tag_t) 0x02U)	

#define LDAP_TAG_LDAPDN		((ber_tag_t) 0x04U)	
#define LDAP_TAG_LDAPCRED	((ber_tag_t) 0x04U)	

#define LDAP_TAG_CONTROLS	((ber_tag_t) 0xa0U)	
#define LDAP_TAG_REFERRAL	((ber_tag_t) 0xa3U)	

#define LDAP_TAG_NEWSUPERIOR	((ber_tag_t) 0x80U)	

#define LDAP_TAG_EXOP_REQ_OID   ((ber_tag_t) 0x80U)	
#define LDAP_TAG_EXOP_REQ_VALUE ((ber_tag_t) 0x81U)	
#define LDAP_TAG_EXOP_RES_OID   ((ber_tag_t) 0x8aU)	
#define LDAP_TAG_EXOP_RES_VALUE ((ber_tag_t) 0x8bU)	

#define LDAP_TAG_IM_RES_OID   ((ber_tag_t) 0x80U)	
#define LDAP_TAG_IM_RES_VALUE ((ber_tag_t) 0x81U)	

#define LDAP_TAG_SASL_RES_CREDS	((ber_tag_t) 0x87U)	

#define LDAP_REQ_BIND		((ber_tag_t) 0x60U)	
#define LDAP_REQ_UNBIND		((ber_tag_t) 0x42U)	
#define LDAP_REQ_SEARCH		((ber_tag_t) 0x63U)	
#define LDAP_REQ_MODIFY		((ber_tag_t) 0x66U)	
#define LDAP_REQ_ADD		((ber_tag_t) 0x68U)	
#define LDAP_REQ_DELETE		((ber_tag_t) 0x4aU)	
#define LDAP_REQ_MODDN		((ber_tag_t) 0x6cU)	
#define LDAP_REQ_MODRDN		LDAP_REQ_MODDN
#define LDAP_REQ_RENAME		LDAP_REQ_MODDN
#define LDAP_REQ_COMPARE	((ber_tag_t) 0x6eU)	
#define LDAP_REQ_ABANDON	((ber_tag_t) 0x50U)	
#define LDAP_REQ_EXTENDED	((ber_tag_t) 0x77U)	

#define LDAP_RES_BIND		((ber_tag_t) 0x61U)	
#define LDAP_RES_SEARCH_ENTRY	((ber_tag_t) 0x64U)	
#define LDAP_RES_SEARCH_REFERENCE	((ber_tag_t) 0x73U)	
#define LDAP_RES_SEARCH_RESULT	((ber_tag_t) 0x65U)	
#define LDAP_RES_MODIFY		((ber_tag_t) 0x67U)	
#define LDAP_RES_ADD		((ber_tag_t) 0x69U)	
#define LDAP_RES_DELETE		((ber_tag_t) 0x6bU)	
#define LDAP_RES_MODDN		((ber_tag_t) 0x6dU)	
#define LDAP_RES_MODRDN		LDAP_RES_MODDN	
#define LDAP_RES_RENAME		LDAP_RES_MODDN	
#define LDAP_RES_COMPARE	((ber_tag_t) 0x6fU)	
#define LDAP_RES_EXTENDED	((ber_tag_t) 0x78U)	
#define LDAP_RES_INTERMEDIATE	((ber_tag_t) 0x79U) 

#define LDAP_RES_ANY			(-1)
#define LDAP_RES_UNSOLICITED	(0)


#define LDAP_SASL_SIMPLE	((char*)0)
#define LDAP_SASL_NULL		("")


#define LDAP_AUTH_NONE   ((ber_tag_t) 0x00U) 
#define LDAP_AUTH_SIMPLE ((ber_tag_t) 0x80U) 
#define LDAP_AUTH_SASL   ((ber_tag_t) 0xa3U) 
#define LDAP_AUTH_KRBV4  ((ber_tag_t) 0xffU) 
#define LDAP_AUTH_KRBV41 ((ber_tag_t) 0x81U) 
#define LDAP_AUTH_KRBV42 ((ber_tag_t) 0x82U) 


#define LDAP_FILTER_AND	((ber_tag_t) 0xa0U)	
#define LDAP_FILTER_OR	((ber_tag_t) 0xa1U)	
#define LDAP_FILTER_NOT	((ber_tag_t) 0xa2U)	
#define LDAP_FILTER_EQUALITY ((ber_tag_t) 0xa3U) 
#define LDAP_FILTER_SUBSTRINGS ((ber_tag_t) 0xa4U) 
#define LDAP_FILTER_GE ((ber_tag_t) 0xa5U) 
#define LDAP_FILTER_LE ((ber_tag_t) 0xa6U) 
#define LDAP_FILTER_PRESENT ((ber_tag_t) 0x87U) 
#define LDAP_FILTER_APPROX ((ber_tag_t) 0xa8U)	
#define LDAP_FILTER_EXT	((ber_tag_t) 0xa9U)	

#define LDAP_FILTER_EXT_OID		((ber_tag_t) 0x81U)	
#define LDAP_FILTER_EXT_TYPE	((ber_tag_t) 0x82U)	
#define LDAP_FILTER_EXT_VALUE	((ber_tag_t) 0x83U)	
#define LDAP_FILTER_EXT_DNATTRS	((ber_tag_t) 0x84U)	

#define LDAP_SUBSTRING_INITIAL	((ber_tag_t) 0x80U)	
#define LDAP_SUBSTRING_ANY		((ber_tag_t) 0x81U)	
#define LDAP_SUBSTRING_FINAL	((ber_tag_t) 0x82U)	

#define LDAP_SCOPE_BASE			((ber_int_t) 0x0000)
#define LDAP_SCOPE_BASEOBJECT	LDAP_SCOPE_BASE
#define LDAP_SCOPE_ONELEVEL		((ber_int_t) 0x0001)
#define LDAP_SCOPE_ONE			LDAP_SCOPE_ONELEVEL
#define LDAP_SCOPE_SUBTREE		((ber_int_t) 0x0002)
#define LDAP_SCOPE_SUB			LDAP_SCOPE_SUBTREE
#define LDAP_SCOPE_SUBORDINATE	((ber_int_t) 0x0003) 
#define LDAP_SCOPE_CHILDREN		LDAP_SCOPE_SUBORDINATE
#define LDAP_SCOPE_DEFAULT		((ber_int_t) -1)	 

#define LDAP_SUBSTRING_INITIAL	((ber_tag_t) 0x80U)	
#define LDAP_SUBSTRING_ANY		((ber_tag_t) 0x81U)	
#define LDAP_SUBSTRING_FINAL	((ber_tag_t) 0x82U)	


#define LDAP_SUCCESS				0x00

#define LDAP_RANGE(n,x,y)	(((x) <= (n)) && ((n) <= (y)))

#define LDAP_OPERATIONS_ERROR		0x01
#define LDAP_PROTOCOL_ERROR			0x02
#define LDAP_TIMELIMIT_EXCEEDED		0x03
#define LDAP_SIZELIMIT_EXCEEDED		0x04
#define LDAP_COMPARE_FALSE			0x05
#define LDAP_COMPARE_TRUE			0x06
#define LDAP_AUTH_METHOD_NOT_SUPPORTED	0x07
#define LDAP_STRONG_AUTH_NOT_SUPPORTED	LDAP_AUTH_METHOD_NOT_SUPPORTED
#define LDAP_STRONG_AUTH_REQUIRED	0x08
#define LDAP_STRONGER_AUTH_REQUIRED	LDAP_STRONG_AUTH_REQUIRED
#define LDAP_PARTIAL_RESULTS		0x09	

#define	LDAP_REFERRAL				0x0a 
#define LDAP_ADMINLIMIT_EXCEEDED	0x0b 
#define	LDAP_UNAVAILABLE_CRITICAL_EXTENSION	0x0c 
#define LDAP_CONFIDENTIALITY_REQUIRED	0x0d 
#define	LDAP_SASL_BIND_IN_PROGRESS	0x0e 

#define LDAP_ATTR_ERROR(n)	LDAP_RANGE((n),0x10,0x15) 

#define LDAP_NO_SUCH_ATTRIBUTE		0x10
#define LDAP_UNDEFINED_TYPE			0x11
#define LDAP_INAPPROPRIATE_MATCHING	0x12
#define LDAP_CONSTRAINT_VIOLATION	0x13
#define LDAP_TYPE_OR_VALUE_EXISTS	0x14
#define LDAP_INVALID_SYNTAX			0x15

#define LDAP_NAME_ERROR(n)	LDAP_RANGE((n),0x20,0x24) 

#define LDAP_NO_SUCH_OBJECT			0x20
#define LDAP_ALIAS_PROBLEM			0x21
#define LDAP_INVALID_DN_SYNTAX		0x22
#define LDAP_IS_LEAF				0x23 
#define LDAP_ALIAS_DEREF_PROBLEM	0x24

#define LDAP_SECURITY_ERROR(n)	LDAP_RANGE((n),0x2F,0x32) 

#define LDAP_X_PROXY_AUTHZ_FAILURE	0x2F 
#define LDAP_INAPPROPRIATE_AUTH		0x30
#define LDAP_INVALID_CREDENTIALS	0x31
#define LDAP_INSUFFICIENT_ACCESS	0x32

#define LDAP_SERVICE_ERROR(n)	LDAP_RANGE((n),0x33,0x36) 

#define LDAP_BUSY					0x33
#define LDAP_UNAVAILABLE			0x34
#define LDAP_UNWILLING_TO_PERFORM	0x35
#define LDAP_LOOP_DETECT			0x36

#define LDAP_UPDATE_ERROR(n)	LDAP_RANGE((n),0x40,0x47) 

#define LDAP_NAMING_VIOLATION		0x40
#define LDAP_OBJECT_CLASS_VIOLATION	0x41
#define LDAP_NOT_ALLOWED_ON_NONLEAF	0x42
#define LDAP_NOT_ALLOWED_ON_RDN		0x43
#define LDAP_ALREADY_EXISTS			0x44
#define LDAP_NO_OBJECT_CLASS_MODS	0x45
#define LDAP_RESULTS_TOO_LARGE		0x46 
#define LDAP_AFFECTS_MULTIPLE_DSAS	0x47

#define LDAP_OTHER					0x50

#define LDAP_CUP_RESOURCES_EXHAUSTED	0x71
#define LDAP_CUP_SECURITY_VIOLATION		0x72
#define LDAP_CUP_INVALID_DATA			0x73
#define LDAP_CUP_UNSUPPORTED_SCHEME		0x74
#define LDAP_CUP_RELOAD_REQUIRED		0x75

#define LDAP_CANCELLED				0x76
#define LDAP_NO_SUCH_OPERATION		0x77
#define LDAP_TOO_LATE				0x78
#define LDAP_CANNOT_CANCEL			0x79

#define LDAP_ASSERTION_FAILED		0x7A

#define LDAP_PROXIED_AUTHORIZATION_DENIED		0x7B

#define LDAP_E_ERROR(n)	LDAP_RANGE((n),0x1000,0x3FFF)

#define LDAP_SYNC_REFRESH_REQUIRED		0x1000


#define LDAP_X_ERROR(n)	LDAP_RANGE((n),0x4000,0xFFFF)

#define LDAP_X_SYNC_REFRESH_REQUIRED	0x4100 
#define LDAP_X_ASSERTION_FAILED			0x410f 

#define LDAP_X_NO_OPERATION				0x410e


#ifdef LDAP_CONTROL_X_CHAINING_BEHAVIOR
#define	LDAP_X_NO_REFERRALS_FOUND		0x4110
#define LDAP_X_CANNOT_CHAIN			0x4111
#endif

#ifdef LDAP_X_DISTPROC_BASE
#define LDAP_X_INVALIDREFERENCE			0x4112
#endif

#ifdef LDAP_X_TXN
#define LDAP_X_TXN_SPECIFY_OKAY		0x4120
#define LDAP_X_TXN_ID_INVALID		0x4121
#endif


#define LDAP_API_ERROR(n)		((n)<0)
#define LDAP_API_RESULT(n)		((n)<=0)

#define LDAP_SERVER_DOWN				(-1)
#define LDAP_LOCAL_ERROR				(-2)
#define LDAP_ENCODING_ERROR				(-3)
#define LDAP_DECODING_ERROR				(-4)
#define LDAP_TIMEOUT					(-5)
#define LDAP_AUTH_UNKNOWN				(-6)
#define LDAP_FILTER_ERROR				(-7)
#define LDAP_USER_CANCELLED				(-8)
#define LDAP_PARAM_ERROR				(-9)
#define LDAP_NO_MEMORY					(-10)
#define LDAP_CONNECT_ERROR				(-11)
#define LDAP_NOT_SUPPORTED				(-12)
#define LDAP_CONTROL_NOT_FOUND			(-13)
#define LDAP_NO_RESULTS_RETURNED		(-14)
#define LDAP_MORE_RESULTS_TO_RETURN		(-15)	
#define LDAP_CLIENT_LOOP				(-16)
#define LDAP_REFERRAL_LIMIT_EXCEEDED	(-17)
#define	LDAP_X_CONNECTING			(-18)




typedef struct ldapmsg LDAPMessage;

typedef struct ldapmod {
	int		mod_op;

#define LDAP_MOD_OP			(0x0007)
#define LDAP_MOD_ADD		(0x0000)
#define LDAP_MOD_DELETE		(0x0001)
#define LDAP_MOD_REPLACE	(0x0002)
#define LDAP_MOD_INCREMENT	(0x0003) 
#define LDAP_MOD_BVALUES	(0x0080)


	char		*mod_type;
	union mod_vals_u {
		char		**modv_strvals;
		struct berval	**modv_bvals;
	} mod_vals;
#define mod_values	mod_vals.modv_strvals
#define mod_bvalues	mod_vals.modv_bvals
} LDAPMod;


typedef struct ldap LDAP;

#define LDAP_DEREF_NEVER		0x00
#define LDAP_DEREF_SEARCHING	0x01
#define LDAP_DEREF_FINDING		0x02
#define LDAP_DEREF_ALWAYS		0x03

#define LDAP_NO_LIMIT			0

#define LDAP_MSG_ONE			0x00
#define LDAP_MSG_ALL			0x01
#define LDAP_MSG_RECEIVED		0x02


typedef struct ldap_url_desc {
	struct ldap_url_desc *lud_next;
	char	*lud_scheme;
	char	*lud_host;
	int		lud_port;
	char	*lud_dn;
	char	**lud_attrs;
	int		lud_scope;
	char	*lud_filter;
	char	**lud_exts;
	int		lud_crit_exts;
} LDAPURLDesc;

#define LDAP_URL_SUCCESS		0x00	
#define LDAP_URL_ERR_MEM		0x01	
#define LDAP_URL_ERR_PARAM		0x02	

#define LDAP_URL_ERR_BADSCHEME	0x03	
#define LDAP_URL_ERR_BADENCLOSURE 0x04	
#define LDAP_URL_ERR_BADURL		0x05	
#define LDAP_URL_ERR_BADHOST	0x06	
#define LDAP_URL_ERR_BADATTRS	0x07	
#define LDAP_URL_ERR_BADSCOPE	0x08	
#define LDAP_URL_ERR_BADFILTER	0x09	
#define LDAP_URL_ERR_BADEXTS	0x0a	



typedef struct ldap_sync_t ldap_sync_t;

typedef enum {
	LDAP_SYNC_CAPI_NONE		= -1,

	LDAP_SYNC_CAPI_PHASE_FLAG	= 0x10U,
	LDAP_SYNC_CAPI_IDSET_FLAG	= 0x20U,
	LDAP_SYNC_CAPI_DONE_FLAG	= 0x40U,

	LDAP_SYNC_CAPI_PRESENT		= LDAP_SYNC_PRESENT,
	LDAP_SYNC_CAPI_ADD		= LDAP_SYNC_ADD,
	LDAP_SYNC_CAPI_MODIFY		= LDAP_SYNC_MODIFY,
	LDAP_SYNC_CAPI_DELETE		= LDAP_SYNC_DELETE,

	LDAP_SYNC_CAPI_PRESENTS		= ( LDAP_SYNC_CAPI_PHASE_FLAG | LDAP_SYNC_CAPI_PRESENT ),
	LDAP_SYNC_CAPI_DELETES		= ( LDAP_SYNC_CAPI_PHASE_FLAG | LDAP_SYNC_CAPI_DELETE ),

	LDAP_SYNC_CAPI_PRESENTS_IDSET	= ( LDAP_SYNC_CAPI_PRESENTS | LDAP_SYNC_CAPI_IDSET_FLAG ),
	LDAP_SYNC_CAPI_DELETES_IDSET	= ( LDAP_SYNC_CAPI_DELETES | LDAP_SYNC_CAPI_IDSET_FLAG ),

	LDAP_SYNC_CAPI_DONE		= ( LDAP_SYNC_CAPI_DONE_FLAG | LDAP_SYNC_CAPI_PRESENTS )
} ldap_sync_refresh_t;


typedef int (*ldap_sync_search_entry_f) LDAP_P((
	ldap_sync_t			*ls,
	LDAPMessage			*msg,
	struct berval			*entryUUID,
	ldap_sync_refresh_t		phase ));


typedef int (*ldap_sync_search_reference_f) LDAP_P((
	ldap_sync_t			*ls,
	LDAPMessage			*msg ));


typedef int (*ldap_sync_intermediate_f) LDAP_P((
	ldap_sync_t			*ls,
	LDAPMessage			*msg,
	BerVarray			syncUUIDs,
	ldap_sync_refresh_t		phase ));


typedef int (*ldap_sync_search_result_f) LDAP_P((
	ldap_sync_t			*ls,
	LDAPMessage			*msg,
	int				refreshDeletes ));


struct ldap_sync_t {
	char				*ls_base;
	int				ls_scope;
	char				*ls_filter;
	char				**ls_attrs;
	int				ls_timelimit;
	int				ls_sizelimit;

	int				ls_timeout;

	ldap_sync_search_entry_f	ls_search_entry;
	ldap_sync_search_reference_f	ls_search_reference;
	ldap_sync_intermediate_f	ls_intermediate;
	ldap_sync_search_result_f	ls_search_result;

	void				*ls_private;

	LDAP				*ls_ld;




	int				ls_msgid;

	int				ls_reloadHint;

	struct berval			ls_cookie;

	ldap_sync_refresh_t		ls_refreshPhase;
};




struct timeval;


LDAP_F( int )
ldap_get_option LDAP_P((
	LDAP *ld,
	int option,
	void *outvalue));

LDAP_F( int )
ldap_set_option LDAP_P((
	LDAP *ld,
	int option,
	LDAP_CONST void *invalue));

typedef int (LDAP_REBIND_PROC) LDAP_P((
	LDAP *ld, LDAP_CONST char *url,
	ber_tag_t request, ber_int_t msgid,
	void *params ));

LDAP_F( int )
ldap_set_rebind_proc LDAP_P((
	LDAP *ld,
	LDAP_REBIND_PROC *rebind_proc,
	void *params ));

typedef int (LDAP_NEXTREF_PROC) LDAP_P((
	LDAP *ld, char ***refsp, int *cntp,
	void *params ));

LDAP_F( int )
ldap_set_nextref_proc LDAP_P((
	LDAP *ld,
	LDAP_NEXTREF_PROC *nextref_proc,
	void *params ));

typedef int (LDAP_URLLIST_PROC) LDAP_P((
	LDAP *ld, 
	LDAPURLDesc **urllist,
	LDAPURLDesc **url,
	void *params ));

LDAP_F( int )
ldap_set_urllist_proc LDAP_P((
	LDAP *ld,
	LDAP_URLLIST_PROC *urllist_proc,
	void *params ));


#if LDAP_DEPRECATED	
LDAP_F( int )
ldap_create_control LDAP_P((	
	LDAP_CONST char *requestOID,
	BerElement *ber,
	int iscritical,
	LDAPControl **ctrlp ));

LDAP_F( LDAPControl * )
ldap_find_control LDAP_P((	
	LDAP_CONST char *oid,
	LDAPControl **ctrls ));
#endif

LDAP_F( int )
ldap_control_create LDAP_P((
	LDAP_CONST char *requestOID,
	int iscritical,
	struct berval *value,
	int dupval,
	LDAPControl **ctrlp ));

LDAP_F( LDAPControl * )
ldap_control_find LDAP_P((
	LDAP_CONST char *oid,
	LDAPControl **ctrls,
	LDAPControl ***nextctrlp ));

LDAP_F( void )
ldap_control_free LDAP_P((
	LDAPControl *ctrl ));

LDAP_F( void )
ldap_controls_free LDAP_P((
	LDAPControl **ctrls ));

LDAP_F( LDAPControl ** )
ldap_controls_dup LDAP_P((
	LDAPControl *LDAP_CONST *controls ));

LDAP_F( LDAPControl * )
ldap_control_dup LDAP_P((
	LDAP_CONST LDAPControl *c ));


LDAP_F( int )
ldap_domain2dn LDAP_P((
	LDAP_CONST char* domain,
	char** dn ));

LDAP_F( int )
ldap_dn2domain LDAP_P((
	LDAP_CONST char* dn,
	char** domain ));

LDAP_F( int )
ldap_domain2hostlist LDAP_P((
	LDAP_CONST char *domain,
	char** hostlist ));


LDAP_F( int )
ldap_extended_operation LDAP_P((
	LDAP			*ld,
	LDAP_CONST char	*reqoid,
	struct berval	*reqdata,
	LDAPControl		**serverctrls,
	LDAPControl		**clientctrls,
	int				*msgidp ));

LDAP_F( int )
ldap_extended_operation_s LDAP_P((
	LDAP			*ld,
	LDAP_CONST char	*reqoid,
	struct berval	*reqdata,
	LDAPControl		**serverctrls,
	LDAPControl		**clientctrls,
	char			**retoidp,
	struct berval	**retdatap ));

LDAP_F( int )
ldap_parse_extended_result LDAP_P((
	LDAP			*ld,
	LDAPMessage		*res,
	char			**retoidp,
	struct berval	**retdatap,
	int				freeit ));

LDAP_F( int )
ldap_parse_intermediate LDAP_P((
	LDAP			*ld,
	LDAPMessage		*res,
	char			**retoidp,
	struct berval	**retdatap,
	LDAPControl		***serverctrls,
	int				freeit ));



LDAP_F( int )
ldap_abandon_ext LDAP_P((
	LDAP			*ld,
	int				msgid,
	LDAPControl		**serverctrls,
	LDAPControl		**clientctrls ));

#if LDAP_DEPRECATED	
LDAP_F( int )
ldap_abandon LDAP_P((	
	LDAP *ld,
	int msgid ));
#endif


LDAP_F( int )
ldap_add_ext LDAP_P((
	LDAP			*ld,
	LDAP_CONST char	*dn,
	LDAPMod			**attrs,
	LDAPControl		**serverctrls,
	LDAPControl		**clientctrls,
	int 			*msgidp ));

LDAP_F( int )
ldap_add_ext_s LDAP_P((
	LDAP			*ld,
	LDAP_CONST char	*dn,
	LDAPMod			**attrs,
	LDAPControl		**serverctrls,
	LDAPControl		**clientctrls ));

#if LDAP_DEPRECATED
LDAP_F( int )
ldap_add LDAP_P((	
	LDAP *ld,
	LDAP_CONST char *dn,
	LDAPMod **attrs ));

LDAP_F( int )
ldap_add_s LDAP_P((	
	LDAP *ld,
	LDAP_CONST char *dn,
	LDAPMod **attrs ));
#endif



LDAP_F( int )
ldap_sasl_bind LDAP_P((
	LDAP			*ld,
	LDAP_CONST char	*dn,
	LDAP_CONST char	*mechanism,
	struct berval	*cred,
	LDAPControl		**serverctrls,
	LDAPControl		**clientctrls,
	int				*msgidp ));


#define LDAP_SASL_AUTOMATIC		0U
#define LDAP_SASL_INTERACTIVE	1U
#define LDAP_SASL_QUIET			2U


typedef int (LDAP_SASL_INTERACT_PROC) LDAP_P((
	LDAP *ld, unsigned flags, void* defaults, void *interact ));

LDAP_F( int )
ldap_sasl_interactive_bind_s LDAP_P((
	LDAP *ld,
	LDAP_CONST char *dn, 
	LDAP_CONST char *saslMechanism,
	LDAPControl **serverControls,
	LDAPControl **clientControls,

	unsigned flags,
	LDAP_SASL_INTERACT_PROC *proc,
	void *defaults ));

LDAP_F( int )
ldap_sasl_bind_s LDAP_P((
	LDAP			*ld,
	LDAP_CONST char	*dn,
	LDAP_CONST char	*mechanism,
	struct berval	*cred,
	LDAPControl		**serverctrls,
	LDAPControl		**clientctrls,
	struct berval	**servercredp ));

LDAP_F( int )
ldap_parse_sasl_bind_result LDAP_P((
	LDAP			*ld,
	LDAPMessage		*res,
	struct berval	**servercredp,
	int				freeit ));

#if LDAP_DEPRECATED

LDAP_F( int )
ldap_bind LDAP_P((	
	LDAP *ld,
	LDAP_CONST char *who,
	LDAP_CONST char *passwd,
	int authmethod ));

LDAP_F( int )
ldap_bind_s LDAP_P((	
	LDAP *ld,
	LDAP_CONST char *who,
	LDAP_CONST char *cred,
	int authmethod ));


LDAP_F( int )
ldap_simple_bind LDAP_P(( 
	LDAP *ld,
	LDAP_CONST char *who,
	LDAP_CONST char *passwd ));

LDAP_F( int )
ldap_simple_bind_s LDAP_P(( 
	LDAP *ld,
	LDAP_CONST char *who,
	LDAP_CONST char *passwd ));

#endif



LDAP_F( int )
ldap_compare_ext LDAP_P((
	LDAP			*ld,
	LDAP_CONST char	*dn,
	LDAP_CONST char	*attr,
	struct berval	*bvalue,
	LDAPControl		**serverctrls,
	LDAPControl		**clientctrls,
	int 			*msgidp ));

LDAP_F( int )
ldap_compare_ext_s LDAP_P((
	LDAP			*ld,
	LDAP_CONST char	*dn,
	LDAP_CONST char	*attr,
	struct berval	*bvalue,
	LDAPControl		**serverctrls,
	LDAPControl		**clientctrls ));

#if LDAP_DEPRECATED
LDAP_F( int )
ldap_compare LDAP_P((	
	LDAP *ld,
	LDAP_CONST char *dn,
	LDAP_CONST char *attr,
	LDAP_CONST char *value ));

LDAP_F( int )
ldap_compare_s LDAP_P((	
	LDAP *ld,
	LDAP_CONST char *dn,
	LDAP_CONST char *attr,
	LDAP_CONST char *value ));
#endif



LDAP_F( int )
ldap_delete_ext LDAP_P((
	LDAP			*ld,
	LDAP_CONST char	*dn,
	LDAPControl		**serverctrls,
	LDAPControl		**clientctrls,
	int 			*msgidp ));

LDAP_F( int )
ldap_delete_ext_s LDAP_P((
	LDAP			*ld,
	LDAP_CONST char	*dn,
	LDAPControl		**serverctrls,
	LDAPControl		**clientctrls ));

#if LDAP_DEPRECATED
LDAP_F( int )
ldap_delete LDAP_P((	
	LDAP *ld,
	LDAP_CONST char *dn ));

LDAP_F( int )
ldap_delete_s LDAP_P((	
	LDAP *ld,
	LDAP_CONST char *dn ));
#endif



LDAP_F( int )
ldap_parse_result LDAP_P((
	LDAP			*ld,
	LDAPMessage		*res,
	int				*errcodep,
	char			**matcheddnp,
	char			**errmsgp,
	char			***referralsp,
	LDAPControl		***serverctrls,
	int				freeit ));

LDAP_F( char * )
ldap_err2string LDAP_P((
	int err ));

#if LDAP_DEPRECATED
LDAP_F( int )
ldap_result2error LDAP_P((	
	LDAP *ld,
	LDAPMessage *r,
	int freeit ));

LDAP_F( void )
ldap_perror LDAP_P((	
	LDAP *ld,
	LDAP_CONST char *s ));
#endif



LDAP_F( int )
ldap_modify_ext LDAP_P((
	LDAP			*ld,
	LDAP_CONST char	*dn,
	LDAPMod			**mods,
	LDAPControl		**serverctrls,
	LDAPControl		**clientctrls,
	int 			*msgidp ));

LDAP_F( int )
ldap_modify_ext_s LDAP_P((
	LDAP			*ld,
	LDAP_CONST char	*dn,
	LDAPMod			**mods,
	LDAPControl		**serverctrls,
	LDAPControl		**clientctrls ));

#if LDAP_DEPRECATED
LDAP_F( int )
ldap_modify LDAP_P((	
	LDAP *ld,
	LDAP_CONST char *dn,
	LDAPMod **mods ));

LDAP_F( int )
ldap_modify_s LDAP_P((	
	LDAP *ld,
	LDAP_CONST char *dn,
	LDAPMod **mods ));
#endif



LDAP_F( int )
ldap_rename LDAP_P((
	LDAP *ld,
	LDAP_CONST char *dn,
	LDAP_CONST char *newrdn,
	LDAP_CONST char *newSuperior,
	int deleteoldrdn,
	LDAPControl **sctrls,
	LDAPControl **cctrls,
	int *msgidp ));

LDAP_F( int )
ldap_rename_s LDAP_P((
	LDAP *ld,
	LDAP_CONST char *dn,
	LDAP_CONST char *newrdn,
	LDAP_CONST char *newSuperior,
	int deleteoldrdn,
	LDAPControl **sctrls,
	LDAPControl **cctrls ));

#if LDAP_DEPRECATED
LDAP_F( int )
ldap_rename2 LDAP_P((	
	LDAP *ld,
	LDAP_CONST char *dn,
	LDAP_CONST char *newrdn,
	LDAP_CONST char *newSuperior,
	int deleteoldrdn ));

LDAP_F( int )
ldap_rename2_s LDAP_P((	
	LDAP *ld,
	LDAP_CONST char *dn,
	LDAP_CONST char *newrdn,
	LDAP_CONST char *newSuperior,
	int deleteoldrdn ));

LDAP_F( int )
ldap_modrdn LDAP_P((	
	LDAP *ld,
	LDAP_CONST char *dn,
	LDAP_CONST char *newrdn ));

LDAP_F( int )
ldap_modrdn_s LDAP_P((	
	LDAP *ld,
	LDAP_CONST char *dn,
	LDAP_CONST char *newrdn ));

LDAP_F( int )
ldap_modrdn2 LDAP_P((	
	LDAP *ld,
	LDAP_CONST char *dn,
	LDAP_CONST char *newrdn,
	int deleteoldrdn ));

LDAP_F( int )
ldap_modrdn2_s LDAP_P((	
	LDAP *ld,
	LDAP_CONST char *dn,
	LDAP_CONST char *newrdn,
	int deleteoldrdn));
#endif



#if LDAP_DEPRECATED
LDAP_F( LDAP * )
ldap_init LDAP_P(( 
	LDAP_CONST char *host,
	int port ));

LDAP_F( LDAP * )
ldap_open LDAP_P((	
	LDAP_CONST char *host,
	int port ));
#endif

LDAP_F( int )
ldap_create LDAP_P((
	LDAP **ldp ));

LDAP_F( int )
ldap_initialize LDAP_P((
	LDAP **ldp,
	LDAP_CONST char *url ));



LDAP_F( int )
ldap_tls_inplace LDAP_P((
	LDAP *ld ));

LDAP_F( int )
ldap_start_tls LDAP_P((
	LDAP *ld,
	LDAPControl **serverctrls,
	LDAPControl **clientctrls,
	int *msgidp ));

LDAP_F( int )
ldap_install_tls LDAP_P((
	LDAP *ld ));

LDAP_F( int )
ldap_start_tls_s LDAP_P((
	LDAP *ld,
	LDAPControl **serverctrls,
	LDAPControl **clientctrls ));


LDAP_F( LDAPMessage * )
ldap_first_message LDAP_P((
	LDAP *ld,
	LDAPMessage *chain ));

LDAP_F( LDAPMessage * )
ldap_next_message LDAP_P((
	LDAP *ld,
	LDAPMessage *msg ));

LDAP_F( int )
ldap_count_messages LDAP_P((
	LDAP *ld,
	LDAPMessage *chain ));


LDAP_F( LDAPMessage * )
ldap_first_reference LDAP_P((
	LDAP *ld,
	LDAPMessage *chain ));

LDAP_F( LDAPMessage * )
ldap_next_reference LDAP_P((
	LDAP *ld,
	LDAPMessage *ref ));

LDAP_F( int )
ldap_count_references LDAP_P((
	LDAP *ld,
	LDAPMessage *chain ));

LDAP_F( int )
ldap_parse_reference LDAP_P((
	LDAP			*ld,
	LDAPMessage		*ref,
	char			***referralsp,
	LDAPControl		***serverctrls,
	int				freeit));



LDAP_F( LDAPMessage * )
ldap_first_entry LDAP_P((
	LDAP *ld,
	LDAPMessage *chain ));

LDAP_F( LDAPMessage * )
ldap_next_entry LDAP_P((
	LDAP *ld,
	LDAPMessage *entry ));

LDAP_F( int )
ldap_count_entries LDAP_P((
	LDAP *ld,
	LDAPMessage *chain ));

LDAP_F( int )
ldap_get_entry_controls LDAP_P((
	LDAP			*ld,
	LDAPMessage		*entry,
	LDAPControl		***serverctrls));



LDAP_F( LDAPMessage * )
ldap_delete_result_entry LDAP_P((
	LDAPMessage **list,
	LDAPMessage *e ));

LDAP_F( void )
ldap_add_result_entry LDAP_P((
	LDAPMessage **list,
	LDAPMessage *e ));



LDAP_F( char * )
ldap_get_dn LDAP_P((
	LDAP *ld,
	LDAPMessage *entry ));

typedef struct ldap_ava {
	struct berval la_attr;
	struct berval la_value;
	unsigned la_flags;
#define LDAP_AVA_NULL				0x0000U
#define LDAP_AVA_STRING				0x0001U
#define LDAP_AVA_BINARY				0x0002U
#define LDAP_AVA_NONPRINTABLE		0x0004U
#define LDAP_AVA_FREE_ATTR			0x0010U
#define LDAP_AVA_FREE_VALUE			0x0020U

	void *la_private;
} LDAPAVA;

typedef LDAPAVA** LDAPRDN;
typedef LDAPRDN* LDAPDN;

#define LDAP_DN_FORMAT_LDAP			0x0000U
#define LDAP_DN_FORMAT_LDAPV3		0x0010U
#define LDAP_DN_FORMAT_LDAPV2		0x0020U
#define LDAP_DN_FORMAT_DCE			0x0030U
#define LDAP_DN_FORMAT_UFN			0x0040U	
#define LDAP_DN_FORMAT_AD_CANONICAL	0x0050U	
#define LDAP_DN_FORMAT_LBER			0x00F0U 
#define LDAP_DN_FORMAT_MASK			0x00F0U

#define LDAP_DN_PRETTY				0x0100U
#define LDAP_DN_SKIP				0x0200U
#define LDAP_DN_P_NOLEADTRAILSPACES	0x1000U
#define LDAP_DN_P_NOSPACEAFTERRDN	0x2000U
#define LDAP_DN_PEDANTIC			0xF000U

LDAP_F( void ) ldap_rdnfree LDAP_P(( LDAPRDN rdn ));
LDAP_F( void ) ldap_dnfree LDAP_P(( LDAPDN dn ));

LDAP_F( int )
ldap_bv2dn LDAP_P(( 
	struct berval *bv, 
	LDAPDN *dn, 
	unsigned flags ));

LDAP_F( int )
ldap_str2dn LDAP_P((
	LDAP_CONST char *str,
	LDAPDN *dn,
	unsigned flags ));

LDAP_F( int )
ldap_dn2bv LDAP_P((
	LDAPDN dn,
	struct berval *bv,
	unsigned flags ));

LDAP_F( int )
ldap_dn2str LDAP_P((
	LDAPDN dn,
	char **str,
	unsigned flags ));

LDAP_F( int )
ldap_bv2rdn LDAP_P((
	struct berval *bv,
	LDAPRDN *rdn,
	char **next,
	unsigned flags ));

LDAP_F( int )
ldap_str2rdn LDAP_P((
	LDAP_CONST char *str,
	LDAPRDN *rdn,
	char **next,
	unsigned flags ));

LDAP_F( int )
ldap_rdn2bv LDAP_P((
	LDAPRDN rdn,
	struct berval *bv,
	unsigned flags ));

LDAP_F( int )
ldap_rdn2str LDAP_P((
	LDAPRDN rdn,
	char **str,
	unsigned flags ));

LDAP_F( int )
ldap_dn_normalize LDAP_P((
	LDAP_CONST char *in, unsigned iflags,
	char **out, unsigned oflags ));

LDAP_F( char * )
ldap_dn2ufn LDAP_P(( 
	LDAP_CONST char *dn ));

LDAP_F( char ** )
ldap_explode_dn LDAP_P(( 
	LDAP_CONST char *dn,
	int notypes ));

LDAP_F( char ** )
ldap_explode_rdn LDAP_P(( 
	LDAP_CONST char *rdn,
	int notypes ));

typedef int LDAPDN_rewrite_func
	LDAP_P(( LDAPDN dn, unsigned flags, void *ctx ));

LDAP_F( int )
ldap_X509dn2bv LDAP_P(( void *x509_name, struct berval *dn,
	LDAPDN_rewrite_func *func, unsigned flags ));

LDAP_F( char * )
ldap_dn2dcedn LDAP_P(( 
	LDAP_CONST char *dn ));

LDAP_F( char * )
ldap_dcedn2dn LDAP_P(( 
	LDAP_CONST char *dce ));

LDAP_F( char * )
ldap_dn2ad_canonical LDAP_P(( 
	LDAP_CONST char *dn ));

LDAP_F( int )
ldap_get_dn_ber LDAP_P((
	LDAP *ld, LDAPMessage *e, BerElement **berout, struct berval *dn ));

LDAP_F( int )
ldap_get_attribute_ber LDAP_P((
	LDAP *ld, LDAPMessage *e, BerElement *ber, struct berval *attr,
	struct berval **vals ));


LDAP_F( char * )
ldap_first_attribute LDAP_P((
	LDAP *ld,
	LDAPMessage *entry,
	BerElement **ber ));

LDAP_F( char * )
ldap_next_attribute LDAP_P((
	LDAP *ld,
	LDAPMessage *entry,
	BerElement *ber ));



LDAP_F( struct berval ** )
ldap_get_values_len LDAP_P((
	LDAP *ld,
	LDAPMessage *entry,
	LDAP_CONST char *target ));

LDAP_F( int )
ldap_count_values_len LDAP_P((
	struct berval **vals ));

LDAP_F( void )
ldap_value_free_len LDAP_P((
	struct berval **vals ));

#if LDAP_DEPRECATED
LDAP_F( char ** )
ldap_get_values LDAP_P((	
	LDAP *ld,
	LDAPMessage *entry,
	LDAP_CONST char *target ));

LDAP_F( int )
ldap_count_values LDAP_P((	
	char **vals ));

LDAP_F( void )
ldap_value_free LDAP_P((	
	char **vals ));
#endif


LDAP_F( int )
ldap_result LDAP_P((
	LDAP *ld,
	int msgid,
	int all,
	struct timeval *timeout,
	LDAPMessage **result ));

LDAP_F( int )
ldap_msgtype LDAP_P((
	LDAPMessage *lm ));

LDAP_F( int )
ldap_msgid   LDAP_P((
	LDAPMessage *lm ));

LDAP_F( int )
ldap_msgfree LDAP_P((
	LDAPMessage *lm ));

LDAP_F( int )
ldap_msgdelete LDAP_P((
	LDAP *ld,
	int msgid ));



LDAP_F( int )
ldap_bv2escaped_filter_value LDAP_P(( 
	struct berval *in, 
	struct berval *out ));

LDAP_F( int )
ldap_search_ext LDAP_P((
	LDAP			*ld,
	LDAP_CONST char	*base,
	int				scope,
	LDAP_CONST char	*filter,
	char			**attrs,
	int				attrsonly,
	LDAPControl		**serverctrls,
	LDAPControl		**clientctrls,
	struct timeval	*timeout,
	int				sizelimit,
	int				*msgidp ));

LDAP_F( int )
ldap_search_ext_s LDAP_P((
	LDAP			*ld,
	LDAP_CONST char	*base,
	int				scope,
	LDAP_CONST char	*filter,
	char			**attrs,
	int				attrsonly,
	LDAPControl		**serverctrls,
	LDAPControl		**clientctrls,
	struct timeval	*timeout,
	int				sizelimit,
	LDAPMessage		**res ));

#if LDAP_DEPRECATED
LDAP_F( int )
ldap_search LDAP_P((	
	LDAP *ld,
	LDAP_CONST char *base,
	int scope,
	LDAP_CONST char *filter,
	char **attrs,
	int attrsonly ));

LDAP_F( int )
ldap_search_s LDAP_P((	
	LDAP *ld,
	LDAP_CONST char *base,
	int scope,
	LDAP_CONST char *filter,
	char **attrs,
	int attrsonly,
	LDAPMessage **res ));

LDAP_F( int )
ldap_search_st LDAP_P((	
	LDAP *ld,
	LDAP_CONST char *base,
	int scope,
	LDAP_CONST char *filter,
    char **attrs,
	int attrsonly,
	struct timeval *timeout,
	LDAPMessage **res ));
#endif


LDAP_F( int )
ldap_unbind_ext LDAP_P((
	LDAP			*ld,
	LDAPControl		**serverctrls,
	LDAPControl		**clientctrls));

LDAP_F( int )
ldap_unbind_ext_s LDAP_P((
	LDAP			*ld,
	LDAPControl		**serverctrls,
	LDAPControl		**clientctrls));

#if LDAP_DEPRECATED
LDAP_F( int )
ldap_unbind LDAP_P(( 
	LDAP *ld ));

LDAP_F( int )
ldap_unbind_s LDAP_P(( 
	LDAP *ld ));
#endif


LDAP_F( int )
ldap_put_vrFilter LDAP_P((
	BerElement *ber,
	const char *vrf ));



LDAP_F( void * )
ldap_memalloc LDAP_P((
	ber_len_t s ));

LDAP_F( void * )
ldap_memrealloc LDAP_P((
	void* p,
	ber_len_t s ));

LDAP_F( void * )
ldap_memcalloc LDAP_P((
	ber_len_t n,
	ber_len_t s ));

LDAP_F( void )
ldap_memfree LDAP_P((
	void* p ));

LDAP_F( void )
ldap_memvfree LDAP_P((
	void** v ));

LDAP_F( char * )
ldap_strdup LDAP_P((
	LDAP_CONST char * ));

LDAP_F( void )
ldap_mods_free LDAP_P((
	LDAPMod **mods,
	int freemods ));


#if LDAP_DEPRECATED

typedef int (LDAP_SORT_AD_CMP_PROC) LDAP_P(( 
	LDAP_CONST char *left,
	LDAP_CONST char *right ));

typedef int (LDAP_SORT_AV_CMP_PROC) LDAP_P(( 
	LDAP_CONST void *left,
	LDAP_CONST void *right ));

LDAP_F( int )	
ldap_sort_entries LDAP_P(( LDAP *ld,
	LDAPMessage **chain,
	LDAP_CONST char *attr,
	LDAP_SORT_AD_CMP_PROC *cmp ));

LDAP_F( int )	
ldap_sort_values LDAP_P((
	LDAP *ld,
	char **vals,
	LDAP_SORT_AV_CMP_PROC *cmp ));

LDAP_F( int ) 
ldap_sort_strcasecmp LDAP_P((
	LDAP_CONST void *a,
	LDAP_CONST void *b ));
#endif


LDAP_F( int )
ldap_is_ldap_url LDAP_P((
	LDAP_CONST char *url ));

LDAP_F( int )
ldap_is_ldaps_url LDAP_P((
	LDAP_CONST char *url ));

LDAP_F( int )
ldap_is_ldapi_url LDAP_P((
	LDAP_CONST char *url ));

LDAP_F( int )
ldap_url_parse LDAP_P((
	LDAP_CONST char *url,
	LDAPURLDesc **ludpp ));

LDAP_F( char * )
ldap_url_desc2str LDAP_P((
	LDAPURLDesc *ludp ));

LDAP_F( void )
ldap_free_urldesc LDAP_P((
	LDAPURLDesc *ludp ));



#define LDAP_API_FEATURE_CANCEL 1000

LDAP_F( int )
ldap_cancel LDAP_P(( LDAP *ld,
	int cancelid,
	LDAPControl		**sctrls,
	LDAPControl		**cctrls,
	int				*msgidp ));

LDAP_F( int )
ldap_cancel_s LDAP_P(( LDAP *ld,
	int cancelid,
	LDAPControl **sctrl,
	LDAPControl **cctrl ));


#define LDAP_API_FEATURE_TURN 1000

LDAP_F( int )
ldap_turn LDAP_P(( LDAP *ld,
	int mutual,
	LDAP_CONST char* identifier,
	LDAPControl		**sctrls,
	LDAPControl		**cctrls,
	int				*msgidp ));

LDAP_F( int )
ldap_turn_s LDAP_P(( LDAP *ld,
	int mutual,
	LDAP_CONST char* identifier,
	LDAPControl **sctrl,
	LDAPControl **cctrl ));


#define LDAP_API_FEATURE_PAGED_RESULTS 2000

LDAP_F( int )
ldap_create_page_control_value LDAP_P((
	LDAP *ld,
	ber_int_t pagesize,
	struct berval *cookie,
	struct berval *value ));

LDAP_F( int )
ldap_create_page_control LDAP_P((
	LDAP *ld,
	ber_int_t pagesize,
	struct berval *cookie,
	int iscritical,
	LDAPControl **ctrlp ));

#if LDAP_DEPRECATED
LDAP_F( int )
ldap_parse_page_control LDAP_P((
	LDAP *ld,
	LDAPControl **ctrls,
	ber_int_t *count,
	struct berval **cookie ));
#endif

LDAP_F( int )
ldap_parse_pageresponse_control LDAP_P((
	LDAP *ld,
	LDAPControl *ctrl,
	ber_int_t *count,
	struct berval *cookie ));


#define LDAP_API_FEATURE_SERVER_SIDE_SORT 2000

typedef struct ldapsortkey {
	char *attributeType;
	char *orderingRule;
	int reverseOrder;
} LDAPSortKey;

LDAP_F( int )
ldap_create_sort_keylist LDAP_P((
	LDAPSortKey ***sortKeyList,
	char *keyString ));

LDAP_F( void )
ldap_free_sort_keylist LDAP_P((
	LDAPSortKey **sortkeylist ));

LDAP_F( int )
ldap_create_sort_control_value LDAP_P((
	LDAP *ld,
	LDAPSortKey **keyList,
	struct berval *value ));

LDAP_F( int )
ldap_create_sort_control LDAP_P((
	LDAP *ld,
	LDAPSortKey **keyList,
	int iscritical,
	LDAPControl **ctrlp ));

LDAP_F( int )
ldap_parse_sortresponse_control LDAP_P((
	LDAP *ld,
	LDAPControl *ctrl,
	ber_int_t *result,
	char **attribute ));


#define LDAP_API_FEATURE_VIRTUAL_LIST_VIEW 2000

typedef struct ldapvlvinfo {
	ber_int_t ldvlv_version;
    ber_int_t ldvlv_before_count;
    ber_int_t ldvlv_after_count;
    ber_int_t ldvlv_offset;
    ber_int_t ldvlv_count;
    struct berval *	ldvlv_attrvalue;
    struct berval *	ldvlv_context;
    void *			ldvlv_extradata;
} LDAPVLVInfo;

LDAP_F( int )
ldap_create_vlv_control_value LDAP_P((
	LDAP *ld,
	LDAPVLVInfo *ldvlistp,
	struct berval *value));

LDAP_F( int )
ldap_create_vlv_control LDAP_P((
	LDAP *ld,
	LDAPVLVInfo *ldvlistp,
	LDAPControl **ctrlp ));

LDAP_F( int )
ldap_parse_vlvresponse_control LDAP_P((
	LDAP          *ld,
	LDAPControl   *ctrls,
	ber_int_t *target_posp,
	ber_int_t *list_countp,
	struct berval **contextp,
	int           *errcodep ));


#define LDAP_API_FEATURE_WHOAMI 1000

LDAP_F( int )
ldap_parse_whoami LDAP_P((
	LDAP *ld,
	LDAPMessage *res,
	struct berval **authzid ));

LDAP_F( int )
ldap_whoami LDAP_P(( LDAP *ld,
	LDAPControl		**sctrls,
	LDAPControl		**cctrls,
	int				*msgidp ));

LDAP_F( int )
ldap_whoami_s LDAP_P((
	LDAP *ld,
	struct berval **authzid,
	LDAPControl **sctrls,
	LDAPControl **cctrls ));


#define LDAP_API_FEATURE_PASSWD_MODIFY 1000

LDAP_F( int )
ldap_parse_passwd LDAP_P((
	LDAP *ld,
	LDAPMessage *res,
	struct berval *newpasswd ));

LDAP_F( int )
ldap_passwd LDAP_P(( LDAP *ld,
	struct berval	*user,
	struct berval	*oldpw,
	struct berval	*newpw,
	LDAPControl		**sctrls,
	LDAPControl		**cctrls,
	int				*msgidp ));

LDAP_F( int )
ldap_passwd_s LDAP_P((
	LDAP *ld,
	struct berval	*user,
	struct berval	*oldpw,
	struct berval	*newpw,
	struct berval *newpasswd,
	LDAPControl **sctrls,
	LDAPControl **cctrls ));

#ifdef LDAP_CONTROL_PASSWORDPOLICYREQUEST

#define LDAP_API_FEATURE_PASSWORD_POLICY 1000

typedef enum passpolicyerror_enum {
       PP_passwordExpired = 0,
       PP_accountLocked = 1,
       PP_changeAfterReset = 2,
       PP_passwordModNotAllowed = 3,
       PP_mustSupplyOldPassword = 4,
       PP_insufficientPasswordQuality = 5,
       PP_passwordTooShort = 6,
       PP_passwordTooYoung = 7,
       PP_passwordInHistory = 8,
       PP_noError = 65535
} LDAPPasswordPolicyError;

LDAP_F( int )
ldap_create_passwordpolicy_control LDAP_P((
        LDAP *ld,
        LDAPControl **ctrlp ));

LDAP_F( int )
ldap_parse_passwordpolicy_control LDAP_P((
        LDAP *ld,
        LDAPControl *ctrl,
        ber_int_t *expirep,
        ber_int_t *gracep,
        LDAPPasswordPolicyError *errorp ));

LDAP_F( const char * )
ldap_passwordpolicy_err2txt LDAP_P(( LDAPPasswordPolicyError ));
#endif 


#define LDAP_API_FEATURE_REFRESH 1000

LDAP_F( int )
ldap_parse_refresh LDAP_P((
	LDAP *ld,
	LDAPMessage *res,
	ber_int_t *newttl ));

LDAP_F( int )
ldap_refresh LDAP_P(( LDAP *ld,
	struct berval	*dn,
	ber_int_t ttl,
	LDAPControl		**sctrls,
	LDAPControl		**cctrls,
	int				*msgidp ));

LDAP_F( int )
ldap_refresh_s LDAP_P((
	LDAP *ld,
	struct berval	*dn,
	ber_int_t ttl,
	ber_int_t *newttl,
	LDAPControl **sctrls,
	LDAPControl **cctrls ));


#ifdef LDAP_X_TXN
LDAP_F( int )
ldap_txn_start LDAP_P(( LDAP *ld,
	LDAPControl		**sctrls,
	LDAPControl		**cctrls,
	int				*msgidp ));

LDAP_F( int )
ldap_txn_start_s LDAP_P(( LDAP *ld,
	LDAPControl **sctrl,
	LDAPControl **cctrl,
	struct berval **rettxnid ));

LDAP_F( int )
ldap_txn_end LDAP_P(( LDAP *ld,
	int	commit,
	struct berval	*txnid,
	LDAPControl		**sctrls,
	LDAPControl		**cctrls,
	int				*msgidp ));

LDAP_F( int )
ldap_txn_end_s LDAP_P(( LDAP *ld,
	int	commit,
	struct berval *txnid,
	LDAPControl **sctrl,
	LDAPControl **cctrl,
	int *retidp ));
#endif




LDAP_F( ldap_sync_t * )
ldap_sync_initialize LDAP_P((
	ldap_sync_t	*ls ));


LDAP_F( void )
ldap_sync_destroy LDAP_P((
	ldap_sync_t	*ls,
	int		freeit ));


LDAP_F( int )
ldap_sync_init LDAP_P((
	ldap_sync_t	*ls,
	int		mode ));


LDAP_F( int )
ldap_sync_init_refresh_only LDAP_P((
	ldap_sync_t	*ls ));


LDAP_F( int )
ldap_sync_init_refresh_and_persist LDAP_P((
	ldap_sync_t	*ls ));


LDAP_F( int )
ldap_sync_poll LDAP_P((
	ldap_sync_t	*ls ));

#ifdef LDAP_CONTROL_X_SESSION_TRACKING


LDAP_F( int )
ldap_create_session_tracking_value LDAP_P((
	LDAP		*ld,
	char		*sessionSourceIp,
	char		*sessionSourceName,
	char		*formatOID,
	struct berval	*sessionTrackingIdentifier,
	struct berval	*value ));

LDAP_F( int )
ldap_create_session_tracking LDAP_P((
	LDAP		*ld,
	char		*sessionSourceIp,
	char		*sessionSourceName,
	char		*formatOID,
	struct berval	*sessionTrackingIdentifier,
	LDAPControl	**ctrlp ));

LDAP_F( int )
ldap_parse_session_tracking_control LDAP_P((
	LDAP *ld,
	LDAPControl *ctrl,
	struct berval *ip,
	struct berval *name,
	struct berval *oid,
	struct berval *id ));

#endif 


LDAP_F (int)
ldap_create_assertion_control_value LDAP_P((
	LDAP		*ld,
	char		*assertion,
	struct berval	*value ));

LDAP_F( int )
ldap_create_assertion_control LDAP_P((
	LDAP		*ld,
	char		*filter,
	int		iscritical,
	LDAPControl	**ctrlp ));

LDAP_END_DECL
#endif 
