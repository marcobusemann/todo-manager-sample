#include "todomapping.h"
#include "personmapping.h"

#include <QMetaOrm/MetaEntityBuilder.h>

template <>
QMetaOrm::MetaEntity::Ptr QMetaOrm::Mappings::mapping<Todo>()
{
    return MetaEntityBuilder::anEntity()
        .forSource("todos")
        .withId("id", "id")
        .withData("title", "title")
        .withData("description", "description")
        .withData("endDate", "end_date", "DateTimeConverter")
        .withData("completed", "is_completed", "BooleanConverter")
        .withOneToMany("owner", "id_owner", mapping<Person>())
        .build<Todo>();
}
