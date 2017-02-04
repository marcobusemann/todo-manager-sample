#include <QMetaOrm/MetaEntity.h>
#include <data/person.h>

namespace QMetaOrm {
    namespace Mappings {
        template <>
        MetaEntity::Ptr mapping<Person>();
    }
}
