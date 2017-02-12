#include "personmapping.h"

#include <QMetaOrm/MetaEntityBuilder.h>

template <>
QMetaOrm::MetaEntity::Ptr QMetaOrm::Mappings::mapping<Person>()
{
    return MetaEntityBuilder::anEntity()
        .forSource("persons")
        .withId("id", "id")
        .withData("name", "name")
        .withData("surname", "surname")
        .build<Person>();
}
