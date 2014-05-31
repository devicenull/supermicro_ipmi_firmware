#ifndef _CONN_INFO_H
#define _CONN_INFO_H

#ifdef __cplusplus
extern "C" {
#endif

struct _TimeoutControl {
  time_t   mTimestampStart;
  time_t   mTimestampLast;
  unsigned mFixups;
};
#ifndef LARGE_VOL_SUPPORT
struct _CMCIConnection {
    CMCIConnectionFT *ft;        
    CURL *mHandle;               // The handle to the curl object
    struct curl_slist *mHeaders; // The list of headers sent with each request
    UtilStringBuffer *mBody;     // The body of the request
    UtilStringBuffer *mUri;      // The uri of the request
    UtilStringBuffer *mUserPass; // The username/password used in authentication
    UtilStringBuffer *mResponse; // Used to store the HTTP response
    CMPIStatus        mStatus;   // returned request status (via HTTP trailers)               
    struct _TimeoutControl mTimeout; /* Used for timeout control */
};
#else
struct _CMCIConnection {
    CMCIConnectionFT *ft;        
    CURL *mHandle;               // The handle to the curl object
    struct curl_slist *mHeaders; // The list of headers sent with each request
    UtilStringBuffer *mBody;     // The body of the request
    UtilStringBuffer *mUri;      // The uri of the request
    UtilStringBuffer *mUserPass; // The username/password used in authentication
    UtilStringBuffer *mResponse; // Used to store the HTTP response
    CMPIStatus        mStatus;   // returned request status (via HTTP trailers)               
    struct _TimeoutControl mTimeout; /* Used for timeout control */
    struct asyncrespcntl   asynRCntl  ;/* sync response                   */ 
};

#endif /* endif LARGE_VOL_SUPPORT */
#ifdef __cplusplus
 }
 
#endif

#endif

