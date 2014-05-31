/**
 * $Id: hash.c,v 1.2 2005/11/20 17:37:05 bestorga-oss Exp $
 *
 * (C) Copyright IBM Corp. 2004
 * 
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:       Viktor Mihajlovski <mihajlov@de.ibm.com>
 * Contributors: 
 *
 * Description: Hashtable Support
 *
 */
 
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "hash.h"

/*
# define NOHASH_HANDLER
# define HASH_PROTOCOL
# define NOSTORAGE
 */  

  typedef enum { HASH_STATUS_OK, HASH_STATUS_FATAL } hash_status;
  

# if defined HASH_HANDLER
  static void hash_error( const char * info )
  {
    fprintf( stderr, "Error in hash routine: %s.\n", info );
    exit( HASH_STATUS_FATAL );
  }
# endif

# if defined STORAGE
#  define COR(x) printf( "Storage for %s left : %u\n", x, coreleft() )
# else
#  define COR(x)
# endif /* STORAGE */

# if defined HASH_PROTOCOL  
    static struct {
    	int insertions;
	int lookups;
	int collisions;
	int duplicates;
	int searchlen;
	int maxsearch;
    } prot_rec = { 0, 0, 0, 0, 0, 0 };
    
#   define HASHPROT(x) ++(prot_rec.x)
#   define HASHCHECK {\
		      if( prot_rec.maxsearch < prot_rec.searchlen )\
			 prot_rec.maxsearch = prot_rec.searchlen;\
		      prot_rec.searchlen = 0;\
		     }
#   define HASHSUM(x) {\
		   printf( "Hash table summary report:\n" );\
                   printf( "\tinsertions:\t%d\n", prot_rec.insertions );\
                   printf( "\tlookups:\t%d\n", prot_rec.lookups );\
                   printf( "\tcollisions:\t%d\n", prot_rec.collisions );\
                   printf( "\tduplicates:\t%d\n", prot_rec.duplicates );\
                   printf( "\tmax search length:\t%d\n", prot_rec.maxsearch );\
		   printf( "\tnumber of table entries: %d\n", (x)->he.number );\
		   }
# else
#   define HASHPROT(x)
#   define HASHCHECK
#   define HASHSUM(x)
# endif /* HASH_PROTOCOL */

# if defined HASH_HANDLER  
#   define HASHCOND(c,x) if( c ) hash_error(x)
# else
#   define HASHCOND(c,x)
# endif /* HASH_HANDLER */

  hashentry * htcreate( const char * table_name )
/*=========================================
 * create a hash table with id "table_name"
 */
  {
    hashentry * help = (hashentry *) calloc( MAXHASH + 1, sizeof( hashentry ) );
  
    COR( "htcreate" );
    if( help != NULL )
      help -> he_key = strdup( table_name );
      
    HASHCOND( help == NULL, "hashtable_create: not enough memory" );
    
    return help;
  }
  
  void htdelete( hashentry * tab )
/*================================
 * destructor for a table
 */
  {
    HASHSUM(tab);
    free( tab );
  }

  void * htlookup( hashentry * tab, const char * key, unsigned len )
/*============================================================
 * lookup a string "key" of length "len" in table "tab"
 */
  {
    unsigned fkey = fold( key, len );
    unsigned prim = primary( fkey );
    unsigned sec  = secondary( fkey );
    unsigned lastpos = 0;
    int cmp = 1;
    
    HASHPROT(lookups);
    HASHCOND( tab == NULL, "htlookup: NULL table as argument" );
    
    lastpos = ( prim + tab -> he.number * sec ) & (MAXHASH);
    ++ tab;	/* set hash table start position to tab */
    
    while(  prim != lastpos
         && tab[prim].he_state == HE_USED
         && ( cmp = strncmp( tab[prim].he_key, key, len ) )
	 )
    {
      HASHPROT(searchlen);
      prim = ( prim + sec ) & MAXHASH;
    }
    
    HASHCHECK;
    if( cmp == 0 )
      return tab[prim].he.ptr;
    else
      return NULL;
  }  
  
  int htinsert( hashentry * tab, const char * key, unsigned len, void * elem )
/*======================================================================
 * inserts a hash table entry for "elem" with key "key"
 */
  {
    unsigned fkey = fold( key, len );
    unsigned prim = primary( fkey );
    unsigned sec  = secondary( fkey );
    hashentry srch;
    unsigned lastpos = 0;
    int cmp = 1;
    
    COR( "htinsert" ); 
    HASHPROT(insertions);
    HASHCOND( ( tab -> he.number ) >= MAXHASH, "htinsert: table full" );
    
    if( ( tab -> he.number ) >= MAXHASH ) {
      return 0;
    }
    else {
      srch.he_key = strdup( key );
      srch.he_state = HE_USED;
      srch.he.ptr = elem;
      lastpos = ( prim + tab -> he.number * sec ) & MAXHASH;
      ++ tab;
      
      while(  prim != lastpos
           && tab[prim].he_state == HE_USED
	   && ( cmp = strncmp( tab[prim].he_key, srch.he_key, len ) ) != 0
	   )
      {
	prim = ( prim + sec ) & MAXHASH;
	HASHPROT(collisions);
	HASHPROT(searchlen);
      }
      
      HASHCHECK;
      if( tab[prim].he_state == HE_FREE ) {
        tab[prim] = srch;
	( tab - 1 ) -> he.number += 1;
	return 1; /* inserted */
      }	
      else {
        HASHCOND( 1, "htinsert: duplicate key " );
	HASHPROT(duplicates);
	return 0;
      }
    }
    
  }

  unsigned fold( const char * id, unsigned len )
  {
    unsigned long fval = 0;
    unsigned long ival;
    unsigned ilen = len / sizeof( long );	/* length in long int units */
    unsigned clen = len % sizeof( long );	/* remainder in char units */
  
    while ( ilen-- ) {
      memcpy(&ival,id,sizeof(long));
      id += sizeof(long);
      fval ^= ival;
    }
  
    while ( clen-- ) {
      fval += *id++;
    }
    
    return (unsigned) fval ;
  }

  unsigned primary( unsigned u )
  {
    return u & MAXHASH;
  }

  unsigned secondary( unsigned u )
  {
    return ( ( u >> ( sizeof( unsigned ) * 4 ) ) + 1 ) & MAXHASH;
  }
