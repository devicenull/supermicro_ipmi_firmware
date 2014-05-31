/*
 * test_objectpath.cpp
 *
 * (C) Copyright Intel Corp. 2005
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:        Steve Vest <stevenx.w.vest@intel.com>
 *
 * Description:
 *
 *  Test for sameCMPIObjectPath() library API.
 */

/** Test the Clp API class.
 */

#include <melunit/melunit-cxx.h>
#include <string>
#include <iostream>

#include <lib/cmci.h>

namespace
{       
    class test_objectpath: public Melunit::Test
    {   
        CMPIObjectPath * op1;
        CMPIObjectPath * op2;
        CMPIObjectPath * op3;
        CMPIObjectPath * op4;
        CMPIObjectPath * op5;
        CMPIObjectPath * op6;
        CMPIObjectPath * op7;
        CMPIObjectPath * op8;

        bool test_AllocateObjects()
        {
            op1 = newCMPIObjectPath("root/cimv2", "CIM_ComputerSystem", NULL);

            op2 = newCMPIObjectPath("root/cimv2", "CIM_ComputerSystem", NULL);
            CMAddKey(op2, "CreationClassName", "CIM_ComputerSystem", CMPI_chars);

            op3 = newCMPIObjectPath("root/cimv3", "CIM_ComputerSystem", NULL);
            op4 = newCMPIObjectPath("root/cimv2", "CIM_ComputerSystem1", NULL);

            op5 = newCMPIObjectPath("root/cimv2", "CIM_ComputerSystem", NULL);
            CMAddKey(op5, "CreationClassName", "Bubba", CMPI_chars);

            op6 = newCMPIObjectPath("root/cimv2", "CIM_ComputerSystem", NULL);

            op7 = newCMPIObjectPath("root/cimv2", "CIM_ComputerSystem", NULL);
            CMAddKey(op7, "CreationClassName", "CIM_ComputerSystem", CMPI_chars);

            op8 = newCMPIObjectPath("root/cimv2", "CIM_ComputerSystem", NULL);
            CMAddKey(op8, "CreationClassName", "CIM_ComputerSystem", CMPI_chars);
            CMAddKey(op8, "Name", "Bubba", CMPI_chars);

            assert_true( op1 );
            assert_true( op2 );
            assert_true( op3 );
            assert_true( op4 );
            assert_true( op5 );
            assert_true( op6 );
            assert_true( op7 );
            assert_true( op8 );

            return (true);
        }

        bool test_SelfCompare()
        {
            // Try comparing things with themselves
            assert_true( sameCMPIObjectPath( op1, op1 ) );
            assert_true( sameCMPIObjectPath( op2, op2 ) );
            assert_true( sameCMPIObjectPath( op2, op7 ) );
            assert_true( sameCMPIObjectPath( op3, op3 ) );
            assert_true( sameCMPIObjectPath( op4, op4 ) );
            assert_true( sameCMPIObjectPath( op5, op5 ) );
            assert_true( sameCMPIObjectPath( op6, op6 ) );
            assert_true( sameCMPIObjectPath( op7, op7 ) );
            assert_true( sameCMPIObjectPath( op8, op8 ) );

            return (true);
        }

        bool test_NotEqual()
        {
            // Try comparing things with differences
            assert_false( sameCMPIObjectPath( op1, op2 ) );
            assert_false( sameCMPIObjectPath( op1, op3 ) );
            assert_false( sameCMPIObjectPath( op1, op4 ) );
            assert_false( sameCMPIObjectPath( op1, op5 ) );
            assert_false( sameCMPIObjectPath( op1, op7 ) );
            assert_false( sameCMPIObjectPath( op1, op8 ) );
            
            return (true);
        }

        bool test_Equal()
        {
            // Try comparing something with a difference instance, but 
            // containing the same data.
            assert_true( sameCMPIObjectPath( op1, op6 ) );
            assert_true( sameCMPIObjectPath( op6, op1 ) );

            return (true);
        }

    public:

        test_objectpath(): Melunit::Test("native")
        {

#define REGISTER(name) register_test(#name, &test_objectpath::name)

            REGISTER( test_AllocateObjects );
            REGISTER( test_SelfCompare );
            REGISTER( test_NotEqual );
            REGISTER( test_Equal );
                        
            Melunit::Suite::instance().register_test(this);
#undef REGISTER
        }

        ~test_objectpath()
        {
            // Free up dynamically created objects.

            if( op1 )
            {
                CMRelease( op1 );
            }

            if( op2 )
            {
                CMRelease( op2 );
            }

            if( op3 )
            {
                CMRelease( op3 );
            }

            if( op4 )
            {
                CMRelease( op4 );
            }

            if( op5 )
            {
                CMRelease( op5 );
            }

            if( op6 )
            {
                CMRelease( op6 );
            }

            if( op7 )
            {
                CMRelease( op7 );            
            }
        }
    };

    test_objectpath t_;
}

// Local variables:
// mode: c++
// End:
