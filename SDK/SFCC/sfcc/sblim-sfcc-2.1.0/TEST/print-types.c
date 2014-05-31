#include <cmcidt.h>

#define PRINT_TYPE(type) printf("The type " #type " is %d\n", type)

int
main() 
{
    PRINT_TYPE(CMPI_null);

    PRINT_TYPE(CMPI_SIMPLE);
    PRINT_TYPE(CMPI_boolean);
    PRINT_TYPE(CMPI_char16);

    PRINT_TYPE(CMPI_REAL);
    PRINT_TYPE(CMPI_real32);
    PRINT_TYPE(CMPI_real64);

    PRINT_TYPE(CMPI_UINT);
    PRINT_TYPE(CMPI_uint8);
    PRINT_TYPE(CMPI_uint16);
    PRINT_TYPE(CMPI_uint32);
    PRINT_TYPE(CMPI_uint64);
    PRINT_TYPE(CMPI_SINT);
    PRINT_TYPE(CMPI_sint8);
    PRINT_TYPE(CMPI_sint16);
    PRINT_TYPE(CMPI_sint32);
    PRINT_TYPE(CMPI_sint64);
    PRINT_TYPE(CMPI_INTEGER);

    PRINT_TYPE(CMPI_ENC);
    PRINT_TYPE(CMPI_instance);
    PRINT_TYPE(CMPI_ref);
    PRINT_TYPE(CMPI_args);
    PRINT_TYPE(CMPI_class);
    PRINT_TYPE(CMPI_filter);
    PRINT_TYPE(CMPI_enumeration);
    PRINT_TYPE(CMPI_string);
    PRINT_TYPE(CMPI_chars);
    PRINT_TYPE(CMPI_dateTime);
    PRINT_TYPE(CMPI_ptr);
    PRINT_TYPE(CMPI_charsptr);

    PRINT_TYPE(CMPI_ARRAY);
    PRINT_TYPE(CMPI_SIMPLEA);
    PRINT_TYPE(CMPI_booleanA);
    PRINT_TYPE(CMPI_char16A);

    PRINT_TYPE(CMPI_REALA);
    PRINT_TYPE(CMPI_real32A);
    PRINT_TYPE(CMPI_real64A);

    PRINT_TYPE(CMPI_UINTA);
    PRINT_TYPE(CMPI_uint8A);
    PRINT_TYPE(CMPI_uint16A);
    PRINT_TYPE(CMPI_uint32A);
    PRINT_TYPE(CMPI_uint64A);
    PRINT_TYPE(CMPI_SINTA);
    PRINT_TYPE(CMPI_sint8A);
    PRINT_TYPE(CMPI_sint16A);
    PRINT_TYPE(CMPI_sint32A);
    PRINT_TYPE(CMPI_sint64A);
    PRINT_TYPE(CMPI_INTEGERA);

    PRINT_TYPE(CMPI_ENCA);
    PRINT_TYPE(CMPI_stringA);
    PRINT_TYPE(CMPI_charsA);
    PRINT_TYPE(CMPI_dateTimeA);
    PRINT_TYPE(CMPI_instanceA);
    PRINT_TYPE(CMPI_refA);
    PRINT_TYPE(CMPI_ptrA);
    PRINT_TYPE(CMPI_charsptrA);

    // the following are CMPIObjectPath key-types synonyms
    // and are valid only when CMPI_keyValue of CMPIValueState is set

    PRINT_TYPE(CMPI_keyInteger);
    PRINT_TYPE(CMPI_keyString);
    PRINT_TYPE(CMPI_keyBoolean);
    PRINT_TYPE(CMPI_keyRef);

    // the following are predicate types only

    PRINT_TYPE(CMPI_charString);
    PRINT_TYPE(CMPI_integerString);
    PRINT_TYPE(CMPI_realString);
    PRINT_TYPE(CMPI_numericString);
    PRINT_TYPE(CMPI_booleanString);
    PRINT_TYPE(CMPI_dateTimeString);
    PRINT_TYPE(CMPI_classNameString);
    PRINT_TYPE(CMPI_nameString);

    PRINT_TYPE(CMPI_goodValue);
    PRINT_TYPE(CMPI_nullValue);
    PRINT_TYPE(CMPI_keyValue);
    PRINT_TYPE(CMPI_notFound);
    PRINT_TYPE(CMPI_badValue);
}
