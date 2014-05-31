
/*
 * $Id: internalProvider.h,v 1.5 2007/10/23 11:14:22 sschuetz Exp $
 *
 * (C) Copyright IBM Corp. 2005
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:        Viktor Mihajlovski <mihajlov@de.ibm.com>
 * Contributions: 
 *
 * Description:
 *
 * Header file for the internal provider external functions
 *
*/

#ifndef INTERNAL_PROVIDER_H
#define INTERNAL_PROVIDER_H

#include "utilft.h"
#include "cmpidt.h"

CMPIStatus InternalProviderEnumInstanceNames(CMPIInstanceMI * mi,
					     const CMPIContext * ctx, 
					     const CMPIResult * rslt, 
					     const CMPIObjectPath * ref);
CMPIStatus InternalProviderEnumInstances(CMPIInstanceMI * mi,
					 const CMPIContext * ctx, 
					 const CMPIResult * rslt, 
					 const CMPIObjectPath * ref, 
					 const char **properties);
CMPIInstance *internalProviderGetInstance(const CMPIObjectPath * cop, 
					  CMPIStatus *rc);
CMPIStatus InternalProviderCreateInstance(CMPIInstanceMI * mi,
					  const CMPIContext * ctx, 
					  const CMPIResult * rslt, 
					  const CMPIObjectPath * cop, 
					  const CMPIInstance * ci);
CMPIStatus InternalProviderModifyInstance(CMPIInstanceMI * mi,
					  const CMPIContext * ctx,
					  const CMPIResult * rslt,
					  const CMPIObjectPath * cop,
					  const CMPIInstance * ci, 
					  const char **properties);					  
CMPIStatus InternalProviderGetInstance(CMPIInstanceMI * mi,
				       const CMPIContext * ctx,
				       const CMPIResult * rslt,
				       const CMPIObjectPath * cop, 
				       const char **properties);
CMPIStatus InternalProviderDeleteInstance(CMPIInstanceMI * mi,
					  const CMPIContext * ctx, 
					  const CMPIResult * rslt, 
					  const CMPIObjectPath * cop);
CMPIStatus InternalProviderAssociatorNames(CMPIAssociationMI * mi,
                                           const CMPIContext * ctx,
                                           const CMPIResult * rslt,
                                           const CMPIObjectPath * cop,
                                           const char *assocClass,
                                           const char *resultClass,
                                           const char *role,
                                           const char *resultRole);
CMPIStatus InternalProviderAssociators(CMPIAssociationMI * mi,
                                       const CMPIContext * ctx,
                                       const CMPIResult * rslt,
                                       const CMPIObjectPath * cop,
                                       const char *assocClass,
                                       const char *resultClass,
                                       const char *role,
                                       const char *resultRole,
                                       const char **propertyList);
CMPIStatus InternalProviderReferenceNames(CMPIAssociationMI * mi,
                                          const CMPIContext * ctx,
                                          const CMPIResult * rslt,
                                          const CMPIObjectPath * cop,
                                          const char *assocClass,
                                          const char *role);
CMPIStatus InternalProviderReferences(CMPIAssociationMI * mi,
                                      const CMPIContext * ctx,
                                      const CMPIResult * rslt,
                                      const CMPIObjectPath * cop,
                                      const char *assocClass,
                                      const char *role, const char **propertyList);                                                                                                                            
CMPIStatus SafeInternalProviderEnumInstances(CMPIInstanceMI * mi,
                                      const CMPIContext * ctx,
                                      const CMPIResult * rslt,
                                      const CMPIObjectPath * ref,
                                      const char **properties,
                                      int ignprov);
extern char *internalProviderNormalizeObjectPath(const CMPIObjectPath *cop);

#endif
